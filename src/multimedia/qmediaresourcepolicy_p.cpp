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

#include "qmediaresourcepolicy_p.h"
#include "qmediapluginloader_p.h"
#include "qmediaresourcepolicyplugin_p.h"
#include "qmediaresourceset_p.h"

namespace {
    class QDummyMediaPlayerResourceSet : public QMediaPlayerResourceSetInterface
    {
    public:
        QDummyMediaPlayerResourceSet(QObject *parent)
            : QMediaPlayerResourceSetInterface(parent)
        {
        }

        bool isVideoEnabled() const override
        {
            return true;
        }

        bool isGranted() const override
        {
            return true;
        }

        bool isAvailable() const override
        {
            return true;
        }

        void acquire() override {}
        void release() override {}
        void setVideoEnabled(bool) override {}
    };
}

QT_BEGIN_NAMESPACE

Q_GLOBAL_STATIC_WITH_ARGS(QMediaPluginLoader, resourcePolicyLoader,
        (QMediaResourceSetFactoryInterface_iid, QLatin1String("resourcepolicy"), Qt::CaseInsensitive))

Q_GLOBAL_STATIC(QObject, dummyRoot)

QObject* QMediaResourcePolicy::createResourceSet(const QString& interfaceId)
{
    QMediaResourceSetFactoryInterface *factory =
            qobject_cast<QMediaResourceSetFactoryInterface*>(resourcePolicyLoader()
                                                             ->instance(QLatin1String("default")));
    QObject *obj = nullptr;
    if (factory)
        obj = factory->create(interfaceId);

    if (!obj) {
        if (interfaceId == QLatin1String(QMediaPlayerResourceSetInterface_iid)) {
            obj = new QDummyMediaPlayerResourceSet(dummyRoot());
        }
    }
    Q_ASSERT(obj);
    return obj;
}

void QMediaResourcePolicy::destroyResourceSet(QObject* resourceSet)
{
    if (resourceSet->parent() == dummyRoot()) {
        delete resourceSet;
        return;
    }
    QMediaResourceSetFactoryInterface *factory =
            qobject_cast<QMediaResourceSetFactoryInterface*>(resourcePolicyLoader()
                                                             ->instance(QLatin1String("default")));
    Q_ASSERT(factory);
    if (!factory)
        return;
    return factory->destroy(resourceSet);
}
QT_END_NAMESPACE
