/* s2i_io.h -- IO header
 *
 * (C) 2021 by Sensor to Image GmbH
 *
 * Version: 1.0.1 Date: 23.02.2024
 *
 * History:
 * Version: 1.0.0 Date: 28.10.2021
 * - initial release
 * Version: 1.0.1 Date: 23.02.2024
 * - grab_param_io: packet_resend. packet_resend_count extended to 16 bits, new packet_timeout  
 * - image_header: grab parameter ResendSendCount, ResendReceiveCount extended to 16 bits
 */
#ifndef __S2I_IO_H
#define __S2I_IO_H
// use 'k' as magic number
#define S2I_IOC_MAGIC   'k'

#include <s2i/types.h>

struct grab_param_io {
  u_int32_t width;
  u_int32_t height;
  u_int32_t cam_ip;       				         // ip address
 u_int16_t cam_port;               			 // port data
  u_int16_t cam_port_ctrl;         			 // port control
  u_int32_t pixel_format;
  u_int32_t adapter_ip;     			        // ip address
  u_int32_t grab_size;  
  u_int16_t packet_size;
  u_int16_t packet_resend;
  u_int16_t packet_resend_count;     // For filter driver update to 16 bits
  SPHINX_CAMNR cam_nr;
  u_int32_t timeout;   			 		 	  // FIXME? bring up to avoid alignment issues ?
  u_int32_t packet_timeout;
  u_int8_t gendc_support;
};

struct image_header {
  u_int64_t FrameCounter;
  u_int64_t TimeStamp;
  u_int32_t PixelType;
  u_int32_t SizeX;
  u_int32_t SizeY;
  u_int32_t OffsetX;
  u_int32_t OffsetY;
  u_int16_t  PaddingX;
  u_int16_t  PaddingY;
  int32_t  MissingPacket;
  u_int16_t PayloadType;
  u_int32_t ChunkDataPayloadLength;
  u_int32_t ChunkLayoutId;
  u_int16_t ResendSendCount;
  u_int16_t ResendReceiveCount;
};

struct image_header_io {
  SPHINX_CAMNR cam_nr;
  struct image_header img_h;
};

struct packet_resend_io {
  SPHINX_CAMNR cam_nr;
  u_int8_t packet_resend_flag;
};

struct image_fps_io {
  SPHINX_CAMNR cam_nr;
  u_int32_t image_fps;
};

struct version_io {
  u_int8_t major;
  u_int8_t minor;
};

struct status_io {
  SPHINX_CAMNR cam_nr;
  u_int32_t img_cnt;
  u_int32_t img_error;
  u_int32_t missing_packets;
  u_int32_t time_img_cnt;
  u_int64_t img_time;
  u_int32_t data_blocks_discarded_on_device;
};

struct buffer_count_io {
  SPHINX_CAMNR cam_nr;
  u_int16_t count;
};

struct packets_out_of_order_io {
  SPHINX_CAMNR cam_nr;
  u_int8_t packets_out_of_order;
};

struct test_packet_resend_io {
  SPHINX_CAMNR cam_nr;
  u_int16_t packet;
  u_int16_t count;
};

// Windows only
struct ring_buffer_io {
  SPHINX_CAMNR cam_nr;
  void * buffer;
  u_int16_t index;
};

// Darwin only
struct get_image_io {
  SPHINX_CAMNR cam_nr;
  u_int32_t count;
};

//Darwin only
struct enable_disable_adapter_io {
  SPHINX_CAMNR cam_nr;
  char adapter_name[20];
};


#endif
