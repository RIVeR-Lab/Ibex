// *****************************************************************************
//
// Copyright (c) 2018, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <PvSoftDeviceGEVInterfaces.h>
#include <PvFPSStabilizer.h>


///
/// \brief This class shows how to implement a streaming channel source
///

class MySource
    : public IPvStreamingChannelSource
{
public:

    MySource();
    virtual ~MySource();

    // IPvStreamingChannelSource implementation
    uint32_t GetWidth() const { return mWidth; }
    uint32_t GetHeight() const { return mHeight; }
    uint32_t GetOffsetX() const { return 0; }
    uint32_t GetOffsetY() const { return 0; }
    PvPixelType GetPixelType() const { return mPixelType; }
    void GetWidthInfo( uint32_t &aMin, uint32_t &aMax, uint32_t &aInc ) const;
    void GetHeightInfo( uint32_t &aMin, uint32_t &aMax, uint32_t &aInc ) const;
    uint32_t GetChunksSize() const { return GetRequiredChunkSize(); }
    uint32_t GetPayloadSize() const { return 0; }
    PvScanType GetScanType() const { return PvScanTypeArea; }
    PvResult GetSupportedPixelType( int aIndex, PvPixelType &aPixelType ) const;
    PvResult GetSupportedChunk( int aIndex, uint32_t &aID, PvString &aName ) const;
    PvResult GetSupportedRegion( int aIndex, uint32_t &aID, PvString &aName ) const;
    bool GetChunkModeActive() const { return mChunkModeActive; }
    bool GetChunkEnable( uint32_t aChunkID ) const;
    PvResult SetWidth( uint32_t aWidth );
    PvResult SetHeight( uint32_t aHeight );
    PvResult SetOffsetX( uint32_t aOffsetX ) { PVUNREFPARAM( aOffsetX ); return PvResult::Code::NOT_SUPPORTED; }
    PvResult SetOffsetY( uint32_t aOffsetY ) { PVUNREFPARAM( aOffsetY ); return PvResult::Code::NOT_SUPPORTED; }
    PvResult SetPixelType( PvPixelType aPixelType );
    PvResult SetChunkModeActive( bool aEnabled ) { mChunkModeActive = aEnabled; return PvResult::Code::OK; }
    PvResult SetChunkEnable( uint32_t aChunkID, bool aEnabled );
    void OnOpen( const PvString &aDestIP, uint16_t aDestPort );
    void OnClose();
    void OnStreamingStart();
    void OnStreamingStop();
    PvBuffer *AllocBuffer();
    void FreeBuffer( PvBuffer *aBuffer );
    PvResult QueueBuffer( PvBuffer *aBuffer );
    PvResult RetrieveBuffer( PvBuffer **aBuffer );
    void AbortQueuedBuffers();
    void CreateRegisters( IPvRegisterMap *aRegisterMap, IPvRegisterFactory *aFactory ) { PVUNREFPARAM( aRegisterMap ); PVUNREFPARAM( aFactory ); }
    void CreateGenApiFeatures( IPvRegisterMap *aRegisterMap, IPvGenApiFactory *aFactory ) { PVUNREFPARAM( aRegisterMap ); PVUNREFPARAM( aFactory ); }

protected:

    uint32_t GetRequiredChunkSize() const;
    void ResizeBufferIfNeeded( PvBuffer *aBuffer );

    void FillTestPatternMono8( PvBuffer *aBuffer );
    void FillTestPatternRGB16( PvBuffer *aBuffer );

    void AddChunkSample( PvBuffer *aBuffer );

private:

    uint32_t mWidth;
    uint32_t mHeight;
    PvPixelType mPixelType;

    PvFPSStabilizer mStabilizer;

    uint32_t mBufferCount;
    PvBuffer *mAcquisitionBuffer;

    uint32_t mSeed;
    uint32_t mFrameCount;

    bool mChunkModeActive;
    bool mChunkSampleEnabled;
};
