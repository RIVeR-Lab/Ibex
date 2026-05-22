/* queue.h -- Queue header
 *
 * (C) 2021 by Sensor to Image GmbH
 *
 * Version: 1.0.0 Date: 15.12.2021
 * History:
 * Version: 1.0.0 Date: 15.12.2021
 * - initial realease
 */
#ifndef __S2I_QUEUE_H
#define __S2I_QUEUE_H
#include <linux/wait.h>
#include <linux/mutex.h>

struct _IMAGE_BUFFER; // <image_buffer.h>
typedef struct _IMAGE_BUFFER IMAGE_BUFFER;

typedef struct os_lockable {
  spinlock_t lock;
  unsigned long flags;
} os_lockable_t;

typedef struct os_signallable {
  wait_queue_head_t signal;
} os_signallable_t;

struct QueueBuffer {
  os_lockable_t lock;
  IMAGE_BUFFER *front;
  IMAGE_BUFFER *last;
  unsigned int size;
  os_signallable_t newBuffer;
};

enum {
  GRAB_FLAG_STOPPED=0,
  GRAB_FLAG_RUNNING,
  GRAB_FLAG_STOPPING
};

struct queue_param {
  spinlock_t grab_lock;
  atomic_t grab_flag;
  wait_queue_head_t grab_wait;
  struct mutex read_mutex;
  struct QueueBuffer m_nQueuedList;
  struct QueueBuffer m_nDeliverList;
};
#endif
