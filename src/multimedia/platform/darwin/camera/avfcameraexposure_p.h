/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef AVFCAMERAEXPOSURE_H
#define AVFCAMERAEXPOSURE_H

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

#include <private/qplatformcameraexposure_p.h>
#include <QtMultimedia/qcamera.h>

#include <QtCore/qglobal.h>

QT_BEGIN_NAMESPACE

class AVFCamera;

class AVFCameraExposure : public QPlatformCameraExposure
{
    Q_OBJECT

public:
    AVFCameraExposure(AVFCamera *camera);

    bool isParameterSupported(ExposureParameter parameter) const override;
    QVariantList supportedParameterRange(ExposureParameter parameter,
                                         bool *continuous) const override;

    QVariant requestedValue(ExposureParameter parameter) const override;
    QVariant actualValue(ExposureParameter parameter) const override;
    bool setValue(ExposureParameter parameter, const QVariant &value) override;

    QCamera::FlashMode flashMode() const override;
    void setFlashMode(QCamera::FlashMode mode) override;
    bool isFlashModeSupported(QCamera::FlashMode mode) const override;
    bool isFlashReady() const override;

    QCamera::TorchMode torchMode() const override;
    void setTorchMode(QCamera::TorchMode mode) override;
    bool isTorchModeSupported(QCamera::TorchMode mode) const override;

private Q_SLOTS:
    void cameraActiveChanged(bool active);

private:
    void applyFlashSettings();

    AVFCamera *m_camera;

    QVariant m_requestedMode;
    QVariant m_requestedCompensation;
    QVariant m_requestedShutterSpeed;
    QVariant m_requestedISO;

    // Aux. setters:
    bool setExposureMode(const QVariant &value);
    bool setExposureCompensation(const QVariant &value);
    bool setShutterSpeed(const QVariant &value);
    bool setISO(const QVariant &value);

    // Set of bits:
    bool isFlashSupported = false;
    bool isFlashAutoSupported = false;
    bool isTorchSupported = false;
    bool isTorchAutoSupported = false;
    QCamera::FlashMode m_flashMode = QCamera::FlashOff;
    QCamera::TorchMode m_torchMode = QCamera::TorchOff;
};

QT_END_NAMESPACE

#endif
