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

#include "qvideoframeconversionhelper_p.h"

#ifdef QT_COMPILER_SUPPORTS_AVX2

QT_BEGIN_NAMESPACE

void QT_FASTCALL qt_convert_BGRA32_to_ARGB32_avx2(const QVideoFrame &frame, uchar *output)
{
    FETCH_INFO_PACKED(frame)
    MERGE_LOOPS(width, height, stride, 4)
    quint32 *argb = reinterpret_cast<quint32*>(output);

    const __m256i shuffleMask = _mm256_set_epi8(12, 13, 14, 15, 8, 9, 10, 11, 4, 5, 6, 7, 0, 1, 2, 3,
                                                12, 13, 14, 15, 8, 9, 10, 11, 4, 5, 6, 7, 0, 1, 2, 3);

    for (int y = 0; y < height; ++y) {
        const quint32 *bgra = reinterpret_cast<const quint32*>(src);

        int x = 0;
        ALIGN(32, argb, x, width) {
            *argb = qConvertBGRA32ToARGB32(*bgra);
            ++bgra;
            ++argb;
        }

        for (; x < width - 15; x += 16) {
            __m256i pixelData = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(bgra));
            __m256i pixelData2 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(bgra + 8));
            bgra += 16;
            pixelData = _mm256_shuffle_epi8(pixelData, shuffleMask);
            pixelData2 = _mm256_shuffle_epi8(pixelData2, shuffleMask);
            _mm256_store_si256(reinterpret_cast<__m256i*>(argb), pixelData);
            _mm256_store_si256(reinterpret_cast<__m256i*>(argb + 8), pixelData2);
            argb += 16;
        }

        // leftovers
        for (; x < width; ++x) {
            *argb = qConvertBGRA32ToARGB32(*bgra);
            ++bgra;
            ++argb;
        }

        src += stride;
    }
}

QT_END_NAMESPACE

#endif
