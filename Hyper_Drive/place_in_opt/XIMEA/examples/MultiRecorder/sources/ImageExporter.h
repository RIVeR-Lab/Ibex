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

#ifndef CXIMAGEEXPORTER_H
#define CXIMAGEEXPORTER_H

#include <string>
#include <xiApi.h>

/**
 * \brief Helper class for exporting images to files in TIFF format
 *
 * Uses libtiff library.
 */
class CxImageExporter
{
public:
    static bool createPath(const std::string &sPath);           //!< creates path including all parent directories in case they don't exist
    static std::string absolutePath(const std::string &sPath);  //!< Returns absolute path from relative path

    /**
    \brief Saves the XI_IMG to TIFF file

    @param[in] sFilename                target filename
    @param[in] pImg                     image to save
    @param[in] bTiffEnable16bit         enable saving non-8bit images to TIFF. The best compatibility is having only 8 bits per channel, but it looses the data precision.
    @param[in] bTiffRescale16bit        when 16bit saving, rescale data to full 16bit range (i.e. maps 10bit image data to 16bit) to increase the compatibility other SW, loosing the data precision.
    @return true when the image was saved
    */
    static bool saveImageToFile(const std::string &sFilename, XI_IMG *pImg, bool bTiffEnable16bit = true, bool bTiffRescale16bit = false);
    static bool xiImageGetBitCount(XI_IMG* image, uint32_t* bit_count, uint32_t* num_channels, bool* is_rgb);  //!< returns number of channels and bit count of the image
    static void minMaxBpcVal(int64_t *piMin, int64_t *piMax, uint32_t uiBpc, bool bDataSigned);     //!< returns min and max possible pixel values for given bits-per-channel
};

#endif // CXIMAGEEXPORTER_H
