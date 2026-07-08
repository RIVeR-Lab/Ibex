// *****************************************************************************
//
//     Copyright (c) 2020, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <PvDeviceLib.h>
#include <PvDeviceEnums.h>

namespace PvVirtualDeviceLib
{
// Layout of the GEV ACTION_CMD packet
struct GEV_ActionCmd
{
    uint8_t  mCode;
    uint8_t  mFlags;
    uint16_t mCommand;
    uint16_t mLength;
    uint16_t mRequestId;
    uint32_t mDeviceKey;
    uint32_t mGroupKey;
    uint32_t mGroupMask;
    uint32_t mActionTimeHigh;
    uint32_t mActionTimeLow;
};

// Layout of the GEV ACTION_ACK packet
struct GEV_ActionAck
{ 
    uint16_t mStatus;
    uint16_t mAcknowledge;
    uint16_t mLength;
    uint16_t mAckId;
};



///
/// \brief An interface to register a handler of ACTION_CMD in the GVCP 
///        processing.
///
class IActionHandler
{
public:    
    virtual ~IActionHandler() {};

    virtual PvResult HandleActionCmd(PvString aFromIp, uint32_t aFromPort, GEV_ActionCmd *aActionCmdPtr) = 0;

private:


};

}

