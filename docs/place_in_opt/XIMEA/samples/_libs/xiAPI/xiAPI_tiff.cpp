// this library is able to save xiAPI image into TIFF file

#include <stdio.h>
#include <memory.h>
#if defined (_WIN32)
#include <xiApi.h>       // Windows
#include <libtiff/tiffio.h>
#else
#include <m3api/xiApi.h> // Linux, OSX
#include <tiffio.h>
#endif

static void xiImageGetBitCount(XI_IMG* image, int* bit_count, int* num_channels, bool* is_rgb)
{
    switch (image->frm)
    {
    case XI_MONO8:
    case XI_RAW8:
        *bit_count = 8;
        *num_channels = 1;
        *is_rgb = false;
        break;
    case XI_RAW8X2:
        *bit_count = 8;
        *num_channels = 2;
        *is_rgb = false;
        break;
    case XI_RAW8X3:
        *bit_count = 8;
        *num_channels = 3;
        *is_rgb = false;
        break;
    case XI_MONO16:
    case XI_RAW16:
        *bit_count = 16;
        *num_channels = 1;
        *is_rgb = false;
        break;
    case XI_RAW16X2:
        *bit_count = 16;
        *num_channels = 2;
        *is_rgb = false;
        break;
    case XI_RAW16X3:
        *bit_count = 16;
        *num_channels = 3;
        *is_rgb = false;
        break;
    case XI_RGB24:
        *bit_count = 8;
        *num_channels = 3;
        *is_rgb = true;
        break;
    case XI_RAW32:
        *bit_count = 32;
        *num_channels = 1;
        *is_rgb = false;
        break;
    case XI_RGB32:
        *bit_count = 8;
        *num_channels = 4;
        *is_rgb = true;
        break;
    case XI_RAW32FLOAT:
        *bit_count = 32;
        *num_channels = 1;
        *is_rgb = false;
        break;
    case XI_RGB48:
        *bit_count = 16;
        *num_channels = 3;
        *is_rgb = true;
        break;
    case XI_RGB64:
        *bit_count = 16;
        *num_channels = 4;
        *is_rgb = true;
        break;
    default:
        throw("xiImageGetBitCount - Unsupported data format\n");
    }
}

void WriteImage(XI_IMG* image, char* filename)
{
	TIFF* tiff_img = TIFFOpen(filename, "w");
	if (!tiff_img)
		throw "Opening image by TIFFOpen";

	// set tiff tags
	int width = image->width;
	int height = image->height;
	int bits_per_sample = 1;
	int num_channels = 1;
	int line_len = 0;
    bool is_rgb = false;

    xiImageGetBitCount(image, &bits_per_sample, &num_channels, &is_rgb);
    line_len = width * ((bits_per_sample / 8) * num_channels) + image->padding_x;

    if (is_rgb && num_channels == 4)
        num_channels = 3;     // remove A
    int tiff_line_len = width * ((bits_per_sample / 8) * num_channels);

	printf("Saving image %dx%d to file:%s\n", width, height, filename);

    TIFFSetField(tiff_img, TIFFTAG_IMAGEWIDTH, width);
	TIFFSetField(tiff_img, TIFFTAG_IMAGELENGTH, height);
	TIFFSetField(tiff_img, TIFFTAG_SAMPLESPERPIXEL, num_channels);
	TIFFSetField(tiff_img, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
	TIFFSetField(tiff_img, TIFFTAG_MINSAMPLEVALUE, 0);
    TIFFSetField(tiff_img, TIFFTAG_MAXSAMPLEVALUE, (int)(image->data_saturation + 0.5));
	TIFFSetField(tiff_img, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
	TIFFSetField(tiff_img, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    if (is_rgb)
		TIFFSetField(tiff_img, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
	else
		TIFFSetField(tiff_img, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
	TIFFSetField(tiff_img, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    if (image->frm == XI_RAW32FLOAT)
        TIFFSetField(tiff_img, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP);
    else if (image->data_sign_mode == XI_DATA_SM_SIGNED_2C)
        TIFFSetField(tiff_img, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_INT);
    else
        TIFFSetField(tiff_img, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);

	// save data
    if (!is_rgb && line_len == tiff_line_len)
    {
        TIFFSetField(tiff_img, TIFFTAG_ROWSPERSTRIP, height);
        if (TIFFWriteEncodedStrip(tiff_img, 0, image->bp, line_len*height) == -1)
        {
            throw("Failed to write image to TIFF");
        }
    }
    else
    {
        // strips
        int x, y;
        int rows_per_strip = (1 << 18)/tiff_line_len; // 256 KB blocks
        if (rows_per_strip < 16)
            rows_per_strip = 16;
        if (rows_per_strip > height/2)
            rows_per_strip = height;     // entire image
        TIFFSetField(tiff_img, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
        int uiStripSize = tiff_line_len * rows_per_strip;
        BYTE* pStrip = (BYTE*)malloc(uiStripSize);

        int strip_row = 0;
        int strip_count = 0;
        if (image->frm == XI_RGB24)
        {
            // remove line padding and swap RGB
            for (y = 0; y < height; y++)
            {
                const BYTE *pLine = (const BYTE*)image->bp + y*line_len;
                BYTE *pLineDst = pStrip + strip_row * tiff_line_len;
                for (x = 0; x < width; x++)
                {
                    pLineDst[2] = *pLine++;
                    pLineDst[1] = *pLine++;
                    pLineDst[0] = *pLine++;
                    pLineDst+=3;
                }

                strip_row++;
                if (strip_row == rows_per_strip)
                {
                    if (TIFFWriteEncodedStrip(tiff_img, strip_count++, pStrip, uiStripSize) == -1)
                    {
                        throw("Failed to write image to TIFF");
                    }
                    strip_row = 0;
                }
            }
        }
        else if (image->frm == XI_RGB32)
        {
            // remove line padding, swap RGB, remove A
            for (y = 0; y < height; y++)
            {
                const BYTE *pLine = (const BYTE*)image->bp + y*line_len;
                BYTE *pLineDst = pStrip + strip_row * tiff_line_len;
                for (x = 0; x < width; x++)
                {
                    pLineDst[2] = *pLine++;
                    pLineDst[1] = *pLine++;
                    pLineDst[0] = *pLine++;
                    pLineDst+=3;
                    pLine++;    // skip A
                }
                strip_row++;
                if (strip_row == rows_per_strip)
                {
                    if (TIFFWriteEncodedStrip(tiff_img, strip_count++, pStrip, uiStripSize) == -1)
                    {
                        throw("Failed to write image to TIFF");
                    }
                    strip_row = 0;
                }
            }
        }
        else if (image->frm == XI_RGB48)
        {
            // remove line padding and swap RGB
            for (y = 0; y < height; y++)
            {
                const USHORT *pLine = (const USHORT*)((BYTE*)image->bp + y*line_len);
                USHORT *pLineDst = (USHORT*)(pStrip + strip_row * tiff_line_len);
                for (x = 0; x < width; x++)
                {
                    pLineDst[2] = *pLine++;
                    pLineDst[1] = *pLine++;
                    pLineDst[0] = *pLine++;
                    pLineDst+=3;
                }

                strip_row++;
                if (strip_row == rows_per_strip)
                {
                    if (TIFFWriteEncodedStrip(tiff_img, strip_count++, pStrip, uiStripSize) == -1)
                    {
                        throw("Failed to write image to TIFF");
                    }
                    strip_row = 0;
                }
            }
        }
        else if (image->frm == XI_RGB64)
        {
            // remove line padding, swap RGB, remove A
            for (y = 0; y < height; y++)
            {
                const USHORT *pLine = (const USHORT*)((const BYTE*)image->bp + y*line_len);
                USHORT *pLineDst = (USHORT*)(pStrip + strip_row * tiff_line_len);
                for (x = 0; x < width; x++)
                {
                    pLineDst[2] = *pLine++;
                    pLineDst[1] = *pLine++;
                    pLineDst[0] = *pLine++;
                    pLineDst+=3;
                    pLine++;    // skip A
                }
                strip_row++;
                if (strip_row == rows_per_strip)
                {
                    if (TIFFWriteEncodedStrip(tiff_img, strip_count++, pStrip, uiStripSize) == -1)
                    {
                        throw("Failed to write image to TIFF");
                    }
                    strip_row = 0;
                }
            }
        }
        else
        {
            // remove line padding
            for (y = 0; y < height; y++)
            {
                const BYTE *pLine = (const BYTE*)image->bp + y*line_len;
                BYTE *pLineDst = pStrip + strip_row * tiff_line_len;
                memcpy(pLineDst, pLine, tiff_line_len);

                strip_row++;
                if (strip_row == rows_per_strip)
                {
                    if (TIFFWriteEncodedStrip(tiff_img, strip_count++, pStrip, uiStripSize) == -1)
                    {
                        throw("Failed to write image to TIFF");
                    }
                    strip_row = 0;
                }
            }
        }
        if (strip_row > 0) // flush last strip
        {
            if (TIFFWriteEncodedStrip(tiff_img, strip_count++, pStrip, uiStripSize) == -1)
            {
                throw("Failed to write image to TIFF");
            }
        }

        free(pStrip);
    }

	TIFFWriteDirectory(tiff_img);
	TIFFClose(tiff_img);
}
