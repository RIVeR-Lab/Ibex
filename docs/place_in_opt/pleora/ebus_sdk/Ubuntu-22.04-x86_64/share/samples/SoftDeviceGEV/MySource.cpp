// *****************************************************************************
//
// Copyright (c) 2018, Pleora Technologies Inc., All rights reserved.
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
    , mHeight( HEIGHT_DEFAULT )
    , mPixelType( PvPixelMono8 )
    , mSource0OnlyInt( 50 )
    , mSource0OnlyBool( false )
    , mBufferCount( 0 )
    , mAcquisitionBuffer( NULL )
    , mSeed( 0 )
    , mFrameCount( 0 )
    , mChunkModeActive( false )
    , mChunkSampleEnabled( false )
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
/// \brief Pixel type setter
///

PvResult MySource::SetPixelType( PvPixelType aPixelType )
{
    mPixelType = aPixelType;
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
/// \brief Optional to configure GevSCPSPacketSize max value
///

PvResult MySource::GetGevSCPSPacketSizeInfo( uint32_t& aMax) const 
{
    aMax = 9000;
    return PvResult::Code::OK;
}

///
/// \brief Chunk enabled setter, by chunk type
///

PvResult MySource::SetChunkEnable( uint32_t aChunkID, bool aEnabled )
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
        return new PvBuffer;
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

/// Define this if you wish to allocate a separate data space for chunk
/// data. Useful when you are attaching a buffer that cannot be resized
/// to accomodate chunk data.
//#define ALLOC_CHUNK_DATA

///
/// \brief Resizes a buffer for acquisition with the current configuration
///

void MySource::ResizeBufferIfNeeded( PvBuffer *aBuffer )
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

void MySource::FillTestPatternMono8( PvBuffer *aBuffer )
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

void MySource::FillTestPatternRGB( PvBuffer *aBuffer )
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

void MySource::FillTestPatternYUV444( PvBuffer *aBuffer )
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

void MySource::FillTestPatternYUV422( PvBuffer *aBuffer )
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

void MySource::AddChunkSample( PvBuffer *aBuffer )
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


///
/// \brief Create the Source 0 only registers
///

void MySource::CreateRegisters( IPvRegisterMap *aRegisterMap, 
		                IPvRegisterFactory *aFactory)
{
    PVUNREFPARAM( aRegisterMap );
    if ( 0 == mChannelNum )
    {
        // Create new registers for Source0 only
        aFactory->AddRegister( "Source0Bool", SOURCE0_BOOL_ADDR, 4, PvGenAccessModeReadWrite, this );
        aFactory->AddRegister( "Source0Int", SOURCE0_INT_ADDR, 4, PvGenAccessModeReadWrite, this );
    }

}


///
/// \brief Create the Extended Source 0 only GenICam features
///

void MySource::CreateGenApiFeatures ( IPvRegisterMap *aRegisterMap,
		                      IPvGenApiFactory *aFactory )
{
    if ( 0 == mChannelNum )
    {
        // Create new features for Source0 only
    	aFactory->SetName( "Source0OnlyBool" );
    	aFactory->SetDescription( "Example of source only boolean." );
    	aFactory->SetCategory( "Source0Only" );
    	aFactory->SetNameSpace( PvGenNameSpaceStandard );
    	aFactory->SetTLLocked( true );
    	aFactory->SetStreamable( true );
    	aFactory->AddEnumEntry( "Off", 0 );
    	aFactory->AddEnumEntry( "On", 1 );
    	aFactory->CreateEnum( aRegisterMap->GetRegisterByAddress( SOURCE0_BOOL_ADDR ) );

    	aFactory->SetName( "Source0OnlyInt" );
    	aFactory->SetDescription( "Example of source only integer" );
    	aFactory->SetCategory( "Source0Only" );
    	aFactory->SetNameSpace( PvGenNameSpaceStandard );
    	aFactory->SetTLLocked( true );
    	aFactory->SetStreamable( true );
    	aFactory->CreateInteger( aRegisterMap->GetRegisterByAddress( SOURCE0_INT_ADDR ), 0, 256 );
    }
}


///
/// \brief Fetch the variable during a Read operation
///

PvResult MySource::PreRead( IPvRegister *aRegister )
{
	uint32_t lValue = 0;
	std::cout << aRegister->GetName().GetAscii() << " MySource PreRead" << std::endl;
	switch ( aRegister->GetAddress() )
	{
	case SOURCE0_BOOL_ADDR:
		lValue = mSource0OnlyBool;
		return aRegister->Write( lValue );    
	case SOURCE0_INT_ADDR:
		lValue = mSource0OnlyInt;
		return aRegister->Write( lValue );    
	}

	return PvResult::Code::OK;
}


///
/// \brief Post-read nofitication
///

void MySource::PostRead( IPvRegister *aRegister )
{
    std::cout << aRegister->GetName().GetAscii() << " MySource PostRead" << std::endl;
}


///
/// \brief Pre-write notification - this is where a new register value is usually validated
///

PvResult MySource::PreWrite( IPvRegister *aRegister )
{
    std::cout << aRegister->GetName().GetAscii() << " MySource PreWrite" << std::endl;
    return PvResult::Code::OK;
}

///
/// \brief Update the variable during a Write operation
///

void MySource::PostWrite( IPvRegister *aRegister )
{
	uint32_t lValue;
	aRegister->Read( lValue );
	std::cout << aRegister->GetName().GetAscii() << " MySource PostWrite" << std::endl;
	switch ( aRegister->GetAddress() )
	{
	case SOURCE0_BOOL_ADDR:
 		mSource0OnlyBool = lValue;
		break;
	case SOURCE0_INT_ADDR:
  		mSource0OnlyInt = lValue;
		break;
	}
}

