// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************
#include <EbTransportLayerLib/OS_KMLMemoryMap.h>

#include <asm/current.h>
#include <linux/mm.h>
#include <linux/page-flags.h>
#include <linux/pagemap.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/version.h>

OS_RESULT OS_MapCreate( OS_DRV_CONTEXT* aContext, OS_MEMORYMAP* aMap, 
    OS_VOID* aUserAddress, OS_UINT32 aSize )
{
    int lPageCount;
    struct page** lPageList;
    void* lKernelPageAddress;
    unsigned int i;
    unsigned long lFirstPage;
    unsigned long lLastPage;

    OS_UNREFERENCED_PARAMETER( aContext );

    aMap->UserAddress = NULL;
    aMap->KernelAddress = NULL; 

    if( unlikely( !aUserAddress 
        || !aSize
#if ( LINUX_VERSION_CODE >= KERNEL_VERSION(5,0,0) || defined (CENTOS_PATCH) )
        || !access_ok( aUserAddress, aSize ) ) )
#else
        || !access_ok( VERIFY_WRITE, aUserAddress, aSize ) ) )
#endif
    {
        return OS_RESULT_INVALID_ARGUMENT;
    }

    lFirstPage = ( ( unsigned long ) aUserAddress ) >> PAGE_SHIFT;
    lLastPage = ( ( ( unsigned long ) aUserAddress ) + ( unsigned long ) aSize ) >> PAGE_SHIFT;
    aMap->PageCount = lLastPage - lFirstPage + 1; 

    aMap->PageList = kmalloc( sizeof( struct page* ) * aMap->PageCount, GFP_KERNEL );
    if( unlikely( !aMap->PageList ) )
    {
        return OS_RESULT_OUT_OF_RESOURCES;
    }
    lPageList = ( struct page** ) aMap->PageList;


#if ( LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27) || !defined( OS_ALLOW_GPL ) )
#if ( LINUX_VERSION_CODE < KERNEL_VERSION(5,8,0) )
    down_read( &current->mm->mmap_sem );
#else
    mmap_read_lock( current->mm) ;
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6,5,0)
    lPageCount = get_user_pages( ( ( unsigned long ) aUserAddress ) & PAGE_MASK,
        aMap->PageCount, FOLL_WRITE, lPageList );
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(4,9,0)
    lPageCount = get_user_pages( ( ( unsigned long ) aUserAddress ) & PAGE_MASK,
        aMap->PageCount, FOLL_WRITE, lPageList, NULL ); 
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(4,6,0)
    lPageCount = get_user_pages( ( ( unsigned long ) aUserAddress ) & PAGE_MASK,
        aMap->PageCount, 1, 0, lPageList, NULL );
#elif defined(UBUNTU_PATCH)
    lPageCount = get_user_pages( current, current->mm, ( ( unsigned long ) aUserAddress ) & PAGE_MASK,
        aMap->PageCount, FOLL_WRITE, lPageList, NULL );
#else
    lPageCount = get_user_pages( current, current->mm, ( ( unsigned long ) aUserAddress ) & PAGE_MASK,
        aMap->PageCount, 1, 0, lPageList, NULL );
#endif
#if ( LINUX_VERSION_CODE < KERNEL_VERSION(5,8,0) )
    up_read( &current->mm->mmap_sem );
#else
    mmap_read_unlock( current->mm );
#endif
    
#else
    lPageCount = get_user_pages_fast( ( ( unsigned long ) aUserAddress ) & PAGE_MASK, aMap->PageCount,
        1, lPageList );
#endif 
    if( unlikely( lPageCount != ( int ) aMap->PageCount ) )
    {
        for( i = 0; i < ( unsigned int ) lPageCount; i++ )
        {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,6,0)
            put_page( lPageList[ i ] );
#else
            page_cache_release( lPageList[ i ] );
#endif
        }
        kfree( lPageList );
        return OS_RESULT_OUT_OF_RESOURCES;
    }
    
    lKernelPageAddress = vmap( ( struct page** ) aMap->PageList, aMap->PageCount, VM_MAP, PAGE_KERNEL );
    if( unlikely( !lKernelPageAddress ) )
    {
        for( i = 0; i < aMap->PageCount; i++ )
        {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,6,0)
            put_page( lPageList[ i ] );
#else
            page_cache_release( lPageList[ i ] );
#endif
        }
        kfree( lPageList );
        return OS_RESULT_OUT_OF_RESOURCES;
    }

    aMap->UserAddress = aUserAddress;
    aMap->KernelAddress = ( OS_VOID* ) ( ( ( unsigned long ) lKernelPageAddress ) 
        + offset_in_page( aUserAddress ) );
    aMap->Size = aSize;

    return OS_RESULT_OK;
}

OS_RESULT OS_MapDelete( OS_MEMORYMAP* aMap )
{
    struct page* lPage;
    struct page** lPageList = ( struct page** ) aMap->PageList;
    unsigned int i;

    vunmap( ( void* ) ( ( ( unsigned long ) aMap->KernelAddress ) & PAGE_MASK ) );

    for( i = 0; i < aMap->PageCount; i++ )
    {
        lPage = lPageList[ i ];
        if( !PageReserved( lPage ) )
        {
            SetPageDirty( lPage );
        }  
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,6,0)
        put_page( lPage );
#else
        page_cache_release( lPage );
#endif
    }        
    kfree( lPageList );

    return OS_RESULT_OK;
}

