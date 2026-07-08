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
/// @file pfcPixelTypes.h
/// @brief Definition of the pixel types supported by the PF SDK.
///
/// Pixel Types Supported.
///
/// @note
/// @author TAH 
/// @version 0.1 
/// @date 2016/11/09
///
#ifndef PFCPIXELTYPES_H
#define PFCPIXELTYPES_H

///
///	@enum pfPixelType
///
/// Enumeration of all the pixel types supported by the PF SDK.
///	All the values are according to GenICam PFNC version 1.1 except notice.
///
typedef enum pfPixelType
{
	PixelMono8 = 0x01080001,
	PixelMono10 = 0x01100003,
	PixelMono10p = 0x010a0046,
	PixelMono10Packed = 0x010C0004,		///<	GigE Vision Specification 2.0
	PixelMono12 = 0x01100005,
	PixelMono12p = 0x010c0047,
	PixelMono12Packed = 0x010C0006,		///<	GigE Vision Specification 2.0
	PixelMono16 = 0x01100007,
  PixelBayerGR8 = 0x01080008,
  PixelBayerRG8 = 0x01080009,
  PixelBayerGB8 = 0x0108000A,
  PixelBayerGR10 = 0x0110000C,
  PixelBayerRG10 = 0x0110000D,
  PixelBayerGB10 = 0x0110000E,
  PixelBayerGR10Packed = 0x010C0026,
  PixelBayerRG10Packed = 0x010C0027,
  PixelBayerGB10Packed = 0x010C0028, 
  PixelBayerGR12 = 0x01100010,
  PixelBayerRG12 = 0x01100011,
  PixelBayerGB12 = 0x01100012,
  PixelBayerGR12Packed = 0x010C002A,
  PixelBayerRG12Packed = 0x010C002B,
  PixelBayerGB12Packed = 0x010C002C,
  PixelRGB8 = 0x02180014,
	PixelRGB10 = 0x02300018,
	PixelRGB12 = 0x0230001A,
	PixelRGB16 = 0x02300033,
	PixelUnknown = 0
}pfPixelType;


#endif //PFCPIXELTYPES_H