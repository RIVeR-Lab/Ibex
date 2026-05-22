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

#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/pagemap.h>
#include <linux/pci.h>
#include <linux/poll.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <linux/vmalloc.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,11,0)
	#include <linux/sched/signal.h>
#else
	#include <linux/sched.h>
#endif

#define SUPPORT_FOR_LEGACY_PCIE_YEAR_2015_ENABLED 0 // =1 if legacy PCIe devices (e.g. golden firmware from 2015) should be enumerated

#include "pcie_common_fpga_regs.h"
#ifdef HAVE_GPUDIRECT
#include "nv-p2p.h"
#define GPU_BOUND_SHIFT		16
#define GPU_BOUND_SIZE		((u64)1 << GPU_BOUND_SHIFT)
#define GPU_BOUND_OFFSET	(GPU_BOUND_SIZE - 1)
#define GPU_BOUND_MASK		(~GPU_BOUND_OFFSET)
#endif

#include "ximea_cam_pcie.h"

// compatibility with newer kernels
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0)
	#define __devinit
	#define __devexit
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,6,0)
	#define page_cache_release(page) put_page(page)
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,4,168) && LINUX_VERSION_CODE < KERNEL_VERSION(4,5,0)
	#define get_user_pages(a, b, c, d, e, f, g, h) get_user_pages(a, b, c, d, e ? FOLL_WRITE : 0, g, h)
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,6,0)
	#if LINUX_VERSION_CODE < KERNEL_VERSION(4,9,0)
		#define get_user_pages(a, b, c, d, e, f, g, h) get_user_pages(c, d, e, f, g, h)
	#elif LINUX_VERSION_CODE < KERNEL_VERSION(6,5,0)
		#define get_user_pages(a, b, c, d, e, f, g, h) get_user_pages(c, d, e ? FOLL_WRITE : 0, g, h)
	#else
		#define get_user_pages(a, b, c, d, e, f, g, h) get_user_pages(c, d, e ? FOLL_WRITE : 0, g)
	#endif
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,8,0)
	#undef DEFINE_DMA_ATTRS
	#define DEFINE_DMA_ATTRS(x) unsigned long x = 0
	#undef dma_set_attr
	#define dma_set_attr(a, b) (*(b) |= (a))
	#define dma_map_single_attrs(a, b, c, d, e) dma_map_single_attrs(a, b, c, d, *(e))
	#define dma_map_sg_attrs(a, b, c, d, e) dma_map_sg_attrs(a, b, c, d, *(e))
	#define dma_unmap_single_attrs(a, b, c, d, e) dma_unmap_single_attrs(a, b, c, d, *(e))
	#define dma_unmap_sg_attrs(a, b, c, d, e) dma_unmap_sg_attrs(a, b, c, d, *(e))
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,8,0)
	#define mmap_sem mmap_lock
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,18,0)
	#define pci_set_dma_mask(a, b) dma_set_mask(&(a)->dev, b)
	#define pci_set_consistent_dma_mask(a, b) dma_set_coherent_mask(&(a)->dev, b)
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6,4,0)
	#define class_create(a, b) class_create(b)
#endif

#define DRV_NAME "ximea_cam_pcie"
/* maximum number of BARs on the device */
#define SH_BAR_NUM 6

static unsigned int MAX_NUM_BUF = 300000;
module_param(MAX_NUM_BUF, uint, S_IRUGO);
MODULE_PARM_DESC(MAX_NUM_BUF, "Maximum number of buffers per device");

#define MAX_DEVICES 100
static atomic_t minors[MAX_DEVICES];
static dev_t chrdev;
#ifndef HAVE_GPUDIRECT
static struct class *devclass;
#endif

#ifdef HAVE_GPUDIRECT
struct sh_dev;
struct gpudirect_ctx {
	struct sh_dev *sh;
	unsigned long index;
};
#endif

struct sh_dev {
	/** the kernel pci device data structure provided by probe() */
	struct pci_dev *pci_dev;
	/* kernel virtual address of the mapped BAR memory and IO regions of
	 * the End Point. Used by map_bars()/unmap_bars().
	 */
	void * __iomem bar[SH_BAR_NUM];
	u32 bar_size[SH_BAR_NUM];
	/* whether this driver enabled msi for the device */
	int msi_enabled;
	/* irq line succesfully requested by this driver, -1 otherwise */
	int irq_line;
	struct cdev cdev;
	atomic_t avail;
#ifndef LOW_LATENCY
	atomic_t event;
#endif
	wait_queue_head_t wait;
	u32 event_count;
	u32 intr_bar;
	u32 intr_offset;
	u32 intr_reset;
	u32 intr_mask;
	u8 intr_type;
	struct page** *mappedpages;
	struct scatterlist **sgs;
	enum dma_data_direction *sg_dir;
	u32 *lockedpages;
	u64 *virtaddr;
#ifdef HAVE_GPUDIRECT
	struct gpudirect_ctx *unlock_ctx;
#endif
	unsigned long *physaddr;
	void* *kernaddr;
	u32 *memsize;
	struct semaphore memsem;
};

static unsigned int PCIe_AbortStreaming(void* ctx);
#define PCIe_SetReg(x, y, z) (((struct sh_dev*)(x))->bar_size[0] >= (y) * 4 + 4 ?  iowrite32((z), ((struct sh_dev*)(x))->bar[0] + (y) * 4), 0 : 1)
#define PCIe_GetReg(x, y, z) (((struct sh_dev*)(x))->bar_size[0] >= (y) * 4 + 4 ? *(z) = ioread32(((struct sh_dev*)(x))->bar[0] + (y) * 4), 0 : 1)
#define iSleep(x) udelay(1000 * (x))
#include "PCIe_AbortStreaming.inc"
#undef iSleep
#undef PCIe_GetReg
#undef PCIe_SetReg

void unlock_mem(struct sh_dev *sh, unsigned long index) {
	int i;
	DEFINE_DMA_ATTRS(attrs);
	dma_set_attr(DMA_ATTR_SKIP_CPU_SYNC, &attrs);

#ifdef HAVE_GPUDIRECT
	if(!sh->sgs[index]) { //GPU memory
		if(nvidia_p2p_put_pages(0, 0, sh->virtaddr[index], (nvidia_p2p_page_table_t*)sh->mappedpages[index]))
			printk(KERN_DEBUG DRV_NAME ": nvidia_p2p_put_pages failed!\n");
	} else { //CPU memory
#endif
	dma_unmap_sg_attrs(&sh->pci_dev->dev, sh->sgs[index], sh->lockedpages[index], sh->sg_dir[index], &attrs);
	for(i=0;i<sh->lockedpages[index];i++) {
		if(!PageReserved(sh->mappedpages[index][i]))
			SetPageDirty(sh->mappedpages[index][i]);
		page_cache_release(sh->mappedpages[index][i]);
	}
	vfree(sh->sgs[index]);
	sh->sgs[index] = 0;
	vfree(sh->mappedpages[index]);
#ifdef HAVE_GPUDIRECT
	}
#endif
	sh->lockedpages[index] = 0;
}

void release_mem(struct sh_dev *sh, unsigned long index) {
	int i;
	DEFINE_DMA_ATTRS(attrs);
	dma_set_attr(DMA_ATTR_SKIP_CPU_SYNC, &attrs);

	for(i=0;i<sh->memsize[index];i+=PAGE_SIZE) {
		ClearPageReserved(virt_to_page(sh->kernaddr[index]+i));
	}
	dma_unmap_single_attrs(&sh->pci_dev->dev, sh->physaddr[index], sh->memsize[index], DMA_TO_DEVICE, &attrs);
	free_pages((unsigned long)sh->kernaddr[index], get_order(sh->memsize[index]));
	sh->memsize[index] = 0;
}

#ifdef HAVE_GPUDIRECT
static void gpudirect_free_callback(void *param) {
	struct gpudirect_ctx *ctx = param;
	down(&ctx->sh->memsem);
	if(ctx->sh->lockedpages[ctx->index] && !ctx->sh->sgs[ctx->index]) {
		printk(KERN_DEBUG DRV_NAME ": automatically stopping DMA on GPU memory deallocation\n");
		if(!PCIe_AbortStreaming(ctx->sh))
			printk(KERN_DEBUG DRV_NAME ": PCIe_AbortStreaming failed!\n");
		nvidia_p2p_free_page_table((nvidia_p2p_page_table_t*)ctx->sh->mappedpages[ctx->index]);
		ctx->sh->lockedpages[ctx->index] = 0;
	} else
		printk(KERN_DEBUG DRV_NAME ": spurious gpudirect_free_callback\n");
	up(&ctx->sh->memsem);
}
#endif

static long ximea_cam_pcie_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
	struct sh_dev *sh = file->private_data;
	ximea_cam_pcie_rdreg_args rdreg;
	ximea_cam_pcie_wrreg_args wrreg;
	ximea_cam_pcie_lockmem_args lockmem;
	ximea_cam_pcie_unlockmem_args unlockmem;
	ximea_cam_pcie_getmem_args getmem;
	ximea_cam_pcie_initintr_args initintr;
	int c;
	unsigned int i, j = 0;
	int ret = -ENOTTY;
	void *bar;
	struct scatterlist *sg;
	struct vm_area_struct *vma;
	DEFINE_DMA_ATTRS(attrs);
	dma_set_attr(DMA_ATTR_SKIP_CPU_SYNC, &attrs);

	if(!(file->f_mode & FMODE_READ) && cmd != XIMEA_CAM_PCIE_IOCREADREG && cmd != XIMEA_CAM_PCIE_IOCGETBUF)
		return -EPERM;

	switch(cmd) {
	case XIMEA_CAM_PCIE_IOCREADREG:
		c = copy_from_user(&rdreg, (void*)arg, sizeof(ximea_cam_pcie_rdreg_args));
		if(c != 0 || rdreg.bar_index >= SH_BAR_NUM || sh->bar_size[rdreg.bar_index] < rdreg.offset+rdreg.size*rdreg.count)
			break;
		bar = sh->bar[rdreg.bar_index]+rdreg.offset;
		switch(rdreg.size) {
		case 1:
			if(put_user(ioread8(bar), (u8*)rdreg.buffer) == 0) ret = 0;
			break;
		case 2:
			if(put_user(ioread16(bar), (u16*)rdreg.buffer) == 0) ret = 0;
			break;
		case 4:
			if(put_user(ioread32(bar), (u32*)rdreg.buffer) == 0) ret = 0;
			break;
		}
		break;
	case XIMEA_CAM_PCIE_IOCREADREG_MULT:
		c = copy_from_user(&rdreg, (void*)arg, sizeof(ximea_cam_pcie_rdreg_args));
		if(c != 0 || rdreg.bar_index >= SH_BAR_NUM || sh->bar_size[rdreg.bar_index] < rdreg.offset+rdreg.size*rdreg.count)
			break;
		bar = sh->bar[rdreg.bar_index]+rdreg.offset;
		switch(rdreg.size) 
		{
		case 4:
			for(i=0;i<rdreg.count;i++) {
				//printk(KERN_DEBUG DRV_NAME "XIMEA_CAM_PCIE_IOCREADREG_MULT i %d/%d buff %x\n",i,rdreg.count,(u32)rdreg.buffer);
				if(put_user(ioread32(bar), (u32*)rdreg.buffer) == 0) {
					// ok
					rdreg.buffer += sizeof(u32);
				} else {
					// error on putting data to user
					break;
				}
			}
			if(i == rdreg.count)
				ret = 0;
			break;
		}
		break;
	case XIMEA_CAM_PCIE_IOCWRITEREG:
		c = copy_from_user(&wrreg, (void*)arg, sizeof(ximea_cam_pcie_wrreg_args));
		if(c != 0 || wrreg.bar_index >= SH_BAR_NUM || sh->bar_size[wrreg.bar_index] < wrreg.offset+wrreg.size)
			break;
		bar = sh->bar[wrreg.bar_index]+wrreg.offset;
		switch(wrreg.size) {
		case 1:
			iowrite8(wrreg.data.size1, bar);
			ret = 0;
			break;
		case 2:
			iowrite16(wrreg.data.size2, bar);
			ret = 0;
			break;
		case 4:
			iowrite32(wrreg.data.size4, bar);
			ret = 0;
			break;
		}
#ifdef LOW_LATENCY
		if(wrreg.offset == FPGA_PCIE_COMM_REG_ACQ_CTRL * 4) {
			sh->event_count = ioread32(sh->bar[0] + FPGA_PCIE_COMM_REG_STREAM_BUFF_DELIVERED * 4);
			//printk(KERN_DEBUG DRV_NAME ": event_count %u at cntrst\n", sh->event_count);
		}
#endif
		break;
	case XIMEA_CAM_PCIE_IOCLOCKMEM:
		if(down_interruptible(&sh->memsem))
			return -ERESTARTSYS;
		c = copy_from_user(&lockmem, (void*)arg, sizeof(ximea_cam_pcie_lockmem_args));
		for(i=0;i<MAX_NUM_BUF;i++)
			if(!sh->lockedpages[i])
				break;
		if(c != 0 || lockmem.pages <= 0 || i == MAX_NUM_BUF) {
			up(&sh->memsem);
			break;
		}
		unlockmem.index = i;
		unlockmem.pages = (unsigned long)(lockmem.buffer+lockmem.bytes-1)/PAGE_SIZE-(unsigned long)lockmem.buffer/PAGE_SIZE+1;
		if(unlockmem.pages > lockmem.pages) {
			up(&sh->memsem);
			break;
		}
		down_read(&current->mm->mmap_sem);
		vma = find_vma(current->mm, (unsigned long)lockmem.buffer);
		if(!vma || vma->vm_start > (unsigned long)lockmem.buffer) { //invalid address
			up_read(&current->mm->mmap_sem);
			up(&sh->memsem);
			break;
		}
		if(vma->vm_flags & (VM_WRITE | VM_READ)) { //buffer points to a valid CPU memory location
			up_read(&current->mm->mmap_sem);
			unlockmem.mappedpages = vmalloc(sizeof(struct page)*unlockmem.pages);
			if(unlockmem.mappedpages == 0) {
				up(&sh->memsem);
				break;
			}
			sh->sgs[unlockmem.index] = vmalloc(sizeof(*sh->sgs[unlockmem.index]) * unlockmem.pages);
			if(!sh->sgs[unlockmem.index]) {
				vfree(unlockmem.mappedpages);
				up(&sh->memsem);
				break;
			}
			sg_init_table(sh->sgs[unlockmem.index], unlockmem.pages);
			sh->sg_dir[unlockmem.index] = lockmem.write ? DMA_TO_DEVICE : DMA_FROM_DEVICE;
			down_read(&current->mm->mmap_sem);
			c = get_user_pages(current, current->mm, (unsigned long)lockmem.buffer/PAGE_SIZE*PAGE_SIZE, unlockmem.pages, 1, 0, (struct page**)unlockmem.mappedpages, 0);
			up_read(&current->mm->mmap_sem);
			if(c != unlockmem.pages) goto LOCK_CLEANUP;
			j = (unsigned long)lockmem.buffer % PAGE_SIZE; //offset
			for_each_sg(sh->sgs[unlockmem.index], sg, unlockmem.pages, i) {
				sg_set_page(sg, ((struct page**)unlockmem.mappedpages)[i], min(PAGE_SIZE - j, (unsigned long)lockmem.bytes), j);
				lockmem.bytes -= PAGE_SIZE - j;
				j = 0;
			}
			j = dma_map_sg_attrs(&sh->pci_dev->dev, sh->sgs[unlockmem.index], unlockmem.pages, sh->sg_dir[unlockmem.index], &attrs);
			if(!j)
				goto LOCK_CLEANUP;
			for_each_sg(sh->sgs[unlockmem.index], sg, j, i) {
				for(unlockmem.addr = sg_dma_address(sg);
						unlockmem.addr < sg_dma_address(sg) + sg_dma_len(sg);
						unlockmem.addr += unlockmem.bytes) {
					unlockmem.bytes = min(((unlockmem.addr >> PAGE_SHIFT) + 1) << PAGE_SHIFT, (u64)sg_dma_address(sg) + sg_dma_len(sg)) - unlockmem.addr;
					if(copy_to_user(lockmem.out++, &unlockmem, sizeof(ximea_cam_pcie_unlockmem_args)) != 0) goto LOCK_CLEANUP;
				}
			}
			sh->virtaddr[unlockmem.index] = (unsigned long)lockmem.buffer;
		} else { //assume GPU memory
			up_read(&current->mm->mmap_sem);
#ifdef HAVE_GPUDIRECT
			c = nvidia_p2p_get_pages(0, 0, (unsigned long)lockmem.buffer & GPU_BOUND_MASK,
					((unsigned long)(lockmem.buffer + lockmem.bytes + GPU_BOUND_SIZE - 1) & GPU_BOUND_MASK) -
					((unsigned long)lockmem.buffer & GPU_BOUND_MASK),
					(nvidia_p2p_page_table_t**)&unlockmem.mappedpages, gpudirect_free_callback, &sh->unlock_ctx[unlockmem.index]);
			if(c || ((nvidia_p2p_page_table_t*)unlockmem.mappedpages)->version != NVIDIA_P2P_PAGE_TABLE_VERSION ||
					((nvidia_p2p_page_table_t*)unlockmem.mappedpages)->page_size != NVIDIA_P2P_PAGE_SIZE_64KB ||
					((nvidia_p2p_page_table_t*)unlockmem.mappedpages)->entries != (((unsigned long)(lockmem.buffer + lockmem.bytes + GPU_BOUND_SIZE - 1) & GPU_BOUND_MASK) - ((unsigned long)lockmem.buffer & GPU_BOUND_MASK)) >> GPU_BOUND_SHIFT) {
				// EINVAL is returned when BAR space is exhausted or GPU doesn't support GPUDirect
				// checked with NVIDIA drivers 370.23
				if(c == -EINVAL || c == -ENOTSUPP) //include ENOTSUPP, though I'm not sure when it is used
					ret = -EINVAL;
				goto GPULOCK_CLEANUP;
			}
			i = j = 0;
			if((unsigned long)lockmem.buffer & GPU_BOUND_OFFSET) {
				unlockmem.bytes = PAGE_SIZE - ((unsigned long)lockmem.buffer % PAGE_SIZE);
				unlockmem.addr = ((nvidia_p2p_page_table_t*)unlockmem.mappedpages)->pages[0]->physical_address + ((unsigned long)lockmem.buffer & GPU_BOUND_OFFSET);
				if(copy_to_user(lockmem.out++, &unlockmem, sizeof(ximea_cam_pcie_unlockmem_args)) != 0) goto GPULOCK_CLEANUP;
				i = (((unsigned long)lockmem.buffer & GPU_BOUND_OFFSET) + PAGE_SIZE) >> GPU_BOUND_SHIFT;
				j = ((((unsigned long)lockmem.buffer & GPU_BOUND_OFFSET) + PAGE_SIZE) >> PAGE_SHIFT) % (GPU_BOUND_SIZE >> PAGE_SHIFT);
				lockmem.bytes -= unlockmem.bytes;
			}
			unlockmem.bytes = PAGE_SIZE;
			while(lockmem.bytes >= PAGE_SIZE) {
				for(; j < GPU_BOUND_SIZE >> PAGE_SHIFT && lockmem.bytes >= PAGE_SIZE; j++) {
					unlockmem.addr = ((nvidia_p2p_page_table_t*)unlockmem.mappedpages)->pages[i]->physical_address + j * PAGE_SIZE;
					if(copy_to_user(lockmem.out++, &unlockmem, sizeof(ximea_cam_pcie_unlockmem_args)) != 0) goto GPULOCK_CLEANUP;
					lockmem.bytes -= PAGE_SIZE;
				}
				if(j == GPU_BOUND_SIZE >> PAGE_SHIFT) {
					j = 0;
					i++;
				}
			}
			if(lockmem.bytes) {
				unlockmem.bytes = lockmem.bytes;
				unlockmem.addr = ((nvidia_p2p_page_table_t*)unlockmem.mappedpages)->pages[i]->physical_address + j * PAGE_SIZE;
				if(copy_to_user(lockmem.out++, &unlockmem, sizeof(ximea_cam_pcie_unlockmem_args)) != 0) goto GPULOCK_CLEANUP;
			}
			sh->virtaddr[unlockmem.index] = (unsigned long)lockmem.buffer & GPU_BOUND_MASK;
#else
			up(&sh->memsem);
			ret = -EOPNOTSUPP;
			break;
#endif
		}
		sh->lockedpages[unlockmem.index] = unlockmem.pages;
		sh->mappedpages[unlockmem.index] = (struct page**)unlockmem.mappedpages;
		up(&sh->memsem);
		ret = 0;
		break;
LOCK_CLEANUP:
		if(j)
			dma_unmap_sg_attrs(&sh->pci_dev->dev, sh->sgs[unlockmem.index], unlockmem.pages, sh->sg_dir[unlockmem.index], &attrs);
		for(;c>0;c--)
			page_cache_release(((struct page**)unlockmem.mappedpages)[c-1]);
		vfree(sh->sgs[unlockmem.index]);
		sh->sgs[unlockmem.index] = 0;
		vfree(unlockmem.mappedpages);
		up(&sh->memsem);
		break;
#ifdef HAVE_GPUDIRECT
GPULOCK_CLEANUP:
		if(!c)
			if(nvidia_p2p_put_pages(0, 0, (unsigned long)lockmem.buffer, (nvidia_p2p_page_table_t*)unlockmem.mappedpages))
				printk(KERN_DEBUG DRV_NAME ": nvidia_p2p_put_pages failed!\n");
		up(&sh->memsem);
		break;
#endif
	case XIMEA_CAM_PCIE_IOCUNLOCKMEM:
		if(down_interruptible(&sh->memsem))
			return -ERESTARTSYS;
		c = copy_from_user(&unlockmem, (void*)arg, sizeof(ximea_cam_pcie_unlockmem_args));
		if(c != 0 || unlockmem.index >= MAX_NUM_BUF || !sh->lockedpages[unlockmem.index] || unlockmem.pages != sh->lockedpages[unlockmem.index] || unlockmem.mappedpages != sh->mappedpages[unlockmem.index]) {
			up(&sh->memsem);
			break;
		}
		unlock_mem(sh, unlockmem.index);
		up(&sh->memsem);
		ret = 0;
		break;
	case XIMEA_CAM_PCIE_IOCGETBUF:
		if(copy_from_user(&unlockmem, (void*)arg, sizeof(ximea_cam_pcie_unlockmem_args)) != 0) break;

		if(down_interruptible(&sh->memsem))
			return -ERESTARTSYS;
		if(unlockmem.index >= MAX_NUM_BUF || !sh->lockedpages[unlockmem.index]) {
			up(&sh->memsem);
			break;
		}
		unlockmem.bytes = 0;
		for(i = 0; i < sh->lockedpages[unlockmem.index]; i++)
			unlockmem.bytes += sh->sgs[unlockmem.index][i].length;
		unlockmem.addr = sh->virtaddr[unlockmem.index];
		up(&sh->memsem);

		if(copy_to_user((void*)arg, &unlockmem, sizeof(ximea_cam_pcie_unlockmem_args)) != 0) break;
		ret = 0;
		break;
	case XIMEA_CAM_PCIE_IOCSYNCMEM4CPU:
		if(down_interruptible(&sh->memsem))
			return -ERESTARTSYS;
		c = copy_from_user(&unlockmem, (void*)arg, sizeof(ximea_cam_pcie_unlockmem_args));
		if(c != 0 || unlockmem.index >= MAX_NUM_BUF || !sh->lockedpages[unlockmem.index] || unlockmem.pages != sh->lockedpages[unlockmem.index] || unlockmem.mappedpages != sh->mappedpages[unlockmem.index]) {
			up(&sh->memsem);
			break;
		}
#ifdef HAVE_GPUDIRECT
		if(sh->sgs[unlockmem.index])
#endif
		dma_sync_sg_for_cpu(&sh->pci_dev->dev, sh->sgs[unlockmem.index], unlockmem.pages, sh->sg_dir[unlockmem.index]);
		up(&sh->memsem);
		ret = 0;
		break;
	case XIMEA_CAM_PCIE_IOCSYNCMEM4DEV:
		if(down_interruptible(&sh->memsem))
			return -ERESTARTSYS;
		c = copy_from_user(&unlockmem, (void*)arg, sizeof(ximea_cam_pcie_unlockmem_args));
		if(c != 0 || unlockmem.index >= MAX_NUM_BUF || !sh->lockedpages[unlockmem.index] || unlockmem.pages != sh->lockedpages[unlockmem.index] || unlockmem.mappedpages != sh->mappedpages[unlockmem.index]) {
			up(&sh->memsem);
			break;
		}
#ifdef HAVE_GPUDIRECT
		if(sh->sgs[unlockmem.index])
#endif
		dma_sync_sg_for_device(&sh->pci_dev->dev, sh->sgs[unlockmem.index], unlockmem.pages, sh->sg_dir[unlockmem.index]);
		up(&sh->memsem);
		ret = 0;
		break;
	case XIMEA_CAM_PCIE_IOCGETMEM:
		if(down_interruptible(&sh->memsem))
			return -ERESTARTSYS;
		c = copy_from_user(&getmem, (void*)arg, sizeof(ximea_cam_pcie_getmem_args));
		for(i=0;i<MAX_NUM_BUF;i++)
			if(!sh->memsize[i])
				break;
		if(c != 0 || getmem.in.size <= 0 || i == MAX_NUM_BUF) {
			up(&sh->memsem);
			break;
		}
		getmem.in.index = i;
		getmem.in.kernaddr = (void*)__get_free_pages(GFP_KERNEL, get_order(getmem.in.size));
		if(getmem.in.kernaddr == 0) {
			up(&sh->memsem);
			break;
		}
		getmem.in.physaddr = dma_map_single_attrs(&sh->pci_dev->dev, getmem.in.kernaddr, getmem.in.size, DMA_TO_DEVICE, &attrs);
		if(dma_mapping_error(&sh->pci_dev->dev, getmem.in.physaddr) ||
				copy_to_user(getmem.out, &getmem.in, sizeof(ximea_cam_pcie_kernmem_args)) != 0) {
			free_pages((unsigned long)getmem.in.kernaddr, get_order(getmem.in.size));
			up(&sh->memsem);
			break;
		}
		for(i=0;i<getmem.in.size;i+=PAGE_SIZE)
			SetPageReserved(virt_to_page(getmem.in.kernaddr+i));
		sh->memsize[getmem.in.index] = getmem.in.size;
		sh->physaddr[getmem.in.index] = getmem.in.physaddr;
		sh->kernaddr[getmem.in.index] = getmem.in.kernaddr;
		up(&sh->memsem);
		ret = 0;
		break;
	case XIMEA_CAM_PCIE_IOCRELEASEMEM:
		if(down_interruptible(&sh->memsem))
			return -ERESTARTSYS;
		c = copy_from_user(&getmem, (void*)arg, sizeof(ximea_cam_pcie_getmem_args));
		if(c != 0 || getmem.in.index >= MAX_NUM_BUF || !sh->memsize[getmem.in.index] || getmem.in.size != sh->memsize[getmem.in.index] || getmem.in.kernaddr != sh->kernaddr[getmem.in.index]) {
			up(&sh->memsem);
			break;
		}
		release_mem(sh, getmem.in.index);
		up(&sh->memsem);
		ret = 0;
		break;
	case XIMEA_CAM_PCIE_IOCINITINTR:
		c = copy_from_user(&initintr, (void*)arg, sizeof(ximea_cam_pcie_initintr_args));
		if(c != 0 || initintr.bar_index >= SH_BAR_NUM || sh->bar_size[initintr.bar_index] < initintr.offset+4 || initintr.type > 2)
			break;
		sh->intr_bar = initintr.bar_index;
		sh->intr_offset = initintr.offset;
		sh->intr_reset = initintr.reset;
		sh->intr_mask = initintr.mask;
		sh->intr_type = initintr.type;
		ret = 0;
		break;
	case XIMEA_CAM_PCIE_IOCDUMMY:
		ret = 0;
		break;
	}
	return ret;
}

int ximea_cam_pcie_mmap(struct file *filp, struct vm_area_struct *vma)
{
	struct sh_dev *sh = filp->private_data;
	int i;

	if(!(filp->f_mode & FMODE_READ))
		return -EPERM;

	for(i=0;i<MAX_NUM_BUF;i++)
		if(sh->memsize[i] && vma->vm_pgoff<<PAGE_SHIFT == sh->physaddr[i] && (vma->vm_end-vma->vm_start)>>PAGE_SHIFT == (sh->memsize[i]-1)/PAGE_SIZE+1)
			break;
	if(i == MAX_NUM_BUF)
		return -EINVAL;
	return remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff, vma->vm_end-vma->vm_start, vma->vm_page_prot);
}

static irqreturn_t ximea_cam_pcie_intr(int irq, void *dev_id)
{
	struct sh_dev *sh = (struct sh_dev *)dev_id;
	u32 reg;

	if(!sh || sh->intr_type > 2)
		return IRQ_NONE;
	reg = ioread32(sh->bar[sh->intr_bar]+sh->intr_offset);
	if(sh->intr_mask) {
		if(!(reg&sh->intr_mask))
			return IRQ_NONE;
	} else {
		if(!reg)
			return IRQ_NONE;
	}
	if(sh->intr_type)
		iowrite32(sh->intr_type == 2 ? reg : sh->intr_reset, sh->bar[sh->intr_bar]+sh->intr_offset);
#ifndef LOW_LATENCY
	atomic_inc(&sh->event);
#endif
	wake_up_interruptible(&sh->wait);
	return IRQ_HANDLED;
}

/* code below was almost entirely copied from uio driver */

static int ximea_cam_pcie_open(struct inode *inode, struct file *filep)
{
	struct sh_dev *sh = container_of(inode->i_cdev, struct sh_dev, cdev);

	if(filep->f_mode & FMODE_READ) {
		// check if some instance is opened
		if(!atomic_dec_and_test(&sh->avail)) {
			// error: other instance is opened
			atomic_inc(&sh->avail);
			return -EBUSY;
		}
#ifndef LOW_LATENCY
		atomic_set(&sh->event, 0);
		sh->event_count = 0;
#else
		sh->event_count = ioread32(sh->bar[0] + FPGA_PCIE_COMM_REG_STREAM_BUFF_DELIVERED * 4);
		//printk(KERN_DEBUG DRV_NAME ": event_count %u at open\n", sh->event_count);
#endif
	}
	filep->private_data = sh;
	return 0;
}

static int ximea_cam_pcie_release(struct inode *inode, struct file *filep)
{
	struct sh_dev *sh = filep->private_data;
	int i;

	if(!(filep->f_mode & FMODE_READ))
		return 0;

#ifdef HAVE_GPUDIRECT
	down(&sh->memsem);
#endif
	for(i=0;i<MAX_NUM_BUF;i++)
		if(sh->lockedpages[i] || sh->memsize[i])
			break;
	if(i < MAX_NUM_BUF) {
		printk(KERN_DEBUG DRV_NAME ": automatically stopping DMA on device release\n");
		if(!PCIe_AbortStreaming(sh))
			printk(KERN_DEBUG DRV_NAME ": PCIe_AbortStreaming failed!\n");
	}
	for(i=0;i<MAX_NUM_BUF;i++) {
		if(sh->lockedpages[i]) {
			//printk(KERN_DEBUG DRV_NAME ": automatically unlocking memory on device release\n");
			unlock_mem(sh, i);
		}
		if(sh->memsize[i]) {
			//printk(KERN_DEBUG DRV_NAME ": automatically releasing memory on device release\n");
			release_mem(sh, i);
		}
	}
#ifdef HAVE_GPUDIRECT
	up(&sh->memsem);
#endif
	atomic_inc(&sh->avail);
	return 0;
}

static unsigned int ximea_cam_pcie_poll(struct file *filep, poll_table *wait)
{
	struct sh_dev *sh = filep->private_data;
#ifdef LOW_LATENCY
#define MAX_LOOPS 1000000
#define DELAY_US 1
	unsigned int loops = MAX_LOOPS;
	u32 event_count;
#endif

	if(!(filep->f_mode & FMODE_READ))
		return POLLERR;

	poll_wait(filep, &sh->wait, wait);
#ifndef LOW_LATENCY
	if (sh->event_count != atomic_read(&sh->event))
		return POLLIN | POLLRDNORM;
#else
	do {
		event_count = ioread32(sh->bar[0] + FPGA_PCIE_COMM_REG_STREAM_BUFF_DELIVERED * 4);
		if(event_count != sh->event_count) {
			//printk(KERN_DEBUG DRV_NAME ": event_count %u after %u loops\n", event_count, MAX_LOOPS - loops + 1);
			return POLLIN | POLLRDNORM;
		}
		if(--loops) {
			yield();
			udelay(DELAY_US);
		}
	} while(loops);
#undef MAX_LOOPS
#undef DELAY_US
	//printk(KERN_DEBUG DRV_NAME ": sleeping\n");
#endif
	return 0;
}

static ssize_t ximea_cam_pcie_read(struct file *filep, char __user *buf,
			size_t count, loff_t *ppos)
{
	struct sh_dev *sh = filep->private_data;
#ifndef LOW_LATENCY
	DECLARE_WAITQUEUE(wait, current);
#endif
	ssize_t retval;
	s32 event_count;

	if (count != sizeof(s32))
		return -EINVAL;
#ifndef LOW_LATENCY
	add_wait_queue(&sh->wait, &wait);
	do {
		set_current_state(TASK_INTERRUPTIBLE);
		event_count = atomic_read(&sh->event);
		if (event_count != sh->event_count) {
			if (copy_to_user(buf, &event_count, count))
				retval = -EFAULT;
			else {
				sh->event_count = event_count;
				retval = count;
			}
			break;
		}
		if (filep->f_flags & O_NONBLOCK) {
			retval = -EAGAIN;
			break;
		}
		if (signal_pending(current)) {
			retval = -ERESTARTSYS;
			break;
		}
		schedule();
	} while (1);
	__set_current_state(TASK_RUNNING);
	remove_wait_queue(&sh->wait, &wait);
#else
	event_count = ioread32(sh->bar[0] + FPGA_PCIE_COMM_REG_STREAM_BUFF_DELIVERED * 4);
	if (copy_to_user(buf, &event_count, count))
		retval = -EFAULT;
	else {
		sh->event_count = event_count;
		//printk(KERN_DEBUG DRV_NAME ": event_count %u at read\n", sh->event_count);
		retval = count;
	}
#endif
	return retval;
}

static struct file_operations ximea_cam_pcie_fops = {
	.owner			= THIS_MODULE,
	.unlocked_ioctl	= ximea_cam_pcie_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl	= ximea_cam_pcie_ioctl,
#endif
	.mmap			= ximea_cam_pcie_mmap,
	.open			= ximea_cam_pcie_open,
	.release		= ximea_cam_pcie_release,
	.read			= ximea_cam_pcie_read,
	.poll			= ximea_cam_pcie_poll,
};

/* code below was heavily based on altpciechdma.c driver, some comments are out-of-date */

/*
 * Unmap the BAR regions that had been mapped earlier using map_bars()
 */
static void unmap_bars(struct sh_dev *sh, struct pci_dev *dev)
{
	int i;

	for (i = 0; i < SH_BAR_NUM; i++) {
		/* is this BAR mapped? */
		if (sh->bar[i]) {
			/* unmap BAR */
			pci_iounmap(dev, sh->bar[i]);
			sh->bar[i] = NULL;
			sh->bar_size[i] = 0;
		}
	}
}

/* Map the device memory regions into kernel virtual address space after
 * verifying their sizes respect the minimum sizes needed, given by the
 * bar_min_len[] array.
 */
static int __devinit map_bars(struct sh_dev *sh, struct pci_dev *dev)
{
	int rc;
	int i;

	/* iterate through all the BARs */
	for (i = 0; i < SH_BAR_NUM; i++) {
		unsigned long bar_start = pci_resource_start(dev, i);
		unsigned long bar_end = pci_resource_end(dev, i);
		unsigned long bar_length = bar_end - bar_start + 1;
		unsigned long bar_flags = pci_resource_flags(dev, i);
		sh->bar[i] = NULL;
		/* do not map BARs with address 0 */
		if (!bar_start || !bar_end) {
			sh->bar_size[i] = 0;
			continue;
		}
		printk(KERN_DEBUG "BAR%d 0x%08lx-0x%08lx flags 0x%08lx\n",
				i, bar_start, bar_end, bar_flags);
		/* map the device memory or IO region into kernel virtual
		 * address space */
		sh->bar[i] = pci_iomap(dev, i, bar_length);
		if (!sh->bar[i]) {
			printk(KERN_DEBUG "Could not map BAR #%d.\n", i);
			rc = -1;
			goto fail;
		}
		sh->bar_size[i] = bar_length;
		printk(KERN_DEBUG "BAR[%d] mapped at 0x%p with length %lu.\n", i,
				sh->bar[i], bar_length);
	}
	/* succesfully mapped all required BAR regions */
	rc = 0;
	goto success;
fail:
	/* unmap any BARs that we did map */
	unmap_bars(sh, dev);
success:
	return rc;
}

/* Called when the PCI sub system thinks we can control the given device.
 * Inspect if we can support the device and if so take control of it.
 *
 * Return 0 when we have taken control of the given device.
 *
 * - allocate board specific bookkeeping
 * - allocate coherently-mapped memory for the descriptor table
 * - enable the board
 * - verify board revision
 * - request regions
 * - query DMA mask
 * - obtain and request irq
 * - map regions into kernel address space
 */
static int __devinit probe(struct pci_dev *dev, const struct pci_device_id *id)
{
	int minor, rc;
	struct sh_dev *sh = NULL;
	u8 irq_pin, irq_line;
#ifndef HAVE_GPUDIRECT
	struct device *sysdev;
#endif
#ifdef HAVE_GPUDIRECT
	unsigned long i;
#endif

	printk(KERN_DEBUG "probe(dev = 0x%p, pciid = 0x%p)\n", dev, id);
	for(minor = 0; minor < MAX_DEVICES; minor++) {
		if(atomic_dec_and_test(&minors[minor]))
			break;
		atomic_inc(&minors[minor]);
	}
	if(minor == MAX_DEVICES) {
		printk(KERN_DEBUG "Maximum number of " DRV_NAME " devices (%d) exceeded, ignoring this one.\n", MAX_DEVICES);
		rc = -ENOMEM;
		goto err_minor;
	}
	/* allocate memory for per-board book keeping */
	sh = kzalloc(sizeof(struct sh_dev), GFP_KERNEL);
	if (!sh) {
		printk(KERN_DEBUG "Could not kzalloc()ate memory.\n");
		rc = -ENOMEM;
		goto err_sh;
	}
	sh->mappedpages = vzalloc(MAX_NUM_BUF * sizeof(*sh->mappedpages));
	sh->lockedpages = vzalloc(MAX_NUM_BUF * sizeof(*sh->lockedpages));
	sh->sgs			= vzalloc(MAX_NUM_BUF * sizeof(*sh->sgs        ));
	sh->sg_dir		= vzalloc(MAX_NUM_BUF * sizeof(*sh->sg_dir     ));
	sh->virtaddr	= vzalloc(MAX_NUM_BUF * sizeof(*sh->virtaddr   ));
#ifdef HAVE_GPUDIRECT
	sh->unlock_ctx	= vzalloc(MAX_NUM_BUF * sizeof(*sh->unlock_ctx ));
#endif
	sh->physaddr    = vzalloc(MAX_NUM_BUF * sizeof(*sh->physaddr   ));
	sh->kernaddr    = vzalloc(MAX_NUM_BUF * sizeof(*sh->kernaddr   ));
	sh->memsize     = vzalloc(MAX_NUM_BUF * sizeof(*sh->memsize    ));
	if (!sh->mappedpages || !sh->lockedpages || !sh->sgs || !sh->sg_dir || !sh->virtaddr ||
#ifdef HAVE_GPUDIRECT
            !sh->unlock_ctx ||
#endif
            !sh->physaddr || !sh->kernaddr || !sh->memsize) {
		printk(KERN_DEBUG "Could not vzalloc()ate memory.\n");
		rc = -ENOMEM;
		goto err_enable;
	}
#ifdef HAVE_GPUDIRECT
	for(i = 0; i < MAX_NUM_BUF; i++) {
		sh->unlock_ctx[i].sh = sh;
		sh->unlock_ctx[i].index = i;
	}
#endif
	sh->pci_dev = dev;
	init_waitqueue_head(&sh->wait);
	sh->intr_type = 255;
	atomic_set(&sh->avail, 1);
	sema_init(&sh->memsem, 1);
	dev_set_drvdata(&dev->dev, (void *)sh);
	printk(KERN_DEBUG "probe() sh = 0x%p\n", sh);
	/* enable device */
	rc = pci_enable_device(dev);
	if (rc) {
		printk(KERN_DEBUG "pci_enable_device() failed\n");
		goto err_enable;
	}
	/* enable bus master capability on device */
	pci_set_master(dev);
	rc = pci_request_regions(dev, DRV_NAME);
	/* could not request all regions? */
	if (rc) {
		/* assume device is in use (and do not disable it later!) */
		goto err_regions;
	}
	// @todo For now, disable 64-bit, because I do not understand the implications (DAC!)
	/* query for DMA transfer */
	/* @see Documentation/DMA-mapping.txt */
	if (!pci_set_dma_mask(dev, DMA_BIT_MASK(64))) {
		pci_set_consistent_dma_mask(dev, DMA_BIT_MASK(64));
		/* use 64-bit DMA */
		printk(KERN_DEBUG "Using a 64-bit DMA mask.\n");
	} else
		if (!pci_set_dma_mask(dev, DMA_BIT_MASK(32))) {
			printk(KERN_DEBUG "Could not set 64-bit DMA mask.\n");
			pci_set_consistent_dma_mask(dev, DMA_BIT_MASK(32));
			/* use 32-bit DMA */
			printk(KERN_DEBUG "Using a 32-bit DMA mask.\n");
		} else {
			printk(KERN_DEBUG "No suitable DMA possible.\n");
			/** @todo Choose proper error return code */
			rc = -1;
			goto err_mask;
		}
	/* map BARs */
	rc = map_bars(sh, dev);
	if (rc)
		goto err_map;
	/* enable message signaled interrupts */
	rc = pci_enable_msi(dev);
	/* could not use MSI? */
	if (rc) {
		/* resort to legacy interrupts */
		printk(KERN_DEBUG "Could not enable MSI interrupting.\n");
		sh->msi_enabled = 0;
		/* MSI enabled, remember for cleanup */
	} else {
		printk(KERN_DEBUG "Enabled MSI interrupting.\n");
		sh->msi_enabled = 1;
	}
	/** XXX check for native or legacy PCIe endpoint? */
	rc = pci_read_config_byte(dev, PCI_INTERRUPT_PIN, &irq_pin);
	/* could not read? */
	if (rc)
		goto err_irq;
	printk(KERN_DEBUG "IRQ pin #%d (0=none, 1=INTA#...4=INTD#).\n", irq_pin);
	/* @see LDD3, page 318 */
	rc = pci_read_config_byte(dev, PCI_INTERRUPT_LINE, &irq_line);
	/* could not read? */
	if (rc) {
		printk(KERN_DEBUG "Could not query PCI_INTERRUPT_LINE, error %d\n", rc);
		goto err_irq;
	}
	printk(KERN_DEBUG "IRQ line #%d.\n", irq_line);
	sh->irq_line = dev->irq;
	/* @see LDD3, page 259 */
	rc = request_irq(sh->irq_line, ximea_cam_pcie_intr, IRQF_SHARED, DRV_NAME, (void *)sh);
	if (rc) {
		printk(KERN_DEBUG "Could not request IRQ #%d, error %d\n", sh->irq_line, rc);
		sh->irq_line = -1;
		goto err_irq;
	}
	printk(KERN_DEBUG "Succesfully requested IRQ #%d with dev_id 0x%p\n", sh->irq_line, sh);
	cdev_init(&sh->cdev, &ximea_cam_pcie_fops);
	rc = cdev_add(&sh->cdev, MKDEV(MAJOR(chrdev), minor), 1);
	if(rc < 0) {
		printk(KERN_DEBUG "Char dev creation failed\n");
		goto err_cdev;
	}
#ifndef HAVE_GPUDIRECT
	sysdev = device_create(devclass, &dev->dev, sh->cdev.dev, NULL, "ximea%02d", minor);
	if(IS_ERR(sysdev)) {
		rc = PTR_ERR(sysdev);
		printk(KERN_DEBUG "Sys device creation failed\n");
		goto err_dev;
	}
#endif
	/* succesfully took the device */
	rc = 0;
	printk(KERN_DEBUG "probe() successful.\n");
	goto end;
#ifndef HAVE_GPUDIRECT
err_dev:
	cdev_del(&sh->cdev);
#endif
err_cdev:
	/* free allocated irq */
	free_irq(sh->irq_line, (void *)sh);
err_irq:
	if (sh->msi_enabled)
		pci_disable_msi(dev);
	/* unmap the BARs */
	unmap_bars(sh, dev);
err_map:
err_mask:
	/* disable the device if it is not in use */
	pci_disable_device(dev);
	pci_release_regions(dev);
err_regions:
err_enable:
	dev_set_drvdata(&dev->dev, NULL);
	vfree(sh->memsize);
	vfree(sh->kernaddr);
	vfree(sh->physaddr);
#ifdef HAVE_GPUDIRECT
	vfree(sh->unlock_ctx);
#endif
	vfree(sh->virtaddr);
	vfree(sh->sg_dir);
	vfree(sh->sgs);
	vfree(sh->lockedpages);
	vfree(sh->mappedpages);
	kfree(sh);
err_sh:
	atomic_inc(&minors[minor]);
err_minor:
end:
	return rc;
}

static void __devexit remove(struct pci_dev *dev)
{
	struct sh_dev *sh;
	int minor;

	printk(KERN_DEBUG "remove(0x%p)\n", dev);
	if ((dev == 0) || (dev_get_drvdata(&dev->dev) == 0)) {
		printk(KERN_DEBUG "remove(dev = 0x%p) dev->dev.driver_data = 0x%p\n", dev, dev_get_drvdata(&dev->dev));
		return;
	}
	sh = (struct sh_dev *)dev_get_drvdata(&dev->dev);
	printk(KERN_DEBUG "remove(dev = 0x%p) where dev->dev.driver_data = 0x%p\n", dev, sh);
	if (sh->pci_dev != dev) {
		printk(KERN_DEBUG "dev->dev.driver_data->pci_dev (0x%08lx) != dev (0x%08lx)\n",
				(unsigned long)sh->pci_dev, (unsigned long)dev);
	}
	/* remove character device */
	minor = MINOR(sh->cdev.dev);
#ifndef HAVE_GPUDIRECT
	device_destroy(devclass, sh->cdev.dev);
#endif
	cdev_del(&sh->cdev);
	/* free IRQ
	 * @see LDD3 page 279
	 */
	printk(KERN_DEBUG "Freeing IRQ #%d for dev_id 0x%08lx.\n", sh->irq_line, (unsigned long)sh);
	free_irq(sh->irq_line, (void *)sh);
	/* MSI was enabled? */
	if (sh->msi_enabled) {
		/* Disable MSI @see Documentation/MSI-HOWTO.txt */
		pci_disable_msi(dev);
		sh->msi_enabled = 0;
	}
	/* unmap the BARs */
	unmap_bars(sh, dev);
	pci_disable_device(dev);
	/* to be called after device disable */
	pci_release_regions(dev);
	dev_set_drvdata(&dev->dev, NULL);
	vfree(sh->memsize);
	vfree(sh->kernaddr);
	vfree(sh->physaddr);
#ifdef HAVE_GPUDIRECT
	vfree(sh->unlock_ctx);
#endif
	vfree(sh->virtaddr);
	vfree(sh->sg_dir);
	vfree(sh->sgs);
	vfree(sh->lockedpages);
	vfree(sh->mappedpages);
	kfree(sh);
	atomic_inc(&minors[minor]);
}

static const struct pci_device_id ids[] = {
#if SUPPORT_FOR_LEGACY_PCIE_YEAR_2015_ENABLED
	{ PCI_DEVICE(0x1556, 0x1100), }, //PLDA ID
#endif
	{ PCI_DEVICE(0xdeda, 0x4001), },
	{ PCI_DEVICE(0xdeda, 0x4002), },
	{ PCI_DEVICE(0xdeda, 0x4003), },
	{ PCI_DEVICE(0xdeda, 0x4004), },
	{ PCI_DEVICE(0xdeda, 0x8001), },
	{ PCI_DEVICE(0xdeda, 0x8002), },
	{ PCI_DEVICE(0xdeda, 0x8003), },
	{ PCI_DEVICE(0xdeda, 0x8004), },
	{ PCI_DEVICE(0xdeda, 0x4021), },
	{ PCI_DEVICE(0xdeda, 0x4022), },
	{ PCI_DEVICE(0xdeda, 0x4023), },
	{ PCI_DEVICE(0xdeda, 0x4024), },
	{ PCI_DEVICE(0xdeda, 0x8021), },
	{ PCI_DEVICE(0xdeda, 0x8022), },
	{ PCI_DEVICE(0xdeda, 0x8023), },
	{ PCI_DEVICE(0xdeda, 0x8024), },
	{ 0, }
};
MODULE_DEVICE_TABLE(pci, ids);

/* used to register the driver with the PCI kernel sub system
 * @see LDD3 page 311
 */
static struct pci_driver pci_driver = {
	.name = DRV_NAME,
	.id_table = ids,
	.probe = probe,
	.remove = remove,
	/* resume, suspend are optional */
};

/*
 * ximea_cam_pcie_init() - Module initialization, registers devices.
 */
static int __init ximea_cam_pcie_init(void)
{
	int i, rc;

	printk(KERN_DEBUG DRV_NAME " init()\n");
	rc = alloc_chrdev_region(&chrdev, 0, MAX_DEVICES, DRV_NAME);
	if(rc < 0) {
		printk(KERN_DEBUG "Char dev allocation failed\n");
		goto exit_chrdev;
	}
#ifndef HAVE_GPUDIRECT
	devclass = class_create(THIS_MODULE, DRV_NAME);
	if(IS_ERR(devclass)) {
		printk(KERN_DEBUG "Class creation failed\n");
		rc = PTR_ERR(devclass);
		goto exit_class;
	}
#endif
	for(i = 0; i < MAX_DEVICES; i++)
		atomic_set(&minors[i], 1);
	/* register this driver with the PCI bus driver */
	rc = pci_register_driver(&pci_driver);
	if (rc < 0)
		goto exit_pci;
	rc = 0;
	goto exit_norm;
exit_pci:
#ifndef HAVE_GPUDIRECT
	class_destroy(devclass);
exit_class:
#endif
	unregister_chrdev_region(chrdev, MAX_DEVICES);
exit_chrdev:
exit_norm:
	return rc;
}

/*
 * ximea_cam_pcie_exit() - Module cleanup, unregisters devices.
 */
static void __exit ximea_cam_pcie_exit(void)
{
	printk(KERN_DEBUG DRV_NAME " exit()\n");
	/* unregister this driver from the PCI bus driver */
	pci_unregister_driver(&pci_driver);
#ifndef HAVE_GPUDIRECT
	class_destroy(devclass);
#endif
	unregister_chrdev_region(chrdev, MAX_DEVICES);
}

MODULE_LICENSE("GPL");

module_init(ximea_cam_pcie_init);
module_exit(ximea_cam_pcie_exit);
