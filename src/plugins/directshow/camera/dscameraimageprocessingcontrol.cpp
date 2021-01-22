/****************************************************************************
**
** Copyright (C) 2016 Denis Shienkov <denis.shienkov@gmail.com>
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

#include "dscameraimageprocessingcontrol.h"
#include "dscamerasession.h"

QT_BEGIN_NAMESPACE

DSCameraImageProcessingControl::DSCameraImageProcessingControl(DSCameraSession *session)
    : QCameraImageProcessingControl(session)
    , m_session(session)
{
}

DSCameraImageProcessingControl::~DSCameraImageProcessingControl() = default;

bool DSCameraImageProcessingControl::isParameterSupported(
        QCameraImageProcessingControl::ProcessingParameter parameter) const
{
    return m_session->isImageProcessingParameterSupported(parameter);
}

bool DSCameraImageProcessingControl::isParameterValueSupported(
        QCameraImageProcessingControl::ProcessingParameter parameter,
        const QVariant &value) const
{
    return m_session->isImageProcessingParameterValueSupported(parameter, value);
}

QVariant DSCameraImageProcessingControl::parameter(
        QCameraImageProcessingControl::ProcessingParameter parameter) const
{
    return m_session->imageProcessingParameter(parameter);
}

void DSCameraImageProcessingControl::setParameter(QCameraImageProcessingControl::ProcessingParameter parameter,
                                                  const QVariant &value)
{
    m_session->setImageProcessingParameter(parameter, value);
}

QT_END_NAMESPACE
