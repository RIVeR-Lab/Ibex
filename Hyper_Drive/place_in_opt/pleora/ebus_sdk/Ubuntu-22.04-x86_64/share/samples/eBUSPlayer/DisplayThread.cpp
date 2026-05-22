// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "eBUSPlayerShared.h"
#include "DisplayThread.h"
#include "ImageSaving.h"
#include "ImageFiltering.h"

#include <limits>
#include <assert.h>


#ifdef _AFXDLL
    IMPLEMENT_DYNAMIC( DisplayThread, CObject )
#endif // _AFXDLL


///
/// \brief Constructor
///

DisplayThread::DisplayThread( 
        IPvDisplayAdapter *aDisplay, ImageFiltering *aImageFiltering,
        ImageSaving* aImageSaving, LogBuffer *aLogBuffer )
    : mLogBuffer( aLogBuffer )
    , mDisplay( aDisplay )
    , mImageFiltering( aImageFiltering )
    , mImageSaving( aImageSaving )
    , mStopMP4( false )
{
}


///
/// \brief Destructor
///

DisplayThread::~DisplayThread()
{
}


///
/// \brief Perform white balance on the current buffer then update display
///

void DisplayThread::WhiteBalance( PvBufferConverterRGBFilter *aFilterRGB )
{
    // Get the latest buffer - this locks the display thread so we can use the buffer safely
    PvBuffer *lBuffer = RetrieveLatestBuffer();
    if ( lBuffer == NULL )
    {
        return;
    }

    PvImage *lFinalImage = lBuffer->GetImage();

    PvBuffer lBufferDisplay;
    PvImage *lImageDisplay = lBufferDisplay.GetImage();
    lImageDisplay->Alloc( lFinalImage->GetWidth(), lFinalImage->GetHeight(), PV_PIXEL_WIN_RGB32 );

    // Convert last good buffer to RGB, one-time use converter
    PvBufferConverter lConverter( 1 );
    lConverter.Convert( lBuffer, &lBufferDisplay );

    aFilterRGB->WhiteBalance( &lBufferDisplay );

    OnBufferDisplay( lBuffer );

    // Important: release the buffer to unlock the display thread
    ReleaseLatestBuffer();
}


///
/// \brief Callback from PvDisplayThread
///

void DisplayThread::OnBufferDisplay( PvBuffer *aBuffer )
{
    PvBuffer *lBuffer = aBuffer;

    if ( aBuffer->GetPayloadType() == PvPayloadTypeImage )
    {
        // Range filter
        mImageFiltering->ConfigureConverter( mImageFiltering->GetRangeFilter()->GetConverter() );
        lBuffer = mImageFiltering->GetRangeFilter()->Process( aBuffer );
    }

    // Display
    mImageFiltering->ConfigureConverter( mDisplay->GetConverter() );
    mDisplay->Display( *lBuffer, GetVSyncEnabled() );

    // Will save if format matches displayed images and meet throttling criteria
    if ( mImageSaving->GetEnabled() )
    {
		mImageSaving->SetDisplaySaved( true );
        mImageSaving->SaveDisplayIfNecessary( mDisplay, this, lBuffer );
        mDisplay->ReleaseInternalBuffer();
    }
}


///
/// \brief Buffer logging callback. Just add to buffer log.
///

void DisplayThread::OnBufferLog( const PvString &aLog )
{
    mLogBuffer->Log( aLog.GetAscii() );
}


///
/// \brief Callback from PvDisplayThread
///

void DisplayThread::OnBufferRetrieved( PvBuffer *aBuffer )
{
    PVUNREFPARAM( aBuffer );

    SetBufferLogErrorEnabled( mLogBuffer->IsBufferErrorEnabled() );
    SetBufferLogAllEnabled( mLogBuffer->IsBufferAllEnabled() );
    SetChunkLogEnabled( mLogBuffer->IsChunkEnabled() );
    SetTapGeometry( mImageFiltering->GetTapGeometry() );
}


///
/// \brief Callback from PvDisplayThread
///

void DisplayThread::OnBufferDone( PvBuffer *aBuffer )
{
    // Will save if format matches pure images and meet throttling criteria

    bool lCompressedBufferExists = false;
    if( RetrieveCompressedBuffer() != nullptr )
    {
        lCompressedBufferExists = true;
    }
    ReleaseCompressedBuffer();

    if ( mImageSaving->GetEnabled() )
    {
        if ( aBuffer->GetPayloadType() == PvPayloadTypePleoraCompressed )
        {
            if ( mImageSaving->GetFormat() == ImageSaving::FormatRaw )
            {
                mImageSaving->SavePure( aBuffer );
            }
            else if ( mImageSaving->GetFormat() == ImageSaving::FormatPleoraDecompressed )
            {
                mImageSaving->SavePure( aBuffer );
            }
        }
        else
        {
            if ( mImageSaving->GetFormat() == ImageSaving::FormatBmp )
            {
                mImageSaving->SaveBMP( aBuffer );
            }
            else if ( mImageSaving->GetFormat() == ImageSaving::FormatPleoraDecompressed )
            {
                // Only save buffer if it originated from a pleora compressed buffer
                PvBuffer *lCompressedBuffer = RetrieveCompressedBuffer();
                if (lCompressedBuffer)
                {
                    mImageSaving->SavePure( aBuffer );
                }
                ReleaseCompressedBuffer();
            }
            else if ( !lCompressedBufferExists )
            {
                mImageSaving->SavePure( aBuffer );
            }
        }
        mImageSaving->SetDisplaySaved( false );
        if ( mStopMP4 )
        {
            mImageSaving->NotifyStreamingStop();
        }
    }
}


///
/// \brief Callback from PvDisplayThread
///

void DisplayThread::OnBufferTextOverlay( const PvString &aText )
{
    mDisplay->SetTextOverlay( aText );
}


///
/// \brief Returns true if the image filtering configuration has changed
///

bool DisplayThread::HasChanged()
{
    // Save filter configuration to writer
    PvPropertyList lList;
    Save( lList );
    PvConfigurationWriter lWriter;
    lWriter.Store( &lList );

    // Save configuration to a string
    PvString lNow;
    lWriter.SaveToString( lNow );

    // Compare what we now have with the baseline
    return mBaseline != lNow;
}


///
/// \brief Resets the configuration baseline for HasChanged test
///

void DisplayThread::ResetChanged()
{
    // Save filter configuration to writer
    PvPropertyList lList;
    Save( lList );
    PvConfigurationWriter lWriter;
    lWriter.Store( &lList );

    // Save configuration baseline to a string
    lWriter.SaveToString( mBaseline );
}


void DisplayThread::SaveInternalBuffer()
{
    mImageSaving->SaveDisplayIfNecessary( mDisplay, this, nullptr, true );
    mDisplay->ReleaseInternalBuffer();
}

