// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "eBUSPlayerShared.h"
#include "ImageSaving.h"
#include "ImageFiltering.h"
#include "IMp4Writer.h"

#include <PvBufferWriter.h>
#include <PvDecompressionFilter.h>

#include <assert.h>
#include <sstream>
#include <iomanip>

#ifdef _AFXDLL
#include <Shlobj.h>
#else
#include <unistd.h>
#endif

#ifdef PV_ENABLE_MP4
#ifdef _AFXDLL
#include "Mp4WriterWin32.h"
#elif _LINUX_
#include "Mp4WriterLinux.h"
#elif __APPLE__
#include "Mp4WriterMac.h"
#endif
#endif

#define IMAGESAVING_VERSION ( "1.0.0.0" )

#define TAG_VERSION ( "imagesavingversion" )
#define TAG_ENABLED ( "saveenabled" )
#define TAG_ONEOUTOF ( "oneoutof" )
#define TAG_MAXRATE ( "maxrate" )
#define TAG_AVERAGETHROUGHPUT ( "averagethroughput" )
#define TAG_THROTTLEOPTION ( "savethrottleoption" )
#define TAG_PATH ( "savepath" )
#define TAG_FORMAT ( "saveformat" )
#define TAG_AVGBITRATE ( "avgbitrate" )

#define VAL_FORMAT_BMP ( "bmp" )
#define VAL_FORMAT_RAW ( "raw" )
#define VAL_FORMAT_TIFF ( "tiff" )
#define VAL_FORMAT_PNG ( "png" )
#define VAL_FORMAT_MP4 ( "mp4" )
#define VAL_FORMAT_PTCD ( "ptcd" )


#ifdef _AFXDLL
IMPLEMENT_DYNAMIC( ImageSaving, CObject )
#endif // _AFXDLL


///
/// \brief Constructor
///

ImageSaving::ImageSaving( ImageFiltering *aImageFiltering )
  : mBufferWriter( NULL )
  , mStopped( false )
  , mImageFiltering( aImageFiltering )
  , mMp4Writer( NULL )
  , mMp4WriterOpenFailed( false )
  , mDisplaySaved( false )
{
#ifdef PV_ENABLE_MP4
#ifdef _AFXDLL
    mMp4Writer = new Mp4WriterWin32;
#elif _LINUX_
    mMp4Writer = new Mp4WriterLinux;
#elif __APPLE__
    mMp4Writer = new Mp4WriterMac;
#endif
#endif

    mBufferWriter = new PvBufferWriter;

    Reset();
}


///
/// \brief Constructor
///

ImageSaving::~ImageSaving()
{
    PVDELETE( mBufferWriter );
    PVDELETE( mMp4Writer );
}


///
/// \brief Resets the configuration of the object
///

void ImageSaving::Reset()
{
    mEnabled = false;
    mThrottling = ThrottleMaxRate;
    mOneOutOf = 10;
    mMaxRate = 100;
    mAverageThroughput = 1;

#ifdef MAKE_PNG_DEFAULT_SAVING
    mFormat = FormatPNG;
#else
    mFormat = FormatBmp;
#endif

    ResetStats();

#ifdef WIN32
    wchar_t myPictures[ MAX_PATH ] = { 0 };
    SHGetSpecialFolderPath( NULL, myPictures, CSIDL_MYPICTURES, true );
    PvString lMyPictures( myPictures );
    mPath = lMyPictures.GetAscii();
#endif // WIN32
}


///
/// \brief Resets the image saving statistics (not the configuration)
///

void ImageSaving::ResetStats()
{
    mFPS = 0.0;
    mMbps = 0.0;
    mFrames = 0;
    mTotalSize = 0;
    mCapturedSince = 0;
    mPrevious = 0;
    mCount = 0;

    mStartTime = ::GetTickCount();
    mElapsedTime = 0;

    mStopped = false;
    mMp4WriterOpenFailed = false;

    if ( mMp4Writer != NULL )
    {
        mMp4Writer->ResetLastError();
    }
}


///
/// \brief Persistence save
///

PvResult ImageSaving::Save( PvConfigurationWriter *aWriter )
{
    std::stringstream lSS;

    // Save enabled
    aWriter->Store( mEnabled ? "1" : "0", TAG_ENABLED );

    // One out of
    lSS << mOneOutOf;
    aWriter->Store( lSS.str().c_str(), TAG_ONEOUTOF );

    // Max rate
    lSS.str( "" );
    lSS << mMaxRate;
    aWriter->Store( lSS.str().c_str(), TAG_MAXRATE );

    // Average throughput
    lSS.str( "" );
    lSS << mAverageThroughput;
    aWriter->Store( lSS.str().c_str(), TAG_AVERAGETHROUGHPUT );

    // Throttling
    lSS.str( "" );
    lSS << mThrottling;
    aWriter->Store( lSS.str().c_str(), TAG_THROTTLEOPTION );

    // Path
    aWriter->Store( mPath.c_str(), TAG_PATH );

    // Format
    switch ( mFormat )
    {
        case FormatBmp:
            aWriter->Store( VAL_FORMAT_BMP, TAG_FORMAT );
            break;

        case FormatRaw:
            aWriter->Store( VAL_FORMAT_RAW, TAG_FORMAT );
            break;

        case FormatTiff:
            aWriter->Store( VAL_FORMAT_TIFF, TAG_FORMAT );
            break;

        case FormatPNG:
            aWriter->Store( VAL_FORMAT_PNG, TAG_FORMAT );
            break;

        case FormatMp4:
            aWriter->Store( VAL_FORMAT_MP4, TAG_FORMAT );
            break;

        case FormatPleoraDecompressed:
            aWriter->Store( VAL_FORMAT_PTCD, TAG_FORMAT );
            break;
    }

    // Average bitrate
    if ( mMp4Writer != NULL )
    {
        lSS.str( "" );
        lSS << mMp4Writer->GetAvgBitrate();
        aWriter->Store( lSS.str().c_str(), TAG_AVGBITRATE );
    }

    return PvResult::Code::OK;
}


///
/// \brief Persistence load
///

PvResult ImageSaving::Load( PvConfigurationReader *aReader )
{
    PvString lPvStr;

    // Start with a clean config
    Reset();

    // Enabled
    if ( aReader->Restore( TAG_ENABLED, lPvStr ).IsOK() )
    {
        mEnabled = ( lPvStr == "1" );
    }

    // One out of
    if ( aReader->Restore( TAG_ONEOUTOF, lPvStr ).IsOK() )
    {
        std::stringstream lSS( lPvStr.GetAscii() );
        lSS >> mOneOutOf;
    }

    // Max rate
    if ( aReader->Restore( TAG_MAXRATE, lPvStr ).IsOK() )
    {
        std::stringstream lSS( lPvStr.GetAscii() );
        lSS >> mMaxRate;
    }

    // Average throughput
    if ( aReader->Restore( TAG_AVERAGETHROUGHPUT, lPvStr ).IsOK() )
    {
        std::stringstream lSS( lPvStr.GetAscii() );
        lSS >> mAverageThroughput;
    }

    // Throttling option
    if ( aReader->Restore( TAG_THROTTLEOPTION, lPvStr ).IsOK() )
    {
        std::stringstream lSS( lPvStr.GetAscii() );
        uint32_t lValue = ThrottleNone;
        lSS >> lValue;
        mThrottling = static_cast<Throttle>( lValue );
    }

    // Path
    if ( aReader->Restore( TAG_PATH, lPvStr ).IsOK() )
    {
        mPath = lPvStr.GetAscii();
    }

    // Format
    if ( aReader->Restore( TAG_FORMAT, lPvStr ).IsOK() )
    {
        if ( lPvStr == VAL_FORMAT_BMP )
        {
            mFormat = FormatBmp;
        }
        else if ( lPvStr == VAL_FORMAT_RAW )
        {
            mFormat = FormatRaw;
        }
        else if ( lPvStr == VAL_FORMAT_TIFF )
        {
            mFormat = FormatTiff;
        }
        else if ( lPvStr == VAL_FORMAT_PNG )
        {
            mFormat = FormatPNG;
        }
        else if ( lPvStr == VAL_FORMAT_MP4 )
        {
            mFormat = FormatMp4;
        }
        else if ( lPvStr == VAL_FORMAT_PTCD )
        {
            mFormat = FormatPleoraDecompressed;
        }
    }

    // Average bitrate
    if ( mMp4Writer != NULL )
    {
        if ( aReader->Restore( TAG_AVGBITRATE, lPvStr ).IsOK() )
        {
            std::stringstream lSS( lPvStr.GetAscii() );
            uint32_t lValue = 0;
            lSS >> lValue;
            mMp4Writer->SetAvgBitrate( lValue );
        }
    }

    return PvResult::Code::OK;
}


///
/// \brief Saves a specific image
///

void ImageSaving::GetPath( PvBuffer *aBuffer, std::string &aLocation, std::string &aFilename )
{
    std::string lExt;
    switch ( mFormat )
    {
        case FormatBmp:
            lExt = ".bmp";
            break;

        case FormatRaw:
            if ( PvPayloadTypePleoraCompressed == aBuffer->GetPayloadType() )
            {
                lExt = ".ptc1";
            }
            else
            {
                lExt = ".bin";
            }
            break;

        case FormatTiff:
            lExt = ".tiff";
            break;

        case FormatPNG:
            lExt = ".png";
            break;

        case FormatMp4:
            lExt = ".mp4";
            break;

        case FormatPleoraDecompressed:
            lExt = ".bin";
            break;

        default:
            assert( 0 );
            break;
    }

    int64_t lTickCount = ::GetTickCount();

    std::stringstream lFileName;
    lFileName << std::setfill( '0' ) << std::setw( 8 ) << mCount++;
    lFileName << "_";
    lFileName << std::uppercase << std::hex << std::setfill( '0' ) << std::setw( 16 ) << lTickCount;

    if ( mFormat == FormatRaw || mFormat == FormatPleoraDecompressed )
    {
        if ( aBuffer->GetPayloadType() == PvPayloadTypePleoraCompressed )
        {
            if ( PvDecompressionFilter::IsCompressed( aBuffer ) )
            {
                uint32_t lWidth, lHeight;
                PvPixelType lPixelType;
                PvDecompressionFilter::GetOutputFormatFor( aBuffer, lPixelType, lWidth, lHeight );
                lFileName << std::dec << std::setw( 0 );
                lFileName << "_w" << lWidth;
                lFileName << "_h" << lHeight;
                lFileName << "_p" << PvImage::PixelTypeToString( lPixelType ).GetAscii();
            }
        }
        else
        {
            PvImage *lImage = aBuffer->GetImage();
            if ( lImage != NULL )
            {
                lFileName << std::dec << std::setw( 0 );
                lFileName << "_w" << lImage->GetWidth();
                lFileName << "_h" << lImage->GetHeight();

                if ( lImage->GetOffsetX() != 0 )
                {
                    lFileName << "_x" << lImage->GetPaddingX();
                }

                lFileName << "_p" << PvImage::PixelTypeToString( lImage->GetPixelType() ).GetAscii();
            }
        }
    }

    lFileName << lExt;

    // Save to output parameter
    aLocation = mPath;
    aFilename = lFileName.str();
}


///
/// \brief Saves a single buffer/image
///

bool ImageSaving::SaveImage( PvBuffer *aBuffer, bool aUpdateStats )
{
    if ( mFormat == FormatMp4 )
    {
        if ( mStopped )
        {
            // This flag is only used to prevent restarting a new MP4 on leftover frames
            return false;
        }

        return SaveMp4( aBuffer );
    }

    std::string lLocation, lFilename;
    GetPath( aBuffer, lLocation, lFilename );

    std::stringstream lPath;
    lPath << lLocation;
#ifdef WIN32
     lPath << "\\";
#else
    lPath << "/";
#endif
    lPath << lFilename;

    uint32_t lBytesWritten = 0;
    switch ( mFormat )
    {
        case FormatBmp:
            mBufferWriter->Store( aBuffer, lPath.str().c_str(), PvBufferFormatBMP, &lBytesWritten );
            break;

        case FormatRaw:
            mBufferWriter->Store( aBuffer, lPath.str().c_str(), PvBufferFormatRaw, &lBytesWritten );
            break;

        case FormatTiff:
            mBufferWriter->Store(aBuffer, lPath.str().c_str(), PvBufferFormatTIFF, &lBytesWritten );
            break;

        case FormatPNG:
            mBufferWriter->Store(aBuffer, lPath.str().c_str(), PvBufferFormatPNG, &lBytesWritten );
            break;

        case FormatPleoraDecompressed:
            mBufferWriter->Store( aBuffer, lPath.str().c_str(), PvBufferFormatPTCDecompressedRaw, &lBytesWritten );
            break;

        default:
            assert( 0 );
            break;
    }

    if ( aUpdateStats )
    {
        UpdateStats( aBuffer, lBytesWritten );
    }

    return true;
}


///
/// \brief Updates the stats with the saved/encoded buffer
///

void ImageSaving::UpdateStats( PvBuffer *aBuffer, uint32_t aLastSize )
{
    PVUNREFPARAM( aBuffer );

    mElapsedTime = ::GetTickCount() - mStartTime;
    int64_t lDelta = mElapsedTime;

    mFrames++;

    mFPS = (double)mFrames * 1000 / (double)lDelta;

    mTotalSize += aLastSize;
    mMbps = ( lDelta != 0 ) ? 
		( ( static_cast<double>( mTotalSize ) * 1000.0 ) / lDelta ) * 8.0 / 1048576.0 : 
		0.0;
}


///
/// \brief Saves the current image
///

bool ImageSaving::SaveCurrentImage( DisplayThread *aDisplayThread )
{
    if ( mFormat == FormatMp4 )
    {
        // No point in saving a single video image
        return false;
    }

    bool lResult = false;

    bool lCompressedBufferExists = false;
    if( aDisplayThread->RetrieveCompressedBuffer() != nullptr )
    {
        lCompressedBufferExists = true;
    }
    aDisplayThread->ReleaseCompressedBuffer();

    if( mFormat == FormatPleoraDecompressed && !lCompressedBufferExists )
    {
        // If raw image was not compressed, do not save anything for "Pleora Decompressed" format
        return lResult;
    }

    if ( mFormat == FormatRaw && lCompressedBufferExists )
    {
        PvBuffer *lCompressedBuffer = aDisplayThread->RetrieveCompressedBuffer();
        lResult = SaveImage( lCompressedBuffer, false ); // false: not updating stats on a save current
        aDisplayThread->ReleaseCompressedBuffer();
        return lResult;
    }

    PvBuffer *lLatestBuffer = aDisplayThread->RetrieveLatestBuffer();
    if ( lLatestBuffer != NULL )
    {
        lResult = SaveImage( lLatestBuffer, false ); // false: not updating stats on a save current
        aDisplayThread->ReleaseLatestBuffer();
    }

    return lResult;
}


///
/// \brief Request to save the display image if it meets throttling requirement, only saved if configuration matches
///

bool ImageSaving::SaveDisplayIfNecessary( IPvDisplayAdapter* mDisplay, DisplayThread* aDisplayThread, PvBuffer* aBuffer, bool aForce )
{
    if ( aForce && nullptr == aBuffer )
    {
        if ( ( mFormat == FormatTiff ) || ( mFormat == FormatPNG ) || ( mFormat == FormatRaw ) || ( mFormat == FormatPleoraDecompressed ) )
        {
            return SaveCurrentImage( aDisplayThread );
        }
        PvBuffer* lInternalBuffer = &mDisplay->GetInternalBuffer();
        if ( nullptr != lInternalBuffer )
        {
            return SaveImage( lInternalBuffer, false ); // Don't update stats when forcing
        }
        else
        {
            return false;
        }
    }

    if ( ( mFormat == FormatTiff ) || ( mFormat == FormatPNG ) || ( mFormat == FormatRaw )  || ( mFormat == FormatPleoraDecompressed ) )
    {
        return false;
    }

    if ( nullptr != aBuffer )
    {
        double aBufferSize = aBuffer->GetAcquiredSize();
        if ( ThrottleMeet( aBufferSize ) )
        {
            PvBuffer* lInternalBuffer = &mDisplay->GetInternalBuffer();
            if ( nullptr != lInternalBuffer )
            {
                lInternalBuffer->SetReceptionTime( aBuffer->GetReceptionTime() );
                return SaveImage( lInternalBuffer, true ); // true: update stats
            }
        }
    }

    return false;
}


///
/// \brief Requests to save the pure image (non-display), only saved if configuration matches
///

bool ImageSaving::SavePure( PvBuffer *aBuffer )
{
    if ( ( mFormat == FormatBmp ) || ( mFormat == FormatMp4 ) )
    {
        return false;
    }

    return SaveIfNecessary( aBuffer );
}


///
/// \brief Requests to save the bmp image (non-display), only saved if configuration matches
///

bool ImageSaving::SaveBMP( PvBuffer *aBuffer )
{
	// If Display is saved, don't save BMP again
    if ( mFormat == FormatBmp && !mDisplaySaved )
    {
        return SaveIfNecessary( aBuffer );
    }
	return false;
}


///
/// \brief Checking if the throttle requirement is met
///

bool ImageSaving::ThrottleMeet( double aBufferSize )
{
    if ( !mEnabled )
    {
        return false;
    }

    bool lSaveThisOne = false;
    double lBitsPerImage;
    double lBitsPerMs;
    int64_t lCurrent;

    switch ( mThrottling )
    {
        case ThrottleOneOutOf:
            // 1 image every mOneOf captured images
            mCapturedSince++;
            if ( mCapturedSince >= mOneOutOf )
            {
                lSaveThisOne = true;
                mCapturedSince = 0;
            }
            break;

        case ThrottleMaxRate:
            // maximum of one out of every mMaxRate ms
            lCurrent = ::GetTickCount();
            if ( ( lCurrent - mPrevious ) >= mMaxRate )
            {
                lSaveThisOne = true;
                mPrevious = lCurrent;
            }
            break;

        case ThrottleAverageThroughput:
            // maintain mAverageThroughput Mbits/s average
            lBitsPerImage = aBufferSize * 8;
            lBitsPerMs = mAverageThroughput * 1048.576;
            lCurrent = ::GetTickCount();
            if ( ( lCurrent - mPrevious ) >= ( lBitsPerImage / lBitsPerMs ) )
            {
                lSaveThisOne = true;
                mPrevious = lCurrent;
            }
            break;

        case ThrottleNone:
            lSaveThisOne = true;
            break;

        default:
            assert( 0 );
            break;
    }
    return lSaveThisOne;
}

///
/// \brief Saves the current buffer if necessary (based on configuration)
///

bool ImageSaving::SaveIfNecessary( PvBuffer *aBuffer )
{
    double aBufferSize = aBuffer->GetAcquiredSize();
    if ( ThrottleMeet( aBufferSize ) )
    {
        return SaveImage( aBuffer, true ); // true: update stats
    }

    return false;
}


///
/// \brief Saves the buffer to an MP4 file encoded as H.264
///

bool ImageSaving::SaveMp4( PvBuffer *aBuffer )
{
    if ( ( mMp4Writer == NULL ) || mMp4WriterOpenFailed )
    {
        return false;
    }

    if ( !mMp4Writer->IsOpened() )
    {
        // Get default filename, path
        std::string lLocation, lFilename;
        GetPath( aBuffer, lLocation, lFilename );

        // We use PvString for unicode conversion
        PvString lString( lLocation.c_str() );

#ifdef _AFXDLL
        // Change working folder (temp workaround for MFCreateSinkWriterFromURL issue)
        SetCurrentDirectory( lString.GetUnicode() );
#elif __APPLE__
        lFilename = lLocation.append( lFilename );
#else
        int lSuccess = chdir( lString.GetAscii() );
        if ( lSuccess != 0 )
        {
            return false;
        }
#endif // _AFXDLL

        // Open the MP4 writer
        if ( !mMp4Writer->Open( lFilename, aBuffer->GetImage() ) )
        {
            mMp4WriterOpenFailed = true;
            return false;
        }
    }

    uint32_t lFileSizeDelta = 0;
    bool lSuccess = mMp4Writer->WriteFrame( aBuffer->GetImage(), &lFileSizeDelta );
    if ( !lSuccess )
    {
        return false;
    }

    UpdateStats( aBuffer, lFileSizeDelta );

    return true;
}

///
/// \brief get mp4 average bitrate in bps
///

uint32_t ImageSaving::GetAvgBitrate() const
{
    return mMp4Writer != NULL ? mMp4Writer->GetAvgBitrate() : 0;
}

///
/// \brief set the mpeg average bitrate in bps
///

void ImageSaving::SetAvgBitrate( uint32_t aValue )
{
    if ( mMp4Writer != NULL )
    {
        mMp4Writer->SetAvgBitrate( aValue );
	}       
}

///
/// \brief check if mp4 writing is supported
///

bool ImageSaving::IsMp4Supported()
{
    return ( mMp4Writer != NULL ) && mMp4Writer->IsAvailable();
}

///
/// \brief check if current save format is video (mp4, avi, wmv, etc)
///

bool ImageSaving::IsFormatVideo()
{
    return ( mFormat == FormatMp4 );
}

///
/// \brief Call to notify the image saving module that we just stopped streaming
///

void ImageSaving::NotifyStreamingStop()
{
    StopMp4();
    mStopped = true;
    mMp4WriterOpenFailed = false;
}


///
/// \brief Stop Mp4 recording
///

void ImageSaving::StopMp4()
{
    if ( ( mMp4Writer != NULL ) && mMp4Writer->IsOpened() )
    {
        mMp4Writer->Close();
    }
}


///
/// \brief Returns a string error for image saving.
///
/// Currently only used by MP4 recording.
///

void ImageSaving::GetLastError( PvString &aString )
{
    if ( mMp4Writer != NULL )
    {
        mMp4Writer->GetLastError( aString );
        return;
    }

    aString = "";
}
