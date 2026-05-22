#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/poll.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/jiffies.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/fs.h>
#include <linux/pagemap.h>
#include <linux/version.h>
#include <asm/iomap.h>

#include "currera_acq.h"

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0)
	// these defines are removed from 3.8+ kernels
	#define __devinit
	#define __devexit
#endif

#define DRV_NAME "currera_acq"
/* maximum number of BARs on the device */
#define SH_BAR_NUM 6

#define MAX_NUM_BUF 4096

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
	atomic_t event;
	wait_queue_head_t wait;
	u32 event_count;
	u32 intr_bar;
	u32 intr_offset;
	u32 intr_reset;
	u32 intr_mask;
	u8 intr_type;
	struct page** mappedpages[MAX_NUM_BUF];
	u32 lockedpages[MAX_NUM_BUF];
	unsigned long physaddr[MAX_NUM_BUF];
	void* kernaddr[MAX_NUM_BUF];
	u32 memsize[MAX_NUM_BUF];
	struct semaphore memsem;
};

void unlock_mem(struct sh_dev *sh, unsigned long index) {
	int i;

	for(i=0;i<sh->lockedpages[index];i++) {
		if(!PageReserved(sh->mappedpages[index][i]))
			SetPageDirty(sh->mappedpages[index][i]);
		page_cache_release(sh->mappedpages[index][i]);
	}
	kfree(sh->mappedpages[index]);
	sh->lockedpages[index] = 0;
}

void release_mem(struct sh_dev *sh, unsigned long index) {
	int i;

	for(i=0;i<sh->memsize[index];i+=PAGE_SIZE) {
		ClearPageReserved(virt_to_page(sh->kernaddr[index]+i));
	}
	free_pages((unsigned long)sh->kernaddr[index], get_order(sh->memsize[index]));
	sh->memsize[index] = 0;
}

static long currera_acq_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
	struct sh_dev *sh = file->private_data;
	currera_acq_rdreg_args rdreg;
	currera_acq_wrreg_args wrreg;
	currera_acq_lockmem_args lockmem;
	currera_acq_unlockmem_args unlockmem;
	currera_acq_getmem_args getmem;
	currera_acq_initintr_args initintr;
	int c;
	unsigned int i;
	int ret = -ENOTTY;
	void *bar;

	switch(cmd) {
	case CURRERA_ACQ_IOCREADREG:
		c = copy_from_user(&rdreg, (void*)arg, sizeof(currera_acq_rdreg_args));
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
	case CURRERA_ACQ_IOCREADREG_MULT:
		c = copy_from_user(&rdreg, (void*)arg, sizeof(currera_acq_rdreg_args));
		if(c != 0 || rdreg.bar_index >= SH_BAR_NUM || sh->bar_size[rdreg.bar_index] < rdreg.offset+rdreg.size*rdreg.count)
			break;
		bar = sh->bar[rdreg.bar_index]+rdreg.offset;
		switch(rdreg.size) 
		{
		case 4:
			for(i=0;i<rdreg.count;i++) {
				//printk(KERN_DEBUG DRV_NAME "CURRERA_ACQ_IOCREADREG_MULT i %d/%d buff %x\n",i,rdreg.count,(u32)rdreg.buffer);
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
	case CURRERA_ACQ_IOCWRITEREG:
		c = copy_from_user(&wrreg, (void*)arg, sizeof(currera_acq_wrreg_args));
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
		break;
	case CURRERA_ACQ_IOCLOCKMEM:
		if(down_interruptible(&sh->memsem))
			return -ERESTARTSYS;
		c = copy_from_user(&lockmem, (void*)arg, sizeof(currera_acq_lockmem_args));
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
		unlockmem.mappedpages = kmalloc(sizeof(struct page)*unlockmem.pages, GFP_KERNEL);
		if(unlockmem.mappedpages == 0) {
			up(&sh->memsem);
			break;
		}
		down_read(&current->mm->mmap_sem);
		c = get_user_pages(current, current->mm, (unsigned long)lockmem.buffer/PAGE_SIZE*PAGE_SIZE, unlockmem.pages, 1, 0, (struct page**)unlockmem.mappedpages, 0);
		up_read(&current->mm->mmap_sem);
		if(c != unlockmem.pages) goto LOCK_CLEANUP;
		i = 0;
		if((unsigned long)lockmem.buffer % PAGE_SIZE != 0) {
			unlockmem.bytes = PAGE_SIZE - ((unsigned long)lockmem.buffer % PAGE_SIZE);
			unlockmem.addr = virt_to_phys(lockmem.buffer);
			if(copy_to_user(lockmem.out++, &unlockmem, sizeof(currera_acq_unlockmem_args)) != 0) goto LOCK_CLEANUP;
			i = 1;
		}
		unlockmem.bytes = PAGE_SIZE;
		for(;i<c-1;i++) {
			unlockmem.addr = page_to_phys(((struct page**)unlockmem.mappedpages)[i]);
			if(copy_to_user(lockmem.out++, &unlockmem, sizeof(currera_acq_unlockmem_args)) != 0) goto LOCK_CLEANUP;
		}
		unlockmem.bytes = (unsigned long)(lockmem.buffer + lockmem.bytes - 1) % PAGE_SIZE + 1;
		unlockmem.addr = page_to_phys(((struct page**)unlockmem.mappedpages)[i]);
		if(copy_to_user(lockmem.out++, &unlockmem, sizeof(currera_acq_unlockmem_args)) != 0) goto LOCK_CLEANUP;
		sh->lockedpages[unlockmem.index] = unlockmem.pages;
		sh->mappedpages[unlockmem.index] = (struct page**)unlockmem.mappedpages;
		up(&sh->memsem);
		ret = 0;
		break;
LOCK_CLEANUP:
		for(;c>=0;c--)
			page_cache_release(((struct page**)unlockmem.mappedpages)[c]);
		kfree(unlockmem.mappedpages);
		up(&sh->memsem);
		break;
	case CURRERA_ACQ_IOCUNLOCKMEM:
		if(down_interruptible(&sh->memsem))
			return -ERESTARTSYS;
		c = copy_from_user(&unlockmem, (void*)arg, sizeof(currera_acq_unlockmem_args));
		if(c != 0 || unlockmem.index >= MAX_NUM_BUF || !sh->lockedpages[unlockmem.index] || unlockmem.pages != sh->lockedpages[unlockmem.index] || unlockmem.mappedpages != sh->mappedpages[unlockmem.index]) {
			up(&sh->memsem);
			break;
		}
		unlock_mem(sh, unlockmem.index);
		up(&sh->memsem);
		ret = 0;
		break;
	case CURRERA_ACQ_IOCGETMEM:
		if(down_interruptible(&sh->memsem))
			return -ERESTARTSYS;
		c = copy_from_user(&getmem, (void*)arg, sizeof(currera_acq_getmem_args));
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
		for(i=0;i<getmem.in.size;i+=PAGE_SIZE)
			SetPageReserved(virt_to_page(getmem.in.kernaddr+i));
		getmem.in.physaddr = virt_to_phys(getmem.in.kernaddr);
		if(copy_to_user(getmem.out, &getmem.in, sizeof(currera_acq_kernmem_args)) != 0) {
			free_pages((unsigned long)getmem.in.kernaddr, get_order(getmem.in.size));
			up(&sh->memsem);
			break;
		}
		sh->memsize[getmem.in.index] = getmem.in.size;
		sh->physaddr[getmem.in.index] = getmem.in.physaddr;
		sh->kernaddr[getmem.in.index] = getmem.in.kernaddr;
		up(&sh->memsem);
		ret = 0;
		break;
	case CURRERA_ACQ_IOCRELEASEMEM:
		if(down_interruptible(&sh->memsem))
			return -ERESTARTSYS;
		c = copy_from_user(&getmem, (void*)arg, sizeof(currera_acq_getmem_args));
		if(c != 0 || getmem.in.index >= MAX_NUM_BUF || !sh->memsize[getmem.in.index] || getmem.in.size != sh->memsize[getmem.in.index] || getmem.in.kernaddr != sh->kernaddr[getmem.in.index]) {
			up(&sh->memsem);
			break;
		}
		release_mem(sh, getmem.in.index);
		up(&sh->memsem);
		ret = 0;
		break;
	case CURRERA_ACQ_IOCINITINTR:
		c = copy_from_user(&initintr, (void*)arg, sizeof(currera_acq_initintr_args));
		if(c != 0 || initintr.bar_index >= SH_BAR_NUM || sh->bar_size[initintr.bar_index] < initintr.offset+4 || initintr.type > 2)
			break;
		sh->intr_bar = initintr.bar_index;
		sh->intr_offset = initintr.offset;
		sh->intr_reset = initintr.reset;
		sh->intr_mask = initintr.mask;
		sh->intr_type = initintr.type;
		ret = 0;
		break;
	case CURRERA_ACQ_IOCDUMMY:
		ret = 0;
		break;
	}
	return ret;
}

int currera_acq_mmap(struct file *filp, struct vm_area_struct *vma)
{
	struct sh_dev *sh = filp->private_data;
	int i;

	for(i=0;i<MAX_NUM_BUF;i++)
		if(sh->memsize[i] && vma->vm_pgoff<<PAGE_SHIFT == sh->physaddr[i] && (vma->vm_end-vma->vm_start)>>PAGE_SHIFT == (sh->memsize[i]-1)/PAGE_SIZE+1)
			break;
	if(i == MAX_NUM_BUF)
		return -EINVAL;
	return remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff, vma->vm_end-vma->vm_start, vma->vm_page_prot);
}

static irqreturn_t currera_acq_intr(int irq, void *dev_id)
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
	atomic_inc(&sh->event);
	wake_up_interruptible(&sh->wait);
	return IRQ_HANDLED;
}

/* code below was almost entirely copied from uio driver */

static int currera_acq_open(struct inode *inode, struct file *filep)
{
	struct sh_dev *sh = container_of(inode->i_cdev, struct sh_dev, cdev);

	if(!atomic_dec_and_test(&sh->avail)) {
		atomic_inc(&sh->avail);
		return -EBUSY;
	}
	atomic_set(&sh->event, 0);
	sh->event_count = 0;
	filep->private_data = sh;
	return 0;
}

static int currera_acq_release(struct inode *inode, struct file *filep)
{
	struct sh_dev *sh = filep->private_data;
	uint32_t reg;
	uint32_t cnt = 300;
	int i;

	for(i=0;i<MAX_NUM_BUF;i++)
		if(sh->lockedpages[i] || sh->memsize[i])
			break;
	if(i < MAX_NUM_BUF && sh->bar_size[0] >= 44) {
		printk(KERN_DEBUG DRV_NAME ": automatically stopping DMA on device release\n");
#define INIT					0x0028
#define INIT_CONST_OUT			0x00010000
#define INIT_I2C				0x00000010
#define INIT_SPI				0x00000040
#define DMA1_INFO				0x001C
#define DMA1_INFO_SGMODE		0x00000010
#define DMA1_INFO_SG_STOP		0x00000020
#define DMA1_INFO_SG_STATE_IDLE	0x00000000
#define DMA1_INFO_ABORT			0x00000008
#define DMA1_INFO_STATUS_BUSY	8
#define DMA1_INFO_SG_STATE			0x00007000
#define DMA1_INFO_SG_STATE_OFFSET	12
#define GET_DMA1_INFO_SG_STATE(val)	((val&DMA1_INFO_SG_STATE) >> DMA1_INFO_SG_STATE_OFFSET)
#define DMA1_INFO_STATUS			0x00000F00
#define GET_DMA1_INFO_STATUS(a)		((a&DMA1_INFO_STATUS) >> 8)
		if(sh->pci_dev->vendor == 0x20f7){ // don't know how to stop FPGA yet and no SG_STOP/SG_STATE on new model
			iowrite32(0xFFFFFFFF & ~INIT_CONST_OUT & ~INIT_I2C & ~INIT_SPI, sh->bar[0]+INIT); // Stop FPGA
			iowrite32(DMA1_INFO_SGMODE | DMA1_INFO_SG_STOP, sh->bar[0]+DMA1_INFO); // Need to stop DMA
			do {
				reg = ioread32(sh->bar[0]+DMA1_INFO);
				printk(KERN_DEBUG DRV_NAME ": - DMA_INFO stop reg=%08X\n", reg);
			} while(GET_DMA1_INFO_SG_STATE(reg) != DMA1_INFO_SG_STATE_IDLE && cnt--); // Wait for dma comes to idle.
		} else {
			cnt = 0;
		}
		if(!cnt) {
			iowrite32(DMA1_INFO_ABORT, sh->bar[0]+DMA1_INFO); // Need to abort DMA
			cnt = 300;
			do {
				reg = ioread32(sh->bar[0]+DMA1_INFO);
				printk(KERN_DEBUG DRV_NAME ": - DMA_INFO abort reg=%08X\n", reg);
			} while(GET_DMA1_INFO_STATUS(reg) >= DMA1_INFO_STATUS_BUSY && cnt--); // Wait for dma comes to idle.
		}
	}
	for(i=0;i<MAX_NUM_BUF;i++) {
		if(sh->lockedpages[i]) {
			printk(KERN_DEBUG DRV_NAME ": automatically unlocking memory on device release\n");
			unlock_mem(sh, i);
		}
		if(sh->memsize[i]) {
			printk(KERN_DEBUG DRV_NAME ": automatically releasing memory on device release\n");
			release_mem(sh, i);
		}
	}
	atomic_inc(&sh->avail);
	return 0;
}

static unsigned int currera_acq_poll(struct file *filep, poll_table *wait)
{
	struct sh_dev *sh = filep->private_data;

	poll_wait(filep, &sh->wait, wait);
	if (sh->event_count != atomic_read(&sh->event))
		return POLLIN | POLLRDNORM;
	return 0;
}

static ssize_t currera_acq_read(struct file *filep, char __user *buf,
			size_t count, loff_t *ppos)
{
	struct sh_dev *sh = filep->private_data;
	DECLARE_WAITQUEUE(wait, current);
	ssize_t retval;
	s32 event_count;

	if (count != sizeof(s32))
		return -EINVAL;
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
	return retval;
}

static struct file_operations currera_acq_fops = {
	.owner			= THIS_MODULE,
	.unlocked_ioctl	= currera_acq_ioctl,
	.mmap			= currera_acq_mmap,
	.open			= currera_acq_open,
	.release		= currera_acq_release,
	.read			= currera_acq_read,
	.poll			= currera_acq_poll,
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
	int rc;
	struct sh_dev *sh = NULL;
	u8 irq_pin, irq_line;
	dev_t chrdev;

	printk(KERN_DEBUG "probe(dev = 0x%p, pciid = 0x%p)\n", dev, id);
	/* allocate memory for per-board book keeping */
	sh = kzalloc(sizeof(struct sh_dev), GFP_KERNEL);
	if (!sh) {
		printk(KERN_DEBUG "Could not kzalloc()ate memory.\n");
		rc = -ENOMEM;
		goto err_sh;
	}
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
	rc = /*pci_enable_msi(dev)*/1;
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
	irq_line = dev->irq;
	/* @see LDD3, page 259 */
	rc = request_irq(irq_line, currera_acq_intr, IRQF_SHARED, DRV_NAME, (void *)sh);
	if (rc) {
		printk(KERN_DEBUG "Could not request IRQ #%d, error %d\n", irq_line, rc);
		sh->irq_line = -1;
		goto err_irq;
	}
	/* remember which irq we allocated */
	sh->irq_line = (int)irq_line;
	printk(KERN_DEBUG "Succesfully requested IRQ #%d with dev_id 0x%p\n", irq_line, sh);
	rc = alloc_chrdev_region(&chrdev, 0, 1, DRV_NAME);
	if(rc < 0) {
		printk(KERN_DEBUG "Char dev allocation failed\n");
		goto err_chrdev;
	}
	cdev_init(&sh->cdev, &currera_acq_fops);
	sh->cdev.owner = THIS_MODULE;
	rc = cdev_add(&sh->cdev, chrdev, 1);
	if(rc < 0) {
		printk(KERN_DEBUG "Char dev creation failed\n");
		goto err_cdev;
	}
	/* succesfully took the device */
	rc = 0;
	printk(KERN_DEBUG "probe() successful.\n");
	goto end;
err_cdev:
	unregister_chrdev_region(sh->cdev.dev, 1);
err_chrdev:
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
	kfree(sh);
err_sh:
end:
	return rc;
}

static void __devexit remove(struct pci_dev *dev)
{
	struct sh_dev *sh;

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
	cdev_del(&sh->cdev);
	unregister_chrdev_region(sh->cdev.dev, 1);
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
	kfree(sh);
}

static const struct pci_device_id ids[] = {
	{ PCI_DEVICE(0x20f7, 0x1100), }, //Currera
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
 * currera_acq_init() - Module initialization, registers devices.
 */
static int __init currera_acq_init(void)
{
	int rc;

	printk(KERN_DEBUG DRV_NAME " init(), built at " __DATE__ " " __TIME__ "\n");
	/* register this driver with the PCI bus driver */
	rc = pci_register_driver(&pci_driver);
	if (rc < 0)
		return rc;
	return 0;
}

/*
 * currera_acq_exit() - Module cleanup, unregisters devices.
 */
static void __exit currera_acq_exit(void)
{
	printk(KERN_DEBUG DRV_NAME " exit(), built at " __DATE__ " " __TIME__ "\n");
	/* unregister this driver from the PCI bus driver */
	pci_unregister_driver(&pci_driver);
}

MODULE_LICENSE("GPL");

module_init(currera_acq_init);
module_exit(currera_acq_exit);
