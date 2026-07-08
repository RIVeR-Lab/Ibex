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
#include <stdio.h>
#include <iostream>
#include <fstream>

#ifdef WIN32
#include <windows.h>
#include <direct.h>
#include <conio.h>
#endif // WIN32

#include "PFCamera.h"

using namespace std;
using namespace PFCameraDLL;

int Configure_MVx_3D06(PFCamera &pfCamera)
{
    PFFeatureParameters pfFeatureParams;
    PFResult pfResult;
    double double_value;
    int64_t int_value;
    char enum_str[64];
    int64_t width;
    int64_t height;

    // Set Width value
    width = 1280;
    // Check if the value is inside the limits
    pfResult = pfCamera.GetFeatureParams("Width", &pfFeatureParams);
    if (width > pfFeatureParams.Max)
        width = pfFeatureParams.Max;
    else if (width < pfFeatureParams.Min)
        width = pfFeatureParams.Min;
    // Set the corresponding value
    pfResult = pfCamera.SetFeatureInt("Width", width);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    // Read back Width value
    pfResult = pfCamera.GetFeatureInt("Width", width);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    else
        cout << "Width: " << (uint16_t)width << endl;

    // Set Height value
    height = 512;
    // Check if the value is inside the limits
    pfResult = pfCamera.GetFeatureParams("Peak0_3DH", &pfFeatureParams);
    if (height > pfFeatureParams.Max)
        height = pfFeatureParams.Max;
    else if (height < pfFeatureParams.Min)
        height = pfFeatureParams.Min;
    // Set the corresponding value
    pfResult = pfCamera.SetFeatureInt("Peak0_3DH", height);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    // Read back Height value
    pfResult = pfCamera.GetFeatureInt("Peak0_3DH", height);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    else
        cout << "Peak0_3DH: " << (uint16_t)height << endl;
    
    // Set LineFinder Mode
    pfResult = pfCamera.SetFeatureEnum("LineFinder_Mode", "Mode_2Dand3D");
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    // Read back LineFinder Mode
    pfResult = pfCamera.GetFeatureEnum("LineFinder_Mode", enum_str);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    else
        cout << "LineFinder_Mode: " << enum_str << endl;

    // Set the LineFinder DataFormat3D
    // Format of the 3D data:
    // 2 : 3D coordinates only(2 rows)
    // 3 : LL_HEIGHT 4bit and LL_WIDTH 6bit (4 rows)
    // 4 : LL_HEIGHT 8bit and LL_WIDTH 4bit (4 rows)
    pfResult = pfCamera.SetFeatureInt("LineFinder_DataFormat3D", 2);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    // Read back LineFinder DataFormat3D value
    pfResult = pfCamera.GetFeatureInt("LineFinder_DataFormat3D", int_value);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    else
        cout << "LineFinder_DataFormat3D: " << int_value << endl;

    // Set Exposure Time value
    double_value = 3000.0;
    // Check if the value is inside the limits
    pfResult = pfCamera.GetFeatureParams("ExposureTime", &pfFeatureParams);
    if (double_value > pfFeatureParams.FloatMax)
        double_value = pfFeatureParams.FloatMax;
    else if (double_value < pfFeatureParams.FloatMin)
        double_value = pfFeatureParams.FloatMin;
    // Set the corresponding value
    pfResult = pfCamera.SetFeatureFloat("ExposureTime", double_value);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    // Read back Exposure Time value
    pfResult = pfCamera.GetFeatureFloat("ExposureTime", double_value);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    else
        cout << "ExposureTime: " << double_value << endl;


    // Set the corresponding Height value
    // First read back HeightInterface value
    pfResult = pfCamera.GetFeatureInt("HeightInterface", height);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    else
        cout << "HeightInterface: " << (uint16_t)height << endl;
    pfResult = pfCamera.SetFeatureInt("Height", height);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    // Read back Height value
    pfResult = pfCamera.GetFeatureInt("Height", height);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    else
        cout << "Height: " << (uint16_t)height << endl;

    return 0;
}
