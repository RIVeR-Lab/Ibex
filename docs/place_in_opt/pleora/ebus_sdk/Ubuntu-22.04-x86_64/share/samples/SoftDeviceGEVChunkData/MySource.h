// *****************************************************************************
//
// Copyright (c) 2023, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <PvSoftDeviceGEVInterfaces.h>
#include <PvStreamingChannelSourceDefault.h>
#include <PvFPSStabilizer.h>


///
/// \brief This class shows how to implement a streaming channel source using
///        using a payload of type ChunkData
///

class MySource
    : public IPvRegisterEventSink
    , public PvStreamingChannelSourceDefault
{
public:

    MySource();
    virtual ~MySource();

    // PvStreamingChannelSourceDefault implementation
    // Assuming only payload will be CHUNKDATA
    uint32_t GetWidth() const { return mWidth; }
    uint32_t GetHeight() const { return mHeight; }
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

protected:

    uint32_t GetRequiredChunkSize() const;
    void ResizeBufferIfNeeded( PvBuffer *aBuffer );
    void AddChunkSample( PvBuffer *aBuffer );

private:
    static uint32_t mChannelCount;
    uint32_t mChannelNum;

    uint32_t mWidth;
    uint32_t mHeight;
    PvPixelType mPixelType;

    PvFPSStabilizer mStabilizer;

    uint32_t mBufferCount;
    PvBuffer *mAcquisitionBuffer;

    uint32_t mFrameCount;

    bool mChunkModeActive;
    bool mChunkSampleEnabled;

};
