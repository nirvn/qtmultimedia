/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd and/or its subsidiary(-ies).
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

#include "avfvideowidget.h"

#import <AVFoundation/AVFoundation.h>
#import <QuartzCore/CATransaction.h>

#if defined(Q_OS_MACOS)
#import <AppKit/AppKit.h>
#else
#import <UIKit/UIKit.h>
#endif

#include <QtCore/QDebug>
#include <QtGui/QResizeEvent>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>

QT_USE_NAMESPACE

AVFVideoWidget::AVFVideoWidget(QWidget *parent)
    : QWidget(parent)
    , m_aspectRatioMode(Qt::KeepAspectRatio)
    , m_playerLayer(nullptr)
    , m_nativeView(nullptr)
{
    setAutoFillBackground(false);
}

AVFVideoWidget::~AVFVideoWidget()
{
#ifdef QT_DEBUG_AVF
    qDebug() << Q_FUNC_INFO;
#endif

    if (m_playerLayer) {
        [m_playerLayer removeFromSuperlayer];
        [m_playerLayer release];
    }
}

QSize AVFVideoWidget::sizeHint() const
{
    return m_nativeSize;
}

Qt::AspectRatioMode AVFVideoWidget::aspectRatioMode() const
{
    return m_aspectRatioMode;
}

void AVFVideoWidget::setAspectRatioMode(Qt::AspectRatioMode mode)
{
    if (m_aspectRatioMode != mode) {
        m_aspectRatioMode = mode;

        updateAspectRatio();
    }
}

void AVFVideoWidget::setPlayerLayer(AVPlayerLayer *layer)
{
    if (m_playerLayer == layer)
        return;

    if (!m_nativeView) {
        //make video widget a native window
#if defined(Q_OS_OSX)
        m_nativeView = (NSView*)this->winId();
        [m_nativeView setWantsLayer:YES];
#else
        m_nativeView = (UIView*)this->winId();
#endif
    }

    if (m_playerLayer) {
        [m_playerLayer removeFromSuperlayer];
        [m_playerLayer release];
    }

    m_playerLayer = layer;

    CALayer *nativeLayer = [m_nativeView layer];

    if (layer) {
        [layer retain];

        m_nativeSize = QSize(m_playerLayer.bounds.size.width,
                             m_playerLayer.bounds.size.height);

        updateAspectRatio();
        [nativeLayer addSublayer:m_playerLayer];
        updatePlayerLayerBounds(this->size());
    }
#ifdef QT_DEBUG_AVF
    NSArray *sublayers = [nativeLayer sublayers];
    qDebug() << "playerlayer: " << "at z:" << [m_playerLayer zPosition]
                << " frame: " << m_playerLayer.frame.size.width << "x"  << m_playerLayer.frame.size.height;
    qDebug() << "superlayer: " << "at z:" << [nativeLayer zPosition]
                << " frame: " << nativeLayer.frame.size.width << "x"  << nativeLayer.frame.size.height;
    int i = 0;
    for (CALayer *layer in sublayers) {
        qDebug() << "layer " << i << ": at z:" << [layer zPosition]
                    << " frame: " << layer.frame.size.width << "x"  << layer.frame.size.height;
        i++;
    }
#endif

}

void AVFVideoWidget::resizeEvent(QResizeEvent *event)
{
    updatePlayerLayerBounds(event->size());
    QWidget::resizeEvent(event);
}

void AVFVideoWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);

    QWidget::paintEvent(event);
}

void AVFVideoWidget::updateAspectRatio()
{
    if (m_playerLayer) {
        switch (m_aspectRatioMode) {
        case Qt::IgnoreAspectRatio:
            [m_playerLayer setVideoGravity:AVLayerVideoGravityResize];
            break;
        case Qt::KeepAspectRatio:
            [m_playerLayer setVideoGravity:AVLayerVideoGravityResizeAspect];
            break;
        case Qt::KeepAspectRatioByExpanding:
            [m_playerLayer setVideoGravity:AVLayerVideoGravityResizeAspectFill];
            break;
        default:
            break;
        }
    }
}

void AVFVideoWidget::updatePlayerLayerBounds(const QSize &size)
{
    [CATransaction begin];
    [CATransaction setDisableActions: YES]; // disable animation/flicks
    m_playerLayer.bounds = QRect(QPoint(0, 0), size).toCGRect();
    [CATransaction commit];
}
