/*
 * XIMEA PCIe camera kernel driver.
 *
 * Copyright (C) 2015-2022 XIMEA, s.r.o.
 *
 * Author: Igor Kuzmin
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __APPLE__
#include <linux/ioctl.h>
#else
#include <sys/ioctl.h>
#endif

typedef struct {
	uint8_t bar_index;
	uint32_t offset;
	uint32_t size;
	uint32_t count; //ignored, assumed always 1
	union {
		void* buffer;
		uint64_t padding;
	};
} ximea_cam_pcie_rdreg_args;

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
} ximea_cam_pcie_wrreg_args;

typedef struct {
	uint32_t bytes;
	uint32_t padding0;
	uint64_t addr;
	union {
		void* mappedpages;
		uint64_t padding1;
	};
	uint64_t pages;
	uint64_t index;
} ximea_cam_pcie_unlockmem_args;

typedef struct {
	union {
		void* buffer;
		uint64_t padding0;
	};
	uint32_t bytes;
	uint32_t pages;
	uint64_t write; //ignored, assumed always 0
	union {
		ximea_cam_pcie_unlockmem_args* out;
		uint64_t padding1;
	};
} ximea_cam_pcie_lockmem_args;

typedef	struct {
	union {
		void* addr;
		uint64_t padding0;
	};
	uint64_t physaddr;
	union {
		void* kernaddr;
		uint64_t padding1;
	};
	uint64_t index;
	uint64_t unused1;
	uint64_t unused2;
	uint32_t size;
	uint32_t padding2;
} ximea_cam_pcie_kernmem_args;

typedef struct {
	ximea_cam_pcie_kernmem_args in;
	union {
		ximea_cam_pcie_kernmem_args* out;
		uint64_t padding;
	};
} ximea_cam_pcie_getmem_args;

typedef struct {
	uint64_t unused;
	uint32_t type;
	uint32_t bar_index;
	uint32_t offset;
	uint32_t reset;
	uint32_t mask;
	uint32_t padding;
} ximea_cam_pcie_initintr_args;

#define XIMEA_CAM_PCIE_IOC_MAGIC 'x'

#define XIMEA_CAM_PCIE_IOCREADREG _IOWR(XIMEA_CAM_PCIE_IOC_MAGIC, 1, ximea_cam_pcie_rdreg_args)
#define XIMEA_CAM_PCIE_IOCWRITEREG _IOW(XIMEA_CAM_PCIE_IOC_MAGIC, 2, ximea_cam_pcie_wrreg_args)
#define XIMEA_CAM_PCIE_IOCLOCKMEM _IOWR(XIMEA_CAM_PCIE_IOC_MAGIC, 3, ximea_cam_pcie_lockmem_args)
#define XIMEA_CAM_PCIE_IOCUNLOCKMEM _IOW(XIMEA_CAM_PCIE_IOC_MAGIC, 4, ximea_cam_pcie_unlockmem_args)
#define XIMEA_CAM_PCIE_IOCGETMEM _IOWR(XIMEA_CAM_PCIE_IOC_MAGIC, 5, ximea_cam_pcie_getmem_args)
#define XIMEA_CAM_PCIE_IOCRELEASEMEM _IOWR(XIMEA_CAM_PCIE_IOC_MAGIC, 6, ximea_cam_pcie_getmem_args)
#define XIMEA_CAM_PCIE_IOCINITINTR _IOW(XIMEA_CAM_PCIE_IOC_MAGIC, 7, ximea_cam_pcie_initintr_args)
#define XIMEA_CAM_PCIE_IOCDUMMY _IO(XIMEA_CAM_PCIE_IOC_MAGIC, 8)
#define XIMEA_CAM_PCIE_IOCREADREG_MULT _IOWR(XIMEA_CAM_PCIE_IOC_MAGIC, 9, ximea_cam_pcie_rdreg_args)
#define XIMEA_CAM_PCIE_IOCSYNCMEM4CPU _IOW(XIMEA_CAM_PCIE_IOC_MAGIC, 10, ximea_cam_pcie_unlockmem_args)
#define XIMEA_CAM_PCIE_IOCSYNCMEM4DEV _IOW(XIMEA_CAM_PCIE_IOC_MAGIC, 11, ximea_cam_pcie_unlockmem_args)
#define XIMEA_CAM_PCIE_IOCGETBUF _IOWR(XIMEA_CAM_PCIE_IOC_MAGIC, 13, ximea_cam_pcie_unlockmem_args)
