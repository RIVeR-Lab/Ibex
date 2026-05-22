/*
  This is the reference example application code for XIMEA cameras.
  You can use it to simplify development of your camera application.
  
  Sample name: 
    xiAPI / Capture-10-concatenated-images-to-file

  Description: 
    Open camera in xiAPI, then using GenTL interface (datastream) capture 2 buffers, each containing 5 images. Sample illustrates mixed xiAPI (for parameters) and GenTL (for buffering) approach.

  Workflow:
    1: Open camera
    2: Set parameters
    3: Get handle to GenTL-datastream and pass it into xiGenTL object
    4: Start acquisition using xiGenTL
    5: For each image captured using xiGenTL - show basic info (id, dimensions, time-stamp)
    6: Convert each image to the XI_IMG object
    7: Save each image to the .TIFF file
*/


#ifdef WIN32
#include <xiApi.h>       // Windows
#else
#include <m3api/xiApi.h> // Linux, OSX
#endif
#include <xi_GenTL.h>
#include <xiAPI_tiff.h>
#include <string.h>

// Error handling macros
#define CE(func) {XI_RETURN stat = (func); if (XI_OK!=stat) {printf("Error:%d returned from function:"#func"\n",stat);throw GenTL_Exception("xiAPIError", stat);}}
#define THROW_IF_ZERO(val) {if (!val) {printf("Fatal error: Value "#val" is zero\n"); throw GenTL_Exception("ValueIsZero", 0);}}

// Configuration
#define IMAGES_PER_BUFFER 5  // Number of images to concatenate in one buffer
#define EXPECTED_BUFFERS 2   // Number of buffers to capture
#define IMAGE_TIMEOUT_MS 10000

// Function to print buffer information
void PrintBufferInfo(ImgBuffer& b);
// Process buffer and extract individual images
void ProcessAndSaveImages(ImgBuffer& buffer, int buffer_index, int img_offset);

int main(int argc, char* argv[])
{
    try
    {
        // Retrieving a handle to the camera device
        HANDLE xiH = NULL;
        printf("Opening first camera...\n");
        CE(xiOpenDevice(0, &xiH));

        // Configure camera for RAW8 image format
        CE(xiSetParamInt(xiH, XI_PRM_IMAGE_DATA_FORMAT, XI_RAW8));
        CE(xiSetParamInt(xiH, XI_PRM_IMAGE_DATA_FORMAT, XI_FRM_TRANSPORT_DATA));

        // Configure concatenation mode
        CE(xiSetParamInt(xiH, XI_PRM_CONCAT_IMG_COUNT, IMAGES_PER_BUFFER));
        CE(xiSetParamInt(xiH, XI_PRM_CONCAT_IMG_MODE, XI_ON));

        // Get new payload size
        uint32_t payload_size = 0;
        CE(xiGetParamInt(xiH, XI_PRM_IMAGE_PAYLOAD_SIZE, (int*) &payload_size));

        // Get transport image offset (important for extracting individual images)
        uint32_t img_offset = 0;
        CE(xiGetParamInt(xiH, XI_PRM_CONCAT_IMG_TRANSPORT_IMG_OFFSET, (int*) &img_offset));

        // Enable GenTL streaming
        CE(xiSetParamInt(xiH, XI_PRM_GENTL_DATASTREAM_ENABLED, XI_ON));

        // Set basic image parameters
        CE(xiSetParamInt(xiH, XI_PRM_EXPOSURE, 10000));
        CE(xiSetParamInt(xiH, XI_PRM_GAIN, 100));

        // Read data stream context handle
        DS_HANDLE gentl_data_stream_handle = NULL;
        CE(xiGetParamString(xiH, XI_PRM_GENTL_DATASTREAM_CONTEXT, &gentl_data_stream_handle, sizeof(void*)));

        // Initialize GenTL stream
        xiGenTL xigentl;
        xigentl.Initialize(gentl_data_stream_handle);

        printf("Preparing image acquisition buffers...\n");
        xigentl.AllocAndAnnounceAllBuffers(payload_size, EXPECTED_BUFFERS);
        xigentl.QueueAllBuffers();

        // Start acquisition
        printf("Starting acquisition...\n");
        xigentl.StartAcquisition();

        // Process buffers
        for (uint32_t i = 0; i < EXPECTED_BUFFERS; i++)
        {
            // Get next buffer
            ImgBuffer buffer;
            xigentl.GetNextBuffer(buffer, IMAGE_TIMEOUT_MS);
            PrintBufferInfo(buffer);

            // Process and save images from this buffer
            ProcessAndSaveImages(buffer, i, img_offset);

            // Queue buffer back for reuse
            xigentl.QueueBuffer(buffer);
        }

        // Stop acquisition and clean up
        printf("Stopping acquisition...\n");
        xigentl.AbortAcquisition();
        xigentl.FlushQueue(ACQ_QUEUE_ALL_DISCARD);
        xigentl.RevokeAllBuffers();
        xigentl.Uninitialize();
        xiCloseDevice(xiH);
    }
    catch (GenTL_Exception& e)
    {
        printf("Error occurred: %s\n", e.what());
    }

    printf("Done\n");
#if defined (_WIN32)
    Sleep(4000);
#endif
    return 0;
}

void PrintBufferInfo(ImgBuffer& b)
{
    printf("New buffer: ");
    if (b.HasNewData())

    {
        size_t width = 0;
        size_t height = 0;
        uint64_t frame_id = 0;
        uint64_t time_stamp = 0;
        uint32_t exp_time_us = 0;
        b.GetInfoUint64(BUFFER_INFO_FRAMEID, &frame_id);
        b.GetInfoSizet(BUFFER_INFO_WIDTH, &width);
        b.GetInfoSizet(BUFFER_INFO_HEIGHT, &height);
        b.GetInfoUint32(BUFFER_INFO_EXPOSURE_TIME_US, &exp_time_us);
        b.GetInfoUint64(BUFFER_INFO_TIMESTAMP, &time_stamp);
        printf("ID:%03d %dx%d ExpTime:%.3fms TimeStamp:%.6fs\n", frame_id, width, height, (float)exp_time_us / 1000, (float)(time_stamp) / 1000000000.0);
    }
    else
    {
        printf(" - Warning: Frame transfer was interrupted on transport - buffer is not valid\n");
    }
}

void ProcessAndSaveImages(ImgBuffer& buffer, int buffer_index, int img_offset)
{
    if (!buffer.HasNewData())
    {
        printf("Buffer has no valid data\n");
        return;
    }

    void* buffer_ptr = NULL;
    size_t width = 0;
    size_t height = 0;

    buffer.GetInfoPtr(BUFFER_INFO_BASE, &buffer_ptr);
    buffer.GetInfoSizet(BUFFER_INFO_WIDTH, &width);
    buffer.GetInfoSizet(BUFFER_INFO_HEIGHT, &height);

    THROW_IF_ZERO(buffer_ptr);

    // Create XI_IMG structure for each image within the buffer
    XI_IMG image;
    memset(&image, 0, sizeof(XI_IMG));
    image.size = sizeof(XI_IMG);
    image.width = width;
    image.height = height;
    image.frm = XI_RAW8;

    // Process each image in the buffer
    for (uint32_t i = 0; i < IMAGES_PER_BUFFER; i++)
    {
        // Calculate image position within the buffer
        image.bp = (unsigned char*)buffer_ptr + (i * (long long)img_offset);

        // Generate filename for this image
        char filename[100];
        sprintf(filename, "buffer%d_image%d.tiff", buffer_index+1, i+1);

        // Save as TIFF
        WriteImage(&image, filename);
    }
}