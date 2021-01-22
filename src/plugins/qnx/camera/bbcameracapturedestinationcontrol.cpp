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
#include "bbcameracapturedestinationcontrol.h"

#include "bbcamerasession.h"

QT_BEGIN_NAMESPACE

BbCameraCaptureDestinationControl::BbCameraCaptureDestinationControl(BbCameraSession *session, QObject *parent)
    : QCameraCaptureDestinationControl(parent)
    , m_session(session)
{
    connect(m_session, SIGNAL(captureDestinationChanged(QCameraImageCapture::CaptureDestinations)),
            this, SIGNAL(captureDestinationChanged(QCameraImageCapture::CaptureDestinations)));
}

bool BbCameraCaptureDestinationControl::isCaptureDestinationSupported(QCameraImageCapture::CaptureDestinations destination) const
{
    return m_session->isCaptureDestinationSupported(destination);
}

QCameraImageCapture::CaptureDestinations BbCameraCaptureDestinationControl::captureDestination() const
{
    return m_session->captureDestination();;
}

void BbCameraCaptureDestinationControl::setCaptureDestination(QCameraImageCapture::CaptureDestinations destination)
{
    m_session->setCaptureDestination(destination);
}

QT_END_NAMESPACE
