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

#ifndef QRADIODATACONTROL_H
#define QRADIODATACONTROL_H

#include <QtMultimedia/qmediacontrol.h>
#include <QtMultimedia/qradiodata.h>

QT_BEGIN_NAMESPACE

// Required for QDoc workaround
class QString;

class Q_MULTIMEDIA_EXPORT QRadioDataControl : public QMediaControl
{
    Q_OBJECT

public:
    ~QRadioDataControl();

    virtual QString stationId() const = 0;
    virtual QRadioData::ProgramType programType() const = 0;
    virtual QString programTypeName() const = 0;
    virtual QString stationName() const = 0;
    virtual QString radioText() const = 0;
    virtual void setAlternativeFrequenciesEnabled(bool enabled) = 0;
    virtual bool isAlternativeFrequenciesEnabled() const = 0;

    virtual QRadioData::Error error() const = 0;
    virtual QString errorString() const = 0;

Q_SIGNALS:
    void stationIdChanged(QString stationId);
    void programTypeChanged(QRadioData::ProgramType programType);
    void programTypeNameChanged(QString programTypeName);
    void stationNameChanged(QString stationName);
    void radioTextChanged(QString radioText);
    void alternativeFrequenciesEnabledChanged(bool enabled);
    void error(QRadioData::Error err);

protected:
    explicit QRadioDataControl(QObject *parent = nullptr);
};

#define QRadioDataControl_iid "org.qt-project.qt.radiodatacontrol/5.0"
Q_MEDIA_DECLARE_CONTROL(QRadioDataControl, QRadioDataControl_iid)

QT_END_NAMESPACE


#endif  // QRADIODATACONTROL_H
