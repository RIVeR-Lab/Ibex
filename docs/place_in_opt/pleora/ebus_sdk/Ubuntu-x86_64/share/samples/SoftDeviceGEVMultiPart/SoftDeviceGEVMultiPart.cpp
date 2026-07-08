// *****************************************************************************
//
// Copyright (c) 2019, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include <PvSampleUtils.h>
#include <PvSoftDeviceGEV.h>
#include <PvMultiPartContainer.h>
#include <PvFPSStabilizer.h>
#include <PvSampleTransmitterConfig.h>
#include <PvStreamingChannelSourceDefault.h>

PV_INIT_SIGNAL_HANDLER();


#define BUFFERCOUNT ( 16 )
#define DEFAULT_FPS ( 30 )

#define DATATYPE1 ( PvMultiPart3DImage )
#define PIXELTYPE1 ( PvPixelCoord3D_A8 )

#define DATATYPE2 ( PvMultiPartConfidenceMap )
#define PIXELTYPE2 ( PvPixelConfidence8 )

#define DATATYPE3 ( PvMultiPartChunkData )
#define CHUNKLAYOUTID ( 0x12345678 )
#define CHUNKSIZE ( 64 )
#define CHUNKID ( 0x4001 )

#define WIDTH_MIN ( 64 )
#define WIDTH_MAX ( 1920 )
#define WIDTH_DEFAULT ( 640 )
#define WIDTH_INC ( 4 )

#define HEIGHT_MIN ( 4 )
#define HEIGHT_MAX ( 1080 )
#define HEIGHT_DEFAULT ( 480 )
#define HEIGHT_INC ( 1 )


// This class shows how to implement a streaming channel source
class MyMultiPartSource
    : public PvStreamingChannelSourceDefault
{
public:

    MyMultiPartSource()
        : mAcquisitionBuffer( NULL )
        , mChunkModeActive( false )
        , mChunkSampleEnabled( false )
        , mMultiPartAllowed( false )
        , mSeed( 0 )
        , mFrameCount( 0 )
        , mWidth( WIDTH_DEFAULT )
        , mHeight( HEIGHT_DEFAULT )
    {
    }

    // Allocs a multi-part buffer
    PvBuffer *AllocBuffer()
    {
        return new PvBuffer( PvPayloadTypeMultiPart );
    }

    // Deletes a buffer allocated with AllocBuffer
    void FreeBuffer( PvBuffer *aBuffer )
    {
        PVDELETE( aBuffer );
    }

    // Width info query
    void GetWidthInfo( uint32_t &aMin, uint32_t &aMax, uint32_t &aInc ) const
    {
        aMin = WIDTH_MIN;
        aMax = WIDTH_MAX;
        aInc = WIDTH_INC;
    }

    // Height info query
    void GetHeightInfo( uint32_t &aMin, uint32_t &aMax, uint32_t &aInc ) const
    {
        aMin = HEIGHT_MIN;
        aMax = HEIGHT_MAX;
        aInc = HEIGHT_INC;
    }

    // Width setter
    PvResult SetWidth( uint32_t aWidth )
    {
        if ( ( aWidth < WIDTH_MIN ) || ( aWidth > WIDTH_MAX ) )
        {
            return PvResult::Code::INVALID_PARAMETER;
        }

        mWidth = aWidth;
        return PvResult::Code::OK;
    }

    // Height setter
    PvResult SetHeight( uint32_t aHeight )
    {
        if ( ( aHeight < HEIGHT_MIN ) || ( aHeight > HEIGHT_MAX ) )
        {
            return PvResult::Code::INVALID_PARAMETER;
        }

        mHeight = aHeight;
        return PvResult::Code::OK;
    }

    // Returns the maximum payload size for our multi-part container
    uint32_t GetPayloadSize() const
    {
        // 1st part, also our default image when multi-part is not yet allowed
        uint32_t lPayloadSize = mWidth * mHeight;

        if ( mMultiPartAllowed )
        {
            // 2nd part
            lPayloadSize += mWidth * mHeight;
        }

        if ( mChunkModeActive && mChunkSampleEnabled )
        {
            lPayloadSize += CHUNKSIZE;
        }

        return lPayloadSize;
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

            PvResult lResult;
            if ( mMultiPartAllowed )
            {
                lResult = FillBufferMultiPart( aBuffer );
            }
            else
            {
                lResult = FillBuffer( aBuffer );
            }

            mFrameCount++;
            return lResult;
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

    uint32_t GetChunksSize() const { return ( mChunkModeActive && mChunkSampleEnabled ) ? CHUNKSIZE : 0; }
    bool GetChunkModeActive() const { return mChunkModeActive; }
    bool GetChunkEnable( uint32_t aChunkID ) const { PVUNREFPARAM( aChunkID ); return mChunkSampleEnabled; }
    PvResult SetChunkModeActive( bool aEnabled ) { mChunkModeActive = aEnabled; return PvResult::Code::OK; }
    PvResult SetChunkEnable( uint32_t aChunkID, bool aEnabled ) { PVUNREFPARAM( aChunkID ); mChunkSampleEnabled = aEnabled; return PvResult::Code::OK; }

    // Provide all supported chunks
    PvResult GetSupportedChunk( int aIndex, uint32_t &aID, PvString &aName ) const
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

        return PvResult::Code::NOT_FOUND;
    }

    // For GigE Vision Validation Framework and TestPayloadFormatMode GenICam XML definition
    bool IsPayloadTypeSupported( PvPayloadType aPayloadType )
    {
        return ( aPayloadType == PvPayloadTypeMultiPart );
    }

    // The streaming source is not allowed to stream multi-part data until allowed by the controlling application.
    // We have to assume it is false (not allowed) until this method is called with aAllowed set to true.
    void SetMultiPartAllowed( bool aAllowed )
    {
        mMultiPartAllowed = aAllowed;
        if ( mMultiPartAllowed )
        {
            std::cout << "Multi-part allowed" << std::endl;
        }
        else
        {
            std::cout << "Multi-part not allowed" << std::endl;
        }
    }

    // When this method is called for multi-part, we have to make sure we can stream valid multi-part buffers.
    // Required for GigE Vision Validation Framework certification.
    // If aPayloadType is PvPayloadTypeMultiPart we have to go in multi-part validation mode.
    // If aPayloadType is PvPayloadTypeNone, we have to go back to the normal multi-part operation mode.
    // In our case we are already using a ready-to-use test pattern, both modes are the same, there is nothing specific to do.
    PvResult SetTestPayloadFormatMode( PvPayloadType aPayloadType )
    {
        switch ( aPayloadType )
        {
        case PvPayloadTypeMultiPart:
            // Here we would make sure multi-part can be streamed from this source
            std::cout << "Setting TestPayloadFormatMode to PvPayloadTypeMultiPart" << std::endl;
            return PvResult::Code::OK;

        case PvPayloadTypeNone:
            // Here we would return the source in normal operation mode
            std::cout << "Disabling TestPayloadFormatMode" << std::endl;
            return PvResult::Code::OK;
            break;

        default:
            break;
        }

        std::cout << "Attempt at setting TestPayloadFormatMode to unexpected mode: " << aPayloadType << std::endl;
        return PvResult::Code::NOT_SUPPORTED;
    }

    // Prepares a non multi-part buffer for the content of what would be part 0, used when multi-part is not allowed
    PvResult Prepare( PvBuffer *aBuffer )
    {
        if ( aBuffer->GetPayloadType() != PvPayloadTypeImage )
        {
            aBuffer->Reset( PvPayloadTypeImage );
        }

        uint32_t lRequiredChunkSize = ( mChunkModeActive && mChunkSampleEnabled ) ? CHUNKSIZE : 0;
        PvImage *lImage = aBuffer->GetImage();
        if ( ( lImage->GetWidth() != mWidth ) ||
             ( lImage->GetHeight() != mHeight ) ||
             ( lImage->GetPixelType() != GetPixelType() ) ||
             ( lImage->GetMaximumChunkLength() != lRequiredChunkSize ) )
        {
            return lImage->Alloc( mWidth, mHeight, GetPixelType(), 0, 0, lRequiredChunkSize );
        }

        return PvResult::Code::OK;
    }

    // Fills a non multi-part buffer with the image content of what would be part 0, used when multi-part is not allowed
    PvResult FillBuffer( PvBuffer *aBuffer )
    {
        // Prepare non-multi-part buffer
        PvResult lResult = Prepare( aBuffer );
        if ( !lResult.IsOK() )
        {
            return lResult;
        }

        // Acquire buffer - could be done in another thread
        IPvImage *lImage = aBuffer->GetImage();
        FillTestPatternMono8( lImage );
        if ( mChunkModeActive && mChunkSampleEnabled )
        {
            FillChunkData( aBuffer );
        }

        return PvResult::Code::OK;
    }

    // Make sure the container is set for what we want to transmit
    PvResult PrepareMultiPart( PvBuffer *aBuffer )
    {
        // Make sure the buffer is of the right payload type
        if ( aBuffer->GetPayloadType() != PvPayloadTypeMultiPart )
        {
            return AllocMultiPart( aBuffer );
        }

        IPvMultiPartContainer *lContainer = aBuffer->GetMultiPartContainer();

        // 1st two parts should match expected data type
        IPvMultiPartSection *l0 = lContainer->GetPart( 0 );
        IPvMultiPartSection *l1 = lContainer->GetPart( 1 );
        if ( ( l0 == NULL ) ||
             ( l0->GetDataType() != DATATYPE1 ) ||
             ( l1 == NULL ) ||
             ( l1->GetDataType() != DATATYPE2 ) )
        {
            return AllocMultiPart( aBuffer );
        }

        // Images should be formatted as expected
        IPvImage *lImage0 = l0->GetImage();
        IPvImage *lImage1 = l1->GetImage();
        if ( ( lImage0->GetPixelType() != PIXELTYPE1 ) ||
             ( lImage0->GetWidth() != mWidth ) ||
             ( lImage0->GetHeight() != mHeight ) ||
             ( lImage1->GetPixelType() != PIXELTYPE2 ) ||
             ( lImage1->GetWidth() != mWidth ) ||
             ( lImage1->GetHeight() != mHeight ) )
        {
            return AllocMultiPart( aBuffer );
        }

        if ( mChunkModeActive && mChunkSampleEnabled )
        {
            // Make sure chunk section is there and configured as expected
            IPvMultiPartSection *l2 = lContainer->GetPart( 2 );
            if ( ( l2 == NULL ) ||
                 ( l2->GetChunkData() == NULL ) ||
                 ( l2->GetChunkData()->GetChunkDataCapacity() != CHUNKSIZE ) )
            {
                return AllocMultiPart( aBuffer );
            }
        }
        else
        {
            // No chunks, we expect two parts
            if ( lContainer->GetPartCount() != 2 )
            {
                return AllocMultiPart( aBuffer );
            }
        }

        return PvResult::Code::OK;
    }

    // Allocs a buffer for our multi-part configuration
    PvResult AllocMultiPart( PvBuffer *aBuffer )
    {
        // Reset buffer, make sure it is set for multi-part
        aBuffer->Reset( PvPayloadTypeMultiPart );
        IPvMultiPartContainer *lContainer = aBuffer->GetMultiPartContainer();

        // Reset container
        lContainer->Reset();

        // Add 1st image part definition
        PvResult lResult = lContainer->AddImagePart( DATATYPE1, mWidth, mHeight, PIXELTYPE1 );
        if ( !lResult.IsOK() )
        {
            return lResult;
        }

        // Add 2nd image part definition
        lResult = lContainer->AddImagePart( DATATYPE2, mWidth, mHeight, PIXELTYPE2 );
        if ( !lResult.IsOK() )
        {
            return lResult;
        }

        // Chunks enabled?
        if ( mChunkModeActive && mChunkSampleEnabled )
        {
            // Add chunk part definition. There can only be one and it needs to be last.
            lResult = lContainer->AddChunkPart( CHUNKSIZE, CHUNKLAYOUTID );
            if ( !lResult.IsOK() )
            {
                return lResult;
            }
        }

        // Alloc buffer memory for all defined parts
        lResult = lContainer->AllocAllParts();
        if ( !lResult.IsOK() )
        {
            return lResult;
        }

        // Validate container and return
        return lContainer->Validate();
    }

    // Fills a buffer with multi-part data
    PvResult FillBufferMultiPart( PvBuffer *aBuffer )
    {
        // Prepare multi-part container
        PvResult lResult = PrepareMultiPart( aBuffer );
        if ( !lResult.IsOK() )
        {
            return lResult;
        }

        // Acquire buffer - could be done in another thread
        IPvMultiPartContainer *lContainer = aBuffer->GetMultiPartContainer();
        FillTestPatternMono8( lContainer->GetPart( 0 )->GetImage() );
        FillTestPatternMono8( lContainer->GetPart( 1 )->GetImage() );
        if ( mChunkModeActive && mChunkSampleEnabled )
        {
            FillChunkData( lContainer->GetPart( 2 )->GetChunkData() );
        }

        return PvResult::Code::OK;
    }

    // Generate a greyscale test pattern in a PvBuffer
    void FillTestPatternMono8( IPvImage *aImage )
    {
        uint32_t lHeight = aImage->GetHeight();
        uint32_t lWidth = aImage->GetWidth();

        for ( uint32_t y = 0; y < lHeight; y++ )
        {
            uint8_t lValue = static_cast<uint8_t>( mSeed + y );
            uint8_t *lPtr = aImage->GetDataPointer();
            lPtr += ( y * static_cast<size_t>( aImage->GetWidth() ) ) + static_cast<size_t>( aImage->GetOffsetX() );

            for ( uint32_t x = 0; x < lWidth; x++ )
            {
                *lPtr++ = lValue++;
            }
        }

        mSeed++;
    }

    // Generate chunk data
    void FillChunkData( IPvChunkData *aChunkData )
    {
        aChunkData->ResetChunks();
        if ( !mChunkModeActive || !mChunkSampleEnabled )
        {
            return;
        }

        // Add chunk data to buffer
        aChunkData->SetChunkLayoutID( CHUNKLAYOUTID );
        aChunkData->AddChunk( CHUNKID, reinterpret_cast<uint8_t *>( &mFrameCount ), sizeof( mFrameCount ) );
    }

private:

    PvFPSStabilizer mStabilizer;

    PvBuffer *mAcquisitionBuffer;

    bool mChunkModeActive;
    bool mChunkSampleEnabled;
    bool mMultiPartAllowed;

    uint32_t mSeed;
    uint32_t mFrameCount;
    uint32_t mWidth;
    uint32_t mHeight;
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

    MyMultiPartSource lMultiPartSource;
    PvSoftDeviceGEV lDevice;
    lDevice.AddStream( &lMultiPartSource );

    IPvSoftDeviceGEVInfo *lInfo = lDevice.GetInfo();
    const PvString lModelName = lInfo->GetModelName();

    PvResult lResult = lDevice.Start( lInterface );
    if ( !lResult.IsOK() )
    {
        std::cout << "Error starting " << lModelName.GetAscii() << std::endl;
        return -1;
    }

    std::cout << lModelName.GetAscii() << " started" << std::endl;

    PvFlushKeyboard();
    while ( !PvKbHit() )
    {
        PvSleepMs( 100 );
    }

    lDevice.Stop();
    std::cout << lModelName.GetAscii() << " stopped" << std::endl;

    return 0;
}


