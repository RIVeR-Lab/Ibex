// *****************************************************************************
//
// Copyright (c) 2022, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __IPVSEMAPHORE_H__
#define __IPVSEMAPHORE_H__


#include <PvVirtualDeviceLib.h>


class PV_VIRTUAL_DEVICE_API IPvSemaphore
{
public:
    virtual ~IPvSemaphore() {}

    virtual void Notify() = 0;
    virtual void Abort() = 0;
    virtual bool Wait() = 0;
    virtual void Reset() = 0;
};

#endif // __IPVSEMAPHORE_H__
