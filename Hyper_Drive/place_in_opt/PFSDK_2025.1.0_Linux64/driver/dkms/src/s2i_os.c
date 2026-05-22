/* s2i_os.c -- Operating systems specific functions
 *
 * (C) 2021 by Sensor to Image GmbH
 *
 * Version: 1.0.0 Date: 15.12.2021
 * History:
 * Version: 1.0.0 Date: 15.12.2021
 * - initial realease
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

#include <s2i/os.h>
#include <s2i_gev.h>
#include "s2i_utils.h"                   

extern struct grab_param *grab_parameter[];
extern struct resend_packet_param *resend_packet_parameter[];
extern struct queue_param *queue_parameter[];

typedef unsigned long OsMallocTag;
static const OsMallocTag ALLOCATED_BY_KMALLOC = 0x6C616D6B;
static const OsMallocTag ALLOCATED_BY_VMALLOC = 0x6C616D76;

static void *MarkMemoryAs(void *memory, OsMallocTag tag)
{
  *(OsMallocTag *)memory = tag;
  return (OsMallocTag *)memory + 1;
}

#if (defined(_UBUNTU_) && LINUX_VERSION_CODE >= KERNEL_VERSION(3,18,0)) || \
    (LINUX_VERSION_CODE >= KERNEL_VERSION(5,0,0) &&  LINUX_VERSION_CODE < KERNEL_VERSION(5,8,0))
void do_gettimeofday(struct timeval *tv)
{
  struct timespec64 ts;
  ktime_get_real_ts64(&ts);
  tv->tv_sec = ts.tv_sec;
  tv->tv_usec = ts.tv_nsec/1000;
}
#endif

/** how many os_frequency_t in one second */
#define OS_ONE_SECOND 1000000
/** how many nanoseconds in one os_frequency_t */
#define OS_NANOS 1000

os_frequency_t EDDI_API OsGetFrequency(void)
{
  return OS_ONE_SECOND;
}

os_time_t EDDI_API OsGetTickCount(void)
{
  u_int64_t us;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,8,0)
  {
      struct timespec64 ts;
      ktime_get_real_ts64(&ts);
      us = (ts.tv_sec * OS_ONE_SECOND) + (ts.tv_nsec / OS_NANOS);
  }
#else
  u_int64_t s_t,u_t;
  struct timeval tv;

  do_gettimeofday(&tv);
  s_t = tv.tv_sec * OS_ONE_SECOND;
  u_t = tv.tv_usec;
  us = s_t + u_t;
#endif
  return us;
}

void EDDI_API OsInitSpin(struct os_lockable *q)
{
  spin_lock_init(&q->lock);
}

void EDDI_API OsCloseSpin(struct os_lockable *q)
{
  
}

void EDDI_API OsLockSpin(struct os_lockable *q)
{
  spin_lock_irqsave(&q->lock, q->flags);
}

void EDDI_API OsUnlockSpin(struct os_lockable *q)
{
  spin_unlock_irqrestore(&q->lock, q->flags);
}

void EDDI_API OsInitSignal(struct os_signallable *s)
{
  init_waitqueue_head(&(s->signal));
}

void EDDI_API OsCloseSignal(struct os_signallable *s)
{
  init_waitqueue_head(&(s->signal)); // FIXME: really ?
}

void EDDI_API OsNotifySignal(struct os_signallable *s)
{
  wake_up_interruptible(&(s->signal));
}

void * EDDI_API OsMalloc(size_t size)
{
  void *memory = NULL;

  if(size == 0)
    return(NULL);

  if (in_irq()) {
    //printk("OsMalloc cannot be used at hardirq level.\n");
    BUG();
  }
 
 size += sizeof(OsMallocTag);
  if (size <= 0x20000) {
    memory = kmalloc(size, in_interrupt() ? GFP_ATOMIC : GFP_KERNEL);
    if (memory != NULL) {
      return MarkMemoryAs(memory, ALLOCATED_BY_KMALLOC);
    }
  }
  if (!in_interrupt()) {
    memory = vmalloc(size);
    if (memory != NULL) {
      return MarkMemoryAs(memory, ALLOCATED_BY_VMALLOC);
    }
  }
  return(NULL);
}

void EDDI_API OsFree(void *memory, size_t size)
{
  OsMallocTag *tag;

  if (in_irq()) {
    //printk("OsFree cannot be used at hardirq level.\n");
    BUG();
  }

  if (memory != NULL) {
    tag = (OsMallocTag *)memory - 1;
    if (*tag == ALLOCATED_BY_KMALLOC) {
        kfree(tag);
    } else if (*tag == ALLOCATED_BY_VMALLOC) {
        vfree(tag);
    } else {
      //printk("Trying to OsFree memory not OsMalloc'd (or memory corruption occured).\n");
      BUG();
    }
  }
}

void EDDI_API *OsMemSet(void *memory, int character, unsigned size)
{
    return memset(memory, character, size);
}

void EDDI_API *OsMemCopy(void *destination, const void *source, unsigned n)
{
    return memcpy(destination, source, n);
}

int EDDI_API OsPrintk(const char* fmt, ...)
{
  va_list args;
  int ret;
  va_start(args, fmt);
  ret = vprintk(fmt, args);
  va_end(args);
  return ret;
}

void EDDI_API OsGetThreadId(u_int32_t *pid, u_int32_t *tid)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 24))
    *pid = (u_int32_t)current->tgid;
    *tid = (u_int32_t)current->pid;
#else
    *pid = (u_int32_t)task_tgid_nr(current);
    *tid = (u_int32_t)task_pid_nr(current);
#endif
}

os_error_t EDDI_API resend_packet(gev_kernel_context_t *pskb, u_int8_t device, u_int64_t block_id,
                                  u_int32_t first_packet_id, u_int32_t last_packet_id, u_int8_t extended_id)
{
  struct sk_buff * skb_resend = NULL;
  struct ethhdr *ethdr = NULL,*ethdr_resend; 
  struct udphdr *udpdr_resend;
  struct iphdr *ipdr,*ipdr_resend;
  struct GigEPacketResend *udp_data;
  int cnt;
  struct sk_buff *skb = NULL;

  skb = (struct sk_buff *)pskb;
  
  // alloc packet
  if(extended_id == 1)
    cnt = 20;
  else
    cnt = 28;

  skb_resend = alloc_skb( cnt + sizeof (struct ethhdr) + sizeof (struct iphdr) + sizeof (struct udphdr) + 10, GFP_ATOMIC);
  if (skb_resend == NULL)
  {
    return OS_ALLOC_FAILURE;
  }
  else
  {
    skb_reserve (skb_resend , 10);

    skb_reset_network_header(skb_resend);

    skb_resend->dev = skb->dev;
    skb_resend->pkt_type = skb->pkt_type;
    skb_resend->protocol = skb->protocol;
    skb_resend->ip_summed = skb->ip_summed ;
    skb_resend->destructor = skb->destructor;
    skb_resend->priority = skb->priority;
    skb_resend->pkt_type = PACKET_OUTGOING;   

    // get send ethernet header
    ethdr_resend= (struct ethhdr *) skb_put (skb_resend, sizeof (struct ethhdr));
    // get receive ethernet header
    //get_ether_header(skb, ethdr);
#if LINUX_VERSION_CODE>= KERNEL_VERSION(2,6,22) 
    ethdr = (struct ethhdr *) skb_mac_header(skb);
#else
    ethdr = (struct ethhdr *) skb->mac.raw;
#endif
    
    // set ethernet header
    memcpy (ethdr_resend->h_dest, ethdr->h_source, ETH_ALEN);
    memcpy (ethdr_resend->h_source, ethdr->h_dest, ETH_ALEN);
    ethdr_resend->h_proto = __constant_htons(ETH_P_IP);

    // get ip send header
    ipdr_resend = (struct iphdr *) skb_put (skb_resend, sizeof (struct iphdr));
    ipdr = (struct iphdr *)ip_hdr(skb);
    memcpy (ipdr_resend, ipdr, sizeof (struct iphdr));

    // set ip header              
    ipdr_resend->daddr = ipdr->saddr;
    ipdr_resend->saddr = ipdr->daddr;
    ipdr_resend->tot_len = __constant_htons(28 + cnt);
    ipdr_resend->id++;
    ipdr_resend->ttl = 64;

    // set ip chechsum
    ip_send_check(ipdr_resend);
    
    // get udp send header
    udpdr_resend  = (struct udphdr *) skb_put (skb_resend, sizeof (struct udphdr));

    // set udp header 
    udpdr_resend->source = htons(resend_packet_parameter[device]->sport);
    udpdr_resend->dest = htons(resend_packet_parameter[device]->dport);
    udpdr_resend->len = __constant_htons(cnt + 8);
    udpdr_resend->check = 0;

    // get udp data pointer
    udp_data  = (struct GigEPacketResend *) skb_put (skb_resend, cnt);
    
    // set the packet resend header 
    gev_set_resend_packet_header(udp_data, device, block_id, first_packet_id, last_packet_id, extended_id);

    skb_resend->csum = skb_checksum (skb_resend, ipdr_resend->ihl*4, skb_resend->len - ipdr_resend->ihl * 4, 0);

    // set udp checksum
    udpdr_resend->check = csum_tcpudp_magic (ipdr_resend->saddr, ipdr_resend->daddr, cnt + sizeof(struct udphdr), IPPROTO_UDP, 
                                           csum_partial((char *)udpdr_resend, cnt + sizeof(struct udphdr), 0));

    // send packet
    if (0 > dev_queue_xmit(skb_resend)) {
      kfree_skb (skb_resend);
      return OS_SEND_FAILURE;
    }
  }
  return OS_OK;
}

