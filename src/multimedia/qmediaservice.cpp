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

#include "qmediaservice.h"
#include "qmediaservice_p.h"

#include <QtCore/qtimer.h>



QT_BEGIN_NAMESPACE


/*!
    \class QMediaService
    \obsolete
    \brief The QMediaService class provides a common base class for media
    service implementations.
    \ingroup multimedia
    \ingroup multimedia_control
    \ingroup multimedia_core
    \inmodule QtMultimedia

    Media services provide implementations of the functionality promised
    by media objects, and allow multiple providers to implement a QMediaObject.

    To provide the functionality of a QMediaObject media services implement
    QMediaControl interfaces.  Services typically implement one core media
    control which provides the core feature of a media object, and some
    number of additional controls which provide either optional features of
    the media object, or features of a secondary media object or peripheral
    object.

    A pointer to media service's QMediaControl implementation can be obtained
    by passing the control's interface name to the requestControl() function.

    \snippet multimedia-snippets/media.cpp Request control

    Media objects can use services loaded dynamically from plug-ins or
    implemented statically within an applications.  Plug-in based services
    should also implement the QMediaServiceProviderPlugin interface.  Static
    services should implement the QMediaServiceProvider interface.  In general,
    implementing a QMediaService is outside of the scope of this documentation
    and support on the relevant mailing lists or IRC channels should be sought.

    \sa QMediaObject, QMediaControl
*/

/*!
    Construct a media service with the given \a parent. This class is meant as a
    base class for Multimedia services so this constructor is protected.
*/

QMediaService::QMediaService(QObject *parent)
    : QObject(*new QMediaServicePrivate, parent)
{
}

/*!
    \internal
*/
QMediaService::QMediaService(QMediaServicePrivate &dd, QObject *parent)
    : QObject(dd, parent)
{
}

/*!
    Destroys a media service.
*/

QMediaService::~QMediaService()
{
}

/*!
    \fn QMediaControl* QMediaService::requestControl(const char *interface)

    Returns a pointer to the media control implementing \a interface.

    If the service does not implement the control, or if it is unavailable a
    null pointer is returned instead.

    Controls must be returned to the service when no longer needed using the
    releaseControl() function.
*/

/*!
    \fn T QMediaService::requestControl()

    Returns a pointer to the media control of type T implemented by a media service.

    If the service does not implement the control, or if it is unavailable a
    null pointer is returned instead.

    Controls must be returned to the service when no longer needed using the
    releaseControl() function.
*/

/*!
    \fn void QMediaService::releaseControl(QMediaControl *control);

    Releases a \a control back to the service.
*/

QT_END_NAMESPACE

#include "moc_qmediaservice.cpp"
