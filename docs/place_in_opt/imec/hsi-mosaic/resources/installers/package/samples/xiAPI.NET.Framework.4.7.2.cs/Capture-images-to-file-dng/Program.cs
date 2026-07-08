using System;
using System.IO;
using System.Threading;
using xiApi.NET;

namespace xiAPI.NET_Capture_images_to_file_dng
{
    internal class Program
    {
        private static void Main(string[] args)
        {
            xiCam myCam = new xiCam();
            string image_path = "images";
            int serie_img_count = 4;
            try
            {
                // create directory for images
                Directory.CreateDirectory(image_path);
                // Get number of connected devicess
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

                //------------------------------------------------------------------------------------
                // Capture images into dng files using XI_IMG structure
                Console.WriteLine("");
                Console.WriteLine("Capturing DNG images");

                // Start image acquisition
                UIntPtr xiH = myCam.GetApiHandle();
                myCam.StartAcquisition();

                for (int i = 0; i < serie_img_count; i++)
                {
                    xiApi.XI_IMG image = new xiApi.XI_IMG();
                    image.Clear();
                    myCam.GetXI_IMG(ref image, 1000);

                    string fName = string.Format(image_path + "/Image{0}.dng", i);
                    Console.WriteLine("Got image: {0}, size {1}x{2} nframe {3}", i, image.width, image.height, image.nframe);

                    unsafe
                    {
                        int dng_ret = 0;
                        dng.XI_DNG_METADATA meta = new dng.XI_DNG_METADATA();
                        dng_ret = dng.InitMetadata(ref meta);
                        if (dng_ret == 0)
                        {
                            dng_ret = dng.FillMetadata((void*)xiH, ref meta);
                            if (dng_ret == 0)
                            {
                                dng_ret = dng.Store(fName, image, meta);
                                if (dng_ret != 0)
                                {
                                    Console.WriteLine("DNG store failed with error {0}", dng_ret);
                                }
                            }
                            else
                            {
                                Console.WriteLine("Failed to fill DNG metadata with error {0}", dng_ret);
                            }
                        }
                        else
                        {
                            Console.WriteLine("Failed to initialize DNG metadata with error {0}", dng_ret);
                        }
                    }
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