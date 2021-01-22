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

#ifndef QCAMERAVIEWFINDERSETTINGS_H
#define QCAMERAVIEWFINDERSETTINGS_H

#include <QtMultimedia/qtmultimediaglobal.h>
#include <QtMultimedia/qvideoframe.h>

#include <QtCore/qshareddata.h>
#include <QtCore/qsize.h>

QT_BEGIN_NAMESPACE

class QCameraViewfinderSettingsPrivate;

class Q_MULTIMEDIA_EXPORT QCameraViewfinderSettings
{
public:
    QCameraViewfinderSettings();
    QCameraViewfinderSettings(const QCameraViewfinderSettings& other);

    ~QCameraViewfinderSettings();

    QCameraViewfinderSettings& operator=(const QCameraViewfinderSettings &other);
#ifdef Q_COMPILER_RVALUE_REFS
    QCameraViewfinderSettings &operator=(QCameraViewfinderSettings &&other) Q_DECL_NOTHROW
    { swap(other); return *this; }
#endif

    void swap(QCameraViewfinderSettings &other) Q_DECL_NOTHROW { d.swap(other.d); }

    friend Q_MULTIMEDIA_EXPORT bool operator==(const QCameraViewfinderSettings &lhs, const QCameraViewfinderSettings &rhs) Q_DECL_NOTHROW;
    bool isNull() const;

    QSize resolution() const;
    void setResolution(const QSize &);
    inline void setResolution(int width, int height)
    { setResolution(QSize(width, height)); }

    qreal minimumFrameRate() const;
    void setMinimumFrameRate(qreal rate);

    qreal maximumFrameRate() const;
    void setMaximumFrameRate(qreal rate);

    QVideoFrame::PixelFormat pixelFormat() const;
    void setPixelFormat(QVideoFrame::PixelFormat format);

    QSize pixelAspectRatio() const;
    void setPixelAspectRatio(const QSize &ratio);
    inline void setPixelAspectRatio(int horizontal, int vertical)
    { setPixelAspectRatio(QSize(horizontal, vertical)); }

private:
    QSharedDataPointer<QCameraViewfinderSettingsPrivate> d;
};
Q_DECLARE_SHARED(QCameraViewfinderSettings)

inline bool operator!=(const QCameraViewfinderSettings &lhs, const QCameraViewfinderSettings &rhs) Q_DECL_NOTHROW
{ return !operator==(lhs, rhs); }


QT_END_NAMESPACE

Q_DECLARE_METATYPE(QCameraViewfinderSettings)

#endif // QCAMERAVIEWFINDERSETTINGS_H
