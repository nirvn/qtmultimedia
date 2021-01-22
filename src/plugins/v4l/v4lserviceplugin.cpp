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

#include <QtCore/qstring.h>
#include <QtCore/qfile.h>
#include <QtCore/qdebug.h>
#include <QtCore/qdir.h>

#include "v4lserviceplugin.h"
#include "v4lradioservice.h"

#include <qmediaserviceprovider.h>


QMediaService* V4LServicePlugin::create(QString const& key)
{
    if (key == QLatin1String(Q_MEDIASERVICE_RADIO))
        return new V4LRadioService;

    return 0;
}

void V4LServicePlugin::release(QMediaService *service)
{
    delete service;
}

QList<QByteArray> V4LServicePlugin::devices(const QByteArray &service) const
{
    return QList<QByteArray>();
}

QString V4LServicePlugin::deviceDescription(const QByteArray &service, const QByteArray &device)
{
    return QString();
}
