/*
  This is the reference example application code for XIMEA cameras.
  You can use it to simplify development of your camera application.
  
  Sample name: 
    xiAPI / Capture-10-images

  Description: 
    Open camera, capture 10 images while printing first pixel from each image.

  Workflow:
    1: Open camera
    2: Set parameters
    3: Start acquisition
    4: Each image captured - print dimensions and value of the first pixel
*/

#include <stdio.h>
#include <memory.h>
#if defined (_WIN32)
#include <xiApi.h>       // Windows
#else
#include <m3api/xiApi.h> // Linux, OSX
#endif

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

		// Note:
		// The default parameters of each camera might be different in different API versions
		// In order to ensure that your application will have camera in expected state,
		// please set all parameters expected by your application to required value.

		printf("Starting acquisition...\n");
		CE(xiStartAcquisition(xiH));

		XI_IMG image; // image buffer
		memset(&image, 0, sizeof(image));
		image.size = sizeof(XI_IMG);

#define EXPECTED_IMAGES 10
		for (int images = 0; images < EXPECTED_IMAGES; images++)
		{
			CE(xiGetImage(xiH, 5000, &image)); // getting next image from the camera opened
			unsigned char pixel = *(unsigned char*)image.bp;
			printf("Image %d (%dx%d) received from camera. First pixel value: %d\n", images, (int)image.width, (int)image.height, pixel);
		}

		printf("Stopping acquisition...\n");
		xiStopAcquisition(xiH);
		xiCloseDevice(xiH);
	}
	catch (const char*)
	{
	}
	printf("Done\n");
#if defined (_WIN32)
	Sleep(4000);
#endif
	return 0;
}
