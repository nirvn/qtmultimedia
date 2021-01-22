/****************************************************************************
**
** Copyright (C) 2017 QNX Software Systems. All rights reserved.
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
#include "mmrendereraudiorolecontrol.h"
#include "mmrendererutil.h"

QT_BEGIN_NAMESPACE

MmRendererAudioRoleControl::MmRendererAudioRoleControl(QObject *parent)
    : QAudioRoleControl(parent)
    , m_role(QAudio::UnknownRole)
{
}

QAudio::Role MmRendererAudioRoleControl::audioRole() const
{
    return m_role;
}

void MmRendererAudioRoleControl::setAudioRole(QAudio::Role role)
{
    if (m_role != role) {
        m_role = role;
        emit audioRoleChanged(m_role);
    }
}

QList<QAudio::Role> MmRendererAudioRoleControl::supportedAudioRoles() const
{
    return qnxSupportedAudioRoles();
}

QT_END_NAMESPACE
