// *****************************************************************************
//
// Copyright (c) 2018, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include <PvSampleUtils.h>
#include <PvSoftDeviceGEV.h>
#include <PvBuffer.h>
#include <PvFPSStabilizer.h>
#include <PvSampleTransmitterConfig.h>
#include <PvStreamingChannelSourceDefault.h>


PV_INIT_SIGNAL_HANDLER();


#define BUFFERCOUNT ( 16 )


// This class shows how to implement a streaming channel source
class MySimpleSource
    : public PvStreamingChannelSourceDefault
{
public:

    MySimpleSource()
        : mAcquisitionBuffer( NULL )
        , mSeed( 0 )
        , mFrameCount( 0 )
    {
    }

    // Request to queue a buffer for acquisition.
    // Return OK if the buffer is queued or any error if no more room in acquisition queue
    PvResult QueueBuffer( PvBuffer *aBuffer )
    {
        // We use mAcqusitionBuffer as a 1-deep acquisition pipeline
        if ( mAcquisitionBuffer == NULL )
        {
            // No buffer queued, accept it
            mAcquisitionBuffer = aBuffer;

            // Acquire buffer - could be done in another thread
            FillTestPatternMono8( mAcquisitionBuffer );
            mFrameCount++;

            return PvResult::Code::OK;
        }

        // We already have a buffer queued for acquisition
        return PvResult::Code::BUSY;
    }

    // Request to give back a buffer ready for transmission.
    // Either block until a buffer is available or return any error
    PvResult RetrieveBuffer( PvBuffer **aBuffer )
    {
        if ( mAcquisitionBuffer == NULL )
        {
            // No buffer queued for acquisition
            return PvResult::Code::NO_AVAILABLE_DATA;
        }

        while ( !mStabilizer.IsTimeToDisplay( DEFAULT_FPS ) )
        {
            PvSleepMs( 1 );
        }

        // Remove buffer from 1-deep pipeline
        *aBuffer = mAcquisitionBuffer;
        mAcquisitionBuffer = NULL;

        return PvResult::Code::OK;
    }

    // Generate a greyscale test pattern in a PvBuffer
    void FillTestPatternMono8( PvBuffer *aBuffer )
    {
        PvImage *lImage = aBuffer->GetImage();
        uint32_t lHeight = lImage->GetHeight();
        uint32_t lWidth = lImage->GetWidth();

        for ( uint32_t y = 0; y < lHeight; y++ )
        {
            uint8_t lValue = static_cast<uint8_t>( mSeed + y );
            uint8_t *lPtr = aBuffer->GetDataPointer();
            lPtr += ( y * lImage->GetWidth() ) + lImage->GetOffsetX();

            for ( uint32_t x = 0; x < lWidth; x++ )
            {
                *lPtr++ = lValue++;
            }
        }

        mSeed++;
    }

private:

    PvFPSStabilizer mStabilizer;

    PvBuffer *mAcquisitionBuffer;

    uint32_t mSeed;
    uint32_t mFrameCount;

};


int main( int aCount, const char **aArgs )
{
    PVUNREFPARAM( aCount );
    PVUNREFPARAM( aArgs );

    PvString lInterface = ( aCount > 1 ) ? PvString( aArgs[ 1 ] ) : PvSelectInterface();
    if ( lInterface.GetLength() == 0 )
    {
        std::cout << "No interface selected, terminating" << std::endl;
        return -1;
    }

    MySimpleSource lSimpleSource;
    PvSoftDeviceGEV lDevice;
    lDevice.AddStream( &lSimpleSource );

    IPvSoftDeviceGEVInfo *lInfo = lDevice.GetInfo();
    const std::string lModelName( lInfo->GetModelName().GetAscii() );

    PvResult lResult = lDevice.Start( lInterface );
    if ( !lResult.IsOK() )
    {
        std::cout << "Error starting " << lModelName << std::endl;
        return -1;
    }

    std::cout << lModelName << " started" << std::endl;

    PvFlushKeyboard();
    while ( !PvKbHit() )
    {
        PvSleepMs( 100 );
    }

    lDevice.Stop();
    std::cout << lModelName << " stopped" << std::endl;

    return 0;
}


