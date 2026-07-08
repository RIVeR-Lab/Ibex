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

int Configure_MVx_D1280_L01_3D07(PFCamera &pfCamera)
{
    PFFeatureParameters pfFeatureParams;
    PFResult pfResult;
    double double_value;
    int64_t int_value;
    char enum_str[64];
    bool bool_value;
    int64_t LineFinder_ROI_Height;
    int64_t LineFinder_ROI_OffsetY;

    // Set Width value
    int_value = 1024;
    // Check if the value is inside the limits
    pfResult = pfCamera.GetFeatureParams("Width", &pfFeatureParams);
    if (int_value > pfFeatureParams.Max)
        int_value = pfFeatureParams.Max;
    else if (int_value < pfFeatureParams.Min)
        int_value = pfFeatureParams.Min;
    // Set the corresponding value
    pfResult = pfCamera.SetFeatureInt("Width", int_value);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    // Read back Width value
    pfResult = pfCamera.GetFeatureInt("Width", int_value);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    else
        cout << "Width: " << (uint16_t)int_value << endl;

    // Set OffsetX value
    int_value = 128;
    // Check if the value is inside the limits
    pfResult = pfCamera.GetFeatureParams("OffsetX", &pfFeatureParams);
    if (int_value > pfFeatureParams.Max)
        int_value = pfFeatureParams.Max;
    else if (int_value < pfFeatureParams.Min)
        int_value = pfFeatureParams.Min;
    // Set the corresponding value
    pfResult = pfCamera.SetFeatureInt("OffsetX", int_value);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    // Read back Width value
    pfResult = pfCamera.GetFeatureInt("OffsetX", int_value);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    else
        cout << "OffsetX: " << (uint16_t)int_value << endl;

    //Set LineFinder_ROI values. 3D07 has only one LineFinder ROI.
    LineFinder_ROI_Height = 600;
    LineFinder_ROI_OffsetY = 200;
    // Check if the value is inside the limits
    pfResult = pfCamera.GetFeatureParams("LineFinder_ROI_Height", &pfFeatureParams);
    if (LineFinder_ROI_Height > pfFeatureParams.Max)
        LineFinder_ROI_Height = pfFeatureParams.Max;
    else if (LineFinder_ROI_Height < pfFeatureParams.Min)
        LineFinder_ROI_Height = pfFeatureParams.Min;
    // Set the corresponding value
    pfResult = pfCamera.SetFeatureInt("LineFinder_ROI_Height", LineFinder_ROI_Height);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    // Read back Height value
    pfResult = pfCamera.GetFeatureInt("LineFinder_ROI_Height", LineFinder_ROI_Height);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    else
        cout << "LineFinder_ROI_Height: " << (uint16_t)LineFinder_ROI_Height << endl;

    // Check if the value is inside the limits
    pfResult = pfCamera.GetFeatureParams("LineFinder_ROI_OffsetY", &pfFeatureParams);
    if (LineFinder_ROI_OffsetY > pfFeatureParams.Max)
        LineFinder_ROI_OffsetY = pfFeatureParams.Max;
    else if (LineFinder_ROI_OffsetY < pfFeatureParams.Min)
        LineFinder_ROI_OffsetY = pfFeatureParams.Min;
    // Set the corresponding value
    pfResult = pfCamera.SetFeatureInt("LineFinder_ROI_OffsetY", LineFinder_ROI_OffsetY);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    // Read back Height value
    pfResult = pfCamera.GetFeatureInt("LineFinder_ROI_OffsetY", LineFinder_ROI_OffsetY);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    else
        cout << "LineFinder_ROI_OffsetY: " << (uint16_t)LineFinder_ROI_OffsetY << endl;

    //pfResult = pfCamera.SetFeatureEnum("LineFinder_Mode", "Mode_3Donly");
    pfResult = pfCamera.SetFeatureEnum("LineFinder_Mode", "Mode_2Dand3D");
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    // Read back LineFinder Mode
    pfResult = pfCamera.GetFeatureEnum("LineFinder_Mode", enum_str);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    else
        cout << "LineFinder_Mode: " << enum_str << endl;

    int_value = 4;
    pfResult = pfCamera.SetFeatureInt("LineFinder_NumberOfPeaks", int_value);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;

    // Read back LineFinder Mode
    pfResult = pfCamera.GetFeatureInt("LineFinder_NumberOfPeaks", int_value);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    else
        cout << "LineFinder_NumberOfPeaks: " << int_value << endl;

    // Set LineFinder Peak Height Enable
    pfResult = pfCamera.SetFeatureBool("LineFinder_PeakHeight_Enable", true);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    // Read back the corresponding value
    pfResult = pfCamera.GetFeatureBool("LineFinder_PeakHeight_Enable", bool_value);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    else
        cout << "LineFinder_PeakHeight_Enable: " << bool_value << endl;

    // Set LineFinder Peak Width Enable
    pfResult = pfCamera.SetFeatureBool("LineFinder_PeakWidth_Enable", true);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    // Read back the corresponding value
    pfResult = pfCamera.GetFeatureBool("LineFinder_PeakWidth_Enable", bool_value);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    else
        cout << "LineFinder_PeakWidth_Enable: " << bool_value << endl;
        
    // Set LineFinder Status Line Enable
    pfResult = pfCamera.SetFeatureBool("LineFinder_StatusLine_Enable", true);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    // Read back the corresponding value
    pfResult = pfCamera.GetFeatureBool("LineFinder_StatusLine_Enable", bool_value);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    else
        cout << "LineFinder_StatusLine_Enable: " << bool_value << endl;


    // Set MPLF SubPixel Resolution
    int_value = 6;
    pfResult = pfCamera.SetFeatureInt("MPLF_SubPixelResolution", int_value);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    // Read back LineFinder Mode
    pfResult = pfCamera.GetFeatureInt("MPLF_SubPixelResolution", int_value);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    else
        cout << "MPLFSubPixelResolution: " << int_value << endl;

    // Set LineFinder_PeakPositionNrOfRows
    int_value = 2;
    pfResult = pfCamera.SetFeatureInt("LineFinder_PeakPositionNrOfRows", int_value);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    // Read back the corresponding value
    pfResult = pfCamera.GetFeatureInt("LineFinder_PeakPositionNrOfRows", int_value);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    else
        cout << "LineFinder_PeakPositionNrOfRows: " << int_value << endl;
    
    // Set LineFinder Test Image Enable
    pfResult = pfCamera.SetFeatureBool("LFTestImage_Enable", false);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    // Read back LineFinder Test Image Enable
    pfResult = pfCamera.GetFeatureBool("LFTestImage_Enable", bool_value);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    else
        cout << "LFTestImage_Enable: " << bool_value << endl;

    // Set the corresponding Height value
    // First read back HeightInterface value
    pfResult = pfCamera.GetFeatureInt("HeightInterface", int_value);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    else 
    {
        cout << "HeightInterface: " << (uint16_t)int_value << endl;
        pfResult = pfCamera.SetFeatureInt("Height", int_value);
        if (pfResult != PFSDK_NOERROR)
            cout << "Error: " << pfResult.GetDescription() << endl;
        // Read back Height value
        pfResult = pfCamera.GetFeatureInt("Height", int_value);
        if (pfResult != PFSDK_NOERROR)
            cout << "Error: " << pfResult.GetDescription() << endl;
        else
            cout << "Height: " << (uint16_t)int_value << endl;
    }
    
    // Set the Peak Selection criterium
    // Selects by which criterium the peaks are selected: FirstPeaks or HighestPeaks.
    pfResult = pfCamera.SetFeatureEnum("MPLF_PeakSel", "FirstPeaks");
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    // Read back the corresponding value
    pfResult = pfCamera.GetFeatureEnum("MPLF_PeakSel", enum_str);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    else
        cout << "MPLF_PeakSel: " << enum_str << endl;

    // Set Exposure Time value
    double_value = 1000.0;
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

    // Enable constant frame rate
    pfResult = pfCamera.SetFeatureBool("EnAcquisitionFrameRate", false);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    // Read back the corresponding value
    pfResult = pfCamera.GetFeatureBool("EnAcquisitionFrameRate", bool_value);
    if (pfResult != PFSDK_NOERROR)
        cout << "Error: " << pfResult.GetDescription() << endl;
    else
        cout << "EnAcquisitionFrameRate: " << bool_value << endl;

    if (bool_value) {
        // Set acquisition frame rate in fps
        double_value = 500.0;
        // Check if the value is inside the limits
        pfResult = pfCamera.GetFeatureParams("AcquisitionFrameRate", &pfFeatureParams);
        if (double_value > pfFeatureParams.FloatMax)
            double_value = pfFeatureParams.FloatMax;
        else if (double_value < pfFeatureParams.FloatMin)
            double_value = pfFeatureParams.FloatMin;
        // Set the corresponding value
        pfResult = pfCamera.SetFeatureFloat("AcquisitionFrameRate", double_value);
        if (pfResult != PFSDK_NOERROR)
            cout << "Error: " << pfResult.GetDescription() << endl;
        // Read back the corresponding value
        pfResult = pfCamera.GetFeatureFloat("AcquisitionFrameRate", double_value);
        if (pfResult != PFSDK_NOERROR)
            cout << "Error: " << pfResult.GetDescription() << endl;
        else
            cout << "AcquisitionFrameRate: " << double_value << endl;
    }

    return 0;
}