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

#ifndef AVFVIDEOOUTPUT_H
#define AVFVIDEOOUTPUT_H

#include <QtCore/qobject.h>

QT_BEGIN_NAMESPACE

class AVFVideoOutput
{
public:
    virtual ~AVFVideoOutput() {}
    virtual void setLayer(void *playerLayer) = 0;
};

#define AVFVideoOutput_iid \
    "org.qt-project.qt.AVFVideoOutput/5.0"
Q_DECLARE_INTERFACE(AVFVideoOutput, AVFVideoOutput_iid)

QT_END_NAMESPACE

#endif // AVFVIDEOOUTPUT_H
