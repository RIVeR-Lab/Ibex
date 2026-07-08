// *****************************************************************************
//
//     Copyright (c) 2019, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVCOREGEVDEVICE_H__
#define __PVCOREGEVDEVICE_H__

#include <PvSoftDeviceGEV.h>
#include <PvCoreGEVDeviceLib.h>
#include <PvStreamingChannelSourceCoreGEV.h>

class PV_COREGEV_DEVICE_API PvCoreGEVDevice
    : public PvSoftDeviceGEV
{
public:

    PvCoreGEVDevice();
    virtual ~PvCoreGEVDevice();

    using PvSoftDeviceGEV::AddStream;
    PvResult AddStream( PvStreamingChannelSourceCoreGEV *aSource );

};


#endif
