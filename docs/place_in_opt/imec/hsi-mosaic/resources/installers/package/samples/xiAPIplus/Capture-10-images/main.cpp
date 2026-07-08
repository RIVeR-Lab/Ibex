/*
  This is the reference example application code for XIMEA cameras.
  You can use it to simplify development of your camera application.
  
  Sample name: 
    xiAPIplus / Capture-10-images

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
#include <xiAPIplus/xiapiplus.h>
#include <chrono>
#include <thread>

int main(int argc, char* argv[])
{
	try
	{
		xiAPIplus_Camera cam;
		printf("Opening first camera...\n");
		cam.OpenFirst();
		printf("Setting exposure time to 10ms...\n");
		cam.SetExposureTime(10000);
		
		// Note:
		// The default parameters of each camera might be different in different API versions
		// In order to ensure that your application will have camera in expected state,
		// please set all parameters expected by your application to required value.
		
		printf("Starting acquisition...\n");
		cam.StartAcquisition();

		#define EXPECTED_IMAGES 10
		for (int images = 0; images < EXPECTED_IMAGES; images++)
		{
			xiAPIplus_Image image;
			cam.GetNextImage(&image);
			unsigned char* pixel = image.GetPixels();
			printf("Image %d (%dx%d) received from camera. First pixel value: %d\n", images, image.GetWidth(), image.GetHeight(), pixel[0]);
		}

		cam.StopAcquisition();
		cam.Close();
		printf("Finished successfully\n");
	}
	catch (xiAPIplus_Exception exception)
	{
		exception.PrintError();
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // wait 2 seconds
	return 0;
}
