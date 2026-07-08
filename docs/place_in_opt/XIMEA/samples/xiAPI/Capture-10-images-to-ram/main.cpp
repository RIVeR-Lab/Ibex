/*
  This is the reference example application code for XIMEA cameras.
  You can use it to simplify development of your camera application.
  
  Sample name: 
    xiAPI / Capture-10-images-to-ram

  Description: 
    Open camera, allocate space for 10 images, capture 10 images and storing each into allocated memory by application, close camera, print first pixel from images.

  Workflow:
    1: Open camera
    2: Set parameters
    3: Allocate RAM for images
    4: Start acquisition
    5: Receive images data from the camera to RAM
    6: close camera
    7: Print image data stored in RAM
*/
//This project captures 10 images and stores them to RAM and prints the first pixel from each image stored in RAM.

#if defined (_WIN32)
#include "xiApi.h"
#else
#include <m3api/xiApi.h>
#endif

#include <stdio.h>
#include <vector> 
#include <chrono>
#include <thread>
#include <cstring>

#define CE(func) {XI_RETURN stat = (func); if (XI_OK!=stat) {printf("Error:%d returned from function:"#func"\n",stat);throw "Error";}}
#define MAX_IMAGES 10
#define IMAGE_TIMEOUT_MS 3000

int main(int argc, char* argv[])
{
	HANDLE xiH = NULL;
	try
	{
		HANDLE camh = NULL;
		printf("Opening camera...\n");
		CE(xiOpenDevice(0, &camh));
		printf("Setting the camera's parameters...\n");
		CE(xiSetParamInt(camh, XI_PRM_IMAGE_DATA_FORMAT, XI_RAW8));
		CE(xiSetParamInt(camh, XI_PRM_BUFFER_POLICY, XI_BP_SAFE));
		CE(xiSetParamInt(camh, XI_PRM_EXPOSURE, 10000));
		printf("Allocating RAM for images...\n");
		int img_size_bytes = 0;
		CE(xiGetParamInt(camh, XI_PRM_IMAGE_PAYLOAD_SIZE, &img_size_bytes));
		std::vector<XI_IMG> images;
		for (int i = 0; i< MAX_IMAGES; i++)
		{
			XI_IMG image;
			memset(&image, 0, sizeof(image));
			image.size = sizeof(XI_IMG);
			image.bp = (unsigned char*)malloc(img_size_bytes);
			image.bp_size = img_size_bytes;
			images.push_back(image);
		}
		printf("Starting acquisition...\n");
		CE(xiStartAcquisition(camh));
		printf("Receiving images data from the camera to RAM...\n");
		for (int i = 0; i < MAX_IMAGES; i++)
		{
			XI_IMG captured_image = images.at(i);
			CE(xiGetImage(camh, IMAGE_TIMEOUT_MS, &captured_image));
			images.at(i) = captured_image;
			unsigned char pixel = *(unsigned char*)captured_image.bp;
			int nframe = captured_image.nframe;
			printf("Image %d received from camera. First pixel value: %d\n", nframe, pixel);
		}
		printf("Stoping acquisition and closing the camera...\n");
		CE(xiStopAcquisition(camh));
		CE(xiCloseDevice(camh));
		// Now all images are stored in RAM and can be accessed without API
		printf("Printing image data stored in RAM...\n");
		for (int i = 0; i < MAX_IMAGES; i++)
		{
			unsigned char pixel = *(unsigned char*)images.at(i).bp;
			int nframe = images.at(i).nframe;
			printf("Image %d access in RAM. First pixel value: %d\n", nframe, pixel);
		}
	}
	catch (const char*)
	{
	}
	xiCloseDevice(xiH);
	printf("Done\n");
	std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // wait 2 seconds
	return 0;
}
