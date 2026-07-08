/*
MultiRecorder - communication with XIMEA cameras over http, fast recording, remote camera control, multiplatform

Copyright (c) 2023-2025 XIMEA s.r.o.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.
*/

#include "ImageExporter.h"

#if defined(_WIN32) && !_HAS_CXX17
#define USE_WIN32_FILEAPI
#endif

#ifdef USE_WIN32_FILEAPI
#include <windows.h>
#else
#include <filesystem>
#endif

#include <cstring>

#ifdef MULTIRECORDER_TIFF_ENABLE
#include <tiffio.h>
#endif

//----------------------------------------------------------------------------------------
bool CxImageExporter::createPath(const std::string &sPath)
{
#ifdef USE_WIN32_FILEAPI
    DWORD dwAttrib = GetFileAttributes(sPath.c_str());
    bool bPathExists = (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
    if (!bPathExists)
        return CreateDirectory(sPath.c_str(), nullptr);
#else
    std::filesystem::path aPath(sPath);
    if (!std::filesystem::exists(aPath))
    {
        std::error_code ec;
        return std::filesystem::create_directories(aPath, ec);
    }
#endif
    return true;
}

//----------------------------------------------------------------------------------------
std::string CxImageExporter::absolutePath(const std::string &sPath)
{
#ifdef USE_WIN32_FILEAPI
    char szAbsolutePath[1024];
    if (GetFullPathNameA(sPath.c_str(), sizeof(szAbsolutePath), szAbsolutePath, NULL) > 0)
        return szAbsolutePath;
#else
    std::filesystem::path aPath(sPath);
    if (!aPath.is_absolute())
    {
        std::error_code ec;
        auto aAbsPath = std::filesystem::absolute(aPath, ec);
        if (!ec)
            return aAbsPath.string();
    }
#endif
    return sPath;
}

//----------------------------------------------------------------------------------------
// taken from Samples/_libs/xiAPI/xiAPI_tiff.cpp
bool CxImageExporter::xiImageGetBitCount(XI_IMG* image, uint32_t* bit_count, uint32_t* num_channels, bool* is_rgb)
{
    switch (image->frm)
    {
    case XI_MONO8:
    case XI_RAW8:
        *bit_count = 8;
        *num_channels = 1;
        *is_rgb = false;
        return true;
    case XI_MONO16:
    case XI_RAW16:
        *bit_count = 16;
        *num_channels = 1;
        *is_rgb = false;
        return true;
    case XI_RGB24:
        *bit_count = 8;
        *num_channels = 3;
        *is_rgb = true;
        return true;
    case XI_RAW32:
        *bit_count = 32;
        *num_channels = 1;
        *is_rgb = false;
        return true;
    case XI_RGB32:
        *bit_count = 8;
        *num_channels = 4;
        *is_rgb = true;
        return true;
    case XI_RAW32FLOAT:
        *bit_count = 32;
        *num_channels = 1;
        *is_rgb = false;
        return true;
    case XI_RGB48:
        *bit_count = 16;
        *num_channels = 3;
        *is_rgb = true;
        return true;
    case XI_RGB64:
        *bit_count = 16;
        *num_channels = 4;
        *is_rgb = true;
        return true;
    case XI_RAW8X2:
        *bit_count = 8;
        *num_channels = 2;
        *is_rgb = false;
        return true;
    case XI_RAW8X3:
        *bit_count = 8;
        *num_channels = 3;
        *is_rgb = false;
        return true;
    case XI_RAW8X4:
        *bit_count = 8;
        *num_channels = 4;
        *is_rgb = false;
        return true;
    case XI_RAW16X2:
        *bit_count = 16;
        *num_channels = 2;
        *is_rgb = false;
        return true;
    case XI_RAW16X3:
        *bit_count = 16;
        *num_channels = 3;
        *is_rgb = false;
        return true;
    case XI_RAW16X4:
        *bit_count = 16;
        *num_channels = 4;
        *is_rgb = false;
        return true;
    case XI_FRM_TRANSPORT_DATA:
    case XI_RGB_PLANAR:
    case XI_RGB16_PLANAR:
        // unspported formats
        return false;
    }
    return false;
}

//---------------------------------------------------------------------------
void CxImageExporter::minMaxBpcVal(int64_t *piMin, int64_t *piMax, uint32_t uiBpc, bool bDataSigned)
{
    int64_t iMaxBpc = (1LL<<uiBpc);
    if (bDataSigned)
    {
        if (piMin != nullptr) *piMin = -iMaxBpc/2;
        if (piMax != nullptr) *piMax = iMaxBpc/2 - 1;
    }
    else
    {
        if (piMin != nullptr) *piMin = 0;
        if (piMax != nullptr) *piMax = iMaxBpc-1;
    }
}

//----------------------------------------------------------------------------------------
bool CxImageExporter::saveImageToFile(const std::string &sFilename, XI_IMG *pImg, bool bTiffEnable16bit, bool bTiffRescale16bit)
{
#ifdef MULTIRECORDER_TIFF_ENABLE
    uint32_t x, y, w, h;
    w = pImg->width;
    h = pImg->height;

    uint16_t uiDstCompression = COMPRESSION_NONE;
    bool bIsRGB = false;
    uint32_t uiSrcBits = 16;
    uint32_t uiDstComps = 1;
    if (!xiImageGetBitCount(pImg, &uiSrcBits, &uiDstComps, &bIsRGB))
        return false;

    uint32_t uiDstBits = uiSrcBits;
    uint32_t uiSrcDataBitDepth = uiSrcBits;     // real bit depth i.e. 1023 mean 10 bits (while uiSrcBits remains 16)
    int32_t iMaxVal = (int32_t)(pImg->data_saturation + 0.5);
    bool bNeeds8BitConversion = false;
    bool bNeeds16bitScale = false;
    if (uiSrcBits > 8 || pImg->data_sign_mode == XI_DATA_SM_SIGNED_2C)
    {
        // detect the data bit depth for rescaling
        for (int b = 8; b <= 16; b+=2)
        {
            if (iMaxVal < (1<<b))
            {
                uiSrcDataBitDepth = b;
                break;
            }
        }

        if (!bTiffEnable16bit && pImg->data_sign_mode != XI_DATA_SM_SIGNED_FLOATING)
        {
            uiDstBits = 8;
            iMaxVal = 255;
            bNeeds8BitConversion = true;
        }
        else if (bTiffRescale16bit && uiSrcBits == 16 && pImg->data_sign_mode == XI_DATA_SM_UNSIGNED)
        {
            iMaxVal = (1 << 16)-1;
            bNeeds16bitScale = true;
        }
        else if (uiSrcBits == 16 && pImg->data_sign_mode == XI_DATA_SM_UNSIGNED)
        {
            // just correct the bits to be even number
            iMaxVal = (1 << uiSrcDataBitDepth)-1;
        }
    }

    uint32_t uiPicStride = w * uiDstComps * (uint32_t)(uiSrcBits/8) + pImg->padding_x;

    if (bIsRGB && uiDstComps == 4)
        uiDstComps = 3;     // remove A
    uint32_t uiTIFFStride = w * uiDstComps * (uint32_t)(uiDstBits/8);

    TIFF *pTiff = TIFFOpen(sFilename.c_str(), "w");
    if (pTiff == nullptr)
        return false;

    TIFFSetField(pTiff, TIFFTAG_IMAGEWIDTH, w);
    TIFFSetField(pTiff, TIFFTAG_IMAGELENGTH, h);
    TIFFSetField(pTiff, TIFFTAG_SAMPLESPERPIXEL, uiDstComps);
    TIFFSetField(pTiff, TIFFTAG_BITSPERSAMPLE, uiDstBits);
    TIFFSetField(pTiff, TIFFTAG_MINSAMPLEVALUE, 0);
    TIFFSetField(pTiff, TIFFTAG_MAXSAMPLEVALUE, iMaxVal);
    TIFFSetField(pTiff, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    TIFFSetField(pTiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(pTiff, TIFFTAG_PHOTOMETRIC, bIsRGB ? PHOTOMETRIC_RGB : PHOTOMETRIC_MINISBLACK);
    TIFFSetField(pTiff, TIFFTAG_COMPRESSION, uiDstCompression);

    if (pImg->data_sign_mode == XI_DATA_SM_SIGNED_FLOATING)
        TIFFSetField(pTiff, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP);
    else if (pImg->data_sign_mode == XI_DATA_SM_SIGNED_2C && !bNeeds8BitConversion)
        TIFFSetField(pTiff, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_INT);
    else
        TIFFSetField(pTiff, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);

    bool bOK = false;

    // in case of RGB, swap R and G and remove padding, see CxTiffImageSaver::save
    if (!bIsRGB && uiPicStride == uiTIFFStride && !bNeeds16bitScale && !bNeeds8BitConversion)
    {
        TIFFSetField(pTiff, TIFFTAG_ROWSPERSTRIP, h);
        bOK = (TIFFWriteEncodedStrip(pTiff, 0, pImg->bp, uiTIFFStride*h) != -1);
    }
    else
    {
        // strips
        uint32_t uiRowsPerStrip = (1 << 18)/uiTIFFStride; // 256 KB blocks
        if (uiRowsPerStrip < 16)
            uiRowsPerStrip = 16;
        if (uiRowsPerStrip > h/2)
            uiRowsPerStrip = h;     // entire image
        TIFFSetField(pTiff, TIFFTAG_ROWSPERSTRIP, uiRowsPerStrip);
        uint32_t uiStripSize = uiTIFFStride * uiRowsPerStrip;
        uint8_t* pStrip = (uint8_t*)malloc(uiStripSize);

        uint32_t uiStripRow = 0;
        uint32_t uiStripCount = 0;

        if (pImg->frm == XI_RGB24)
        {
            // remove line padding and swap RGB
            for (y = 0; y < h; y++)
            {
                const uint8_t *pLine = (const uint8_t*)pImg->bp + y*uiPicStride;
                uint8_t *pLineDst = pStrip + uiStripRow * uiTIFFStride;
                for (x = 0; x < w; x++)
                {
                    pLineDst[2] = *pLine++;
                    pLineDst[1] = *pLine++;
                    pLineDst[0] = *pLine++;
                    pLineDst+=3;
                }

                uiStripRow++;
                if (uiStripRow == uiRowsPerStrip)
                {
                    bOK = (TIFFWriteEncodedStrip(pTiff, uiStripCount++, pStrip, uiStripSize) != -1);
                    uiStripRow = 0;
                }
            }
        }
        else if (pImg->frm == XI_RGB32)
        {
            // remove line padding, swap RGB, remove A
            for (y = 0; y < h; y++)
            {
                const uint8_t *pLine = (const uint8_t*)pImg->bp + y*uiPicStride;
                uint8_t *pLineDst = pStrip + uiStripRow * uiTIFFStride;
                for (x = 0; x < w; x++)
                {
                    pLineDst[2] = *pLine++;
                    pLineDst[1] = *pLine++;
                    pLineDst[0] = *pLine++;
                    pLineDst+=3;
                    pLine++;    // skip A
                }
                uiStripRow++;
                if (uiStripRow == uiRowsPerStrip)
                {
                    bOK = (TIFFWriteEncodedStrip(pTiff, uiStripCount++, pStrip, uiStripSize) != -1);
                    uiStripRow = 0;
                }
            }
        }
        else if (bNeeds8BitConversion)
        {
            uint32_t uiShift = uiSrcDataBitDepth - 8;
            uint32_t wc = w * uiDstComps;
            bool bSrcDataSigned = (pImg->data_sign_mode == XI_DATA_SM_SIGNED_2C);
            if (uiSrcBits == 16 && !bSrcDataSigned)
            {
                for (y = 0; y < h; y++)
                {
                    const uint16_t *pLine = (const uint16_t*)((const uint8_t*)pImg->bp + y*uiPicStride);
                    uint8_t *pLineDst = pStrip + uiStripRow * uiTIFFStride;
                    if (pImg->frm == XI_RGB48)
                    {
                        for (x = 0; x < w; x++)
                        {
                            pLineDst[2] = (uint8_t)(*pLine++ >> uiShift);
                            pLineDst[1] = (uint8_t)(*pLine++ >> uiShift);
                            pLineDst[0] = (uint8_t)(*pLine++ >> uiShift);
                            pLineDst+=3;
                        }
                    }
                    else if (pImg->frm == XI_RGB64)
                    {
                        for (x = 0; x < w; x++)
                        {
                            pLineDst[2] = (uint8_t)(*pLine++ >> uiShift);
                            pLineDst[1] = (uint8_t)(*pLine++ >> uiShift);
                            pLineDst[0] = (uint8_t)(*pLine++ >> uiShift);
                            pLineDst+=3;
                            pLine++;    // skip A
                        }
                    }
                    else
                    {
                        for (x = 0; x < wc; x++)
                        {
                            *pLineDst++ = (uint8_t)(*pLine++ >> uiShift);
                        }
                    }
                    uiStripRow++;
                    if (uiStripRow == uiRowsPerStrip)
                    {
                        bOK = (TIFFWriteEncodedStrip(pTiff, uiStripCount++, pStrip, uiStripSize) != -1);
                        uiStripRow = 0;
                    }
                }
            }
            else if (uiSrcBits == 32 && !bSrcDataSigned)
            {
                for (y = 0; y < h; y++)
                {
                    const uint32_t *pLine = (const uint32_t*)((const uint8_t*)pImg->bp + y*uiPicStride);
                    uint8_t *pLineDst = pStrip + uiStripRow * uiTIFFStride;
                    for (x = 0; x < wc; x++)
                    {
                        *pLineDst++ = (uint8_t)(*pLine++ >> uiShift);
                    }
                    uiStripRow++;
                    if (uiStripRow == uiRowsPerStrip)
                    {
                        bOK = (TIFFWriteEncodedStrip(pTiff, uiStripCount++, pStrip, uiStripSize) != -1);
                        uiStripRow = 0;
                    }
                }
            }
            else if (uiSrcBits == 8 && bSrcDataSigned)
            {
                // offset by 128
                for (y = 0; y < h; y++)
                {
                    const int8_t *pLine = (const int8_t*)((const uint8_t*)pImg->bp + y*uiPicStride);
                    uint8_t *pLineDst = pStrip + uiStripRow * uiTIFFStride;
                    for (x = 0; x < wc; x++)
                    {
                        *pLineDst++ = (uint8_t)((int32_t)*pLine++ + 128);
                    }
                    uiStripRow++;
                    if (uiStripRow == uiRowsPerStrip)
                    {
                        bOK = (TIFFWriteEncodedStrip(pTiff, uiStripCount++, pStrip, uiStripSize) != -1);
                        uiStripRow = 0;
                    }
                }
            }
            else if (uiSrcBits == 16 && bSrcDataSigned)
            {
                // offset by half bpc
                int64_t iOffset;
                CxImageExporter::minMaxBpcVal(&iOffset, nullptr, uiSrcDataBitDepth, bSrcDataSigned);
                for (y = 0; y < h; y++)
                {
                    const int16_t *pLine = (const int16_t*)((const uint8_t*)pImg->bp + y*uiPicStride);
                    uint8_t *pLineDst = pStrip + uiStripRow * uiTIFFStride;
                    for (x = 0; x < wc; x++)
                    {
                        *pLineDst++ = (uint8_t)(((int64_t)*pLine++ - iOffset) >> uiShift);
                    }
                    uiStripRow++;
                    if (uiStripRow == uiRowsPerStrip)
                    {
                        bOK = (TIFFWriteEncodedStrip(pTiff, uiStripCount++, pStrip, uiStripSize) != -1);
                        uiStripRow = 0;
                    }
                }
            }                    }
        else if (bNeeds16bitScale)
        {
            // 10 bit to 16 bit
            uint32_t uiShift = 16 - uiSrcDataBitDepth;
            uint32_t wc = w * uiDstComps;
            for (y = 0; y < h; y++)
            {
                const uint16_t *pLine = (const uint16_t*)((const uint8_t*)pImg->bp + y*uiPicStride);
                uint16_t *pLineDst = (uint16_t*)(pStrip + uiStripRow * uiTIFFStride);

                if (pImg->frm == XI_RGB48)
                {
                    for (x = 0; x < w; x++)
                    {
                        pLineDst[2] = (uint16_t)(*pLine++ << uiShift);
                        pLineDst[1] = (uint16_t)(*pLine++ << uiShift);
                        pLineDst[0] = (uint16_t)(*pLine++ << uiShift);
                        pLineDst+=3;
                    }
                }
                else if (pImg->frm == XI_RGB64)
                {
                    for (x = 0; x < w; x++)
                    {
                        pLineDst[2] = (uint16_t)(*pLine++ << uiShift);
                        pLineDst[1] = (uint16_t)(*pLine++ << uiShift);
                        pLineDst[0] = (uint16_t)(*pLine++ << uiShift);
                        pLineDst+=3;
                        pLine++;    // skip A
                    }
                }
                else
                {
                    for (x = 0; x < wc; x++)
                    {
                        *pLineDst++ = (uint16_t)(*pLine++ << uiShift);
                    }
                }
                uiStripRow++;
                if (uiStripRow == uiRowsPerStrip)
                {
                    bOK = (TIFFWriteEncodedStrip(pTiff, uiStripCount++, pStrip, uiStripSize) != -1);
                    uiStripRow = 0;
                }
            }
        }
        else if (pImg->frm == XI_RGB48)
        {
            // remove line padding and swap RGB
            for (y = 0; y < h; y++)
            {
                const uint16_t *pLine = (const uint16_t*)((const uint8_t*)pImg->bp + y*uiPicStride);
                uint16_t *pLineDst = (uint16_t*)(pStrip + uiStripRow * uiTIFFStride);
                for (x = 0; x < w; x++)
                {
                    pLineDst[2] = *pLine++;
                    pLineDst[1] = *pLine++;
                    pLineDst[0] = *pLine++;
                    pLineDst+=3;
                }

                uiStripRow++;
                if (uiStripRow == uiRowsPerStrip)
                {
                    bOK = (TIFFWriteEncodedStrip(pTiff, uiStripCount++, pStrip, uiStripSize) != -1);
                    uiStripRow = 0;
                }
            }
        }
        else if (pImg->frm == XI_RGB64)
        {
            // remove line padding, swap RGB, remove A
            for (y = 0; y < h; y++)
            {
                const uint16_t *pLine = (const uint16_t*)((const uint8_t*)pImg->bp + y*uiPicStride);
                uint16_t *pLineDst = (uint16_t*)(pStrip + uiStripRow * uiTIFFStride);
                for (x = 0; x < w; x++)
                {
                    pLineDst[2] = *pLine++;
                    pLineDst[1] = *pLine++;
                    pLineDst[0] = *pLine++;
                    pLineDst+=3;
                    pLine++;    // skip A
                }
                uiStripRow++;
                if (uiStripRow == uiRowsPerStrip)
                {
                    bOK = (TIFFWriteEncodedStrip(pTiff, uiStripCount++, pStrip, uiStripSize) != -1);
                    uiStripRow = 0;
                }
            }
        }
        else
        {
            // no data conversion, just remove line padding
            for (y = 0; y < h; y++)
            {
                const uint8_t *pLine = (const uint8_t*)pImg->bp + y*uiPicStride;
                uint8_t *pLineDst = pStrip + uiStripRow * uiTIFFStride;
                memcpy(pLineDst, pLine, uiTIFFStride);

                uiStripRow++;
                if (uiStripRow == uiRowsPerStrip)
                {
                    bOK = (TIFFWriteEncodedStrip(pTiff, uiStripCount++, pStrip, uiStripSize) != -1);
                    uiStripRow = 0;
                }
            }
        }
        if (uiStripRow > 0) // flush last strip
            bOK = (TIFFWriteEncodedStrip(pTiff, uiStripCount++, pStrip, uiStripSize) != -1);

        free(pStrip);
    }

    TIFFWriteDirectory(pTiff);
    TIFFClose(pTiff);
    return bOK;
#else
    return false;
#endif  // MULTIRECORDER_TIFF_ENABLE
}
