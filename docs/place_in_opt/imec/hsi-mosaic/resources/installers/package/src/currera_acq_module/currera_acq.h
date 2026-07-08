#include <linux/ioctl.h>

typedef struct {
	uint8_t bar_index;
	uint32_t offset;
	uint32_t size;
	uint32_t count; //ignored, assumed always 1
	void* buffer;
} currera_acq_rdreg_args;

typedef struct {
	uint8_t bar_index;
	uint32_t offset;
	uint32_t size;
	uint32_t count; //ignored, assumed always 1
	union {
		uint8_t size1;
		uint16_t size2;
		uint32_t size4;
	} data;
} currera_acq_wrreg_args;

typedef struct {
	uint32_t bytes;
	unsigned long addr;
	void* mappedpages;
	unsigned long pages;
	unsigned long index;
} currera_acq_unlockmem_args;

typedef struct {
	void* buffer;
	uint32_t bytes;
	uint32_t pages;
	uint8_t	write; //ignored, assumed always 0
	currera_acq_unlockmem_args* out;
} currera_acq_lockmem_args;

typedef	struct {
	void* addr;
	unsigned long physaddr;
	void* kernaddr;
	unsigned long index;
	void* unused1;
	void* unused2;
	uint32_t size;
} currera_acq_kernmem_args;

typedef struct {
	currera_acq_kernmem_args in;
	currera_acq_kernmem_args* out;
} currera_acq_getmem_args;

typedef struct {
	void* unused;
	uint32_t type;
	uint32_t bar_index;
	uint32_t offset;
	uint32_t reset;
	uint32_t mask;
} currera_acq_initintr_args;

#define CURRERA_ACQ_IOC_MAGIC 'x'

#define CURRERA_ACQ_IOCREADREG _IOWR(CURRERA_ACQ_IOC_MAGIC, 1, currera_acq_rdreg_args)
#define CURRERA_ACQ_IOCWRITEREG _IOW(CURRERA_ACQ_IOC_MAGIC, 2, currera_acq_wrreg_args)
#define CURRERA_ACQ_IOCLOCKMEM _IOWR(CURRERA_ACQ_IOC_MAGIC, 3, currera_acq_lockmem_args)
#define CURRERA_ACQ_IOCUNLOCKMEM _IOW(CURRERA_ACQ_IOC_MAGIC, 4, currera_acq_unlockmem_args)
#define CURRERA_ACQ_IOCGETMEM _IOWR(CURRERA_ACQ_IOC_MAGIC, 5, currera_acq_getmem_args)
#define CURRERA_ACQ_IOCRELEASEMEM _IOWR(CURRERA_ACQ_IOC_MAGIC, 6, currera_acq_getmem_args)
#define CURRERA_ACQ_IOCINITINTR _IOW(CURRERA_ACQ_IOC_MAGIC, 7, currera_acq_initintr_args)
#define CURRERA_ACQ_IOCDUMMY _IO(CURRERA_ACQ_IOC_MAGIC, 8)
#define CURRERA_ACQ_IOCREADREG_MULT _IOWR(CURRERA_ACQ_IOC_MAGIC, 9, currera_acq_rdreg_args)
