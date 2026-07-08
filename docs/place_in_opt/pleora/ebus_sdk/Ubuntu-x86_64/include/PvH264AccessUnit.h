// *****************************************************************************
//
//     Copyright (c) 2010, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVH264ACCESSUNIT_H__
#define __PVH264ACCESSUNIT_H__

#include <PvBufferLib.h>
#include <PvPixelType.h>


namespace PvBufferLib
{
    class H264AccessUnit;
    class Buffer;
}


class PV_BUFFER_API IPvH264AccessUnit
{
public:

    virtual ~IPvH264AccessUnit() {}

    virtual uint32_t GetSize() const = 0;
    virtual uint32_t GetNALDataSize() const = 0;
    virtual uint8_t *GetNALDataPtr() = 0;

    virtual uint32_t GetNALCount() const = 0;
    virtual PvResult GetNALPtr( uint32_t aIndex, uint8_t **aPtr, uint32_t &aLength ) = 0;
    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

};


class PV_BUFFER_API PvH264AccessUnit
    : public IPvH264AccessUnit
{
public:

    virtual ~PvH264AccessUnit() {}

    virtual uint32_t GetSize() const = 0;
    virtual uint32_t GetNALDataSize() const = 0;
    virtual uint8_t *GetNALDataPtr() = 0;
    virtual PvResult Alloc( uint64_t aPayloadLength, uint32_t aMaximumChunkLength = 0 ) = 0;
    virtual void Free() = 0;
    virtual PvResult Clear() = 0;
    virtual PvResult Init( uint32_t aWidth, uint32_t aHeight ) = 0;
    virtual PvResult AddNAL( const uint8_t *aPtr, uint32_t aLength ) = 0;
    virtual PvResult CopyNALData( const uint8_t *aPtr, uint32_t aLength, uint32_t aWidth, uint32_t aHeight ) = 0;

};


#endif
