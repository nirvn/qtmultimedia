/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd and/or its subsidiary(-ies).
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

#ifndef QWINRTSERVICEPLUGIN_H
#define QWINRTSERVICEPLUGIN_H

#include <QtMultimedia/QMediaServiceProviderPlugin>

QT_BEGIN_NAMESPACE

class QWinRTServicePlugin : public QMediaServiceProviderPlugin
        , public QMediaServiceFeaturesInterface
        , public QMediaServiceCameraInfoInterface
        , public QMediaServiceSupportedDevicesInterface
        , public QMediaServiceDefaultDeviceInterface
{
    Q_OBJECT
    Q_INTERFACES(QMediaServiceFeaturesInterface)
    Q_INTERFACES(QMediaServiceCameraInfoInterface)
    Q_INTERFACES(QMediaServiceSupportedDevicesInterface)
    Q_INTERFACES(QMediaServiceDefaultDeviceInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.qt.mediaserviceproviderfactory/5.0" FILE "winrt.json")
public:
    QMediaService *create(QString const &key) override;
    void release(QMediaService *service) override;

    QMediaServiceProviderHint::Features supportedFeatures(const QByteArray &service) const override;

    QCamera::Position cameraPosition(const QByteArray &device) const override;
    int cameraOrientation(const QByteArray &device) const override;

    QList<QByteArray> devices(const QByteArray &service) const override;
    QString deviceDescription(const QByteArray &service, const QByteArray &device) override;

    QByteArray defaultDevice(const QByteArray &service) const override;
};

QT_END_NAMESPACE

#endif // QWINRTSERVICEPLUGIN_H
