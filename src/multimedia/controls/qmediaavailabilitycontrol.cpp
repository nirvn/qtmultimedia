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

#include "qmediaavailabilitycontrol.h"
#include "qmediacontrol_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QMediaAvailabilityControl
    \obsolete

    \brief The QMediaAvailabilityControl class supplies a control for reporting availability of a service.

    \inmodule QtMultimedia


    \ingroup multimedia_control

    An instance of QMediaObject (or its derived classes) can report any changes
    in availability via this control.

    The interface name of QMediaAvailabilityControl is \c org.qt-project.qt.mediaavailabilitycontrol/5.0 as
    defined in QMediaAvailabilityControl_iid.

    \sa QMediaService::requestControl(), QMediaObject
*/

/*!
    \macro QMediaAvailabilityControl_iid

    \c org.qt-project.qt.mediaavailabilitycontrol/5.0

    Defines the interface name of the QMediaAvailabilityControl class.

    \relates QMediaAvailabilityControl
*/

/*!
    Constructs an availability control object with \a parent.
*/
QMediaAvailabilityControl::QMediaAvailabilityControl(QObject *parent)
    : QMediaControl(*new QMediaControlPrivate, parent)
{
}

/*!
    Destruct the availability control object.
*/
QMediaAvailabilityControl::~QMediaAvailabilityControl()
{
}


/*!
  \fn QMultimedia::AvailabilityStatus QMediaAvailabilityControl::availability() const

  Returns the current availability of this instance of the media service.
  If the availability changes at run time (for example, some other media
  client takes all media resources) the availabilityChanges() signal
  should be emitted.
*/


/*!
    \fn void QMediaAvailabilityControl::availabilityChanged(QMultimedia::AvailabilityStatus availability)

    Signal emitted when the current \a availability value changed.
*/

QT_END_NAMESPACE

#include "moc_qmediaavailabilitycontrol.cpp"
