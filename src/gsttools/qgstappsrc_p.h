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

#ifndef QGSTAPPSRC_H
#define QGSTAPPSRC_H

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
#include <QtCore/qobject.h>
#include <QtCore/qiodevice.h>

#include <gst/gst.h>
#include <gst/app/gstappsrc.h>

#if GST_VERSION_MAJOR < 1
#include <gst/app/gstappbuffer.h>
#endif

QT_BEGIN_NAMESPACE

class Q_GSTTOOLS_EXPORT QGstAppSrc  : public QObject
{
    Q_OBJECT
public:
    QGstAppSrc(QObject *parent = 0);
    ~QGstAppSrc();

    bool setup(GstElement *);

    void setStream(QIODevice *);
    QIODevice *stream() const;

    GstAppSrc *element();

    qint64 queueSize() const { return m_maxBytes; }

    bool& enoughData() { return m_enoughData; }
    bool& dataRequested() { return m_dataRequested; }
    unsigned int& dataRequestSize() { return m_dataRequestSize; }

    bool isStreamValid() const
    {
        return m_stream != 0 &&
               m_stream->isOpen();
    }

private slots:
    void pushDataToAppSrc();
    bool doSeek(qint64);
    void onDataReady();

    void streamDestroyed();
private:
    static gboolean on_seek_data(GstAppSrc *element, guint64 arg0, gpointer userdata);
    static void on_enough_data(GstAppSrc *element, gpointer userdata);
    static void on_need_data(GstAppSrc *element, uint arg0, gpointer userdata);
    static void destroy_notify(gpointer data);

    void sendEOS();

    QIODevice *m_stream = nullptr;
    GstAppSrc *m_appSrc = nullptr;
    bool m_sequential = false;
    GstAppStreamType m_streamType = GST_APP_STREAM_TYPE_RANDOM_ACCESS;
    GstAppSrcCallbacks m_callbacks;
    qint64 m_maxBytes = 0;
    unsigned int m_dataRequestSize = ~0;
    bool m_dataRequested = false;
    bool m_enoughData = false;
    bool m_forceData = false;
};

QT_END_NAMESPACE

#endif
