/*
******************************************************************************
* @attention
*
*<h2><center>&copy; COPYRIGHT(c) 2021 Photonfocus AG</center></h2>
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
* 1. Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
* 3. Neither the name of Photonfocus nor the names of its contributors
* may be used to endorse or promote products derived from this software
* without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*/

///
/// @file pfcImageTypes.h
/// @brief Definition of different data structs used in the PF SDK.
///
/// Further Description.
///
/// @note
/// @author TAH 
/// @version 0.1 
/// @date 2016/11/09
///
#ifndef PFCIMAGETYPES_H
#define PFCIMAGETYPES_H

#include "pfcPixelTypes.h"
#include <stdint.h>

/// Image header information
typedef struct pfimage_header_t
{
	int64_t FrameCounter;			///< Image frame counter
	uint64_t TimeStamp;		///< Image timestamp
	uint32_t PixelType;			///< Image pixel type
	uint32_t SizeX;				///< Image X size
	uint32_t SizeY;				///< Image Y size
	uint32_t OffsetX;				///< Image X offset
	uint32_t OffsetY;				///< Image Y offset
	uint16_t PaddingX;				///< Image X padding
	uint16_t PaddingY;				///< Image Y padding
	int  MissingPacket;			///< Image missing packets
	uint16_t PayloadType;
	uint32_t ChunkDataPayloadLength;
	uint32_t ChunkLayoutId;
} PFImageHeader;

/// The struct #PFFrame contains the image header and a buffer with image data.
typedef struct pfframe_t
{
	PFImageHeader ImageHeader;	    ///< Image header
	unsigned char *ImagePointer;	///< Image pointer to buffer with image data
}PFFrame;

#endif //PFCIMAGETYPES_H