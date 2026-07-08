/*
  This is the reference example application code for XIMEA cameras.
  You can use it to simplify development of your camera application.
  
  Sample name: 
    xiAPI / Capture-10-images-to-file

  Description: 
    Open camera, capture 10 images and storing each into TIFF file

  Workflow:
    1: Open camera
    2: Set parameters
    3: Start acquisition
    4: Wait 10 images to find good exposure time by auto-exposure
    5: Each image captured - store all pixels into TIFF file image00x.tif
*/
// This program that captures 10 images. Prints first pixel value from each image.

#include <stdio.h>
#include <memory.h>
#if defined (_WIN32)
#include <xiApi.h>       // Windows
#else
#include <m3api/xiApi.h> // Linux, OSX
#include <unistd.h>      // usleep
#endif
#include <xiAPI/xiAPI_tiff.h>

#define EXPECTED_IMAGES 10
#define AUTO_EXP_IMAGES 10

// Check error macro. It executes function. Print and throw error if result is not OK.
#define CE(func) {XI_RETURN stat = (func); if (XI_OK!=stat) {printf("Error:%d returned from function:"#func"\n",stat);throw "Error";}}

int main(int argc, char* argv[])
{
	HANDLE xiH = NULL;
	try
	{
		printf("Opening first camera...\n");
		CE(xiOpenDevice(0, &xiH));

		printf("Setting exposure time to 10ms...\n");
		CE(xiSetParamInt(xiH, XI_PRM_EXPOSURE, 10000));
		CE(xiSetParamInt(xiH,XI_PRM_BUFFER_POLICY,XI_BP_SAFE));
		CE(xiSetParamInt(xiH, XI_PRM_AEAG, XI_ON));
		int is_color = 0;
		CE(xiGetParamInt(xiH, XI_PRM_IMAGE_IS_COLOR, &is_color));
		if (is_color)
		{
			// color camera
			CE(xiSetParamInt(xiH, XI_PRM_IMAGE_DATA_FORMAT, XI_RGB24));
			CE(xiSetParamInt(xiH, XI_PRM_AUTO_WB, XI_ON));
			CE(xiSetParamFloat(xiH, XI_PRM_EXP_PRIORITY, 1));
		}

		printf("Starting acquisition...\n");
		CE(xiStartAcquisition(xiH));
		for (int image_id = 0; image_id < EXPECTED_IMAGES + AUTO_EXP_IMAGES; image_id++)
		{
			XI_IMG image; // image buffer
			memset(&image, 0, sizeof(image));
			image.size = sizeof(XI_IMG);
			CE(xiGetImage(xiH, 5000, &image)); // getting next image from the camera opened
			unsigned char pixel = *(unsigned char*)image.bp;
			printf("Image %d (%dx%d) received from camera. First pixel value: %d\n", image_id, (int)image.width, (int)image.height, pixel);
			if (image_id < AUTO_EXP_IMAGES)
				continue; // wait for autoexposure stabilize
			char filename[100] = "";
			sprintf(filename, "image%03d.tif", image_id);
			WriteImage(&image, filename);
		}

		printf("Stopping acquisition...\n");
		xiStopAcquisition(xiH);
	}
	catch (const char* err)
	{
		printf("Error: %s\n", err);
	}
	xiCloseDevice(xiH);
	printf("Done\n");
#if defined (_WIN32)
	Sleep(5000);
#else
	usleep(5000*1000);
#endif
	return 0;
}
