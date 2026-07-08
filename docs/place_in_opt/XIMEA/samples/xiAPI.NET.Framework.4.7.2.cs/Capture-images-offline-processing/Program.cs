using System;
using System.Drawing;
using System.IO;
using System.Threading;
using xiApi;
using xiApi.NET;

namespace xiProc_NET_Example
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("TestOfflineProcessing_UsingAPI_WithCamera V3\n");

            const int max_images = 10;
            xiCam myCam = new xiCam();
            xiProc myProc = new xiProc();
            XI_IMG[] images = new XI_IMG[max_images];

            string image_path = "images";
            // create directory for images
            Directory.CreateDirectory(image_path);

            try
            {
                // Get number of connected devices
                int numDevices = 0;
                myCam.GetNumberDevices(out numDevices);

                if (0 == numDevices)
                {
                    Console.WriteLine("No devices found");
                    Thread.Sleep(3000);
                    return;
                }

                // Initialize the device and return the device handle.
                myCam.OpenDevice(0);
                myCam.SetParam(PRM.IMAGE_DATA_FORMAT, IMG_FORMAT.RAW8);
                myCam.SetParam(PRM.BUFFER_POLICY, BUFF_POLICY.SAFE);
                // set the same parameters like when camera processing is running online
                // these parameters will be part of API context list
                myCam.SetParam(PRM.EXPOSURE, 10000);
                myCam.SetParam(PRM.WB_KR, 1.05F);
                myCam.SetParam(PRM.WB_KG, 1.0F);
                myCam.SetParam(PRM.WB_KB, 2.10F);
                myCam.SetParam(PRM.GAMMAY, 1.0F);
                myCam.SetParam(PRM.SHARPNESS, 1.0F);

                // capture images
                myCam.StartAcquisition();
                for (int i = 0; i < max_images; i++)
                {
                    images[i].Clear();
                    myCam.GetXI_IMG(ref images[i], 1000);
                }

                // offline processing
                Console.WriteLine("Offline processing started");
                myProc.ProcOpen();

                string cam_context = string.Empty;
                cam_context = myCam.GetParamString(PRM.API_CONTEXT_LIST);
                myProc.ProcSetParam(PRM.API_CONTEXT_LIST, cam_context);
                myProc.ProcSetParamInt(PRM.IMAGE_DATA_FORMAT, IMG_FORMAT.RGB24);

                for (int i = 0; i < max_images; i++)
                {
                    Bitmap processed_image;
                    myProc.ProcPushImage(images[i]);
                    myProc.ProcPullImage(out processed_image, 1000);
                    string fName = string.Format(image_path + "/RGBAimage{0}.png", i);
                    Console.WriteLine("Got image: {0}, size {1}x{2} saving to {3}", i, processed_image.Width, processed_image.Height, fName);
                    processed_image.Save(fName);
                }

                // clear acquisition buffers and close camera
                myCam.StopAcquisition();
                myCam.CloseDevice();
            }
            catch (xiExc exc)
            {
                Console.WriteLine(exc.Message);
                Thread.Sleep(3000);
                myCam.CloseDevice();
            }
        }
    }
}