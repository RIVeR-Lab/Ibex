/*
  This is the reference example application code for XIMEA cameras.
  You can use it to simplify development of your camera application.
  
  Sample name: 
    xiAPI / Capture-50-images-gentl

  Description: 
    Open camera in xiAPI, then using GenTL interface (datastream) capture 50 images. Sample illustrates mixed xiAPI (for parameters) and GenTL (for buffering) approach.

  Workflow:
    1: Open camera
    2: Set parameters
    3: Get handle to GenTL-datastream and pass it into xiGenTL object
    4: Start acquisition using xiGenTL
    5: For each image captured using xiGenTL - show basic info (id, dimensions, time-stamp)
*/

#include <inttypes.h>
#include <stdio.h>
#include <xi_GenTL.h>

#if defined (_WIN32)
#include <xiApi.h>       // Windows
#else
#include <m3api/xiApi.h> // Linux, OSX
#endif
#include <chrono>
#include <thread>

// Check error macro. It executes function. Print and throw error if result is not OK.
#define CE(func) {XI_RETURN stat = (func); if (XI_OK!=stat) {printf("Error:%d returned from function:"#func"\n",stat);throw GenTL_Exception("xiAPIError", stat);}}
#define TOTAL_BUFFERS   10
#define EXPECTED_IMAGES 50
#define IMAGE_TIMEOUT_MS 1000

//-----------------------------------------------------

void PrintBufferInfo(ImgBuffer& b);

//-----------------------------------------------------

int main(int argc, char* argv[])
{
	try
	{
		// Retrieving a handle to the camera device
		HANDLE xiH = NULL;
		printf("Opening first camera...\n");
		CE(xiOpenDevice(0, &xiH));

		CE(xiSetParamInt(xiH, XI_PRM_IMAGE_DATA_FORMAT, XI_RAW8));
		CE(xiSetParamInt(xiH, XI_PRM_IMAGE_DATA_FORMAT, XI_FRM_TRANSPORT_DATA));
		int payload_size = 0;
		CE(xiGetParamInt(xiH, XI_PRM_IMAGE_PAYLOAD_SIZE, &payload_size));
		CE(xiSetParamInt(xiH, XI_PRM_EXPOSURE, 100000));
		// enable GenTL streaming
		CE(xiSetParamInt(xiH, XI_PRM_GENTL_DATASTREAM_ENABLED, XI_ON));
		// read data stream context handle
		DS_HANDLE gentl_data_stream_handle = NULL;
		CE(xiGetParamString(xiH, XI_PRM_GENTL_DATASTREAM_CONTEXT, &gentl_data_stream_handle, sizeof(void*)));

		// GenTL stream acquisition part
		xiGenTL xigentl;
		xigentl.Initialize(gentl_data_stream_handle);
		printf("Preparing image acquisition buffers...\n");
		xigentl.AllocAndAnnounceAllBuffers(payload_size, TOTAL_BUFFERS);
		xigentl.QueueAllBuffers();
		// acquire buffers
		printf("Starting acquisition...\n");
		xigentl.StartAcquisition();

		for (int i = 0; i < EXPECTED_IMAGES; i++)
		{
			ImgBuffer b;
			xigentl.GetNextBuffer(b, IMAGE_TIMEOUT_MS);
			PrintBufferInfo(b);
			xigentl.QueueBuffer(b);
		}
		// stop streaming and clear buffers
		printf("Stopping acquisition...\n");
		xigentl.StopAcquisition();
        xigentl.FlushQueue(ACQ_QUEUE_ALL_DISCARD);
		xigentl.RevokeAllBuffers();
		xigentl.Uninitialize();
		xiCloseDevice(xiH);
	}
	catch (GenTL_Exception e)
	{
		printf("Error occurred:%s\n", e.what());
	}
	printf("Done\n");
	std::this_thread::sleep_for(std::chrono::milliseconds(5000)); // wait 5 seconds
	return 0;
}

//-----------------------------------------------------

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
		printf("ID:%03d %dx%d ExpTime:%.3fms TimeStamp:%.6fs\n", frame_id, width, height, (float)exp_time_us/1000, (float)(time_stamp) / 1000000000.0);
	}
	else
	{
		printf(" - Warning: Frame transfer was interrupted on transport - buffer is not valid\n");
	}
}

//-----------------------------------------------------
