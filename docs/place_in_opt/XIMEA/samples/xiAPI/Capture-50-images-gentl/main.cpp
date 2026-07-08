/*
  This is the reference example application code for XIMEA cameras.
  You can use it to simplify development of your camera application.
  
  Sample name: 
    xiAPI / Capture-50-images-gentl

  Description: 
    Open camera in xiAPI, then using GenTL interface (datastream) capture 10 images. Sample illustrates mixed xiAPI (for parameters) and GenTL (for buffering) approach.

  Workflow:
    1: Open camera
    2: Set parameters
    3: Get handle to GenTL-datastream and pass it into xiGenTL object
    4: Start acquisition using xiGenTL
    5: For each image captured using xiGenTL - show basic info (id, dimensions, time-stamp)
*/

#include <inttypes.h>
#include <stdio.h>
#include <GenTLStream.h>

#if defined (_WIN32)
#include <xiApi.h>       // Windows
#else
#include <m3api/xiApi.h> // Linux, OSX
#endif
#include <chrono>
#include <thread>

// Check error macro. It executes function. Print and throw error if result is not OK.
#define CE(func) {XI_RETURN stat = (func); if (XI_OK!=stat) {printf("Error:%d returned from function:"#func"\n",stat);throw GenTL_Exception("xiAPIError", stat);}}
#define EXPECTED_IMAGES 50
#define IMAGE_TIMEOUT_MS 1000

//-----------------------------------------------------

void PrintBufferInfo(GenTLStream* stream, EVENT_NEW_BUFFER_DATA* buffer_handle);

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
		GenTLStream stream;
		stream.SetStreamHandle(gentl_data_stream_handle);
		stream.RegisterEvent(EVENT_NEW_BUFFER);

		// Allocation of buffer
		#define PCIE_TRANSPORT_BUFFER_ALLIGN 4096
		char* rawImgBuffer = (char*)_aligned_malloc(payload_size, PCIE_TRANSPORT_BUFFER_ALLIGN);
		// Announcing of buffer
		GenTL::BUFFER_HANDLE rawImgBuffer_allocated_buffer_context = GENTL_INVALID_HANDLE;
		stream.AnnounceBuffer(rawImgBuffer, payload_size, &rawImgBuffer_allocated_buffer_context);
		stream.QueueBuffer(rawImgBuffer_allocated_buffer_context);
		// acquire buffers
		printf("Starting acquisition...\n");
		stream.StartAcquisition();

		for (int i = 0; i < EXPECTED_IMAGES; i++)
		{
			EVENT_NEW_BUFFER_DATA new_data;
			size_t buff_size = sizeof(EVENT_NEW_BUFFER_DATA);
			memset(&new_data, 0, sizeof(EVENT_NEW_BUFFER_DATA));
			int timeout_ms = 5000;
			GC_ERROR event_res = stream.EventGetData(&new_data, &buff_size, timeout_ms);
			switch (event_res)
			{
			case GC_ERR_SUCCESS:
				PrintBufferInfo(&stream, &new_data);
				stream.QueueBuffer(rawImgBuffer_allocated_buffer_context);
				break;
			default:
				printf("Error on EventGetData %d\n",event_res);
				break;
			}
		}
		// stop streaming and clear buffers
		printf("Stopping acquisition...\n");
		stream.StopAcquisition();
        stream.FlushQueue(ACQ_QUEUE_ALL_DISCARD);
		stream.RevokeBuffer(rawImgBuffer_allocated_buffer_context, NULL);
		
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

void PrintBufferInfo(GenTLStream* stream, EVENT_NEW_BUFFER_DATA* new_data)
{
	printf("New buffer: ");
	BUFFER_HANDLE buff_handle = new_data->BufferHandle;
	{
		uint8_t value = 0;
		size_t buff_size = sizeof(value);
		GC_ERROR gc_ret = GC_ERR_SUCCESS;
		INFO_DATATYPE type = INFO_DATATYPE_BOOL8;		

		stream->GetBufferInfo(buff_handle, BUFFER_INFO_NEW_DATA, &type, &value, &buff_size);
		if (!value)
		{
			printf("Warning: Frame transfer was interrupted on transport - buffer is not valid\n");
			return;
		}
	}

	uint64_t frame_id = 0;
	{
		size_t buff_size = sizeof(frame_id);
		INFO_DATATYPE type = INFO_DATATYPE_UINT64;
		stream->GetBufferInfo(buff_handle, BUFFER_INFO_FRAMEID, &type, &frame_id, &buff_size);
	}
	uint32_t exp_time_us = 0;
	{
		size_t buff_size = sizeof(exp_time_us);
		INFO_DATATYPE type = INFO_DATATYPE_UINT32;
		stream->GetBufferInfo(buff_handle, BUFFER_INFO_EXPOSURE_TIME_US, &type, &exp_time_us, &buff_size);
	}
	uint64_t time_stamp = 0;
	{
		size_t buff_size = sizeof(time_stamp);
		INFO_DATATYPE type = INFO_DATATYPE_UINT64;
		stream->GetBufferInfo(buff_handle, BUFFER_INFO_TIMESTAMP, &type, &time_stamp, &buff_size);
	}
	uint8_t* first_data_byte=NULL;
	{
		size_t buff_size = sizeof(first_data_byte);
		INFO_DATATYPE type = INFO_DATATYPE_UINT64;
		stream->GetBufferInfo(buff_handle, BUFFER_INFO_BASE, &type, &first_data_byte, &buff_size);
	}
	printf("ID:%03d ExpTime:%.3fms TimeStamp:%.3fs first byte:x%02x\n", frame_id, (float)exp_time_us / 1000, (float)(time_stamp) / 1000000000.0, *first_data_byte);
}

//-----------------------------------------------------
