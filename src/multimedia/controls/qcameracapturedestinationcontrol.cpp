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

#include <qcameracapturedestinationcontrol.h>
#include <QtCore/qstringlist.h>

QT_BEGIN_NAMESPACE

/*!
    \class QCameraCaptureDestinationControl
    \obsolete

    \brief The QCameraCaptureDestinationControl class provides a control for setting capture destination.

    Depending on backend capabilities capture to file, buffer or both can be supported.

    \inmodule QtMultimedia

    \ingroup multimedia_control

    The interface name of QCameraCaptureDestinationControl is \c org.qt-project.qt.cameracapturedestinationcontrol/5.0 as
    defined in QCameraCaptureDestinationControl_iid.


    \sa QMediaService::requestControl()
*/

/*!
    \macro QCameraCaptureDestinationControl_iid

    \c org.qt-project.qt.cameracapturedestinationcontrol/5.0

    Defines the interface name of the QCameraCaptureDestinationControl class.

    \relates QCameraCaptureDestinationControl
*/

/*!
    Constructs a new image capture destination control object with the given \a parent
*/
QCameraCaptureDestinationControl::QCameraCaptureDestinationControl(QObject *parent)
    :QMediaControl(parent)
{
}

/*!
    Destroys an image capture destination control.
*/
QCameraCaptureDestinationControl::~QCameraCaptureDestinationControl()
{
}

/*!
    \fn QCameraCaptureDestinationControl::isCaptureDestinationSupported(QCameraImageCapture::CaptureDestinations destination) const

    Returns true if the capture \a destination is supported; and false if it is not.
*/

/*!
    \fn QCameraCaptureDestinationControl::captureDestination() const

    Returns the current capture destination. The default destination is QCameraImageCapture::CaptureToFile.
*/

/*!
    \fn QCameraCaptureDestinationControl::setCaptureDestination(QCameraImageCapture::CaptureDestinations destination)

    Sets the capture \a destination.
*/

/*!
    \fn QCameraCaptureDestinationControl::captureDestinationChanged(QCameraImageCapture::CaptureDestinations destination)

    Signals the image capture \a destination changed.
*/

QT_END_NAMESPACE

#include "moc_qcameracapturedestinationcontrol.cpp"
