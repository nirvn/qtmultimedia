// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qaudiostatemachine_p.h"
#include "qaudiosystem_p.h"
#include <qpointer.h>
#include <qdebug.h>

QT_BEGIN_NAMESPACE

using Guard = QAudioStateMachine::StateChangeGuard;
using RawState = QAudioStateMachine::RawState;

namespace {
constexpr RawState DrainingFlag = 1 << 16;
constexpr RawState InProgressFlag = 1 << 17;
constexpr RawState WaitingFlags = DrainingFlag | InProgressFlag;

inline bool isWaitingState(RawState state)
{
    return (state & WaitingFlags) != 0;
}

inline bool isDrainingState(RawState state)
{
    return (state & DrainingFlag) != 0;
}

inline RawState fromWaitingState(RawState state)
{
    return state & ~WaitingFlags;
}

inline QAudio::State toAudioState(RawState state)
{
    return QAudio::State(fromWaitingState(state));
}

template<typename... States>
constexpr std::pair<RawState, uint32_t> makeStatesSet(QAudio::State first, States... others)
{
    return { first, ((1 << first) | ... | (1 << others)) };
}

// ensures compareExchange (testAndSet) operation with opportunity
// to check several states, can be considered as atomic
template<typename T, typename Predicate>
bool multipleCompareExchange(std::atomic<T> &target, T &prevValue, T newValue, Predicate predicate)
{
    Q_ASSERT(predicate(prevValue));
    do {
        if (target.compare_exchange_strong(prevValue, newValue))
            return true;
    } while (predicate(prevValue));

    return false;
}

} // namespace

struct QAudioStateMachine::Synchronizer {
    QMutex m_mutex;
    QWaitCondition m_condition;

    template <typename Changer>
    void changeState(Changer&& changer) {
        {
            QMutexLocker locker(&m_mutex);
            changer();
        }

        m_condition.notify_all();
    }

    void waitForOperationFinished(std::atomic<RawState>& state)
    {
        QMutexLocker locker(&m_mutex);
        while (isWaitingState(state))
            m_condition.wait(&m_mutex);
    }

    void waitForDrained(std::atomic<RawState>& state, std::chrono::milliseconds timeout) {
        QMutexLocker locker(&m_mutex);
        if (isDrainingState(state))
            m_condition.wait(&m_mutex, timeout.count());
    }
};

QAudioStateMachine::QAudioStateMachine(QAudioStateChangeNotifier &notifier, bool synchronize) :
    m_notifier(&notifier),
    m_sychronizer(synchronize ? std::make_unique<Synchronizer>() : nullptr)
{
}

QAudioStateMachine::~QAudioStateMachine() = default;

QAudio::State QAudioStateMachine::state() const
{
    return toAudioState(m_state);
}

QAudio::Error QAudioStateMachine::error() const
{
    return m_error;
}

Guard QAudioStateMachine::changeState(std::pair<RawState, uint32_t> prevStatesSet,
                                      RawState newState, QAudio::Error error, bool shouldDrain)
{
    auto checkState = [flags = prevStatesSet.second](RawState state) {
        return (flags >> state) & 1;
    };

    if (!m_sychronizer) {
        RawState prevState = prevStatesSet.first;
        const auto exchanged = multipleCompareExchange(
                m_state, prevState, newState, checkState);

        if (Q_LIKELY(exchanged))
            return { this, newState, prevState, error };

        return {};
    }

    while (true) {
        RawState prevState = prevStatesSet.first;

        const auto newWaitingState = newState | (shouldDrain ? WaitingFlags : InProgressFlag);

        const auto exchanged = multipleCompareExchange(
                m_state, prevState, newWaitingState, [checkState](RawState state) {
                    return !isWaitingState(state) && checkState(state);
                });

        if (Q_LIKELY(exchanged))
            return { this, newState, prevState, error };

        if (!isWaitingState(prevState))
            return {};

        if (!checkState(fromWaitingState(prevState)))
            return {};

        m_sychronizer->waitForOperationFinished(m_state);
    }
}

Guard QAudioStateMachine::stop(QAudio::Error error, bool shouldDrain, bool forceUpdateError)
{
    auto result = changeState(
            makeStatesSet(QAudio::ActiveState, QAudio::IdleState, QAudio::SuspendedState),
            QAudio::StoppedState, error, shouldDrain);

    if (!result && forceUpdateError)
        setError(error);

    return result;
}

Guard QAudioStateMachine::start(bool active)
{
    return changeState(makeStatesSet(QAudio::StoppedState),
                       active ? QAudio::ActiveState : QAudio::IdleState);
}

void QAudioStateMachine::waitForDrained(std::chrono::milliseconds timeout)
{
    if (m_sychronizer)
        m_sychronizer->waitForDrained(m_state, timeout);
}

void QAudioStateMachine::onDrained()
{
    if (m_sychronizer)
        m_sychronizer->changeState([this]() { m_state &= ~DrainingFlag; });
}

bool QAudioStateMachine::isDraining() const
{
    return isDrainingState(m_state);
}

bool QAudioStateMachine::isActiveOrIdle() const {
    const auto state = this->state();
    return state == QAudio::ActiveState || state == QAudio::IdleState;
}

std::pair<bool, bool> QAudioStateMachine::getDrainedAndStopped() const
{
    const RawState state = m_state;
    return { !isDrainingState(state), toAudioState(state) == QAudio::StoppedState };
}

Guard QAudioStateMachine::suspend()
{
    // Due to the current documentation, we set QAudio::NoError.
    // TBD: leave the previous error should be more reasonable (IgnoreError)
    const auto error = QAudio::NoError;
    auto result = changeState(makeStatesSet(QAudio::ActiveState, QAudio::IdleState),
                              QAudio::SuspendedState, error);

    if (result)
        m_suspendedInState = result.prevState();

    return result;
}

Guard QAudioStateMachine::resume()
{
    // Due to the current documentation, we set QAudio::NoError.
    // TBD: leave the previous error should be more reasonable (IgnoreError)
    const auto error = QAudio::NoError;
    return changeState(makeStatesSet(QAudio::SuspendedState), m_suspendedInState, error);
}

Guard QAudioStateMachine::activateFromIdle()
{
    return changeState(makeStatesSet(QAudio::IdleState), QAudio::ActiveState);
}

Guard QAudioStateMachine::updateActiveOrIdle(bool isActive, QAudio::Error error)
{
    const auto state = isActive ? QAudio::ActiveState : QAudio::IdleState;
    return changeState(makeStatesSet(QAudio::ActiveState, QAudio::IdleState), state, error);
}

void QAudioStateMachine::setError(QAudio::Error error)
{
    if (m_error.exchange(error) != error && m_notifier)
        emit m_notifier->errorChanged(error);
}

Guard QAudioStateMachine::forceSetState(QAudio::State state, QAudio::Error error)
{
    return changeState(makeStatesSet(QAudio::ActiveState, QAudio::IdleState, QAudio::SuspendedState,
                                     QAudio::StoppedState),
                       state, error);
}

void QAudioStateMachine::reset(RawState state, RawState prevState, QAudio::Error error)
{
    Q_ASSERT(!isWaitingState(state));

    if (!m_sychronizer && m_state != state)
        return;

    const auto isErrorChanged = m_error.exchange(error) != error;

    if (m_sychronizer)
        m_sychronizer->changeState([&](){ m_state = state; });

    auto notifier = m_notifier;

    if (prevState != state && notifier)
        emit notifier->stateChanged(toAudioState(state));

    // check the notifier in case the object was deleted in
    if (isErrorChanged && notifier)
        emit notifier->errorChanged(error);
}

QT_END_NAMESPACE
