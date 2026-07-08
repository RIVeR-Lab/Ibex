/*
  This is the reference example application code for XIMEA cameras.
  You can use it to simplify development of your camera application.
  
  Sample name: 
    xiAPIplus / Capture-10-images-to-file

  Description: 
    Open camera in xiAPIplus, capture 10 images while storing each into TIFF file

  Workflow:
    1: Open camera
    2: Set parameters
    3: Start acquisition
    4: Wait 10 images to find good exposure time by auto-exposure
    5: Each image captured - store all pixels into TIFF file image00x.tif
*/

#include <stdio.h>
#include <memory.h>
#include <xiAPIplus/xiapiplus.h>
#include <xiAPIplus/xiAPIplus_tiff.h>

#define EXPECTED_IMAGES 10
#define AUTO_EXP_IMAGES 10

int main(int argc, char* argv[])
{
	try
	{
		xiAPIplus_Camera cam;
		printf("Opening first camera...\n");
		cam.OpenFirst();
		printf("Setting exposure time to 10ms...\n");
		cam.SetExposureTime(10000);

		cam.SetBufferPolicy(XI_BP_SAFE);
		cam.EnableAutoExposureAutoGain();
		if (cam.IsSensorColor())
		{
			// color camera
			cam.SetImageDataFormat(XI_RGB24);
			cam.EnableWhiteBalanceAuto();
			cam.SetAutoExposureAutoGainExposurePriority(1);
		}
		printf("Starting acquisition...\n");
		cam.StartAcquisition();
		for (int image_id = 0; image_id < EXPECTED_IMAGES + AUTO_EXP_IMAGES; image_id++)
		{
			xiAPIplus_Image cam_image;
			cam.GetNextImage(&cam_image);
			if (image_id < AUTO_EXP_IMAGES)
				continue; // wait for autoexposure stabilize
			char filename[100] = "";
			sprintf(filename, "image%03d.tif", image_id);
			WriteImage(&cam_image, filename);
		}
		printf("Stopping acquisition...\n");
		cam.StopAcquisition();
		cam.Close();
		printf("Finished successfully\n");
	}
	catch (xiAPIplus_Exception exception)
	{
		printf("Error occurred:\n");
		exception.PrintError();
	}
	catch (const char* err)
	{
		printf("Error occurred:%s\n",err);
	}
#if defined (_WIN32)
	Sleep(2000);
#endif
	return 0;
}
