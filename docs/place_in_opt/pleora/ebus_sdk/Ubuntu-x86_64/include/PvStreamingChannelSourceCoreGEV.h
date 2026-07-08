// *****************************************************************************
//
// Copyright (c) 2018, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __IPVSTREAMINGCHANNELSOURCECOREGEV_H__
#define __IPVSTREAMINGCHANNELSOURCECOREGEV_H__


#include <PvSoftDeviceGEVInterfaces.h>
#include <PvCoreGEVDeviceLib.h>


class PV_COREGEV_DEVICE_API IPvStreamingChannelSourceCoreGEV
    : public IPvStreamingChannelSource
{
public:

    enum class DigitizationTaps
    {
        One,
        Two,
        Four
    };

    virtual PvResult SetDigitizationTaps( DigitizationTaps aTaps ) = 0;
};


class PV_COREGEV_DEVICE_API PvStreamingChannelSourceCoreGEV
    : public IPvStreamingChannelSourceCoreGEV
{
public:

    PvStreamingChannelSourceCoreGEV();
    virtual ~PvStreamingChannelSourceCoreGEV();

    // IPvStreamingChannelSource
    virtual uint32_t GetWidth() const override;
    virtual uint32_t GetHeight() const override;
    virtual uint32_t GetOffsetX() const override;
    virtual uint32_t GetOffsetY() const override;
    virtual PvPixelType GetPixelType() const override;
    virtual void GetWidthInfo( uint32_t &aMin, uint32_t &aMax, uint32_t &aInc ) const override;
    virtual void GetHeightInfo( uint32_t &aMin, uint32_t &aMax, uint32_t &aInc ) const override;
    uint32_t GetChunksSize() const override;
    uint32_t GetPayloadSize() const override;
    PvScanType GetScanType() const override;
    bool GetChunkModeActive() const override;
    bool GetChunkEnable( uint32_t aChunkID ) const override;
    PvResult GetSupportedPixelType( int aIndex, PvPixelType &aPixelType ) const override;
    PvResult GetSupportedChunk( int aIndex, uint32_t &aID, PvString &aName ) const override;

    virtual PvResult SetWidth( uint32_t aWidth ) override;
    virtual PvResult SetHeight( uint32_t aHeight ) override;
    virtual PvResult SetOffsetX( uint32_t aOffsetX ) override;
    virtual PvResult SetOffsetY( uint32_t aOffsetY ) override;
    virtual PvResult SetPixelType( PvPixelType aPixelType ) override;
    PvResult SetChunkModeActive( bool aEnabled ) override;
    PvResult SetChunkEnable( uint32_t aChunkID, bool aEnabled ) override;

    virtual void OnOpen( const PvString &aDestIP, uint16_t aDestPort ) override;
    virtual void OnClose() override;

    virtual void OnStreamingStart() override;
    virtual void OnStreamingStop() override;

    PvBuffer *AllocBuffer() override;
    void FreeBuffer( PvBuffer *aBuffer ) override;

    PvResult QueueBuffer(PvBuffer *aBuffer) override;
    PvResult RetrieveBuffer(PvBuffer **aBuffer) override;
    void AbortQueuedBuffers() override;

    virtual void CreateRegisters( IPvRegisterMap *aRegisterMap, IPvRegisterFactory *aFactory ) override;
    virtual void CreateGenApiFeatures( IPvRegisterMap *aRegisterMap, IPvGenApiFactory *aFactory ) override;

    // IPvStreamingChannelSourceCoreGEV
    virtual PvResult SetDigitizationTaps( DigitizationTaps aTaps ) override;

    // Local interface.
    void SetSource( IPvStreamingChannelSourceCoreGEV *aSource );

private:

    IPvStreamingChannelSourceCoreGEV *mSourceDevice = nullptr;
};


#endif // __IPVSTREAMINGCHANNELSOURCECOREGEV_H__
