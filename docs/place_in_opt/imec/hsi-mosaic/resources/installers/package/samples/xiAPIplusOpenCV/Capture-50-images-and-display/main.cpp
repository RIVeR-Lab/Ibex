/*
  This is the reference example application code for XIMEA cameras.
  You can use it to simplify development of your camera application.
  
  Sample name: 
    xiAPIplusOpenCV / Capture-50-images-and-display

  Description: 
    Open camera, capture 50 images while displaying each on the screen

  Workflow:
    1: Open camera
    2: Set parameters
    3: Start acquisition
    4: For each image captured - display on screen
*/

#include <stdio.h>
#include <xiApiPlusOcv.hpp>

#define EXPECTED_IMAGES 50

using namespace cv;
using namespace std;
int main(int argc, char* argv[])
{
	try
	{
		// Sample for XIMEA OpenCV
		xiAPIplusCameraOcv cam;

		// Retrieving a handle to the camera device
		printf("Opening first camera...\n");
		cam.OpenFirst();
		
		// Set exposure
		cam.SetExposureTime(10000); //10000 us = 10 ms
		// Note: The default parameters of each camera might be different in different API versions
		
		printf("Starting acquisition...\n");
		cam.StartAcquisition();
		
		printf("First pixel value \n");
		XI_IMG_FORMAT format = cam.GetImageDataFormat();
		for (int images=0;images < EXPECTED_IMAGES;images++)
		{
			Mat cv_mat_image = cam.GetNextImageOcvMat();
			if (format == XI_RAW16 || format == XI_MONO16)
			{
				normalize(cv_mat_image, cv_mat_image, 0, 65536, NORM_MINMAX, -1, Mat()); // 0 - 65536, 16 bit unsigned integer range
				printf("\t%d\n", cv_mat_image.at<ushort>(0, 0));
			}
			else
				printf("\t%d\n", cv_mat_image.at<unsigned char>(0, 0));
			cv::imshow("Image from camera",cv_mat_image);
			cvWaitKey(20);
		}
		
		cam.StopAcquisition();
		cam.Close();
		printf("Done\n");
		
		cvWaitKey(500);
	}
	catch(xiAPIplus_Exception& exp)
	{
		printf("Error:\n");
		exp.PrintError();
#if defined (_WIN32)
		Sleep(2000);
#endif
		cvWaitKey(2000);
		return -1;
	}
	return 0;
}

