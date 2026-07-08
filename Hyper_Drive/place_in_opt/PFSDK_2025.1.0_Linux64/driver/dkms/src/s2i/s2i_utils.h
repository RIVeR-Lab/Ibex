/* s2i_utils.h -- Utils header
 *
 *  (c) Copyright 2021 Sensor to Image GmbH
 *
 * Version: 1.0.0 Date: 28.10.2021
 *
 * History:
 * Version: 1.0.0 Date: 28.10.2021
 * - initial release 
 */
#ifndef __S2I_UTILS_H
#define __S2I_UTILS_H

#include <s2i/queue.h>

IMAGE_BUFFER* deque_pop_front(struct QueueBuffer *q);
IMAGE_BUFFER* deque_pop_timed(struct QueueBuffer *q, os_delay_t allowedWait);
void deque_push_back(struct QueueBuffer *q, IMAGE_BUFFER *buffer);
size_t deque_size(struct QueueBuffer *q);
void deque_init(struct QueueBuffer *q);
void deque_close(struct QueueBuffer *q);
void deque_push_front(struct QueueBuffer *q, IMAGE_BUFFER *buffer);
void deque_cancel(struct QueueBuffer *q);

PIMAGE_BUFFER remove_buffer_from_deque(struct QueueBuffer *q,void *buffer);


void set_fps(gev_camno_t device);
os_error_t InitImageBuffer(gev_camno_t device, u_int32_t image_size, u_int32_t packet_size);
void CloseImageBuffer(gev_camno_t device);

void ResetTimeStats(gev_camno_t device, os_time_t start, os_frequency_t freq);
void InitTimeStats(gev_camno_t device);
void CloseTimeStats(gev_camno_t device);
void GetTimeStats(gev_camno_t device, u_int32_t *img_cnt, u_int64_t *img_time);
double GetFpsFromTimeStats(gev_camno_t device);

void ResetWorkspaceImageBuffer(gev_camno_t device, IMAGE_BUFFER** p);
void ResetStatistics(gev_camno_t device);
void ResetAcquisitionStatus(gev_camno_t device);

struct time_stats {
  os_lockable_t lock;
  os_time_t start;
  os_time_t stop;
  os_frequency_t frequency;
  struct fps {
    u_int32_t img_cnt;
    u_int64_t duration; //!< 1 second if value equals frequency.
  } ongoing, last;
};

struct filter_param {
  u_int8_t grab_flag;
  u_int32_t CamIp;    
  u_int16_t CamPort;
  u_int16_t CamPortCtrl;
  u_int32_t adapter_ip;
  u_int32_t grab_timeout;
  struct image_header cur_img_header;
  struct time_stats time;
};

struct shared_stat {
  struct time_stats time;
  struct queue_param queue;
};

extern struct filter_param *filter_parameter[];
extern struct shared_stat cancam_common[];

#endif
 
