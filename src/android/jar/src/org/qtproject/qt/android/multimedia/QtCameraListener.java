// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

package org.qtproject.qt.android.multimedia;

import android.hardware.Camera;
import android.hardware.Camera.CameraInfo;

import android.graphics.ImageFormat;
import android.graphics.SurfaceTexture;
import android.util.Log;
import java.lang.Math;

public class QtCameraListener implements Camera.ShutterCallback,
                                         Camera.PictureCallback,
                                         Camera.AutoFocusCallback,
                                         Camera.PreviewCallback
{
    private static final String TAG = "Qt Camera";

    private static final int BUFFER_POOL_SIZE = 2;

    private int m_cameraId = -1;

    private boolean m_notifyNewFrames = false;
    private boolean m_notifyWhenFrameAvailable = false;
    private byte[][] m_previewBuffers = null;
    private byte[] m_lastPreviewBuffer = null;
    private Camera.Size m_previewSize = null;
    private int m_previewFormat = ImageFormat.NV21; // Default preview format on all devices
    private int m_previewBytesPerLine = -1;

    private QtCameraListener(int id)
    {
        m_cameraId = id;
    }

    public void notifyNewFrames(boolean notify)
    {
        m_notifyNewFrames = notify;
    }

    public void notifyWhenFrameAvailable(boolean notify)
    {
        m_notifyWhenFrameAvailable = notify;
    }

    public byte[] lastPreviewBuffer()
    {
        return m_lastPreviewBuffer;
    }

    public int previewWidth()
    {
        if (m_previewSize == null)
            return -1;

        return m_previewSize.width;
    }

    public int previewHeight()
    {
        if (m_previewSize == null)
            return -1;

        return m_previewSize.height;
    }

    public int previewFormat()
    {
        return m_previewFormat;
    }

    public int previewBytesPerLine()
    {
        return m_previewBytesPerLine;
    }

    public void clearPreviewCallback(Camera camera)
    {
        camera.setPreviewCallbackWithBuffer(null);
    }

    public void setupPreviewCallback(Camera camera)
    {
        // Clear previous callback (also clears added buffers)
        clearPreviewCallback(camera);
        m_lastPreviewBuffer = null;

        final Camera.Parameters params = camera.getParameters();
        m_previewSize = params.getPreviewSize();
        m_previewFormat = params.getPreviewFormat();

        int bufferSizeNeeded = 0;
        if (m_previewFormat == ImageFormat.YV12) {
            // For YV12, bytes per line must be a multiple of 16
            final int yStride = (int) Math.ceil(m_previewSize.width / 16.0) * 16;
            final int uvStride = (int) Math.ceil((yStride / 2) / 16.0) * 16;
            final int ySize = yStride * m_previewSize.height;
            final int uvSize = uvStride * m_previewSize.height / 2;
            bufferSizeNeeded = ySize + uvSize * 2;

            m_previewBytesPerLine = yStride;

        } else {
            double bytesPerPixel = ImageFormat.getBitsPerPixel(m_previewFormat) / 8.0;
            bufferSizeNeeded = (int) Math.ceil(bytesPerPixel * m_previewSize.width * m_previewSize.height);

            // bytes per line are calculated only for the first plane
            switch (m_previewFormat) {
            case ImageFormat.NV21:
                m_previewBytesPerLine = m_previewSize.width; // 1 byte per sample and tightly packed
                break;
            case ImageFormat.RGB_565:
            case ImageFormat.YUY2:
                m_previewBytesPerLine = m_previewSize.width * 2; // 2 bytes per pixel
                break;
            default:
                m_previewBytesPerLine = -1;
                break;
            }
        }

        // We could keep the same buffers when they are already bigger than the required size
        // but the Android doc says the size must match, so in doubt just replace them.
        if (m_previewBuffers == null || m_previewBuffers[0].length != bufferSizeNeeded)
            m_previewBuffers = new byte[BUFFER_POOL_SIZE][bufferSizeNeeded];

        // Add callback and queue all buffers
        camera.setPreviewCallbackWithBuffer(this);
        for (byte[] buffer : m_previewBuffers)
            camera.addCallbackBuffer(buffer);
    }

    @Override
    public void onPreviewFrame(byte[] data, Camera camera)
    {
        // Re-enqueue the last buffer
        if (m_lastPreviewBuffer != null)
            camera.addCallbackBuffer(m_lastPreviewBuffer);

        m_lastPreviewBuffer = data;

        if (data != null) {
            if (m_notifyWhenFrameAvailable) {
                m_notifyWhenFrameAvailable = false;
                notifyFrameAvailable(m_cameraId);
            }
            if (m_notifyNewFrames) {
                notifyNewPreviewFrame(m_cameraId, data,
                                      m_previewSize.width, m_previewSize.height,
                                      m_previewFormat,
                                      m_previewBytesPerLine);
            }
        }
    }

    @Override
    public void onShutter()
    {
        notifyPictureExposed(m_cameraId);
    }

    @Override
    public void onPictureTaken(byte[] data, Camera camera)
    {
        notifyPictureCaptured(m_cameraId, data);
    }

    @Override
    public void onAutoFocus(boolean success, Camera camera)
    {
        notifyAutoFocusComplete(m_cameraId, success);
    }

    private static native void notifyAutoFocusComplete(int id, boolean success);
    private static native void notifyPictureExposed(int id);
    private static native void notifyPictureCaptured(int id, byte[] data);
    private static native void notifyNewPreviewFrame(int id, byte[] data, int width, int height,
                                                     int pixelFormat, int bytesPerLine);
    private static native void notifyFrameAvailable(int id);
}
