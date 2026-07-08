/*
******************************************************************************
* @attention
*
*<h2><center>&copy; COPYRIGHT(c) 2021 Photonfocus AG</center></h2>
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
* 1. Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
* 3. Neither the name of Photonfocus nor the names of its contributors
* may be used to endorse or promote products derived from this software
* without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*/
/**
//  \file Console_Offline_DR_OpenCV.cpp
//
//  \brief
//  This example shows the generic process to demodulate an image captured previously with double rate camera and saved to ".BMP" file.
//  See related sample PFCameraLib_ConfigAndGrabConsole_Online_DR to verify how capture double rate images.
//
//  Description: This sample uses OpenCV library to load images from file with BMP format. A few assumptions of the input images is done:
//
//  1. The images are saved in BMP format.
//  2. The images captured are in monochrome format. PixelMono8 is the format used.
//  3. "Window_W" is 2048 (this value cannot be obtained directly from the input image but can be read in the camera).
//
// After the image is loaded from disk can be demodulated with method PFImage::DemodulateDR(). Finally the demodulated image is saved to ".BMP" file.
//
*/

#include <stdio.h>
#include <iostream>

#include "PFCamera.h"
#include "PFStreamGEV.h"
#include "PFStreamU3V.h"
#include "PFDiscovery.h"
#include "PFImage.h"
#include "pfcPixelTypes.h"

// Headers used for OpenCV libraries
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#ifdef WIN32
#include <Windows.h>
#include <conio.h>
#include <direct.h>
#else
#include <unistd.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <termios.h>
#if HAVE_STROPTS_H
    #include <stropts.h>
#endif

#define _getcwd getcwd
#define MAX_PATH  4096

int _kbhit() {
    static const int STDIN = 0;
    static bool initialized = false;

    if (!initialized) {
        // Use termios to turn off line buffering
        struct termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

char _getch(void)
{
    char buf = 0;
    struct termios old = { 0 };
    fflush(stdout);
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return buf;
}
#endif

int KeyPressed(char key)
{
    return (_kbhit() != 0) && (tolower(_getch()) == tolower(key));
}


using namespace cv;
using namespace std;
using namespace pfcameralib;
using namespace pfcameralib::image::type_util;

int main()
{
    char filename[512];
    char input_path[512];
    
    // Load image from disk
    for (int i = 0; i < 5; i++)
    {
        sprintf(input_path, "image_%d_mod.bmp",i);
        // Use imread to load BMP monochrome image
        Mat img = imread(input_path, IMREAD_GRAYSCALE);

        if (img.empty())
        {
            std::cout << "Could not read the image: " << input_path << std::endl;
            return 1;
        }

        size_t sizeInBytes = img.total() * img.elemSize();

        PFImage pfImage(PixelMono8, img.cols, img.rows, 0, 0, 0, 0, (uint64_t)sizeInBytes, img.data);
        PFImage pfImageDest;

        // Allocate image for demodulation, sizeX correspond to Window_W feature value that you will find in double rate cameras, this value corresponds to the width of the image demodulated 
        uint32_t Window_W = 2048;
        pfImageDest.ReserveImage(PixelMono8, Window_W, (uint32_t)img.rows);

        // Cameras with support for color formats decode the image different, this is why isColor may be true even using mono formats
        PFResult pfResult = pfImage.DemodulateDR(pfImageDest, true);

        sprintf(filename, "image_%d_demod.bmp", i);
        // Save demodulated image to a file
        pfImageDest.SaveToFile(filename, pfImageFileType::BmpFileType);
        pfImageDest.ReleaseImage();
    }
    
    std::cout << "\r\nPress any key to finish...\r\n" << endl;
    _getch();
    return 0;
}



