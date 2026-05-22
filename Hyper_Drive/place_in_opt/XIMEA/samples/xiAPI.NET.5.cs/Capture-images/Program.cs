using System;
using System.IO;
using System.Threading;
using xiApi.NET;

namespace xiAPI.NET_example
{
    class Program
    {
        static void Main(string[] args)
        {
            xiCam myCam = new xiCam();
            string image_path = "images";
            int serie_img_count = 4;
            try
            {
                // create directory for images
                Directory.CreateDirectory(image_path);
                // Get number of connected devices
                int numDevices = 0;
                myCam.GetNumberDevices(out numDevices);

                if (0 == numDevices)
                {
                    Console.WriteLine("No devices found");
                    Thread.Sleep(3000);
                    return;
                }
                else
                {
                    Console.WriteLine("Found {0} connected devices.", numDevices);
                }

                // Initialize the device and return the device handle.
                myCam.OpenDevice(0);

                // Get device model name
                string strVal;
                myCam.GetParam(PRM.DEVICE_NAME, out strVal);
                Console.WriteLine("Found device {0}.", strVal);

                // Get device type
                myCam.GetParam(PRM.DEVICE_TYPE, out strVal);

                Console.WriteLine("Device type {0}.", strVal);

                // Get device serial number
                myCam.GetParam(PRM.DEVICE_SN, out strVal);
                Console.WriteLine("Device serial number {0}", strVal);

                // Set device exposure to 2 milliseconds
                int exposure_us = 2000;
                myCam.SetParam(PRM.EXPOSURE, exposure_us);
                Console.WriteLine("Exposure was set to {0} milliseconds", exposure_us / 1000);

                // Set device gain to 5 decibels
                float gainVal = 0;
                myCam.SetParam(PRM.GAIN, gainVal);
                Console.WriteLine("Gain was set to {0} decibels.", gainVal);

                // Set image output format to monochrome 8 bit
                myCam.SetParam(PRM.IMAGE_DATA_FORMAT, IMG_FORMAT.MONO8);

                int width = 0, height = 0;

                //------------------------------------------------------------------------------------
                // Capture images into raw files using Byte Array
                Console.WriteLine("");
                Console.WriteLine("Capturing images to ByteArray");

                myCam.SetParam(PRM.BUFFER_POLICY, BUFF_POLICY.SAFE);

                // Set image output format to monochrome 8 bit
                myCam.SetParam(PRM.IMAGE_DATA_FORMAT, IMG_FORMAT.RAW8);
                int BytePerPix = 1;

                byte[] im_array = null;

                width = myCam.GetParamInt(PRM.WIDTH);
                height = myCam.GetParamInt(PRM.HEIGHT);
                im_array = new byte[width * height * BytePerPix];

                myCam.StartAcquisition();

                for (int i = 0; i < serie_img_count; i++)
                {
                    myCam.GetImage(im_array, 1000);
                    string fName = string.Format(image_path + "/BWimage{0}.raw", i);
                    File.WriteAllBytes(fName, im_array);
                    Console.WriteLine("Saving Image  to {0}", fName);
                }

                myCam.StopAcquisition();

                //------------------------------------------------------------------------------------
                // Capture raw 16bit images into files using BitmapSource
                Console.WriteLine("");
                Console.WriteLine("Capturing 16bit images to XI_IMG class");

                myCam.SetParam(PRM.BUFFER_POLICY, BUFF_POLICY.UNSAFE);

                myCam.StartAcquisition();

                for (int i = 0; i < serie_img_count; i++)
                {
                    xiApi.XI_IMG image = myCam.GetXI_IMG(1000);
                    Console.WriteLine("Got image: {0}, size {1}x{2}", image.acq_nframe, image.width, image.height);
                }
                myCam.StopAcquisition();
                myCam.CloseDevice();
            }

            //------------------------------------------------------------------------------------
            catch (xiExc exc)
            {
                Console.WriteLine(exc.Message);
                Thread.Sleep(3000);
                myCam.CloseDevice();
            }
            //------------------------------------------------------------------------------------
        }
    }
}