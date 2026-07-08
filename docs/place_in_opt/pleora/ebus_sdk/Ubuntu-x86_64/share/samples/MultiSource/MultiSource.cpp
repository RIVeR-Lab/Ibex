// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

//
// To receive images from a multi-source device using PvPipeline
//

#include <PvSampleUtils.h>
#include <PvDeviceGEV.h>
#include <PvDeviceU3V.h>
#include <PvPipeline.h>
#include <PvBuffer.h>
#include <PvStreamGEV.h>
#include <PvStreamU3V.h>
#include <PvGenStateStack.h>

#include <iostream>
#include <iomanip>
#include <list>

#include <PvMultiPartContainer.h>

// #define PVDISPLAYWND

#ifdef PVDISPLAYWND
#include <PvDisplayWnd.h>
#include <map>
typedef std::map<std::string, PvDisplayWnd *> PvDisplayWndMap;
#endif


using namespace std;


PV_INIT_SIGNAL_HANDLER();


#define BUFFER_COUNT ( 16 )


const char sDoodle[] = "|\\-|-/";
const int sDoodleLength = 6;


//
// Simple class used to control a source and get images from it
//

class Source
{
public:

    // Constructor
    Source( PvDevice *aDevice, const PvString &aConnectionID, const PvString &aSource )
        : mDevice( aDevice )
        , mStream( NULL )
        , mPipeline( NULL )
        , mConnectionID( aConnectionID )
        , mSource( aSource )
        , mDoodleIndex( 0 )
    {
    }

    // Destructor
    ~Source()
    {
        PVDELETE( mPipeline );
        PVDELETE( mStream );
    }

    // Open source: opens the stream, set destination, starts pipeline
    bool Open()
    {
        cout << "Opening source " << mSource.GetAscii() << endl;

        // Select source (if applicable)
        PvGenStateStack lStack( mDevice->GetParameters() );
        SelectSource( &lStack );

        // Retrieve the source channel
        cout << "  Reading source channel on device" << endl;
        int64_t lSourceChannel = 0;
        PvResult lResult = mDevice->GetParameters()->GetIntegerValue( "SourceStreamChannel", lSourceChannel );
        uint16_t lChannel = static_cast<uint16_t>( lSourceChannel );

        // Create and open stream, use same protocol as PvDevice. PvStream::CreateAndOpen is not used as we
        // need to specify additional parameters like streaming channel
        cout << "  Opening stream from device" << endl;
        PvDeviceGEV *lDeviceGEV = dynamic_cast<PvDeviceGEV *>( mDevice );
        PvDeviceU3V *lDeviceU3V = dynamic_cast<PvDeviceU3V *>( mDevice );
        if ( lDeviceGEV != NULL )
        {
            // Create and open stream
            PvStreamGEV *lStreamGEV = new PvStreamGEV;
            lResult = lStreamGEV->Open( mConnectionID, 0, lChannel );
            if ( !lResult.IsOK() )
            {
                cout << "  Error opening stream to GigE Vision device" << endl;
                return false;
            }
                 
            // Save pointer to stream object
            mStream = lStreamGEV;

            // Get stream local information
            PvString lLocalIP = lStreamGEV->GetLocalIPAddress();
            uint16_t lLocalPort = lStreamGEV->GetLocalPort();

            // Set source destination on GigE Vision device
            cout << "  Setting source destination on device (channel " << lChannel << ") " << endl;
            cout << "    to " << lLocalIP.GetAscii() << " port " << lLocalPort << endl;
            lDeviceGEV->SetStreamDestination( lLocalIP, lLocalPort, lChannel );

        }
        else if ( lDeviceU3V != NULL )
        {
            // Create and open stream
            PvStreamU3V *lStreamU3V = new PvStreamU3V;
            lResult = lStreamU3V->Open( mConnectionID, lChannel );
            if ( !lResult.IsOK() )
            {
                cout << "  Error opening stream to USB3 Vision device" << endl;
                return false;
            }
        }

        // Reading payload size from device
        int64_t lSize = mDevice->GetPayloadSize();

        // Dynamically allocate pipeline (required PvStream pointer)
        mPipeline = new PvPipeline( mStream );

        // Set the Buffer size and the Buffer count
        mPipeline->SetBufferSize( static_cast<uint32_t>( lSize ) );
        mPipeline->SetBufferCount( BUFFER_COUNT ); // Increase for high frame rate without missing block IDs

        // Start pipeline thread
        cout << "  Starting pipeline thread" << endl;
        mPipeline->Start();

        return true;
    }

    // Close source: closes the stream, reset destination
    void Close()
    {
        cout << "Closing source " << mSource.GetAscii() << endl;

        // Stop pipeline thread
        cout << "  Stopping pipeline thread" << endl;
        mPipeline->Stop();

        // Close stream
        cout << "  Closing stream" << endl;
        mStream->Close();
    }

    // Start acquisition: set payload size, reset pipeline and stats
    void StartAcquisition()
    {
        cout << "Start acquisition" << mSource.GetAscii() << endl;

        // Select source (if applicable)
        PvGenStateStack lStack( mDevice->GetParameters() );
        SelectSource( &lStack );

        // Enables stream before sending the AcquisitionStart command.
        cout << "Enable streaming on the controller." << endl;
        mDevice->StreamEnable();

        // The pipeline is already "armed", we just have to tell the device
        // to start sending us images
        cout << "  Sending AcquisitionStart command to device" << endl;
        mDevice->GetParameters()->ExecuteCommand( "AcquisitionStart" );
    }

    // Stop acquisition: stop pipeline
    void StopAcquisition()
    {
        cout << "Stop acquisition " << mSource.GetAscii() << endl;

        // Select source (if applicable)
        PvGenStateStack lStack( mDevice->GetParameters() );
        SelectSource( &lStack );

        // The pipeline is already "armed", we just have to tell the device
        // to start sending us images
        cout << "  Sending AcquisitionStop command to device" << endl;
        mDevice->GetParameters()->ExecuteCommand( "AcquisitionStop" );

        // Disable stream after sending the AcquisitionStop command.
        cout << "Disable streaming on the controller." << endl;
        mDevice->StreamDisable();

#ifdef PVDISPLAYWND
        PvDisplayWndMap::iterator lIt = mDisplays.begin();
        while ( lIt != mDisplays.end() )
        {
            ( lIt->second )->Close();
            PVDELETE( lIt->second );
            lIt++;
        }
        mDisplays.clear();
#endif
    }

    // Pumps all the images out of the pipeline and return
    void RetrieveImages( uint32_t aTimeout )
    {
        // Set first wait timeout
        uint32_t lTimeout = aTimeout;

        // Loop for as long as images are available
        for ( ;; )
        {
            PvBuffer *lBuffer = NULL;
            PvResult lOperationResult;

            PvResult lResult = mPipeline->RetrieveNextBuffer( &lBuffer, lTimeout, &lOperationResult );
            if ( !lResult.IsOK() )
            {
                return;
            }

            // We got a buffer, check the acquisition result
            if ( lOperationResult.IsOK() )
            {
                Display( lBuffer );

                // Here you would typically do your image manipulation/processing
                mDoodleIndex++;
                mDoodleIndex %= sDoodleLength;
            }

            // We made it here, at least release the buffer back to the pipeline
            mPipeline->ReleaseBuffer( lBuffer );

            // Set timeout to 0 for next RetrieveNextBufer operations
            lTimeout = 0;
        }
    }

    // Attempts to display the content of a buffer
    void Display( PvBuffer *aBuffer )
    {
        switch ( aBuffer->GetPayloadType() )
        {
        case PvPayloadTypeImage:
        {
            std::stringstream lSS1;
            lSS1 << mSource.GetAscii();
            std::string lName = lSS1.str();

#ifdef PVDISPLAYWND
            PvDisplayWnd *lDisplay = GetDisplay( lName );
            lDisplay->Display( aBuffer->GetImage() );
            lDisplay->DoEvents();
#endif
        }
            break;

        case PvPayloadTypeMultiPart:
        {
            IPvMultiPartContainer *lContainer = aBuffer->GetMultiPartContainer();
            for ( uint32_t i = 0; i < lContainer->GetPartCount(); i++ )
            {
                IPvMultiPartSection *lSection = lContainer->GetPart( i );

                IPvImage *lImage = lSection->GetImage();
                if ( lImage != NULL )
                {
                    std::stringstream lSS1;
                    lSS1 << mSource.GetAscii() << ":Part" << i;
                    std::string lName = lSS1.str();

                    std::stringstream lSS2;
                    lSS2 << "Block:" << lImage->GetBlockID();
                    lSS2 << "  " << mSource.GetAscii();
                    lSS2 << "  Part:" << i << std::endl;
                    lSS2 << "Data Purpose:" << lSection->GetDataPurposeID();
                    lSS2 << "  Region:" << lSection->GetRegionID();
                    lSS2 << "  Section:" << lSection->GetSourceID();
                    lSS2 << std::endl;
                    switch ( lSection->GetDataType() )
                    {
                    case PvMultiPart2DImage:
                        lSS2 << "2D Image";
                        break;

                    case PvMultiPart2DPlaneOfBiPlanar:
                        lSS2 << "2D Plane of BipPanar";
                        break;

                    case PvMultiPart2DPlaneOfTriPlanar:
                        lSS2 << "2D Plane of TriPlanar";
                        break;

                    case PvMultiPart2DPlaneOfQuadPlanar:
                        lSS2 << "2D Plane of QuadPlanar";
                        break;

                    case PvMultiPart3DImage:
                        lSS2 << "3D Image";
                        break;

                    case PvMultiPart3DPlaneOfBiPlanar:
                        lSS2 << "3D Plane of BiPlanar";
                        break;

                    case PvMultiPart3DPlaneOfTriPlanar:
                        lSS2 << "3D Plane of TriPlanar";
                        break;

                    case PvMultiPart3DPlaneOfQuadPlanar:
                        lSS2 << "3D Plane of QuadPlanar";
                        break;

                    case PvMultiPartConfidenceMap:
                        lSS2 << "Confidence Map";
                        break;

                    default:
                        break;
                    }
                    lSS2 << "  " << lImage->GetWidth() << "x" << lImage->GetHeight();
                    lSS2 << "  " << PvImage::PixelTypeToString( lImage->GetPixelType() ).GetAscii();

#ifdef PVDISPLAYWND
                    PvDisplayWnd *lDisplay = GetDisplay( lName );
                    lDisplay->Display( lImage );
                    lDisplay->SetTextOverlay( lSS2.str().c_str() );
                    lDisplay->DoEvents();
#endif
                }
            }
        }
        break;

        default:
            break;
        }
    }

#ifdef PVDISPLAYWND
    // Returns a display by its name, creates and configures it if it does not yet exist
    PvDisplayWnd *GetDisplay( const std::string &aName )
    {
        PvDisplayWndMap::iterator lIt = mDisplays.find( aName );
        if ( lIt != mDisplays.end() )
        {
            // Return existing display
            return lIt->second;
        }

        // Create new display
        PvDisplayWnd *lDisplay = new PvDisplayWnd;
        if ( lDisplay == NULL )
        {
            return NULL;
        }

        // Store display in map
        mDisplays[ aName ] = lDisplay;

        // Setup display
        lDisplay->ShowModeless();
        lDisplay->SetTitle( aName.c_str() );

        return lDisplay;
    }
#endif

    // Returns statistics in a string for stream of the source
    void PrintStatistics()
    {
        // Get frame rate
        double lFPS = 0.0;
        mStream->GetParameters()->GetFloatValue( "AcquisitionRate", lFPS );

        // Get bandwidth, convert in Mb/s
        double lBandwidth = 0.0;
        mStream->GetParameters()->GetFloatValue( "Bandwidth", lBandwidth );
        lBandwidth /= 1000000.0;

        // Display the source name (if available)
        if ( mSource.GetLength() > 0 )
        {
            cout << mSource.GetAscii() << ": ";
        }

        // Print spinning doodle
        cout << sDoodle[ mDoodleIndex ] << " ";

        // Display FPS, bandwidth
        cout << fixed;
        cout << setprecision( 1 ) << lFPS << " FPS ";
        cout << setprecision( 1 ) << lBandwidth << " Mb/s ";
    }

    // Returns recommended timeout when calling RetrieveNextBuffer
    double GetRecommendedTimeout()
    {
        // Get frame rate
        double lFPS = 0.0;
        mStream->GetParameters()->GetFloatValue( "AcquisitionRate", lFPS );

        // If no frame rate, recommend 1s timeout
        if ( lFPS == 0.0 )
        {
            return 1.0;
        }

        // Convert from frequency to period, s to ms
        double lTimeout = ( 1.0 / lFPS ) * 1000.0;

        // Be a bit more aggressive
        lTimeout /= 2.0;

        // Make sure we have at least 1 ms
        if ( lTimeout < 1.0 )
        {
            lTimeout = 1.0;
        }

        return lTimeout;
    }

protected:

    void SelectSource( PvGenStateStack *aStack )
    {
        // If no source defined, there is likely no source selector, nothing to select
        if ( mSource.GetLength() <= 0 )
        {
            return;
        }

        // Select source. When stack goes out of scope, the previous value will be restored
        aStack->SetEnumValue( "SourceSelector", mSource );
    }

private:

    PvDevice *mDevice;
    PvStream *mStream;
    PvPipeline *mPipeline;

    PvString mConnectionID;
    PvString mSource;

    int mDoodleIndex;

#ifdef PVDISPLAYWND
    PvDisplayWndMap mDisplays;
#endif

};


// Vector of sources
typedef std::list<Source *> SourceList;


//
// Shows how to use a PvPipeline object to acquire images from a device
//

bool AcquireImages()
{
    PvResult lResult = PvResult::Code::INVALID_PARAMETER;

    // Select the device
    PvString lConnectionID;
    PvDeviceInfoType lType;
    if ( !PvSelectDevice( &lConnectionID, &lType ) )
    {
        cout << "No device selected." << endl;
        return false;
    }

    // This sample only supports GigE Vision devices
    if ( lType != PvDeviceInfoTypeGEV )
    {
        cout << "The selected device is not currently supported by this sample." << endl;
        return false;
    }

    // Connect to the GigE Vision device
    PvDeviceGEV* lDevice = new PvDeviceGEV();
    cout << "Connecting to device" << endl;
    lResult = lDevice->Connect( lConnectionID );
    if ( !lResult.IsOK() )
    {
        cout << "Unable to connect to device" << endl;
        return false;
    }
    cout << "Successfully connected to device" << endl << endl;

    SourceList lSources;

    // Get source selector.
    PvGenEnum *lSourceSelector = lDevice->GetParameters()->GetEnum( "SourceSelector" );
    if ( lSourceSelector != NULL )
    {
        // Go through all sources, create source objects.
        int64_t lCount = 0;
        lSourceSelector->GetEntriesCount( lCount );
        for ( int64_t i = 0; i < lCount; i++ )
        {
            // Get source enum entry.
            const PvGenEnumEntry *lEE = NULL;
            lSourceSelector->GetEntryByIndex( i, &lEE );

            // If available, create source.
            if ( ( lEE != NULL ) && lEE->IsAvailable() )
            {
                // Get source name
                PvString lSourceName;
                lEE->GetName( lSourceName );

                // Create source.
                Source *lSource = new Source( lDevice, lConnectionID, lSourceName );
                if ( lSource->Open() )
                {
                    // Add to sources list.
                    lSources.push_back( lSource );
                }
                else
                {
                    delete lSource;
                    lSource = NULL;
                }

                cout << endl;
            }
        }
    }
    else
    {
        // If no source selector, just create a single source.
        Source *lSource = new Source( lDevice, lConnectionID, "" );
        lSource->Open();

        // Add to sources list.
        lSources.push_back( lSource );

        cout << endl;
    }

    // Start the acquisition on all sources.
    SourceList::iterator lIt = lSources.begin();
    while ( lIt != lSources.end() )
    {
        ( *( lIt++ ) )->StartAcquisition();
        cout << endl;
    }

    // Aggressive initial value, will be adjusted vs frame rate.
    uint32_t lTimeout = 1;

    // Acquire images until the user instructs us to stop.
    cout << "<press a key to stop streaming>" << endl;
    while ( !PvKbHit() )
    {
        double lNewTimeout = 1000.0;

        lIt = lSources.begin();
        while ( lIt != lSources.end() )
        {
            ( *lIt )->RetrieveImages( lTimeout );
            ( *lIt )->PrintStatistics();

            // Always use the smallest recommended timeout.
            double lRecommendedTimeout = ( *lIt )->GetRecommendedTimeout();
            if ( lRecommendedTimeout < lNewTimeout )
            {
                lNewTimeout = lRecommendedTimeout;
            }

            lIt++;
        }

        // Update timeout for next round - smallest recommended divided by number of sources.
        lTimeout = static_cast<uint32_t>( lNewTimeout / static_cast<double>( lSources.size() ) + 0.5 );

        cout << "\r";
    }

    PvGetChar(); // Flush key buffer for next stop.
    cout << endl << endl;

    // Stop the acquisition on all sources.
    lIt = lSources.begin();
    while ( lIt != lSources.end() )
    {
        ( *( lIt++ ) )->StopAcquisition();
        cout << endl;
    }

    // Close and delete sources.
    lIt = lSources.begin();
    while ( lIt != lSources.end() )
    {
        ( *lIt )->Close();
        cout << endl;

        delete *lIt;

        lIt++;
    }

    // Disconnect and free the device object
    PvDevice::Free( lDevice );

    return true;
}


//
// Main function
//

int main()
{
    PV_SAMPLE_INIT();

    // PvPipeline used to acquire images from a device.
    cout << "MultiSource sample" << endl << endl;
    cout << "Acquires images from a GigE Vision device" << endl << endl;
    AcquireImages();

    cout << "<press a key to exit>" << endl;
    PvWaitForKeyPress();

    PV_SAMPLE_TERMINATE();
    return 0;
}

