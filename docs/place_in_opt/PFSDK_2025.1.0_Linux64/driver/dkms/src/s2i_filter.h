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

/* s2i_filter netfilter modul header
 *
 * (C) 2021 by Sensor to Image GmbH
 *
 * Version: 2.6.0 Date: 26.05.2021
 *
 * History:
 * Version: 1.0.0 Date: 11.03.2009
 * - initial realease
 * Version: 1.0.1 Date: 25.06.2009
 * - change in strucht image_fps_io the variable image_fps form unsigned long to u_int32_t. On 64 bit linux we got always 0
 * - insert define CAMERA_COUNT 10
 * Version: 1.0.3 Date: 25.09.2009
 * - insert grab_buffer_read
 * Version: 1.4 Date: 06.10.2009
 * - insert PacketSize in grab_param and grab_param_io struct
 * Version: 1.5 Date: 14.10.2009
 * - insert version_io struct
 * - insert DRIVER_MAJOR_VERSION and DRIVER_MINOR_VERSION
 * - insert IOCTL_CMD_VERSION
 * - change filter driver parameter
 * Version: 1.0.6 Date: 20.01.2010
 * - change filter driver parameter
 * - version to 1.6
 * Version: 1.0.7 Date: 04.02.2010
 * - change filter driver parameter
 * - insert image_header struct
 * - insert IOCTL_CMD_IMAGE_HEADER
 * - version to 1.7
 * Version: 1.0.8 Date: 07.04.2010
 * - change filter driver parameter
 * - version to 1.8
 * Version: 2.0.0 Date: 17.03.2011
 * - insert PaddingX, PaddingY and MissingPacket
 * - change driver name to s2igevfilter 
 * - version to 2.0
 * Version: 2.0.4 Date: 15.05.2012
 * - change minor version to minor and sub minor X.X.X
 * Version: 2.0.5 Date: 12.12.2012
 * - version to 2.0.5
 * Version: 2.0.6 Date: 13.12.2013
 * - version to 2.0.6
 * Version: 2.0.8 Date: 09.09.2014
 * - increment device/camera count to 20
 * - version to 2.0.8
 * Version: 2.0.9 Date: 19.01.2015
 * - added BlockIDMask
 * - version to 2.1.0
 * Version: 2.1.0 Date: 11.11.2015
 * - added set buffer count
 * - added set packets out of order
 * - added test packet resend
 * - Using macros (_IOWR) to generade ioctl command numbers
 * - version to 2.1.0
 * Version: 2.1.1 Date: 26.01.2017
 * - added  pImageBufferBeforeBefore and pImageBufferNext;
 * - version to 2.1.1
 * Version: 2.1.2 Date: 08.03.2017
 * - support multi-part data payload type
 * - version to 2.1.2
 * Version: 2.1.3 Date: 29.06.2017
 * - added variables to grab param struct for fps measurement
 * - added count parameter to test_packet_resend_io struct
 * - added test_packet_resend_count to grab param struct
 * - added ImageDataChunkTrailerPacket struct
 * - added PayloadType, ChunkDataPayloadLength and ChunkLayoutId to image_header struct
 * - version to 2.1.3
 * Version: 2.1.4 Date: 22.08.2018
 * - added data_blocks_discarded_on_device to grab_param struct  
 * - added payload type defines
 * - added GEV_FLAG_PREVIOUS_BLOCK_DROPPED
 * - added data_blocks_discarded_on_device to status_io struct  
 * - version to 2.1.4
 * 
 * Version: 2.2.0 Date: 09.07.2019
 * - added packet resend info to image header
 * - buffer handling has been changed
 * - added MultiPartDataPayloadPacket struct
 * - version to 2.2.0
 * 
 * Version: 2.3.0 Date: 08.01.2020
 * - added GenDC support
 * - version to 2.3.0
 * - added gendc_support to grab_param and grab_param struct
 
 * Version: 2.4.0 Date: 05.05.2020
 * - version to 2.4.0
 * - added PacketIndexSav to IMAGE_BUFFER struct
 * - added pImageBufferCurrent to grab_param struct
 
 * Version: 2.5.0 Date: 13.01.2021
 * - version to 2.5.0
 * - now supports 50 devices
 * - moved the gige vision specific code in separate files outsourced, to create precompiled file
 * - fixed mutex issue

 * Version: 2.6.0 Date: 26.05.2021
 * - Access to queues is not thread safe, use spin lock 
 * - version to 2.6.0
 * - migrate the next pointer into IMAGE_BUFFER and stop allocating/freeing NodeBuffer structures.
 * - removed unused function deque_front
 * - fixed kernel panic due to binary blob interface using kernel structures
 
 * Version: 2.7.0 Date: 15.12.2021
 * - refactoring 
 
 * Version: 2.7.5 Date: 22.02.2024
   - Add ring buffer support
 */
#ifndef __S2I_FILTER_H
#define __S2I_FILTER_H

#include "s2i_gev.h"                   
#include "s2i_ioctl.h"      
#include <linux/module.h>
#define USE_HIGH_RESOLUTION_TIMER
#ifdef USE_HIGH_RESOLUTION_TIMER
#include <linux/hrtimer.h>
#else 
#include <linux/timer.h>
#endif

#define DRIVER_MAJOR_VERSION    2
#define DRIVER_MINOR_VERSION    0x75

#define S2IGEVFILTER_MAJOR     120
#define MODUL_NAME             "s2igevfilter"
#define S2IGEVFILTER_MINOR     0

// Ring Buffer use
struct ring_buffer_io_filter {
  SPHINX_CAMNR cam_nr;
  void* buffer;
  u_int16_t index;
  u_int32_t length;
};

#endif
 
