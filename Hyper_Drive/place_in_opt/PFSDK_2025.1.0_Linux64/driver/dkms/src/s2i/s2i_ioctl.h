#ifndef _S2I_IOCTL_H
#define _S2I_IOCTL_H

// use 'k' as magic number
#define S2I_IOC_MAGIC   'k'

#define IOCTL_CMD_START_GRAB                _IOWR(S2I_IOC_MAGIC,1,int)
#define IOCTL_CMD_STOP_GRAB                 _IOWR(S2I_IOC_MAGIC,2,int)
#define IOCTL_CMD_PACKET_RESEND             _IOWR(S2I_IOC_MAGIC,3,int)
#define IOCTL_CMD_VERSION                   _IOWR(S2I_IOC_MAGIC,4,int)
#define IOCTL_CMD_IMAGE_HEADER              _IOWR(S2I_IOC_MAGIC,5,int)
#define IOCTL_CMD_GET_STATUS                _IOWR(S2I_IOC_MAGIC,6,int)
#define IOCTL_CMD_BUFFER_COUNT              _IOWR(S2I_IOC_MAGIC,7,int)
#define IOCTL_CMD_PACKETS_OUT_OF_ORDER      _IOWR(S2I_IOC_MAGIC,8,int)
#define IOCTL_CMD_TEST_PACKET_RESEND        _IOWR(S2I_IOC_MAGIC,9,int)
#define IOCTL_CMD_SET_GRAB_NET_SETTINGS     _IOWR(S2I_IOC_MAGIC,10,int)
#define IOCTL_CMD_RESET_STATUS              _IOWR(S2I_IOC_MAGIC,11,int)
#define IOCTL_CMD_RESET_STATISTICS          _IOWR(S2I_IOC_MAGIC,12,int)
#define IOCTL_CMD_SET_RING_BUFFER           _IOWR(S2I_IOC_MAGIC,13,int)
#define IOCTL_CMD_RELEASE_RING_BUFFER       _IOWR(S2I_IOC_MAGIC,14,int)
#define IOCTL_CMD_QUEUE_RING_BUFFER         _IOWR(S2I_IOC_MAGIC,15,int)
#endif