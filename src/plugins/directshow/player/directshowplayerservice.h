/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
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

#ifndef DIRECTSHOWPLAYERSERVICE_H
#define DIRECTSHOWPLAYERSERVICE_H

#include <dshow.h>

#include "qmediaplayer.h"
#include "qmediaresource.h"
#include "qmediaservice.h"
#include "qmediatimerange.h"

#include "directshoweventloop.h"
#include "directshowglobal.h"

#include <QtCore/qcoreevent.h>
#include <QtCore/qmutex.h>
#include <QtCore/qurl.h>
#include <QtCore/qwaitcondition.h>

QT_BEGIN_NAMESPACE

class DirectShowAudioEndpointControl;
class DirectShowMetaDataControl;
class DirectShowPlayerControl;
class DirectShowVideoRendererControl;
class DirectShowAudioProbeControl;
class DirectShowVideoProbeControl;
class DirectShowSampleGrabber;

class QMediaContent;
class QVideoWindowControl;

class DirectShowPlayerService : public QMediaService
{
    Q_OBJECT
public:
    enum StreamType
    {
        AudioStream = 0x01,
        VideoStream = 0x02
    };

    DirectShowPlayerService(QObject *parent = nullptr);
    ~DirectShowPlayerService() override;

    QMediaControl *requestControl(const char *name) override;
    void releaseControl(QMediaControl *control) override;

    void load(const QMediaContent &media, QIODevice *stream);
    void play();
    void pause();
    void stop();

    qint64 position() const;
    QMediaTimeRange availablePlaybackRanges() const;

    void seek(qint64 position);
    void setRate(qreal rate);

    int bufferStatus() const;

    void setAudioOutput(IBaseFilter *filter);
    void setVideoOutput(IBaseFilter *filter);

protected:
    void customEvent(QEvent *event) override;

private Q_SLOTS:
    void videoOutputChanged();

    void onAudioBufferAvailable(double time, const QByteArray &data);
    void onVideoBufferAvailable(double time, const QByteArray &data);

private:
    void releaseGraph();
    void updateStatus();

    void updateAudioProbe();
    void updateVideoProbe();

    int findStreamTypes(IBaseFilter *source) const;
    int findStreamType(IPin *pin) const;

    bool isConnected(IBaseFilter *filter, PIN_DIRECTION direction) const;
    IBaseFilter *getConnected(IBaseFilter *filter, PIN_DIRECTION direction) const;

    void run();

    void doSetUrlSource(QMutexLocker *locker);
    void doSetStreamSource(QMutexLocker *locker);
    void doRender(QMutexLocker *locker);
    void doFinalizeLoad(QMutexLocker *locker);
    void doSetRate(QMutexLocker *locker);
    void doSeek(QMutexLocker *locker);
    void doPlay(QMutexLocker *locker);
    void doPause(QMutexLocker *locker);
    void doStop(QMutexLocker *locker);
    void doReleaseAudioOutput(QMutexLocker *locker);
    void doReleaseVideoOutput(QMutexLocker *locker);
    void doReleaseGraph(QMutexLocker *locker);
    void doSetVideoProbe(QMutexLocker *locker);
    void doSetAudioProbe(QMutexLocker *locker);
    void doReleaseVideoProbe(QMutexLocker *locker);
    void doReleaseAudioProbe(QMutexLocker *locker);

    void graphEvent(QMutexLocker *locker);

    enum Task
    {
        Shutdown           = 0x00001,
        SetUrlSource       = 0x00002,
        SetStreamSource    = 0x00004,
        SetSource          = SetUrlSource | SetStreamSource,
        SetAudioOutput     = 0x00008,
        SetVideoOutput     = 0x00010,
        SetOutputs         = SetAudioOutput | SetVideoOutput,
        SetAudioProbe      = 0x00020,
        SetVideoProbe      = 0x00040,
        SetProbes          = SetAudioProbe | SetVideoProbe,
        Render             = 0x00080,
        FinalizeLoad       = 0x00100,
        SetRate            = 0x00200,
        Seek               = 0x00400,
        Play               = 0x00800,
        Pause              = 0x01000,
        Stop               = 0x02000,
        ReleaseGraph       = 0x04000,
        ReleaseAudioOutput = 0x08000,
        ReleaseVideoOutput = 0x10000,
        ReleaseAudioProbe  = 0x20000,
        ReleaseVideoProbe  = 0x40000,
        ReleaseFilters     = ReleaseGraph | ReleaseAudioOutput
                             | ReleaseVideoOutput | ReleaseAudioProbe
                             | ReleaseVideoProbe
    };

    enum Event
    {
        FinalizedLoad = QEvent::User,
        Error,
        RateChange,
        Started,
        Paused,
        DurationChange,
        StatusChange,
        EndOfMedia,
        PositionChange
    };

    enum GraphStatus
    {
        NoMedia,
        Loading,
        Loaded,
        InvalidMedia
    };

    DirectShowPlayerControl *m_playerControl = nullptr;
    DirectShowMetaDataControl *m_metaDataControl = nullptr;
    DirectShowVideoRendererControl *m_videoRendererControl = nullptr;
    QVideoWindowControl *m_videoWindowControl = nullptr;
    DirectShowAudioEndpointControl *m_audioEndpointControl = nullptr;
    DirectShowAudioProbeControl *m_audioProbeControl = nullptr;
    DirectShowVideoProbeControl *m_videoProbeControl = nullptr;
    DirectShowSampleGrabber *m_audioSampleGrabber = nullptr;
    DirectShowSampleGrabber *m_videoSampleGrabber = nullptr;

    QThread *m_taskThread = nullptr;
    DirectShowEventLoop *m_loop;
    int m_pendingTasks = 0;
    int m_executingTask = 0;
    int m_executedTasks = 0;
    int m_streamTypes = 0;
    HANDLE m_taskHandle;
    HANDLE m_eventHandle = nullptr;
    GraphStatus m_graphStatus = NoMedia;
    QMediaPlayer::Error m_error = QMediaPlayer::NoError;
    QIODevice *m_stream = nullptr;
    IFilterGraph2 *m_graph = nullptr;
    ICaptureGraphBuilder2 *m_graphBuilder = nullptr;
    IBaseFilter *m_source = nullptr;
    IBaseFilter *m_audioOutput = nullptr;
    IBaseFilter *m_videoOutput = nullptr;
    qreal m_rate = 1;
    qint64 m_position = 0;
    qint64 m_seekPosition = -1;
    qint64 m_duration = 0;
    QMediaTimeRange m_playbackRange;
    QUrl m_url;
    QString m_errorString;
    QMutex m_mutex;
    bool m_buffering = false;
    bool m_seekable = false;
    bool m_atEnd = false;
    bool m_dontCacheNextSeekResult = false;
    QVariantMap m_metadata;

    friend class DirectShowPlayerServiceThread;
};

QT_END_NAMESPACE

#endif
