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

#ifndef QMEDIARECORDER_P_H
#define QMEDIARECORDER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qmediarecorder.h"
#include "qmediaobject_p.h"
#include <QtCore/qurl.h>

QT_BEGIN_NAMESPACE

class QMediaRecorderControl;
class QMediaContainerControl;
class QAudioEncoderSettingsControl;
class QVideoEncoderSettingsControl;
class QMetaDataWriterControl;
class QMediaAvailabilityControl;
class QTimer;

class QMediaRecorderPrivate
{
    Q_DECLARE_NON_CONST_PUBLIC(QMediaRecorder)

public:
    QMediaRecorderPrivate();
    virtual ~QMediaRecorderPrivate() {}

    void applySettingsLater();
    void restartCamera();

    QMediaObject *mediaObject;

    QMediaRecorderControl *control;
    QMediaContainerControl *formatControl;
    QAudioEncoderSettingsControl *audioControl;
    QVideoEncoderSettingsControl *videoControl;
    QMetaDataWriterControl *metaDataControl;
    QMediaAvailabilityControl *availabilityControl;

    bool settingsChanged;

    QTimer* notifyTimer;

    QMediaRecorder::State state;
    QMediaRecorder::Error error;
    QString errorString;
    QUrl actualLocation;

    void _q_stateChanged(QMediaRecorder::State state);
    void _q_error(int error, const QString &errorString);
    void _q_serviceDestroyed();
    void _q_updateActualLocation(const QUrl &);
    void _q_notify();
    void _q_updateNotifyInterval(int ms);
    void _q_applySettings();
    void _q_availabilityChanged(QMultimedia::AvailabilityStatus availability);

    QMediaRecorder *q_ptr;
};

QT_END_NAMESPACE

#endif

