/* os.h -- Operating systems specific header
 *
 * (C) 2021 by Sensor to Image GmbH
 *
 * Version: 1.0.1 Date: 22.02.2024
 * History:
 * 
 Version: 1.0.0 Date: 15.12.2021
 * - initial release
 *
 *
 Version: 1.0.1 Date: 23.02.2024
 * - Add OsRingBuffer structure
 *
 */
#ifndef __S2I_OS_H
#define __S2I_OS_H

#include "types.h"
#include "queue.h"
#include <linux/wait.h>
#include <linux/mutex.h>

os_time_t EDDI_API OsGetTickCount(void);

struct os_lockable_t;
struct os_signallable_t;
struct queue_param;

// Ring Buffer structure used for Linux Filter Driver
// The Ring Buffer is implemented as a queue
struct OsRingBuffer {
  struct page** pages;              // struct page, Linux physical page frame data structure
  unsigned long page_count;         // number of pages    
  void *virt_addr;
  unsigned long virt_addr_offset;
  struct OsRingBuffer* next;        // Pointer to next item, last item points to NULL
};

void EDDI_API OsInitSpin(os_lockable_t *q);
void EDDI_API OsCloseSpin(os_lockable_t *q);
void EDDI_API OsLockSpin(os_lockable_t *q);
void EDDI_API OsUnlockSpin(os_lockable_t *q);
                 
void EDDI_API OsInitSignal(os_signallable_t *s);
void EDDI_API OsCloseSignal(os_signallable_t *s);
void EDDI_API OsNotifySignal(os_signallable_t *s);

void EDDI_API *OsMalloc(size_t size);
void EDDI_API OsFree(void *memory, size_t size);

int EDDI_API OsPrintk(const char* fmt, ...);
void * EDDI_API OsMemSet(void *destination, int character, unsigned n);
void * EDDI_API OsMemCopy(void *destination, const void *source, unsigned n);
void * EDDI_API OsMalloc(size_t size);
void EDDI_API OsFree(void *memory, size_t size);

#define M_KIND M_KIND_GEV
#if defined(EURESYS_WITH_MEMENTO) || defined(EURESYS_HAS_MEMENTO)
int EDDI_API mc_SetupMemento(void);
void EDDI_API mc_CleanupMemento(void);
#else
#define mc_SetupMemento() 0
#define mc_CleanupMemento() do {} while (0)
#endif

#ifdef EURESYS_HAS_MEMENTO
#include <mementok.h>
#else
#ifndef EURESYS_HAS_OSAL_TYPES
#include <linux/kernel.h>
#endif
#define TRACE_TO_PRINTF OsPrintk
#define TRACEX_TO_PRINTF(l, ...)
#define TRACEX_TO_PRINTFM_DEBUG(l, ...)
#define M_ERROR KERN_ERR
#define M_WARNING KERN_WARNING
#define M_VERBOSE KERN_SOH "8"
#define M_DEBUG KERN_DEBUG
#define M_INFO KERN_INFO
#define M_NOTICE KERN_NOTICE
#define TRACE_EOL "\n"
#define TRACE_MACROS
#define TRACE_NOOP(fmt, ...) do {} while (0)

#define TRACE_LOGLEVELM_WARNING
#define TRACE_LOGLEVELM_ERROR
#define TRACE_LOGLEVELM_DEBUG
#define PRId64 "lld"

#include <s2i_trace.h>
#endif

#define llabs(x) ({ u_int64_t _x_ = (x); \
      ((_x_<0)?(-_x_):_x_); \
    })

#ifndef _LINUX_KERNEL_H
#define abs(x) ({ long _x_ = (x); \
      ((_x_<0)?(-_x_):_x_);       \
    })

#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#define htonl(x) __swap32(x)
#define htons(x) __swap16(x)
#define ntohl(x) __swap32(x)
#define ntohs(x) __swap16(x)
#endif
#endif

static inline void* OsAllocMdl(struct OsRingBuffer *buffer, u_int32_t size) {
  OsPrintk(KERN_WARNING "OsAllocMdl not supported\n");
  return NULL;
}

static inline void* OsGetRingBufferAddress(struct OsRingBuffer *buffer) {
  #if __linux__
    // OsPrintk ("OsSetRingBufferAddress virtual address: %p offset: %ld", buffer->virt_addr, buffer->virt_addr_offset);
    return buffer->virt_addr + buffer->virt_addr_offset; 
  #else
    OsPrintk(KERN_WARNING "OsGetRingBufferAddress not supported\n");
    return NULL;
  #endif
}


static inline void OsFreeMdl(struct OsRingBuffer *buffer) {
}

static inline u_int64_t OsTickDelta(os_time_t now, os_time_t then) {
  static const u_int64_t DEWRAP = 1ULL << 32;
  if (now < then) {
    return ((u_int64_t) now + DEWRAP) - then;
  } else {
    return now - then;
  }
}
os_frequency_t EDDI_API OsGetFrequency(void);

extern struct filter_param *filter_parameter[];
#define time_param(i) &(filter_parameter[i]->time)
#endif
 
