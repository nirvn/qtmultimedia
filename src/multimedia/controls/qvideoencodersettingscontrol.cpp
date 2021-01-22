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

#include "qvideoencodersettingscontrol.h"
#include <QtCore/qstringlist.h>
#include <QtCore/qvariant.h>

QT_BEGIN_NAMESPACE

/*!
    \class QVideoEncoderSettingsControl
    \obsolete

    \inmodule QtMultimedia


    \ingroup multimedia_control

    \brief The QVideoEncoderSettingsControl class provides access to the settings
    of a media service that performs video encoding.

    If a QMediaService supports encoding video data it will implement
    QVideoEncoderSettingsControl.  This control provides information about the limits
    of restricted video encoder options and allows the selection of a set of
    video encoder settings as specified in a QVideoEncoderSettings object.

    The functionality provided by this control is exposed to application code
    through the QMediaRecorder class.

    The interface name of QVideoEncoderSettingsControl is \c org.qt-project.qt.videoencodersettingscontrol/5.0 as
    defined in QVideoEncoderSettingsControl_iid.

    \sa QMediaRecorder, QVideoEncoderSettings, QMediaService::requestControl()
*/

/*!
    \macro QVideoEncoderSettingsControl_iid

    \c org.qt-project.qt.videoencodersettingscontrol/5.0

    Defines the interface name of the QVideoEncoderSettingsControl class.

    \relates QVideoEncoderSettingsControl
*/

/*!
    Create a new video encoder settings control object with the given \a parent.
*/
QVideoEncoderSettingsControl::QVideoEncoderSettingsControl(QObject *parent)
    :QMediaControl(parent)
{
}

/*!
    Destroys a video encoder settings control.
*/
QVideoEncoderSettingsControl::~QVideoEncoderSettingsControl()
{
}

/*!
    \fn QVideoEncoderSettingsControl::supportedVideoCodecs() const

    Returns the list of supported video codecs.
*/

/*!
    \fn QVideoEncoderSettingsControl::videoCodecDescription(const QString &codec) const

    Returns a description of a video \a codec.
*/

/*!
    \fn QVideoEncoderSettingsControl::supportedResolutions(const QVideoEncoderSettings &settings = QVideoEncoderSettings(),
                                                   bool *continuous = 0) const

    Returns a list of supported resolutions.

    If non null video \a settings parameter is passed,
    the returned list is reduced to resolution supported with partial settings like
    \l {QVideoEncoderSettings::setCodec()}{video codec} or
    \l {QVideoEncoderSettings::setFrameRate()}{frame rate} applied.

    If the encoder supports arbitrary resolutions within the supported resolutions range,
    *\a continuous is set to true, otherwise *\a continuous is set to false.

    \sa QVideoEncoderSettings::resolution()
*/

/*!
    \fn QVideoEncoderSettingsControl::supportedFrameRates(const QVideoEncoderSettings &settings = QVideoEncoderSettings(),
                                                  bool *continuous = 0) const

    Returns a list of supported frame rates.

    If non null video \a settings parameter is passed,
    the returned list is reduced to frame rates supported with partial settings like
    \l {QVideoEncoderSettings::setCodec()}{video codec} or
    \l {QVideoEncoderSettings::setResolution()}{video resolution} applied.

    If the encoder supports arbitrary frame rates within the supported range,
    *\a continuous is set to true, otherwise *\a continuous is set to false.

    \sa QVideoEncoderSettings::frameRate()
*/

/*!
    \fn QVideoEncoderSettingsControl::videoSettings() const

    Returns the video encoder settings.

    The returned value may be different tha passed to QVideoEncoderSettingsControl::setVideoSettings()
    if the settings contains the default or undefined parameters.
    In this case if the undefined parameters are already resolved, they should be returned.
*/

/*!
    \fn QVideoEncoderSettingsControl::setVideoSettings(const QVideoEncoderSettings &settings)

    Sets the selected video encoder \a settings.
*/

QT_END_NAMESPACE

#include "moc_qvideoencodersettingscontrol.cpp"
