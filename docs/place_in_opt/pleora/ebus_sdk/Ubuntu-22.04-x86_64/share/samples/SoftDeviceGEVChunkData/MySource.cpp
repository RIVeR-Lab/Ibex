// *****************************************************************************
//
// Copyright (c) 2023, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "Defines.h"
#include "MySource.h"
#include "Utilities.h"

#include <PvSampleUtils.h>

uint32_t MySource::mChannelCount = 0;

///
/// \brief Constructor
///

MySource::MySource()
    : mChannelNum( mChannelCount++ )
    , mWidth( WIDTH_DEFAULT )
    , mHeight( HEIGHT_DEFAULT)
    , mPixelType( PvPixelMono8 )
    , mBufferCount( 0 )
    , mAcquisitionBuffer( NULL )
    , mFrameCount( 0 )
    , mChunkModeActive( true )
    , mChunkSampleEnabled( true )
{
}


///
/// \brief Destructor
///

MySource::~MySource()
{
}

///
/// \brief Width info query
///

void MySource::GetWidthInfo( uint32_t &aMin, uint32_t &aMax, uint32_t &aInc ) const
{
    aMin = WIDTH_MIN;
    aMax = WIDTH_MAX;
    aInc = WIDTH_INC;
}


///
/// \brief Height info query
///

void MySource::GetHeightInfo( uint32_t &aMin, uint32_t &aMax, uint32_t &aInc ) const
{
    aMin = HEIGHT_MIN;
    aMax = HEIGHT_MAX;
    aInc = HEIGHT_INC;
}

///
/// \brief Width setter
///

PvResult MySource::SetWidth( uint32_t aWidth )
{
    if ( ( aWidth < WIDTH_MIN ) || ( aWidth > WIDTH_MAX ) )
    {
        return PvResult::Code::INVALID_PARAMETER;
    }

    mWidth = aWidth;
    return PvResult::Code::OK;
}

///
/// \brief Height setter
///

PvResult MySource::SetHeight( uint32_t aHeight )
{
    if ( ( aHeight < HEIGHT_MIN ) || ( aHeight > HEIGHT_MAX ) )
    {
        return PvResult::Code::INVALID_PARAMETER;
    }

    mHeight = aHeight;
    return PvResult::Code::OK;
}



///
/// \brief Supported pixel types query
///

PvResult MySource::GetSupportedPixelType( int aIndex, PvPixelType &aPixelType ) const
{
    switch ( aIndex )
    {
    case 0:
        aPixelType = PvPixelMono8;
        return PvResult::Code::OK;

    default:
        break;
    }

    return PvResult::Code::INVALID_PARAMETER;
}


///
/// \brief Chunk support
///

PvResult MySource::GetSupportedChunk( int aIndex, uint32_t &aID, PvString &aName ) const
{
    switch ( aIndex )
    {
    case 0:
        aID = CHUNKID;
        aName = "Sample";
        return PvResult::Code::OK;

    default:
        break;
    }

    return PvResult::Code::INVALID_PARAMETER;
}


///
/// \brief Region support
///

PvResult MySource::GetSupportedRegion( int aIndex, uint32_t &aID, PvString &aName ) const
{
    PVUNREFPARAM( aIndex );
    PVUNREFPARAM( aID );
    PVUNREFPARAM( aName );

    return PvResult::Code::NOT_SUPPORTED;
}

///
/// \brief Chunk enabled getter, by chunk type
///

bool MySource::GetChunkEnable( uint32_t aChunkID ) const
{
    switch ( aChunkID )
    {
    case CHUNKID:
        return mChunkSampleEnabled;

    default:
        break;
    }

    return false;
}


///
/// \brief Chunk enabled setter, by chunk type
///

PvResult MySource::SetChunkEnable( uint32_t aChunkID, bool aEnabled )
{
    printf("SetChunkEnable for Chunkid %d and aEnabled %d \n", aChunkID, aEnabled);
    switch ( aChunkID )
    {
    case CHUNKID:
        mChunkSampleEnabled = aEnabled;
        return PvResult::Code::OK;

    default:
        break;
    }

    return PvResult::Code::INVALID_PARAMETER;
}


///
/// \brief Stream channel open notification
///

void MySource::OnOpen( const PvString &aDestIP, uint16_t aDestPort )
{
    std::cout << "Streaming channel opened to " << aDestIP.GetAscii() << ":" << aDestPort << std::endl;
}


///
/// \brief Stream channel close notification
///

void MySource::OnClose()
{
    std::cout << "Streaming channel closed" << std::endl;
}


///
/// \brief Streaming start notification
///

void MySource::OnStreamingStart()
{
    std::cout << "Streaming start" << std::endl;
    mStabilizer.Reset();
}


///
/// \brief Streaming stop notification
///

void MySource::OnStreamingStop()
{
    std::cout << "Streaming stop" << std::endl;
}


///
// \brief Request for a new buffer.
///
/// Return a new buffer until buffer count is reached, then return NULL to 
/// signal that all buffers have been allocated.
///

PvBuffer *MySource::AllocBuffer()
{
    if ( mBufferCount < BUFFERCOUNT )
    {
        mBufferCount++;
        return new PvBuffer(PvPayloadTypeChunkData);
    }

    return NULL;
}


///
/// \brief Request to free one of the buffers allocated with AllocBuffer
///

void MySource::FreeBuffer( PvBuffer *aBuffer )
{
    delete aBuffer;
    mBufferCount--;
}


///
/// \brief Request to queue a buffer for acquisition.
///
/// Return OK if the buffer is queued or any error if no more room in acquisition queue
///

PvResult MySource::QueueBuffer( PvBuffer *aBuffer )
{
    // We use mAcqusitionBuffer as a 1-deep acquisition pipeline
    if ( mAcquisitionBuffer == NULL )
    {
        // No buffer queued, accept it
        mAcquisitionBuffer = aBuffer;

        // Acquire buffer - could be done in another thread
        ResizeBufferIfNeeded( mAcquisitionBuffer );
        AddChunkSample( mAcquisitionBuffer );
        mFrameCount++;

        return PvResult::Code::OK;
    }

    // We already have a buffer queued for acquisition
    return PvResult::Code::BUSY;
}


///
/// \brief Request to give back a buffer ready for transmission.
///
/// Either block until a buffer is available or return any error
///

PvResult MySource::RetrieveBuffer( PvBuffer **aBuffer )
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


///
/// \brief Signal to abort all buffers waiting for acquisition.
///
/// Nothing to do here // with our simple 1-deep with in-line test pattern 
/// generation acquisition sample.
///

void MySource::AbortQueuedBuffers()
{
}

///
/// \brief Returns the required chunk size for the current configuration
///

uint32_t MySource::GetRequiredChunkSize() const
{
    return ( mChunkModeActive && mChunkSampleEnabled ) ? CHUNKSIZE : 0;
}


///
/// \brief Resizes a buffer for acquisition with the current configuration
///

void MySource::ResizeBufferIfNeeded( PvBuffer *aBuffer )
{
    uint32_t lRequiredChunkSize = GetRequiredChunkSize();
    PvChunkData *lChunkData = aBuffer->GetChunkData();

    if ( 
         ( lChunkData->GetChunkDataSize() != lRequiredChunkSize ) )
    {
        lChunkData->Alloc( lRequiredChunkSize );
    }
}


///
/// \brief Add chunk data to a buffer
///

void MySource::AddChunkSample( PvBuffer *aBuffer )
{
    aBuffer->ResetChunks();
    if ( !mChunkModeActive || !mChunkSampleEnabled )
    {
        return;
    }

    // Add frame count to chunk data
    uint8_t lData[ 36 ] = { 0 };

    // Add frame count to chunk data
    memcpy( lData, &mFrameCount, sizeof( mFrameCount ) );

    // Add current time string to chunk data
    Time2UInt8( lData + 4, sizeof( lData ) - 4 );

    // Add chunk data to buffer
    aBuffer->SetChunkLayoutID( CHUNKLAYOUTID );
    aBuffer->AddChunk( CHUNKID, lData, sizeof( lData ) );

}

