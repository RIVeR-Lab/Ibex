using System;
using System.Drawing;
using System.IO;
using System.Threading;
using System.Windows.Media.Imaging;
using xiApi.NET;

namespace xiAPI.NET_Capture_images_to_file
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

                //------------------------------------------------------------------------------------
                // Capture images using safe buffer policy
                Console.WriteLine("");
                Console.WriteLine("Capturing images with safe buffer policy");
                int width = 0, height = 0;
                myCam.GetParam(PRM.WIDTH, out width);
                myCam.GetParam(PRM.HEIGHT, out height);
                Bitmap safeImage = new Bitmap(width, height, System.Drawing.Imaging.PixelFormat.Format8bppIndexed);
                myCam.SetParam(PRM.BUFFER_POLICY, BUFF_POLICY.SAFE);

                // Start image acquisition
                myCam.StartAcquisition();

                //Bitmap myImage;
                int timeout = 10000;
                for (int i = 0; i < serie_img_count; i++)
                {
                    myCam.GetImage(safeImage, timeout);
                    string fName = string.Format(image_path + "/BWimage{0}.png", i);
                    Console.WriteLine("Got image: {0}, size {1}x{2} saving to {3}", i, safeImage.Width, safeImage.Height, fName);
                    safeImage.Save(fName);
                }

                myCam.StopAcquisition();

                //------------------------------------------------------------------------------------
                // Capture images using unsafe buffer policy
                Console.WriteLine("");
                Console.WriteLine("Capturing images with unsafe buffer policy");
                Bitmap unSafeImage;
                myCam.SetParam(PRM.BUFFER_POLICY, BUFF_POLICY.UNSAFE);

                // Set image output format to color RGBA
                myCam.SetParam(PRM.IMAGE_DATA_FORMAT, IMG_FORMAT.RGB32);
                myCam.SetParam(PRM.WIDTH, width);

                myCam.StartAcquisition();

                for (int i = 0; i < serie_img_count; i++)
                {
                    myCam.GetImage(out unSafeImage, timeout);
                    string fName = string.Format(image_path + "/RGBAimage{0}.png", i);
                    Console.WriteLine("Got image: {0}, size {1}x{2} saving to {3}", i, unSafeImage.Width, unSafeImage.Height, fName);
                    unSafeImage.Save(fName);
                }

                myCam.StopAcquisition();

                //------------------------------------------------------------------------------------
                // Capture images using WPF BitmapSource
                Console.WriteLine("");
                Console.WriteLine("Capturing images to BitmapSource object");
                BitmapSource myBitmapSrc;

                myCam.StartAcquisition();

                for (int i = 0; i < serie_img_count; i++)
                {
                    myCam.GetImage(out myBitmapSrc, timeout);
                    string fName = string.Format(image_path + "/BSRC_image{0}.png", i);
                    Console.WriteLine("Got image: {0}, size {1}x{2} saving to {3}", i, myBitmapSrc.Width, myBitmapSrc.Height, fName);

                    FileStream stream = new FileStream(fName, FileMode.Create);
                    PngBitmapEncoder encoder = new PngBitmapEncoder();
                    encoder.Frames.Add(BitmapFrame.Create(myBitmapSrc));
                    encoder.Save(stream);
                    GC.Collect();
                }

                myCam.StopAcquisition();

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
                Console.WriteLine("Capturing 16bit images to BitmapSource");

                myCam.SetParam(PRM.BUFFER_POLICY, BUFF_POLICY.UNSAFE);

                // Set image output format to raw16
                myCam.SetParam(PRM.IMAGE_DATA_FORMAT, IMG_FORMAT.RAW16);
                BitmapSource rawBitmapSrc;

                myCam.StartAcquisition();

                for (int i = 0; i < serie_img_count; i++)
                {
                    myCam.GetImage(out rawBitmapSrc, 1000);
                    string fName = string.Format(image_path + "/BSRC_Raw16_image{0}.tif", i);
                    Console.WriteLine("Got image: {0}, size {1}x{2} saving to {3}", i, rawBitmapSrc.Width, rawBitmapSrc.Height, fName);

                    FileStream stream = new FileStream(fName, FileMode.Create);
                    TiffBitmapEncoder encoder = new TiffBitmapEncoder();
                    encoder.Frames.Add(BitmapFrame.Create(rawBitmapSrc));
                    encoder.Save(stream);
                    GC.Collect();
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