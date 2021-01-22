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

#ifndef QGSTREAMERVIDEOWIDGET_H
#define QGSTREAMERVIDEOWIDGET_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <private/qgsttools_global_p.h>
#include <qvideowidgetcontrol.h>

#include "qgstreamervideorendererinterface_p.h"
#include <private/qgstreamerbushelper_p.h>
#include <private/qgstreamervideooverlay_p.h>

QT_BEGIN_NAMESPACE

class Q_GSTTOOLS_EXPORT QGstreamerVideoWidget;

class Q_GSTTOOLS_EXPORT QGstreamerVideoWidgetControl
        : public QVideoWidgetControl
        , public QGstreamerVideoRendererInterface
        , public QGstreamerSyncMessageFilter
        , public QGstreamerBusMessageFilter
{
    Q_OBJECT
    Q_INTERFACES(QGstreamerVideoRendererInterface QGstreamerSyncMessageFilter QGstreamerBusMessageFilter)
public:
    explicit QGstreamerVideoWidgetControl(QObject *parent = 0, const QByteArray &elementName = QByteArray());
    virtual ~QGstreamerVideoWidgetControl();

    GstElement *videoSink() override;
    void setVideoSink(GstElement *) override;

    QWidget *videoWidget() override;

    void stopRenderer() override;

    Qt::AspectRatioMode aspectRatioMode() const override;
    void setAspectRatioMode(Qt::AspectRatioMode mode) override;

    bool isFullScreen() const override;
    void setFullScreen(bool fullScreen) override;

    int brightness() const override;
    void setBrightness(int brightness) override;

    int contrast() const override;
    void setContrast(int contrast) override;

    int hue() const override;
    void setHue(int hue) override;

    int saturation() const override;
    void setSaturation(int saturation) override;

    bool eventFilter(QObject *object, QEvent *event) override;

signals:
    void sinkChanged();
    void readyChanged(bool);

private Q_SLOTS:
    void onOverlayActiveChanged();
    void onNativeVideoSizeChanged();

private:
    void createVideoWidget();
    void updateWidgetAttributes();

    bool processSyncMessage(const QGstreamerMessage &message) override;
    bool processBusMessage(const QGstreamerMessage &message) override;

    QGstreamerVideoOverlay m_videoOverlay;
    QGstreamerVideoWidget *m_widget = nullptr;
    bool m_stopped = false;
    WId m_windowId = 0;
    bool m_fullScreen = false;
};

QT_END_NAMESPACE

#endif // QGSTREAMERVIDEOWIDGET_H
