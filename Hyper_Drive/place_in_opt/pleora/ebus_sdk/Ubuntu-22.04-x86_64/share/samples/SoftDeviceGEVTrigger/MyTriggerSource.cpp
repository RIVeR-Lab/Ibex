// *****************************************************************************
//
// Copyright (c) 2022, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "Defines.h"
#include "MyTriggerSource.h"
#include "Utilities.h"

#include <PvSampleUtils.h>


uint32_t MyTriggerSource::sCurrentChannelIndex = 0;


///
/// \brief Constructor
///

MyTriggerSource::MyTriggerSource()
    : PvStreamingChannelSourceTrigger( sCurrentChannelIndex, BASE_ADDR )
    , mWidth( WIDTH_DEFAULT )
    , mHeight( HEIGHT_DEFAULT )
    , mPixelType( PvPixelMono8 )
    , mBufferCount( 0 )
    , mAcquisitionBuffer( NULL )
    , mSeed( 0 )
    , mFrameCount( 0 )
    , mChunkModeActive( false )
    , mChunkSampleEnabled( false )
    , mChannel( sCurrentChannelIndex )
{
    // Should use std::make_unique if available to your compiler.
    auto lAcquisitionStart = std::unique_ptr<PvTriggerSelectorAcquisitionStart>( new PvTriggerSelectorAcquisitionStart() );
    AddSelector( TRIGGER_SELECTOR_ACQUISITIONSTART, std::move( lAcquisitionStart ) );
    AddSelectorTriggerCallback( TRIGGER_SELECTOR_ACQUISITIONSTART, this );

    auto lFrameStart = std::unique_ptr<PvTriggerSelectorFrameStart>( new PvTriggerSelectorFrameStart() );
    AddSelector( TRIGGER_SELECTOR_FRAMESTART, std::move( lFrameStart ) );

    auto lLineStart = std::unique_ptr<PvTriggerSelectorLineStart>( new PvTriggerSelectorLineStart() );
    AddSelector( TRIGGER_SELECTOR_LINESTART, std::move( lLineStart ) );

    AddSource( 0, "Software" );
    AddSource( 1, "SingleEndedInputs" );
    AddSource( 2, "DifferentialLine" );

    sCurrentChannelIndex++;
}


///
/// \brief Destructor
///

MyTriggerSource::~MyTriggerSource()
{
}


///
/// \brief Width info query
///

void MyTriggerSource::GetWidthInfo( uint32_t &aMin, uint32_t &aMax, uint32_t &aInc ) const
{
    aMin = WIDTH_MIN;
    aMax = WIDTH_MAX;
    aInc = WIDTH_INC;
}


///
/// \brief Height info query
///

void MyTriggerSource::GetHeightInfo( uint32_t &aMin, uint32_t &aMax, uint32_t &aInc ) const
{
    aMin = HEIGHT_MIN;
    aMax = HEIGHT_MAX;
    aInc = HEIGHT_INC;
}


///
/// \brief Height setter
///

PvResult MyTriggerSource::SetHeight( uint32_t aHeight )
{
    if ( ( aHeight < HEIGHT_MIN ) || ( aHeight > HEIGHT_MAX ) )
    {
        return PvResult::Code::INVALID_PARAMETER;
    }

    mHeight = aHeight;
    return PvResult::Code::OK;
}


///
/// \brief Pixel type setter
///

PvResult MyTriggerSource::SetPixelType( PvPixelType aPixelType )
{
    mPixelType = aPixelType;
    return PvResult::Code::OK;
}


///
/// \brief Supported pixel types query
///

PvResult MyTriggerSource::GetSupportedPixelType( int aIndex, PvPixelType &aPixelType ) const
{
    switch ( aIndex )
    {
    case 0:
        aPixelType = PvPixelMono8;
        return PvResult::Code::OK;

    case 1:
        aPixelType = PvPixelRGB8;
        return PvResult::Code::OK;

    case 2:
        aPixelType = PvPixelRGBa8;
        return PvResult::Code::OK;

    case 3:
        aPixelType = PvPixelBGR8;
        return PvResult::Code::OK;

    case 4:
        aPixelType = PvPixelBGRa8;
        return PvResult::Code::OK;

    case 5:
        aPixelType = PvPixelYCbCr422_8_CbYCrY;
        return PvResult::Code::OK;

    case 6:
        aPixelType = PvPixelYCbCr8_CbYCr;
        return PvResult::Code::OK;

    default:
        break;
    }

    return PvResult::Code::INVALID_PARAMETER;
}


///
/// \brief Width setter
///

PvResult MyTriggerSource::SetWidth( uint32_t aWidth )
{
    if ( ( aWidth < WIDTH_MIN ) || ( aWidth > WIDTH_MAX ) )
    {
        return PvResult::Code::INVALID_PARAMETER;
    }

    mWidth = aWidth;
    return PvResult::Code::OK;
}


///
/// \brief Chunk support
///

PvResult MyTriggerSource::GetSupportedChunk( int aIndex, uint32_t &aID, PvString &aName ) const
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

PvResult MyTriggerSource::GetSupportedRegion( int aIndex, uint32_t &aID, PvString &aName ) const
{
    PVUNREFPARAM( aIndex );
    PVUNREFPARAM( aID );
    PVUNREFPARAM( aName );

    return PvResult::Code::NOT_SUPPORTED;
}


///
/// \brief Chunk enabled getter, by chunk type
///

bool MyTriggerSource::GetChunkEnable( uint32_t aChunkID ) const
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

PvResult MyTriggerSource::SetChunkEnable( uint32_t aChunkID, bool aEnabled )
{
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

void MyTriggerSource::OnOpen( const PvString &aDestIP, uint16_t aDestPort )
{
    std::cout << "Streaming channel opened to " << aDestIP.GetAscii() << ":" << aDestPort << std::endl;
}


///
/// \brief Stream channel close notification
///

void MyTriggerSource::OnClose()
{
    std::cout << "Streaming channel closed" << std::endl;
}


///
/// \brief Streaming start notification
///

void MyTriggerSource::OnStreamingStart()
{
    std::cout << "Streaming start" << std::endl;
    mStabilizer.Reset();
    PvStreamingChannelSourceTrigger::OnStreamingStart();
}


///
/// \brief Streaming stop notification
///

void MyTriggerSource::OnStreamingStop()
{
    std::cout << "Streaming stop" << std::endl;
    PvStreamingChannelSourceTrigger::OnStreamingStop();
}


///
// \brief Request for a new buffer.
///
/// Return a new buffer until buffer count is reached, then return NULL to 
/// signal that all buffers have been allocated.
///

PvBuffer *MyTriggerSource::AllocBuffer()
{
    if ( mBufferCount < BUFFERCOUNT )
    {
        mBufferCount++;
        return new PvBuffer;
    }

    return NULL;
}


///
/// \brief Request to free one of the buffers allocated with AllocBuffer
///

void MyTriggerSource::FreeBuffer( PvBuffer *aBuffer )
{
    delete aBuffer;
    mBufferCount--;
}


///
/// \brief Request to queue a buffer for acquisition.
///
/// Return OK if the buffer is queued or any error if no more room in acquisition queue
///

PvResult MyTriggerSource::QueueBuffer( PvBuffer *aBuffer )
{
    // We use mAcqusitionBuffer as a 1-deep acquisition pipeline
    if ( mAcquisitionBuffer == NULL )
    {
        // No buffer queued, accept it
        mAcquisitionBuffer = aBuffer;

        // Acquire buffer - could be done in another thread
        ResizeBufferIfNeeded( mAcquisitionBuffer );
        switch ( GetPixelType() )
        {
        case PvPixelMono8:
            FillTestPatternMono8( mAcquisitionBuffer );
            break;

        case PvPixelBGR8:
        case PvPixelBGRa8:
        case PvPixelRGB8:
        case PvPixelRGBa8:
            FillTestPatternRGB( mAcquisitionBuffer );
            break;

        case PvPixelYCbCr8_CbYCr:
            FillTestPatternYUV444( mAcquisitionBuffer );
            break;

        case PvPixelYCbCr422_8_CbYCrY:
            FillTestPatternYUV422( mAcquisitionBuffer );
            break;

        default:
            break;
        }
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

PvResult MyTriggerSource::RetrieveBuffer( PvBuffer **aBuffer )
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

void MyTriggerSource::AbortQueuedBuffers()
{
}

///
/// \brief Returns the required chunk size for the current configuration
///

uint32_t MyTriggerSource::GetRequiredChunkSize() const
{
    return ( mChunkModeActive && mChunkSampleEnabled ) ? CHUNKSIZE : 0;
}

/// Define this if you wish to allocate a separate data space for chunk
/// data. Useful when you are attaching a buffer that cannot be resized
/// to accomodate chunk data.
//#define ALLOC_CHUNK_DATA

///
/// \brief Resizes a buffer for acquisition with the current configuration
///

void MyTriggerSource::ResizeBufferIfNeeded( PvBuffer *aBuffer )
{
    uint32_t lRequiredChunkSize = GetRequiredChunkSize();
    PvImage *lImage = aBuffer->GetImage();
#ifndef ALLOC_CHUNK_DATA
    if ( ( lImage->GetWidth() != mWidth ) ||
         ( lImage->GetHeight() != mHeight ) ||
         ( lImage->GetPixelType() != mPixelType ) ||
         ( lImage->GetMaximumChunkLength() != lRequiredChunkSize ) )
    {
        lImage->Alloc( mWidth, mHeight, mPixelType, 0, 0, lRequiredChunkSize );
    }
#else
    if ( ( lImage->GetWidth() != mWidth ) ||
        ( lImage->GetHeight() != mHeight ) ||
         ( lImage->GetPixelType() != mPixelType ) )
    {
        lImage->Alloc( mWidth, mHeight, mPixelType, 0, 0, 0 );
    }
    if ( lRequiredChunkSize > aBuffer->GetChunkDataCapacity() )
    {
        aBuffer->AllocChunk( lRequiredChunkSize );
    }
#endif
}


///
/// \brief Generate a greyscale test pattern in a PvBuffer
///

void MyTriggerSource::FillTestPatternMono8( PvBuffer *aBuffer )
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


///
/// \brief Generate a color test pattern in a PvBuffer
///

void MyTriggerSource::FillTestPatternRGB( PvBuffer *aBuffer )
{
    PvImage *lImage = aBuffer->GetImage();
    uint32_t lHeight = lImage->GetHeight();
    uint32_t lWidth = lImage->GetWidth();
    uint32_t lPixelBytes = lImage->GetBitsPerPixel() / 8;

    for ( uint32_t y = 0; y < lHeight; y++ )
    {
        uint32_t lValue = mSeed + y;
        uint8_t *lPixelPtr = aBuffer->GetDataPointer();
        lPixelPtr += ( y * lImage->GetWidth() * lPixelBytes ) + lImage->GetOffsetX();

        uint8_t *lPtr = NULL;
        for ( uint32_t x = 0; x < lWidth; x++ )
        {
            // Write current pixel
            lPtr = lPixelPtr;
            *lPtr++ = static_cast<uint8_t>( lValue << 4 );
            *lPtr++ = static_cast<uint8_t>( lValue << 2 );
            *lPtr++ = static_cast<uint8_t>( lValue );

            lValue++;

            // Move to next pixel
            lPixelPtr += lPixelBytes;
        }
    }

    mSeed++;
}


///
/// \brief Generate a color test pattern in a PvBuffer
///

void MyTriggerSource::FillTestPatternYUV444( PvBuffer *aBuffer )
{
    PvImage *lImage = aBuffer->GetImage();
    uint32_t lHeight = lImage->GetHeight();
    uint32_t lWidth = lImage->GetWidth();
    uint32_t lPixelBytes = 3;

    for ( uint32_t y = 0; y < lHeight; y++ )
    {
        uint32_t lValue = mSeed + y;
        uint8_t *lPixelPtr = aBuffer->GetDataPointer();
        lPixelPtr += ( y * lImage->GetWidth() * lPixelBytes ) + lImage->GetOffsetX();

        uint8_t *lPtr = NULL;
        for ( uint32_t x = 0; x < lWidth; x++ )
        {
            // Write current pixel
            lPtr = lPixelPtr;
            *lPtr++ = static_cast<uint8_t>( lValue << 1 );
            *lPtr++ = static_cast<uint8_t>( lValue );
            *lPtr++ = 255 - static_cast<uint8_t>( lValue << 2 );

            lValue++;

            // Move to next pixel
            lPixelPtr += lPixelBytes;
        }
    }

    mSeed++;
}


///
/// \brief Generate a color test pattern in a PvBuffer
///

void MyTriggerSource::FillTestPatternYUV422( PvBuffer *aBuffer )
{
    PvImage *lImage = aBuffer->GetImage();
    uint32_t lHeight = lImage->GetHeight();
    uint32_t lWidth = lImage->GetWidth();
    uint32_t lPixelBytes = 2;

    for ( uint32_t y = 0; y < lHeight; y++ )
    {
        uint32_t lValue = mSeed + y;
        uint8_t *lPixelPtr = aBuffer->GetDataPointer();
        lPixelPtr += ( y * lImage->GetWidth() * lPixelBytes ) + lImage->GetOffsetX();

        uint8_t *lPtr = NULL;
        for ( uint32_t x = 0; x < lWidth; x++ )
        {
            // Write current pixel
            lPtr = lPixelPtr;
            *lPtr++ = ( ( x & 1 ) == 0 ) ?
                static_cast<uint8_t>( lValue << 1 ) :
                255 - static_cast<uint8_t>( lValue << 2 );
            *lPtr++ = static_cast<uint8_t>( lValue );

            lValue++;

            // Move to next pixel
            lPixelPtr += lPixelBytes;
        }
    }

    mSeed++;
}


///
/// \brief Add chunk data to a buffer
///

void MyTriggerSource::AddChunkSample( PvBuffer *aBuffer )
{
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
    aBuffer->ResetChunks();
    aBuffer->SetChunkLayoutID( CHUNKLAYOUTID );
    aBuffer->AddChunk( CHUNKID, lData, sizeof( lData ) );

}

void MyTriggerSource::TriggerCallbackMethod( const IPvTriggerSelector *aSelector )
{
    std::cout << "Entering the callback for MyTriggerSource \n";
    std::cout << "Trigger fired on channel " << mChannel << ", " << aSelector->GetName().GetAscii() << "\n";
    std::cout << "Leaving the callback for MyTriggerSource \n";
}

