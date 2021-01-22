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

#ifndef QANDROIDMEDIACONTAINERCONTROL_H
#define QANDROIDMEDIACONTAINERCONTROL_H

#include <qmediacontainercontrol.h>

QT_BEGIN_NAMESPACE

class QAndroidCaptureSession;

class QAndroidMediaContainerControl : public QMediaContainerControl
{
    Q_OBJECT
public:
    QAndroidMediaContainerControl(QAndroidCaptureSession *session);

    QStringList supportedContainers() const override;
    QString containerFormat() const override;
    void setContainerFormat(const QString &format) override;
    QString containerDescription(const QString &formatMimeType) const override;

private:
    QAndroidCaptureSession *m_session;
};

QT_END_NAMESPACE

#endif // QANDROIDMEDIACONTAINERCONTROL_H
