/****************************************************************************
**
** Copyright (C) 2016 Research In Motion
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
****************************************************************************/

#ifndef QNXAUDIOINPUT_H
#define QNXAUDIOINPUT_H

#include "qaudiosystem.h"

#include <QSocketNotifier>
#include <QIODevice>
#include <QTime>
#include <QTimer>

#include <sys/asoundlib.h>

QT_BEGIN_NAMESPACE

class QnxAudioInput : public QAbstractAudioInput
{
    Q_OBJECT

public:
    QnxAudioInput();
    ~QnxAudioInput();

    void start(QIODevice*) override;
    QIODevice* start() override;
    void stop() override;
    void reset() override;
    void suspend() override;
    void resume() override;
    int bytesReady() const override;
    int periodSize() const override;
    void setBufferSize(int ) override;
    int bufferSize() const  override;
    void setNotifyInterval(int ) override;
    int notifyInterval() const override;
    qint64 processedUSecs() const override;
    qint64 elapsedUSecs() const override;
    QAudio::Error error() const override;
    QAudio::State state() const override;
    void setFormat(const QAudioFormat&) override;
    QAudioFormat format() const override;
    void setVolume(qreal) override;
    qreal volume() const override;

private slots:
    void userFeed();
    bool deviceReady();

private:
    friend class InputPrivate;

    bool open();
    void close();
    qint64 read(char *data, qint64 len);
    void setError(QAudio::Error error);
    void setState(QAudio::State state);

    QTime m_timeStamp;
    QTime m_clockStamp;
    QAudioFormat m_format;

    QIODevice *m_audioSource;
    snd_pcm_t *m_pcmHandle;
    QSocketNotifier *m_pcmNotifier;

    QAudio::Error m_error;
    QAudio::State m_state;

    qint64 m_bytesRead;
    qint64 m_elapsedTimeOffset;
    qint64 m_totalTimeValue;

    qreal m_volume;

    int m_bytesAvailable;
    int m_bufferSize;
    int m_periodSize;
    int m_intervalTime;

    bool m_pullMode;
};

class InputPrivate : public QIODevice
{
    Q_OBJECT
public:
    InputPrivate(QnxAudioInput *audio);

    qint64 readData(char *data, qint64 len) override;
    qint64 writeData(const char *data, qint64 len) override;

    void trigger();

private:
    QnxAudioInput *m_audioDevice;
};

QT_END_NAMESPACE

#endif
