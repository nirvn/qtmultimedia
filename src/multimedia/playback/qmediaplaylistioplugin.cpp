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

#include "qmediaplaylistioplugin_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QMediaPlaylistReader
    \internal

    \brief The QMediaPlaylistReader class provides an interface for reading a playlist file.
    \inmodule QtMultimedia

    \ingroup multimedia
    \ingroup multimedia_playback

    \sa QMediaPlaylistIOPlugin
*/

/*!
    Destroys a media playlist reader.
*/
QMediaPlaylistReader::~QMediaPlaylistReader()
{
}

/*!
    \fn QMediaPlaylistReader::atEnd() const

    Identifies if a playlist reader has reached the end of its input.

    Returns true if the reader has reached the end; and false otherwise.
*/

/*!
    \fn QMediaPlaylistReader::readItem()

    Reads an item of media from a playlist file.

    Returns the read media, or a null QMediaContent if no more media is available.
*/

/*!
    \fn QMediaPlaylistReader::close()

    Closes a playlist reader's input device.
*/

/*!
    \class QMediaPlaylistWriter
    \internal

    \brief The QMediaPlaylistWriter class provides an interface for writing a playlist file.

    \sa QMediaPlaylistIOPlugin
*/

/*!
    Destroys a media playlist writer.
*/
QMediaPlaylistWriter::~QMediaPlaylistWriter()
{
}

/*!
    \fn QMediaPlaylistWriter::writeItem(const QMediaContent &media)

    Writes an item of \a media to a playlist file.

    Returns true if the media was written successfully; and false otherwise.
*/

/*!
    \fn QMediaPlaylistWriter::close()

    Finalizes the writing of a playlist and closes the output device.
*/

/*!
    \class QMediaPlaylistIOPlugin
    \internal

    \brief The QMediaPlaylistIOPlugin class provides an interface for media playlist I/O plug-ins.
*/

/*!
    Constructs a media playlist I/O plug-in with the given \a parent.
*/
QMediaPlaylistIOPlugin::QMediaPlaylistIOPlugin(QObject *parent)
    :QObject(parent)
{
}

/*!
    Destroys a media playlist I/O plug-in.
*/
QMediaPlaylistIOPlugin::~QMediaPlaylistIOPlugin()
{
}

/*!
    \fn QMediaPlaylistIOPlugin::canRead(QIODevice *device, const QByteArray &format) const

    Identifies if plug-in can read \a format data from an I/O \a device.

    Returns true if the data can be read; and false otherwise.
*/

/*!
    \fn QMediaPlaylistIOPlugin::canRead(const QUrl& location, const QByteArray &format) const

    Identifies if a plug-in can read \a format data from a URL \a location.

    Returns true if the data can be read; and false otherwise.
*/

/*!
    \fn QMediaPlaylistIOPlugin::canWrite(QIODevice *device, const QByteArray &format) const

    Identifies if a plug-in can write \a format data to an I/O \a device.

    Returns true if the data can be written; and false otherwise.
*/

/*!
    \fn QMediaPlaylistIOPlugin::createReader(QIODevice *device, const QByteArray &format)

    Returns a new QMediaPlaylistReader which reads \a format data from an I/O \a device.

    If the device is invalid or the format is unsupported this will return a null pointer.
*/

/*!
    \fn QMediaPlaylistIOPlugin::createReader(const QUrl& location, const QByteArray &format)

    Returns a new QMediaPlaylistReader which reads \a format data from a URL \a location.

    If the location or the format is unsupported this will return a null pointer.
*/

/*!
    \fn QMediaPlaylistIOPlugin::createWriter(QIODevice *device, const QByteArray &format)

    Returns a new QMediaPlaylistWriter which writes \a format data to an I/O \a device.

    If the device is invalid or the format is unsupported this will return a null pointer.
*/

QT_END_NAMESPACE

#include "moc_qmediaplaylistioplugin_p.cpp"
