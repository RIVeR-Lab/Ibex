
#include <EbTransportLayerLib/OS_KMLTime.h>
#include <EbTransportLayerLib/OS_KMLGCCCompiler.h>

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6,8,0)
#include <linux/jiffies.h>
#include <linux/timekeeping.h>
#endif

OS_UINT64 OS_TimeGetUS( OS_VOID )
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0)
	struct timespec lTime;
	lTime = current_kernel_time();
#else
	struct timespec64 lTime;
	ktime_get_coarse_real_ts64( &lTime );
#endif
	return ( lTime.tv_sec * 1000000 + OS_DIV64( lTime.tv_nsec, 1000 ) );
}

OS_UINT64 OS_TimeGetPreciseUS( OS_VOID )
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,8,0)
	struct timespec lTime;
#else
	struct timespec64 lTime;
#endif

#ifdef OS_ALLOW_GPL
	getnstimeofday( &lTime );
#else
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,8,0)
    ktime_get_real_ts64( &lTime );
#  elif LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,28)
	getnstimeofday( &lTime );
#  else
	lTime = current_kernel_time();
#  endif 
#endif 
	return ( lTime.tv_sec * 1000000 + OS_DIV64( lTime.tv_nsec, 1000 ) );
}

OS_VOID OS_TimeYield( OS_VOID )
{
    yield();
}

OS_VOID OS_TimeSleep( OS_UINT32 aTime )
{
    set_current_state( TASK_INTERRUPTIBLE );
    schedule_timeout( msecs_to_jiffies( aTime ) );
}

OS_VOID OS_TimeSinceEpoch( OS_UINT32* aSecond, OS_UINT32* aMicroSecond )
{
    #if LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0)
    struct timeval lTimeVal;

    do_gettimeofday( &lTimeVal );
    *aMicroSecond = lTimeVal.tv_usec;
#else 
    struct timespec64 lTimeVal;

    ktime_get_real_ts64(&lTimeVal);
    *aMicroSecond = lTimeVal.tv_nsec / 1000;
#endif
    
    *aSecond = lTimeVal.tv_sec;
}
