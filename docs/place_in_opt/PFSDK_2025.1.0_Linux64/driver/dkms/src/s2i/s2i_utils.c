/*****************************************************************************/
/*
 * s2i_utils.c -- Utils functions
 *
 *  (c) Copyright 2021 Sensor to Image GmbH
 *
 * Version: 1.0.1 Date: 22.02.2024
 *
 * History:
 * Version: 1.0.0 Date: 28.10.2021
 * - initial release 
 *
 * Version: 1.0.1 Date: 22.02.2024
 * -  Use RingBuffer: Set ring buffer items 
 *
*/
/*****************************************************************************/
#define TRACE_CONTEXT (device + 1)
#define M_KIND M_KIND_GEV
#include "s2i_gev.h"
#include "s2i_utils.h"  
#include <s2i/os.h>                 
#include <s2i/queue.h>                  


/** Get the first image buffer from the queue
 *  returns NULL if no buffer is available right now
 */
IMAGE_BUFFER* deque_pop_front(struct QueueBuffer *q)
{
  IMAGE_BUFFER* image_buffer = NULL;

  if (q == NULL)
    return NULL;

  OsLockSpin(&q->lock);
  if (q->front)
  {
    image_buffer = q->front;
    q->size--;
    q->front = q->front->NextBuffer;
    image_buffer->NextBuffer = NULL;

    if (q->front == NULL) 
      q->last = NULL;
  }
  OsUnlockSpin(&q->lock);

  return image_buffer;
}

void deque_push_back(struct QueueBuffer *q, IMAGE_BUFFER *buffer)
{
  if (q == NULL || buffer == NULL)
    return;
  buffer->NextBuffer = NULL;

  OsLockSpin(&q->lock);
  if (q->last != NULL) {
    q->last->NextBuffer = buffer;
    q->last = buffer;
  }
  else {
    q->front = buffer;
    q->last = buffer;
  }
  ++(q->size);
  OsUnlockSpin(&q->lock);
  OsNotifySignal(&q->newBuffer);
}

void deque_cancel(struct QueueBuffer *q)
{
  if (q == NULL)
    return;
  OsNotifySignal(&q->newBuffer);
}


size_t deque_size(struct QueueBuffer *q)
{
  size_t s;
	
  if (q == NULL)
    return 0;

  OsLockSpin(&q->lock);
  s = q->size;
  OsUnlockSpin(&q->lock);
  return s;
}

void deque_init(struct QueueBuffer *q)
{
  if (q == NULL)
    return;

  OsInitSpin(&q->lock);
  OsInitSignal(&q->newBuffer);
  q->front = NULL;
  q->last = NULL;
  q->size = 0;
}

void deque_close(struct QueueBuffer *q)
{
  if (q == NULL)
    return;

  while (q->size && q->front)
  {
    deque_pop_front(q);
  }
  q->size = 0;
  q->front = NULL;
  q->last = NULL;
  OsCloseSpin(&q->lock);
  OsCloseSignal(&q->newBuffer);
}

void set_fps(gev_camno_t device)
{
  struct time_stats *ts = time_param(device);
  if (OsTimeDefined(ts->start))
  {
    ts->stop = OsGetTickCount();
  }
  else
  {
    ts->start = OsGetTickCount();
    return;
  }
  OsLockSpin(&ts->lock);
  ts->ongoing.duration = OsTickDelta(ts->stop, ts->start);
  ts->ongoing.img_cnt++;

  if (ts->ongoing.duration >= ts->frequency)
  {
    TRACE_3x(M_VERBOSE, M_KIND_SPHINX_GEV, "set_fps update: %d frames in %"PRId64" time ticks (%"PRId64" ticks/sec)",
            ts->ongoing.img_cnt, ts->ongoing.duration, ts->frequency);

    ts->last.img_cnt = ts->ongoing.img_cnt;
    ts->last.duration = ts->ongoing.duration;
    ts->ongoing.img_cnt = 0;
    ts->start = ts->stop;
  }
  OsUnlockSpin(&ts->lock);
}

void EDDI_API set_image(gev_camno_t device, IMAGE_BUFFER *pImage)
{
  if (pImage == NULL)
    return;

  if (pImage->ResendSendCount || pImage->ResendReceiveCount)
    TRACE_3x(M_INFO, M_KIND_SPHINX_GEV, "Packet resend [BlockID %"PRId64"] -> requested: %d, received: %d packets", pImage->ImageHeader.FrameCounter, pImage->ResendSendCount, pImage->ResendReceiveCount);

  if (pImage->ImageHeader.MissingPacket == 0)
    pImage->Error = IMAGE_SUCCESS;
  else
  {
    pImage->Error = IMAGE_GRAB_ERROR;
    grab_parameter[device]->missing_packets += pImage->ImageHeader.MissingPacket;
  }
  grab_parameter[device]->img_cnt++;

  TRACE_1x(M_DEBUG, M_KIND_SPHINX_GEV, "Enqueueing image #%d ...", pImage->ImageHeader.FrameCounter);
  deque_push_back(&queue_parameter[device]->m_nDeliverList, pImage);
  TRACE_1x(M_DEBUG, M_KIND_SPHINX_GEV, "Image #%d enqueued. Updating fps.", pImage->ImageHeader.FrameCounter);

  set_fps(device);

  if (grab_parameter[device]->BlockIDCheck != 0)
  {
    if (pImage->ImageHeader.FrameCounter != grab_parameter[device]->BlockIDCheck)
    {
      int64_t lost;

      lost = pImage->ImageHeader.FrameCounter - grab_parameter[device]->BlockIDCheck;
      if (lost < 0)
        lost = -lost;
      grab_parameter[device]->img_cnt += (u_int32_t)lost;
      grab_parameter[device]->img_error += (u_int32_t) lost;
      grab_parameter[device]->missing_packets += grab_parameter[device]->nNumberOfPacket * (u_int32_t)lost;
      if (lost > 1)
      {
        TRACE_2x(M_ERROR, M_KIND, "Missing frames: %"PRId64" -> %"PRId64"", grab_parameter[device]->BlockIDCheck, pImage->ImageHeader.FrameCounter - 1);
      }
      else
      {
        TRACE_1x(M_ERROR, M_KIND, "Missing frame: %"PRId64"", grab_parameter[device]->BlockIDCheck);
      }
    }
    else
    {
      TRACE_1x(M_DEBUG, M_KIND_SPHINX_GEV, "BlockIDCheck ok: %"PRId64"", 
               pImage->ImageHeader.FrameCounter);
    }
  }
  else
  {
    TRACE_1x(M_DEBUG, M_KIND_SPHINX_GEV, "Setting up BlockIDCheck: %"PRId64"",
             pImage->ImageHeader.FrameCounter);
  }
  gev_complete_block(device, pImage->ImageHeader.FrameCounter);
}

os_error_t InitImageBuffer(gev_camno_t device, u_int32_t image_size, u_int32_t packet_size)
{
  gev_bufno_t i;
  struct OsRingBuffer* rb = NULL;
  size_t allocSize = sizeof(IMAGE_BUFFER) * grab_parameter[device]->buffer_count;
  
  // Check packet_size is greater than 0
  if (packet_size == 0)
  {
    OsPrintk("Uexpected parameters: packet-size is 0 !!!");
    return -1;
  }
  
  if (allocSize)
  {
    grab_parameter[device]->Image = (IMAGE_BUFFER *)OsMalloc(allocSize);
    if (grab_parameter[device]->Image == NULL)
      return OS_ALLOC_FAILURE; 
    OsMemSet(grab_parameter[device]->Image, 0, allocSize);
  }

  if (grab_parameter[device]->ring_buffer_count)
     rb = grab_parameter[device]->ring_buffer;
  
  for (i = 0; i < grab_parameter[device]->buffer_count; i++)
  {
    grab_parameter[device]->Image[i].Index = i;
    grab_parameter[device]->Image[i].SizePacket = packet_size;
    grab_parameter[device]->Image[i].SizeLastPacket = (image_size % packet_size);
    if (grab_parameter[device]->Image[i].SizeLastPacket == 0)
      grab_parameter[device]->Image[i].SizeLastPacket = packet_size;
    if (grab_parameter[device]->ring_buffer_count)
    {
        // When using Ring Buffer assign its virtual items addresses to each Image Buffer item
		// grab_parameter[device]->Image[i].pData = OsAllocMdl(&grab_parameter[device]->ring_buffer[i], grab_parameter[device]->isize);
        grab_parameter[device]->Image[i].pData = OsGetRingBufferAddress(rb);
        // Move to next item
		rb = rb->next;
    }
    else
      grab_parameter[device]->Image[i].pData = (unsigned char*)OsMalloc(grab_parameter[device]->isize);

    if (grab_parameter[device]->Image[i].pData == NULL)
       return OS_ALLOC_FAILURE;
      
    grab_parameter[device]->Image[i].pPacket = (unsigned char*)OsMalloc(grab_parameter[device]->nNumberOfPacket);
    if (grab_parameter[device]->Image[i].pPacket == NULL)
       return OS_ALLOC_FAILURE;
      
    /** gev_setup_parameters() should have been called before we run this */
    grab_parameter[device]->Image[i].ImageHeader.PixelType = grab_parameter[device]->pixel_format;
    grab_parameter[device]->Image[i].ImageHeader.SizeX = grab_parameter[device]->width;
    grab_parameter[device]->Image[i].ImageHeader.SizeY = grab_parameter[device]->height;
  } 

  grab_parameter[device]->current_buffer_count = 0;
  deque_init(&queue_parameter[device]->m_nQueuedList);

  for (i = 0; i < grab_parameter[device]->buffer_count; i++)
  {
    deque_push_back(&queue_parameter[device]->m_nQueuedList, &grab_parameter[device]->Image[i]);
  }
  deque_init(&queue_parameter[device]->m_nDeliverList);
  grab_parameter[device]->init_buffer = 1;
  return OS_OK;
}

void CloseImageBuffer(gev_camno_t device)
{
  int i;

  if (grab_parameter[device] == 0 || grab_parameter[device]->Image == NULL)
    return;

  // free image ring buffer 
  for (i = 0; i < grab_parameter[device]->buffer_count; i++)
  {
    if (grab_parameter[device]->ring_buffer_count)
    {
      OsFreeMdl(&grab_parameter[device]->ring_buffer[i]);
    }
    else
      OsFree((void *)grab_parameter[device]->Image[i].pData, grab_parameter[device]->isize);

    OsFree((void *)grab_parameter[device]->Image[i].pPacket, grab_parameter[device]->nNumberOfPacket);
    grab_parameter[device]->Image[i].pData = NULL;
    grab_parameter[device]->Image[i].pPacket = NULL;
  }

  if (grab_parameter[device]->init_buffer)
  {
    deque_close(&queue_parameter[device]->m_nQueuedList);
    deque_close(&queue_parameter[device]->m_nDeliverList);
  }
  OsFree((void *)grab_parameter[device]->Image, sizeof(IMAGE_BUFFER) * grab_parameter[device]->buffer_count);

  grab_parameter[device]->Image = NULL;

  grab_parameter[device]->init_buffer = 0;
}

PIMAGE_BUFFER EDDI_API get_clean_buffer(gev_camno_t device)
{
  PIMAGE_BUFFER img_buffer = NULL;

  img_buffer = deque_pop_front(&queue_parameter[device]->m_nQueuedList);

  if(NULL == img_buffer)
    TRACE_2x(M_WARNING, M_KIND, "There is no buffer available in the queue to store the data. (QueuedList: %d, DeliverList: %d)\n", queue_parameter[device]->m_nQueuedList.size, queue_parameter[device]->m_nDeliverList.size);

  return(img_buffer);
}

void InitTimeStats(gev_camno_t device)
{
  struct time_stats *ts = time_param(device);
  OsInitSpin(&ts->lock);
}

void ResetTimeStats(gev_camno_t device, os_time_t start, os_frequency_t freq)
{
  struct time_stats *ts = time_param(device);
  (void) start;
  ts->start = OS_TIME_UNDEF;
  ts->frequency = freq;
  ts->ongoing.img_cnt = 0;
  ts->ongoing.duration = 0;
  ts->last.img_cnt = 0;
  ts->last.duration = 0;
}

void CloseTimeStats(gev_camno_t device)
{
  struct time_stats *ts = time_param(device);
  OsCloseSpin(&ts->lock);
}

void GetTimeStats(gev_camno_t device, u_int32_t *img_cnt, u_int64_t *img_time)
{
  struct time_stats *ts = time_param(device);
  u_int64_t freq = ts->frequency ? ts->frequency : 1;
  TRACE_3x(M_DEBUG, M_KIND_SPHINX_GEV, "Time stats: last = (%d / %"PRId64"), ongoing = (%d)",
          ts->last.img_cnt, ts->last.duration, ts->ongoing.img_cnt);
  OsLockSpin(&ts->lock);
  if(ts->last.img_cnt == 0 || ts->last.duration == 0)
  {
    *img_cnt = ts->ongoing.img_cnt;
    *img_time = (1000ULL * ts->ongoing.duration) / freq;
  }
  else
  {
    *img_cnt = ts->last.img_cnt;
    *img_time = (1000ULL * ts->last.duration) / freq;
  }
  OsUnlockSpin(&ts->lock);
}

#if !defined(_KERNEL_MODE) && !defined(__KERNEL__)
double GetFpsFromTimeStats(gev_camno_t device)
{
  struct time_stats *ts = time_param(device);
  double freq = ts->frequency ? (double) ts->frequency : 1.0;
  u_int32_t img_cnt;
  double img_time;
  OsLockSpin(&ts->lock);
  if(ts->last.img_cnt == 0)
  {
    TRACE_1x(M_NOTICE, M_KIND_SPHINX_GEV, "Fps computation for camera %d hasn't stabilized yet. Using running average", device);
    img_cnt = ts->ongoing.img_cnt;
    img_time = (double) ts->ongoing.duration / freq;
  }
  else
  {
    img_cnt = ts->last.img_cnt;
    img_time = (double) ts->last.duration / freq;
  }
  OsUnlockSpin(&ts->lock);
  if (img_cnt && img_time != 0.0)
  {
    double averageDuration = img_time / (double) img_cnt;
    return 1.0 / averageDuration;
  }
  else
  {
    return 0.0;
  }
}
#endif

void ResetWorkspaceImageBuffer(gev_camno_t device, IMAGE_BUFFER** p) {
  if (*p) {
    deque_push_back(&queue_parameter[device]->m_nQueuedList, *p);
    *p = NULL;
  }
}

void ResetAcquisitionStatus(gev_camno_t device) {
  unsigned long grab_lock_flags;
  IMAGE_BUFFER* image_buffer = NULL;

  spin_lock_irqsave(&queue_parameter[device]->grab_lock, grab_lock_flags);
  ResetStatistics(device);

  grab_parameter[device]->BlockIDCheck = 1;
  grab_parameter[device]->PacketLarger = 0;

  ResetWorkspaceImageBuffer(device, &grab_parameter[device]->pImageBuffer);
  ResetWorkspaceImageBuffer(device, &grab_parameter[device]->pImageBufferBefore);
  ResetWorkspaceImageBuffer(device, &grab_parameter[device]->pImageBufferNext);

  while ((image_buffer = deque_pop_front(&queue_parameter[device]->m_nDeliverList))) {
    deque_push_back(&queue_parameter[device]->m_nQueuedList, image_buffer);
  }

  spin_unlock_irqrestore(&queue_parameter[device]->grab_lock, grab_lock_flags);
}

void ResetStatistics(gev_camno_t device) {
  grab_parameter[device]->img_error = 0;
  grab_parameter[device]->img_cnt = 0;
  grab_parameter[device]->missing_packets = 0;
  grab_parameter[device]->data_blocks_discarded_on_device = 0;
}

