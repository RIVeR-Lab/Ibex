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

/* s2i_filter netfilter modul
 *
 * (C) 2021 by Sensor to Image GmbH
 *
 * Version: 2.7.5 Date: 22.02.2024
 * History:
 * Version: 1.0 Date: 11.03.2009
 * - initial realease
 * Version: 1.1 Date: 25.06.2009
 * - remove wait_event_interruptible and insert interruptible_sleep_on_timeout in read function to get an timeout when
 *   no image data is incomming   
 * - change in strucht image_fps_io the variable image_fps form unsigned long to u_int32_t. On 64 bit linux we got always 0
 * - check in main_hook function CAMERA_COUNT instead cam_cnt
 *
 * Version: 1.2 Date: 14.08.2009
 * - check pointer in main_hook function
 * - check cam_nr in cancamgige_read function
 *
 * Version: 1.3 Date: 25.09.2009
 * - resend packet with dev_queue_xmit function
 * - blocks in-buffer when user read from same as out-buffer
 *
 * Version: 1.4 Date: 06.10.2009
 * - get packetsize in IOCTL_CMD_START_GRAB command
 * - if jumbo frames comming in, the data are not linearly
 *
 * Version: 1.5 Date: 23.10.2009
 * - insert grab end event
 * - insert get driver version in cancamgige_ioctl function
 * - remove bug to set buffer event every time
 * - increment measure image count only when image is ok
 *
 * Version: 1.0.6 Date: 20.01.2010
 * - change filter driver parameter
 * - check image with width, height and pixelformat parameter
 *
 * Version: 1.0.7 Date: 04.02.2010
 * - get image parameter from stream
 *- insert get_image_header in FltDevIoControl function

 * Version: 1.0.8 Date: 07.04.2010
 * - grab event for every buffer
 * - change measure time

 * Version: 2.0.0 Date: 17.03.2011
 * - change buffer management
 * - change filter driver parameter 
 * - remove measure frames per seconds
 * - remove bug measure number of packets
 * - change buffer and error handling 
 * - change driver name to s2igevfilter

 * Version: 2.0.3 Date: 02.08.2011
 * - change packet resend implementation

 * Version: 2.0.4 Date: 15.05.2012
 * - check resend packets in next frame
 * - change minor version to minor and sub minor X.X.X
 * - added PaddingX in s2iGEVFilter_read function
 * - set padding parameter to 0, when the image header is corrupt
 * - changed buffer handling
 * - insert extended ID (GigEVision Specifikation 2.0)

 * Version: 2.0.5 Date: 12.12.2012
 * - changed buffer handling
 * - wait of end event (drop packets) when stop grabbing

 * Version: 2.0.6 Date: 13.12.2013
 * - remove bug in check next block id 
  
 * Version: 2.0.8 Date: 09.09.2014
 * - get absolute value of packet id 
 * - increment device/camera count to 20
  
 * Version: 2.0.9 Date: 19.01.2015
 * - added BlockIDMask
 * - change check missing frames
  
 * Version: 2.1.0 Date: 11.11.2015
 * - check grab_flag before set end event
 * - check payload when width, height and pixel format == 0 in start grab
 * - when lost packets at trailer, wait next frame of packets 
 * - added set buffer count
 * - added set packets out of order
 * - added test packet resend
 * - remove bug in resend_packet. (mac header)
 
 * Version: 2.1.1 Date: 26.01.2017
 * - added pImageBufferBeforeBefore and pImageBufferNext;
 
 * Version: 2.1.2 Date: 08.03.2017
 * - support extended chunk data payload type
 * - support multi-part data payload type

 * Version: 2.1.3 Date: 29.06.2017
 * - return image time and image time cnt in IOCTL_CMD_GET_STATUS
 * - added calculation of frames per second
 * - change buffer handling
 * - supported more than one packet to test packet resend
 * - no error when leader SizeY != trailer SizeY
 * - fill new IMAGE_HEADER parameter
 * - supported image extended chunk and chunk payload type
 * - make nodes in /dev by driver 
 
 * Version: 2.1.4 Date: 22.08.2018
 * - added payload type defines
 * - check of  GEV_FLAG_PREVIOUS_BLOCK_DROPPED flag
 * - return data blocks discarded on device count in ioctl get status command

 * Version: 2.2.0 Date: 09.07.2019
 * - added packet resend info to image header
 * - buffer handling has been changed
 * - the address offset is now used to sort multipart data packets

 * Version: 2.3.0 Date: 08.01.2020
 * - check if trailer sizeX smaller than leader sizeX
 * - copy image header after change SizeY with TrailerSizeY
 * - added GenDC support
 * - set current number of packets for GenDC
 * - check the write index of pPacket array

 * Version: 2.4.0 Date: 05.05.2020
 * - do_gettimeofday no longer available from kernel 3.18
 * - nf_register_hook/nf_unregister_hook has been replaced by the function nf_register_net_hook/nf_unregister_net_hook from kernel 4.13
 * - increase the size after allocated NodeBuffer in deque functions
 * - decrease the size after free NodeBuffer in deque functions
 * - clear new image event before wait
 * - added PacketIndexSav to IMAGE_BUFFER struct
 * - changed packet resend handling
 * - check missing frames in set_image function
 * - changed buffer handling

 * Version: 2.5.0 Date: 13.01.2021
 * - return the leader parameter if feature and leader parameter not equal
 * - set current missing packets when leader SizeY != trailer SizeY
 * - now supports 50 devices
 * - moved the gige vision specific code in separate files outsourced, to create precompiled file
 * - if it's a multicast packet than set in all open multicast devices
 * - fixed mutex issue

 * Version: 2.6.0 Date: 26.05.2021
 * - fixed queue handling
 * - Access to queues is not thread safe, use spin lock 
 * - migrate the next pointer into IMAGE_BUFFER and stop allocating/freeing NodeBuffer structures.
 * - removed unused function deque_front
 * - check if pPacket are correct allocated 
 * - fixed kernel panic due to binary blob interface using kernel structures
 * - check if we can frees the buffers in CloseImageBuffer
 * - fixed, struct timeval is no longer defined in the kernel kernel 5.8.0
 * - remove the two extra buffer space in InitImageBuffer
 
 * Version: 2.7.0 Date: 15.12.2021
 * - refactoring 
 
 * Version: 2.7.5 Date: 22.02.2024
   - Add ring buffer support
   - Use GPL License to support ring buffer functions

make clean
make
sudo make install
sudo modprobe s2igevfilter

sudo cp s2igevfilter.ko /lib/modules/3.13.0-101-generic/extra
sudo insmod s2igevfilter.ko
sudo rmmod s2igevfilter

tail -f /var/log/kern.log & 


 */
#include <linux/ip.h>            
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/netdevice.h>     
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/skbuff.h>        
#include <linux/udp.h>                   
#include <linux/vmalloc.h>
#include <linux/delay.h>
#include <linux/wait.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,18,0)
#include <linux/ktime.h>
#else
#include <linux/time.h>
#endif
  
#include <linux/mutex.h>

#include <net/ip.h>
#include <net/udp.h>
#include <linux/netfilter_ipv4.h>

#include <s2i_gev.h>   
#include <s2i/queue.h>
#include <s2i/os.h>
#include <s2i/s2i_io.h>                  
#include <s2i/s2i_utils.h>                   
#include <s2i/image_buffer.h>

#include "s2i_filter.h" 

MODULE_AUTHOR("Sensor to Image GmbH");
MODULE_DESCRIPTION("s2i GigE-Vision netfilter module");

// Required GPL Licence: Ring Buffer functions uses GPL-only symbols: 'get_user_pages_fast', '__class_create', 'class_destroy', 'device_create', 'net_namespace_list', 'device_destroy'
MODULE_LICENSE("GPL");
//MODULE_LICENSE("Proprietary");

struct grab_param *grab_parameter[CAMERA_COUNT];
struct filter_param *filter_parameter[CAMERA_COUNT];
struct resend_packet_param *resend_packet_parameter[CAMERA_COUNT];
struct queue_param *queue_parameter[CAMERA_COUNT];

static unsigned char cam_cnt = 0;
static unsigned char opened = 0;
static unsigned char nonblock = 0;

static struct nf_hook_ops netfilter_ops;                       

static struct class *s2igevfilter_class; 

#if LINUX_VERSION_CODE>= KERNEL_VERSION(2,6,24) 
unsigned int main_hook(unsigned int hooknum,
                  struct sk_buff *skb,
                  const struct net_device *in,
                  const struct net_device *out,
                  int (*okfn)(struct sk_buff*))
#else
unsigned int main_hook(unsigned int hooknum,
                  struct sk_buff **skb,
                  const struct net_device *in,
                  const struct net_device *out,
                  int (*okfn)(struct sk_buff*))
#endif
{
  gev_camno_t i;
  void *p;
  struct sk_buff *sock_buff = NULL;
  struct udphdr *udp_header;                             
  struct iphdr *ip_header;
  unsigned int ret = NF_ACCEPT;
  unsigned long grab_lock_flags;

  if(cam_cnt == 0)
    return(ret);

#if LINUX_VERSION_CODE>= KERNEL_VERSION(2,6,24) 
  sock_buff = skb;
#else
  sock_buff = *skb;
#endif

  if(!sock_buff)
   return (ret);                  

  // If jumbo frames comming in, the data are not linearly
  // skb_linearize checks if sock_buff is linear, when not than make linearly
  skb_linearize(sock_buff);

  // get ip header (works on all linux versions)
  ip_header = (struct iphdr *)ip_hdr(sock_buff);    

  if(ip_header == NULL)
    return (ret);

  if(ip_header->protocol != 17)
    return (ret); 

  // get udp header
  p = (void *) ip_header + sizeof( struct iphdr );
  udp_header = (struct udphdr *) p;

  if(udp_header == NULL)
    return (ret);

  for(i = 0;i < CAMERA_COUNT;i++)
  {
    //Must lockout stop grab from deallocating resources while this is running
    // only check if acquisition is in progress
    if (atomic_read_acquire(&queue_parameter[i]->grab_flag) == GRAB_FLAG_RUNNING)
    {
      spin_lock_irqsave(&queue_parameter[i]->grab_lock, grab_lock_flags);
      if (atomic_read_acquire(&queue_parameter[i]->grab_flag) == GRAB_FLAG_RUNNING) {
        if((ntohs(udp_header->dest) == filter_parameter[i]->CamPort) && (ip_header->saddr == filter_parameter[i]->CamIp) && (ip_header->daddr == filter_parameter[i]->adapter_ip))
        {
          // set packet resend parameter
          if(resend_packet_parameter[i]->init_param == 0) 
          {
            memcpy(resend_packet_parameter[i]->ip_daddr,&ip_header->saddr,4);
            memcpy(resend_packet_parameter[i]->ip_saddr,&ip_header->daddr,4);
            resend_packet_parameter[i]->init_param = 1;
          }                  
         
          ret = gev_process(i, sock_buff, sock_buff->data + sizeof( struct iphdr ) + sizeof( struct udphdr ), skb_headlen(sock_buff));

          // if not multicast address than exit
          //  multicast addresses are defined by the most-significant bit pattern of 1110.
          if ((filter_parameter[i]->adapter_ip & 0x000000F0) != 0x000000E0) {
            spin_unlock_irqrestore(&queue_parameter[i]->grab_lock, grab_lock_flags);
            break;
          }
        }
      }
      spin_unlock_irqrestore(&queue_parameter[i]->grab_lock, grab_lock_flags);
    }

  }
  return (ret == S2I_CAPTURED_PACKET) ? NF_DROP : NF_ACCEPT;
}

static ssize_t s2iGEVFilter_read(struct file *file, char *buf,
         size_t count, loff_t * ppos)
{
  ssize_t ret = 0;
  unsigned char cam_nr;
  PIMAGE_BUFFER pMyBuffer = NULL;
  unsigned char *psrc, *pdest,*psrclast;
  int i, x,bpp,imgWidth, cnt, wait_result;
  int status;
 
  // printk(KERN_INFO "s2iGEVFilter_read start");
  
  if (*ppos)
    return -ESPIPE;

  // get camera number from data pointer first byte
  if (copy_from_user(&cam_nr, buf, 1) != 0) {
    return -EINVAL;
  }

  // printk(KERN_INFO "s2iGEVFilter_read - cam: %d",cam_nr);
  
  if(cam_nr >= CAMERA_COUNT) {
    return -ESPIPE;
  }

  status = atomic_read_acquire(&queue_parameter[cam_nr]->grab_flag);
  // printk(KERN_INFO "s2iGEVFilter_read - status: %d",status);


  if(status == GRAB_FLAG_STOPPING) {
    ret = -EPIPE;
    printk(KERN_INFO "GRAB_FLAG_STOPPING");
    goto CompleteRead;
  }
  else if (status == GRAB_FLAG_STOPPED) {
    //Allow waiting on image before grab starts, but do so before acquiring mutex otherwise
    //at least 1 timeout will be triggers before the grab start ioctl can be performed
    wait_result = wait_event_interruptible_timeout(queue_parameter[cam_nr]->grab_wait, 
      atomic_read_acquire(&queue_parameter[cam_nr]->grab_flag) == GRAB_FLAG_RUNNING, 
      msecs_to_jiffies(1000));

    if(0 == wait_result || wait_result == -ERESTARTSYS) {
      ret = -EPIPE;

      // printk(KERN_INFO "pMyBuffer error: %d", pMyBuffer->Error);
      // printk(KERN_INFO "GRAB_FLAG_STOPPED");
      
      goto CompleteRead;
    } 
  }

  //Since copy_to_user may sleep, mutex should be used
  //This mutex should not be used in interrupt handler
  wait_result = mutex_lock_interruptible(&queue_parameter[cam_nr]->read_mutex);
  if(wait_result == -EINTR) {
    ret = -EPIPE;
    goto CompleteRead;
  }
 // printk(KERN_INFO "s2iGEVFilter_read - mutex_lock_interruptible");


  // Elements in queue might be freed on acquisition stop, we have to lock out image deallocation
  wait_result = wait_event_interruptible_timeout(queue_parameter[cam_nr]->m_nDeliverList.newBuffer.signal, 
      (pMyBuffer = deque_pop_front(&queue_parameter[cam_nr]->m_nDeliverList)) != NULL, 
      msecs_to_jiffies(filter_parameter[cam_nr]->grab_timeout));

  // printk(KERN_INFO "pMyBuffer error: %d", pMyBuffer->Error);
  // printk(KERN_INFO "SizeX : %d SizeY: %d", pMyBuffer->ImageHeader.SizeX, pMyBuffer->ImageHeader.SizeY); 
  
  //Can also be interrupted and return non-zero
  if(0 == wait_result || wait_result == -ERESTARTSYS) {
    ret = -EPIPE;
    goto CompleteReadUnlockMutex;
  } 


  switch(pMyBuffer->Error)
  {
    case IMAGE_SUCCESS: ret = 0;         // ok
            break;
      
    case IMAGE_GRAB_ERROR: ret = -EIO;      // grab error
            break;
  
    case IMAGE_DIFFERENT_HEADER: ret = -EINVAL;    // invalite header
            // copy image header
            memcpy(&filter_parameter[cam_nr]->cur_img_header,&pMyBuffer->ImageHeader, sizeof(struct image_header));

            pMyBuffer->ImageHeader.ResendReceiveCount = (u_int16_t)pMyBuffer->ResendReceiveCount;
            pMyBuffer->ImageHeader.ResendSendCount = (u_int16_t)pMyBuffer->ResendSendCount;
            
            deque_push_back(&queue_parameter[cam_nr]->m_nQueuedList, pMyBuffer);
            goto CompleteReadUnlockMutex;
  }

  pMyBuffer->ImageHeader.ResendReceiveCount = (u_int16_t)pMyBuffer->ResendReceiveCount;
  pMyBuffer->ImageHeader.ResendSendCount = (u_int16_t)pMyBuffer->ResendSendCount;


  if (grab_parameter[cam_nr]->ring_buffer_count > 0)
  {
    // printk(KERN_INFO "s2iGEVFilter_read - ring_buffer");

    // check if trailer sizeX smaller than leader sizeX
    if (pMyBuffer->ImageHeader.SizeY != pMyBuffer->TrailerSizeY)
      pMyBuffer->ImageHeader.SizeY = pMyBuffer->TrailerSizeY;

    // copy image header
    memcpy(&filter_parameter[cam_nr]->cur_img_header, &pMyBuffer->ImageHeader, sizeof(struct image_header));
    
    // return the buffer index
    copy_to_user (buf, &pMyBuffer->Index, sizeof(pMyBuffer->Index));

    if ((pMyBuffer->Error) && (pMyBuffer->ImageHeader.MissingPacket == 0))
      pMyBuffer->Error = 0;

    if ((pMyBuffer->Error == 0) && (pMyBuffer->ImageHeader.MissingPacket != 0))
    {
      filter_parameter[cam_nr]->cur_img_header.MissingPacket = 0;
    }
    else {
      if (pMyBuffer->Error != 0) {
        ret = -EIO;
    	printk(KERN_INFO "pMyBuffer error: %d", pMyBuffer->Error);
      }
    }
  } else {
    // copy pixel data in image memory
    if(pMyBuffer->ImageHeader.PaddingX)
    {
      psrc = pMyBuffer->pData;
      pdest = buf;

      bpp = gev_decodeBytesPerPixel(pMyBuffer->ImageHeader.PixelType);

      imgWidth = (pMyBuffer->ImageHeader.SizeX * bpp);
      x = imgWidth + pMyBuffer->ImageHeader.PaddingX;
      psrclast = psrc + (x * pMyBuffer->ImageHeader.SizeY);
      
      
      for(i = 0; i < (int)pMyBuffer->ImageHeader.SizeY;i++)
      {
        
        if((psrc + imgWidth) < psrclast)
        {
          if (copy_to_user (pdest, psrc, imgWidth))
          { 
            ret = -EFAULT;
            break;
          }
        }
        pdest += imgWidth;
        psrc += x;
      }
    }
    else
    {
      if (pMyBuffer->ImageHeader.SizeX == -1)
      {
        if (pMyBuffer->ImageHeader.SizeY > count)
          cnt = count;
        else
          cnt = pMyBuffer->ImageHeader.SizeY;

        if (copy_to_user(buf, pMyBuffer->pData, cnt))
          ret = -EFAULT;
      }
      else
      {
        // check if trailer sizeX smaller than leader sizeX
        if (pMyBuffer->ImageHeader.SizeY != pMyBuffer->TrailerSizeY)
        {
          int lsize;

          pMyBuffer->ImageHeader.SizeY = pMyBuffer->TrailerSizeY;
          
          bpp = gev_decodeBytesPerPixel(pMyBuffer->ImageHeader.PixelType);

          lsize = (pMyBuffer->ImageHeader.SizeX * bpp) * pMyBuffer->ImageHeader.SizeY;
          if (lsize >(int)count)
            lsize = (int)count;

          if (copy_to_user(buf, pMyBuffer->pData, lsize))
            ret = -EFAULT;
        }
        else
        {
          if (copy_to_user(buf, pMyBuffer->pData, count))
            ret = -EFAULT;
        }
      }
    }
  
    // copy image header
    memcpy(&filter_parameter[cam_nr]->cur_img_header, &pMyBuffer->ImageHeader, sizeof(struct image_header));

    deque_push_back(&queue_parameter[cam_nr]->m_nQueuedList, pMyBuffer);
  } 

  CompleteReadUnlockMutex:
  mutex_unlock(&queue_parameter[cam_nr]->read_mutex);
  CompleteRead:
  // printk(KERN_INFO "s2iGEVFilter_read completed, ret: %d",ret);
  return ret;
}

int _ioctl_cmd_start_grab(struct grab_param_io *ib_param, int rest_cnt) {
  unsigned char cam_nr;
  os_error_t result;
  cam_nr = ib_param->cam_nr - 1;
  
  
  if (atomic_read_acquire(&queue_parameter[cam_nr]->grab_flag) == GRAB_FLAG_RUNNING)
  {
    return -EBUSY;
  }
  
  
  printk("cam_nr: %d\n",cam_nr);
  printk("width: %d\n",ib_param->width);
  printk("height: %d\n",ib_param->height);
  printk("pixel_format: %d\n",ib_param->pixel_format);
  printk("CamIP: %08X\n",ib_param->cam_ip);
  printk("CamPort: %d\n",ib_param->cam_port);
  printk("PacketResendCount: %d\n",ib_param->packet_resend_count);
  printk("PacketSize: %d\n",ib_param->packet_size);
 
  gev_setup_parameters(cam_nr, ib_param, 0);
  
  printk("isize_2: %d\n",grab_parameter[cam_nr]->isize);
  printk("PacketSize_2: %d\n",grab_parameter[cam_nr]->PacketSize);
    
  if(rest_cnt == 0) {
    grab_parameter[cam_nr]->gendc_support = ib_param->gendc_support;
  }
  else
    grab_parameter[cam_nr]->gendc_support = 0;
  
  if(grab_parameter[cam_nr]->buffer_count == 0)
    grab_parameter[cam_nr]->buffer_count = 4;

  result = InitImageBuffer(cam_nr, grab_parameter[cam_nr]->isize, grab_parameter[cam_nr]->PacketSize);

  if(result  != OS_OK)
  {
    CloseImageBuffer(cam_nr);
    return -ENOMEM;
  }

  
  if(gev_init_extra_buffers(cam_nr) == OS_ALLOC_FAILURE)
  {
    gev_close_extra_buffers(cam_nr);
    CloseImageBuffer(cam_nr);
    return -ENOMEM;
  }

  // printk(KERN_INFO "gev_init_extra_buffers");

  filter_parameter[cam_nr]->grab_timeout = ib_param->timeout;
  filter_parameter[cam_nr]->CamIp = ib_param->cam_ip;
  filter_parameter[cam_nr]->CamPort = ib_param->cam_port;
  filter_parameter[cam_nr]->CamPortCtrl = ib_param->cam_port_ctrl;
  filter_parameter[cam_nr]->adapter_ip = ib_param->adapter_ip;

  grab_parameter[cam_nr]->init_flag = 1;


  gev_init_grab_counters(cam_nr);
  // printk(KERN_INFO "gev_init_grab_counters");
 
  gev_init_grab_parameters(cam_nr);
  // printk(KERN_INFO "gev_init_grab_parameters");

  resend_packet_parameter[cam_nr]->sport = filter_parameter[cam_nr]->CamPortCtrl;                // GigE control port
  resend_packet_parameter[cam_nr]->dport = 3956;       

  ResetTimeStats(cam_nr, OsGetTickCount(), OsGetFrequency());
  // printk(KERN_INFO "ResetTimeStats");

  grab_parameter[cam_nr]->data_blocks_discarded_on_device = 0;
  atomic_set_release(&queue_parameter[cam_nr]->grab_flag, GRAB_FLAG_RUNNING);
  cam_cnt++;

   printk(KERN_INFO "grab_start OK");
   return 0;
}

int ioctl_cmd_start_grab(struct grab_param_io *ib_param, int rest_cnt) {
  unsigned long grab_lock_flags;
  int rc = 0, cam_nr;
  cam_nr = ib_param->cam_nr - 1;

  if(cam_nr < 0 || cam_nr >= CAMERA_COUNT) {
    return -EINVAL;
  }

  mutex_lock(&queue_parameter[cam_nr]->read_mutex);
  spin_lock_irqsave(&queue_parameter[cam_nr]->grab_lock, grab_lock_flags);
  rc = _ioctl_cmd_start_grab(ib_param, rest_cnt);
  spin_unlock_irqrestore(&queue_parameter[cam_nr]->grab_lock, grab_lock_flags);
  mutex_unlock(&queue_parameter[cam_nr]->read_mutex);

  if(rc == 0) {
    wake_up_interruptible(&queue_parameter[cam_nr]->grab_wait);
  }
  return rc;
}

int _ioctl_cmd_stop_grab(unsigned char cam_nr) {
    
  printk("cam_nr: %d\n",cam_nr);
  filter_parameter[cam_nr]->CamIp = 0;
  filter_parameter[cam_nr]->CamPort = 0;

  CloseImageBuffer(cam_nr);

  gev_close_extra_buffers(cam_nr);

  memset(resend_packet_parameter[cam_nr],0,sizeof( struct resend_packet_param  ));
  CloseTimeStats(cam_nr);
                    
  if(cam_cnt)
    cam_cnt--;

  atomic_set_release(&queue_parameter[cam_nr]->grab_flag, GRAB_FLAG_STOPPED);

  return 0;
}

int ioctl_cmd_stop_grab(unsigned char cam_nr) {
  
  int rc = 0;
  unsigned long grab_lock_flags;

  atomic_set_release(&queue_parameter[cam_nr]->grab_flag, GRAB_FLAG_STOPPING);
  deque_cancel(&queue_parameter[cam_nr]->m_nDeliverList);
  // wake_up_interruptible(&queue_parameter[cam_nr]->hNewImage);

  //Has to be done in two steps, first stop network hook from using resources
  //Then clean up and wake up potential read thread
  mutex_lock(&queue_parameter[cam_nr]->read_mutex);
  spin_lock_irqsave(&queue_parameter[cam_nr]->grab_lock, grab_lock_flags);
  rc = _ioctl_cmd_stop_grab(cam_nr);
  spin_unlock_irqrestore(&queue_parameter[cam_nr]->grab_lock, grab_lock_flags);
  mutex_unlock(&queue_parameter[cam_nr]->read_mutex);

  if(rc == 0) {
    wake_up_interruptible(&queue_parameter[cam_nr]->grab_wait);
  }
  return rc;
}

int ioctl_ring_buffer_set(struct ring_buffer_io_filter* rb_param) {
  unsigned long page_count, pinned_pages, first_page_offset;
  size_t array_size;
  struct page **pages = NULL;
  void* vmapped_addr = NULL;
  SPHINX_CAMNR cam_nr = rb_param->cam_nr-1;
  RING_BUFFER *new_rb, *current_rb;

  printk(KERN_INFO "ioctl_ring_buffer_set - cam_nr: %d",cam_nr);

  if (atomic_read_acquire(&queue_parameter[cam_nr]->grab_flag) != GRAB_FLAG_STOPPED) {
    return -1; //BUSY
  }

  //User address is not necessarily aligned to page start, so we need to compute the offset to the page start
  first_page_offset = (unsigned long)rb_param->buffer % PAGE_SIZE;
  page_count = ((unsigned long)rb_param->buffer + rb_param->length + PAGE_SIZE - 1) >> PAGE_SHIFT;
	page_count -= (unsigned long)rb_param->buffer  >> PAGE_SHIFT;

  array_size = (page_count * sizeof(struct page *));
  // printk(KERN_INFO "ioctl_ring_buffer_set - array_size: %d page_count: %d", array_size, page_count);

  pages = kmalloc(array_size, GFP_KERNEL);

  pinned_pages = get_user_pages_fast((unsigned long)rb_param->buffer, page_count, 1, pages);

  if(pinned_pages != page_count) {
    kfree(pages);
    return -1; //Pages could not be pinned.
  }
  // Map kernel pages
  vmapped_addr = vmap(pages, page_count, VM_MAP, PAGE_KERNEL);

  // Reserve ring buffer item
  new_rb = kmalloc(sizeof(  RING_BUFFER ), GFP_KERNEL);
  new_rb->page_count = page_count;
  new_rb->pages = pages;
  new_rb->virt_addr = vmapped_addr;
  new_rb->virt_addr_offset = first_page_offset;
  new_rb->next = NULL;  // Next item for last item points to null

  if(grab_parameter[cam_nr]->ring_buffer == NULL) {
    grab_parameter[cam_nr]->ring_buffer = new_rb;
  } else {
    current_rb = grab_parameter[cam_nr]->ring_buffer;
    while(current_rb->next) {
      current_rb = current_rb->next;
    }
    // Enqueue last item
    current_rb->next = new_rb;
  }
  grab_parameter[cam_nr]->ring_buffer_count++;
  grab_parameter[cam_nr]->buffer_count = grab_parameter[cam_nr]->ring_buffer_count;

  return 0; //OK
}

int ioctl_ring_buffer_release(unsigned char cam_nr) {
  RING_BUFFER *current_rb;

  if (atomic_read_acquire(&queue_parameter[cam_nr]->grab_flag) != GRAB_FLAG_STOPPED) {
    return -1; //BUSY
  }

  current_rb = grab_parameter[cam_nr]->ring_buffer;
  grab_parameter[cam_nr]->ring_buffer = NULL;
  grab_parameter[cam_nr]->ring_buffer_count = 0;
  //For each buffer
  while(current_rb) {
    unsigned long i;
    RING_BUFFER *old_rb;

    vunmap(current_rb->virt_addr);
    for(i = 0; i < current_rb->page_count; ++i) {
      //For each page
      //Set dirty
      set_page_dirty_lock(current_rb->pages[i]);
      put_page(current_rb->pages[i]);
    }
    kfree(current_rb->pages);
    old_rb = current_rb;
    current_rb = current_rb->next;
    kfree(old_rb);
  }

  return 0; //OK
}

int ioctl_ring_buffer_queue(struct ring_buffer_io_filter* rb_param) {
  unsigned char Device = rb_param->cam_nr-1;
  if (grab_parameter[Device]->ring_buffer && grab_parameter[Device]->Image) {
    deque_push_back(&queue_parameter[Device]->m_nQueuedList, &grab_parameter[Device]->Image[rb_param->index]);
    return 0;
  }
  return -1;
}

int ioctl_ring_buffer(unsigned int cmd, struct ring_buffer_io_filter* rb_param) {
  int result = 0;
  switch (cmd) {
  case IOCTL_CMD_SET_RING_BUFFER:
    // printk("IOCTL_CMD_SET_RING_BUFFER\n");
    result = ioctl_ring_buffer_set(rb_param);
    break;
  case IOCTL_CMD_RELEASE_RING_BUFFER:
    // printk("IOCTL_CMD_RELEASE_RING_BUFFER\n");
    result = ioctl_ring_buffer_release(rb_param->cam_nr-1);
    break;
  case IOCTL_CMD_QUEUE_RING_BUFFER:
    // printk("IOCTL_CMD_QUEUE_RING_BUFFER\n");
    result = ioctl_ring_buffer_queue(rb_param);
    break;
  }
  return result;
}

static int s2iGEVFilter_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
  int rc = 0;
  struct grab_param_io ib_param;
  struct packet_resend_io pr_param;
  struct version_io version_param;
  struct image_header_io img_header_param;
  struct status_io status_param;
  struct buffer_count_io bc_param;
  struct packets_out_of_order_io po_param;
  struct test_packet_resend_io tp_param;
  struct ring_buffer_io_filter rb_param;
  SPHINX_CAMNR cam_nr = 0;
  int rest_cnt;
  int nBytes;
  unsigned long grab_lock_flags;

  
  switch (cmd) {
    case IOCTL_CMD_START_GRAB:
      nBytes = sizeof(struct grab_param_io);
      rest_cnt = copy_from_user(&ib_param,(int *)arg, nBytes);

      printk(KERN_INFO "IOCTL_CMD_START_GRAB - cam_nr: %d Bytes: %d", ib_param.cam_nr,nBytes);
      rc = ioctl_cmd_start_grab(&ib_param, rest_cnt);
      printk(KERN_INFO "IOCTL_CMD_START_GRAB done %d\n", rc);
      break;

    case IOCTL_CMD_STOP_GRAB:
      rest_cnt = copy_from_user(&ib_param,(int *)arg, sizeof(struct grab_param_io));

      cam_nr = ib_param.cam_nr - 1;
      printk("IOCTL_CMD_STOP_GRAB\n");
      printk("cam_nr: %d\n",cam_nr);
      rc = ioctl_cmd_stop_grab(cam_nr);
      printk("IOCTL_CMD_STOP_GRAB done %d\n", rc);

      rest_cnt = copy_to_user((int *)arg, &ib_param, sizeof(struct grab_param_io));
      break;
	  
    case IOCTL_CMD_SET_GRAB_NET_SETTINGS:
      nBytes = sizeof(struct grab_param_io);
      rest_cnt = copy_from_user(&ib_param,(int *)arg, nBytes);
      printk(KERN_INFO "IOCTL_CMD_SET_GRAB_NET_SETTINGS - cam_nr: %d Bytes: %d", ib_param.cam_nr,nBytes);
      
      cam_nr = ib_param.cam_nr - 1;
      
      spin_lock_irqsave(&queue_parameter[cam_nr]->grab_lock, grab_lock_flags);
      
      filter_parameter[cam_nr]->CamIp = ib_param.cam_ip;
      filter_parameter[cam_nr]->CamPort = ib_param.cam_port;
      filter_parameter[cam_nr]->CamPortCtrl = ib_param.cam_port_ctrl;
      filter_parameter[cam_nr]->adapter_ip = ib_param.adapter_ip;
      
      spin_unlock_irqrestore(&queue_parameter[cam_nr]->grab_lock, grab_lock_flags);
      // printk("IOCTL_CMD_SET_GRAB_NET_SETTINGS done %d\n", rc);
      break;

    case IOCTL_CMD_PACKET_RESEND:
      rest_cnt = copy_from_user(&pr_param,(int *)arg, sizeof(struct packet_resend_io));
      cam_nr = pr_param.cam_nr - 1;
      grab_parameter[cam_nr]->PacketResendFlag = pr_param.packet_resend_flag;
      break;

    case IOCTL_CMD_VERSION:
      version_param.major = DRIVER_MAJOR_VERSION;
      version_param.minor = DRIVER_MINOR_VERSION;
      rest_cnt = copy_to_user((int *)arg, &version_param, sizeof(struct version_io));
      break;

    case IOCTL_CMD_IMAGE_HEADER:
      rest_cnt = copy_from_user(&img_header_param,(int *)arg, sizeof(struct image_header_io));

      cam_nr = img_header_param.cam_nr - 1;

      memcpy(&img_header_param.img_h,&filter_parameter[cam_nr]->cur_img_header, sizeof(struct image_header));

      if(rest_cnt == 0)
        rest_cnt = copy_to_user((int *)arg, &img_header_param, sizeof(struct image_header_io));
      else
        rest_cnt = copy_to_user((int *)arg, &img_header_param, sizeof(struct image_header_io) - rest_cnt);
      break;

    case IOCTL_CMD_GET_STATUS:
      rest_cnt = copy_from_user(&status_param,(int *)arg, sizeof(struct status_io));

      cam_nr = status_param.cam_nr - 1;
  
      status_param.img_cnt = grab_parameter[cam_nr]->img_cnt;
      status_param.img_error = grab_parameter[cam_nr]->img_error;
      status_param.missing_packets = grab_parameter[cam_nr]->missing_packets;
      GetTimeStats(cam_nr, &status_param.time_img_cnt, &status_param.img_time);
      // Configure Ticks in appropiate scale for FPS calculation 
      status_param.img_time = status_param.img_time * 1000; 
    
      // printk(KERN_INFO "IOCTL_CMD_GET_STATUS time_img_cnt: %ld img_error: %ld missing_packets: %d",status_param.img_cnt,status_param.img_error,status_param.missing_packets);
      // printk(KERN_INFO "time_img_count: %ld img_time: %lld",status_param.time_img_cnt, status_param.img_time);
        
      //copy to user might sleep! do after unlock
      if(rest_cnt == 0) {
        status_param.data_blocks_discarded_on_device = grab_parameter[cam_nr]->data_blocks_discarded_on_device;
        rest_cnt = copy_to_user((int *)arg, &status_param, sizeof(struct status_io));
      } else { 
        rest_cnt = copy_to_user((int *)arg, &status_param, sizeof(struct status_io) - rest_cnt);
      }
      break;

    case IOCTL_CMD_RESET_STATUS: 
      rest_cnt = copy_from_user(&status_param,(int *)arg, sizeof(struct status_io));
      cam_nr = status_param.cam_nr - 1;

      ResetAcquisitionStatus(cam_nr);

      break;

    case IOCTL_CMD_RESET_STATISTICS: 
      rest_cnt = copy_from_user(&status_param,(int *)arg, sizeof(struct status_io));
      cam_nr = status_param.cam_nr - 1;

      ResetStatistics(cam_nr);

      break;

    case IOCTL_CMD_BUFFER_COUNT:
      rest_cnt = copy_from_user(&bc_param,(int *)arg, sizeof(struct buffer_count_io));

      cam_nr = bc_param.cam_nr - 1;
      if(grab_parameter[cam_nr]->ring_buffer_count == 0) {
        //Only valid if ring buffer is not used
        grab_parameter[cam_nr]->buffer_count = bc_param.count;
      }
      break;

    case IOCTL_CMD_PACKETS_OUT_OF_ORDER:
      rest_cnt = copy_from_user(&po_param,(int *)arg, sizeof(struct packets_out_of_order_io));

      cam_nr = po_param.cam_nr - 1;
      grab_parameter[cam_nr]->packets_out_of_order = po_param.packets_out_of_order;
      break;

    case IOCTL_CMD_TEST_PACKET_RESEND:
      rest_cnt = copy_from_user(&tp_param,(int *)arg, sizeof(struct test_packet_resend_io));

      cam_nr = tp_param.cam_nr - 1;
      grab_parameter[cam_nr]->test_packet_resend = tp_param.packet;
      grab_parameter[cam_nr]->test_packet_resend_count = tp_param.count;
      break;
          
    case IOCTL_CMD_SET_RING_BUFFER:
    case IOCTL_CMD_QUEUE_RING_BUFFER:
    case IOCTL_CMD_RELEASE_RING_BUFFER:
      rest_cnt = copy_from_user(&rb_param,(int *)arg, sizeof(struct ring_buffer_io_filter));
      rc = ioctl_ring_buffer(cmd, &rb_param);
      break;
    default:
      rc = -EINVAL;
      break;
  }
  return(rc);
}

#if LINUX_VERSION_CODE>= KERNEL_VERSION(2,6,24) 
static long s2iGEVFilter_unlocked_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
  return s2iGEVFilter_ioctl(NULL, f, cmd, (unsigned long)arg);
}
#endif

static int s2iGEVFilter_open(struct inode *inode, struct file *filp)
{
  int cam_nr = MINOR (inode->i_rdev);
  printk(KERN_INFO "open s2igevfilter %d\n", cam_nr);

  if(filp->f_flags & O_NONBLOCK)
    nonblock = 1;

  opened = 1;
  return 0;
}

static int s2iGEVFilter_release (struct inode *inode, struct file *filp)
{
  int cam_nr = MINOR (inode->i_rdev);
  printk(KERN_INFO "[cam_nr %d] release s2igevfilter\n", cam_nr);

  if (atomic_read_acquire(&queue_parameter[cam_nr]->grab_flag) == GRAB_FLAG_RUNNING) {
    printk(KERN_INFO "[cam_nr %d] grab flag running... \n", cam_nr);
    ioctl_cmd_stop_grab(cam_nr);    
  }
  //ring buffer may be allocated even if not running
  if(ioctl_ring_buffer_release(cam_nr)) {
    printk(KERN_INFO "[cam_nr %d] release ring buffer failed\n", cam_nr);
  }
  printk(KERN_INFO "[cam_nr %d] release s2igevfilter - done\n", cam_nr);

  opened = 0; 
  nonblock = 0;
  return 0;
}


static struct file_operations s2iGEVFilter_fops =
{
   owner:   THIS_MODULE,
#if LINUX_VERSION_CODE>= KERNEL_VERSION(3,0,13)
    unlocked_ioctl: s2iGEVFilter_unlocked_ioctl,
#else
    ioctl:   s2iGEVFilter_ioctl,
#endif
   open:    s2iGEVFilter_open,
   read:    s2iGEVFilter_read,
   release: s2iGEVFilter_release,
};

static char *s2i_devnode(struct device *dev, umode_t *mode)
{
  if (!mode)
    return NULL;
  if(MAJOR(dev->devt) == S2IGEVFILTER_MAJOR)
    *mode = 0666;
  return NULL;
}

static int __init s2iGEVFilter_init(void)
{
    int result,i;
    char lstr[20];
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,13,0)
    struct net *init_net;
#endif

    sprintf(lstr,"V%d.%d.%d",DRIVER_MAJOR_VERSION,DRIVER_MINOR_VERSION >> 4, DRIVER_MINOR_VERSION & 0x0f);
    printk(KERN_INFO "module_init s2igevfilter %s\n",lstr);

    result = register_chrdev(S2IGEVFILTER_MAJOR, "s2igevfilter", &s2iGEVFilter_fops);
    if (result < 0)
    {
        printk(KERN_WARNING "s2igevfilter: can't get major %d\n", S2IGEVFILTER_MAJOR);
        return 1;
    }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6,5,0)
    if ((s2igevfilter_class = class_create("chardrv")) == NULL)
    {
      unregister_chrdev(S2IGEVFILTER_MAJOR, "s2igevfilter");
      return 1;
    }    
#else
    if ((s2igevfilter_class = class_create(THIS_MODULE, "chardrv")) == NULL)
    {
      unregister_chrdev(S2IGEVFILTER_MAJOR, "s2igevfilter");
      return 1;
    }    
#endif



    #if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 0, 0)
      s2igevfilter_class->devnode = (char *(*)(const struct device *, umode_t *))s2i_devnode;    
    #else
      s2igevfilter_class->devnode = s2i_devnode;    
    #endif
    
    netfilter_ops.hook              =       (nf_hookfn *)main_hook;
    netfilter_ops.pf                =       PF_INET;  
#if LINUX_VERSION_CODE>= KERNEL_VERSION(2,6,24) 
    netfilter_ops.hooknum           =       0;
#else
    netfilter_ops.hooknum           =       NF_IP_PRE_ROUTING;
#endif
    netfilter_ops.priority          =       NF_IP_PRI_FIRST;

    //Allocation has to be done before the hook is registered!
    //Once the register function is called the hook can get called if packets arrive,
    //which will result in using non allocated memory...
    for(i = 0;i < CAMERA_COUNT;i++)
    {  
      grab_parameter[i] = kmalloc (sizeof( struct grab_param ), GFP_KERNEL);
      memset(grab_parameter[i],0,sizeof( struct grab_param ));

      filter_parameter[i] = kmalloc(sizeof( struct filter_param ), GFP_KERNEL);
      memset(filter_parameter[i],0, sizeof( struct filter_param ));

      InitTimeStats(i);

      resend_packet_parameter[i] = kmalloc (sizeof( struct resend_packet_param  ), GFP_KERNEL);
      memset(resend_packet_parameter[i],0,sizeof( struct resend_packet_param  ));

      queue_parameter[i] = kmalloc (sizeof( struct queue_param ), GFP_KERNEL);
      memset(queue_parameter[i],0,sizeof( struct queue_param ));

      atomic_set_release(&queue_parameter[i]->grab_flag, GRAB_FLAG_STOPPED);
      spin_lock_init(&queue_parameter[i]->grab_lock);
      init_waitqueue_head(&(queue_parameter[i]->grab_wait));
      
      deque_init(&queue_parameter[i]->m_nQueuedList);
      deque_init(&queue_parameter[i]->m_nDeliverList);
      grab_parameter[i]->ring_buffer = NULL;
      grab_parameter[i]->ring_buffer_count = 0;

      // Create device driver 
      if(device_create(s2igevfilter_class, NULL, MKDEV(S2IGEVFILTER_MAJOR, i), NULL, "s2igevfilter%d", i) == NULL)       
      {
        printk("Error creating device s2igevfilter%d\n",i);
        class_destroy(s2igevfilter_class);
        unregister_chrdev(S2IGEVFILTER_MAJOR, "s2igevfilter");
        return 1;
      }       
    }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,13,0)
    for_each_net(init_net)
      nf_register_net_hook(init_net, &netfilter_ops);
#else
    nf_register_hook(&netfilter_ops);
#endif
    printk("s2igevfilter hook registered\n");
    cam_cnt = 0;
    return 0;
}

static void __exit s2iGEVFilter_fini(void) 
{
  int i;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,13,0)
  struct net *init_net;
#endif
  printk(KERN_INFO "module_exit s2igevfilter\n");

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,13,0)
  for_each_net(init_net)
    nf_unregister_net_hook(init_net, &netfilter_ops);
#else
  nf_unregister_hook(&netfilter_ops); 
#endif    
  unregister_chrdev(S2IGEVFILTER_MAJOR, "s2igevfilter");

  for(i = 0;i < CAMERA_COUNT;i++)
  {  
    kfree (grab_parameter[i]);
    kfree (resend_packet_parameter[i]);
    kfree (queue_parameter[i]);
    
    device_destroy(s2igevfilter_class, MKDEV(S2IGEVFILTER_MAJOR, i));
  }

  class_destroy(s2igevfilter_class);
}


module_init(s2iGEVFilter_init);
module_exit(s2iGEVFilter_fini);


