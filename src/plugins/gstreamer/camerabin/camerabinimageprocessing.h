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

#ifndef CAMERABINIMAGEPROCESSINGCONTROL_H
#define CAMERABINIMAGEPROCESSINGCONTROL_H

#include <QtMultimedia/private/qtmultimediaglobal_p.h>
#include <qcamera.h>
#include <qcameraimageprocessingcontrol.h>

#include <gst/gst.h>
#include <glib.h>

#if QT_CONFIG(gstreamer_photography)
# include <gst/interfaces/photography.h>
# if !GST_CHECK_VERSION(1,0,0)
typedef GstWhiteBalanceMode GstPhotographyWhiteBalanceMode;
typedef GstColourToneMode GstPhotographyColorToneMode;
# endif
#endif

QT_BEGIN_NAMESPACE

#if QT_CONFIG(linux_v4l)
class CameraBinV4LImageProcessing;
#endif

class CameraBinSession;

class CameraBinImageProcessing : public QCameraImageProcessingControl
{
    Q_OBJECT

public:
    CameraBinImageProcessing(CameraBinSession *session);
    virtual ~CameraBinImageProcessing();

    QCameraImageProcessing::WhiteBalanceMode whiteBalanceMode() const;
    bool setWhiteBalanceMode(QCameraImageProcessing::WhiteBalanceMode mode);
    bool isWhiteBalanceModeSupported(QCameraImageProcessing::WhiteBalanceMode mode) const;

    bool isParameterSupported(ProcessingParameter) const override;
    bool isParameterValueSupported(ProcessingParameter parameter, const QVariant &value) const override;
    QVariant parameter(ProcessingParameter parameter) const override;
    void setParameter(ProcessingParameter parameter, const QVariant &value) override;

#if QT_CONFIG(gstreamer_photography)
    void lockWhiteBalance();
    void unlockWhiteBalance();
#endif

private:
    bool setColorBalanceValue(const QString& channel, qreal value);
    void updateColorBalanceValues();

private:
    CameraBinSession *m_session;
    QMap<QCameraImageProcessingControl::ProcessingParameter, int> m_values;
#if QT_CONFIG(gstreamer_photography)
    QMap<GstPhotographyWhiteBalanceMode, QCameraImageProcessing::WhiteBalanceMode> m_mappedWbValues;
    QMap<QCameraImageProcessing::ColorFilter, GstPhotographyColorToneMode> m_filterMap;
#endif
    QCameraImageProcessing::WhiteBalanceMode m_whiteBalanceMode;

#if QT_CONFIG(linux_v4l)
    CameraBinV4LImageProcessing *m_v4lImageControl;
#endif
};

QT_END_NAMESPACE

#endif // CAMERABINIMAGEPROCESSINGCONTROL_H
