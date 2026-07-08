#
##############################################################################
# @attention
#
#<h2><center>&copy; COPYRIGHT(c) 2021 Photonfocus AG</center></h2>
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
# 1. Redistributions of source code must retain the above copyright notice,
# this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation
# and/or other materials provided with the distribution.
# 3. Neither the name of Photonfocus nor the names of its contributors
# may be used to endorse or promote products derived from this software
# without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
##############################################################################
#
##Auto generated code snippet##
#Add PFSDK bin folder to Python path so the bindings can be found
import os, sys
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '../../..', 'lib')))


##End of auto generated code##

import os, sys
from kbhit import KBHit
import colorama

import PFPyCameraLib as pf

import numpy as np
import cv2 as cv
from matplotlib import pyplot as plt

def PromptEnterAndExit(code = 0):
    input("\n\n\nPress enter to exit ...")
    colorama.deinit()
    sys.exit(code)

def ExitWithErrorPrompt(errString, pfResult = None):
    print(errString)
    if pfResult is not None:
        print(pfResult)
    PromptEnterAndExit(-1)

def EventErrorCallback(cameraNumber, errorCode, errorMessage):
    print("[Communication error callback] Camera(",cameraNumber,") Error(", errorCode, ", ", errorMessage, ")\n")

def DiscoverAndSelectCamera():
    #Discover cameras in the network or connected to the USB port
    discovery = pf.PFDiscovery();
    pfResult = discovery.DiscoverCameras()

    if pfResult != pf.Error.NONE:
        ExitWithErrorPrompt("Discovery error:", pfResult)

    #Print all available cameras
    num_discovered_cameras = discovery.GetCameraCount()
    camera_info_list = []
    for x in range(num_discovered_cameras):
        [pfResult, camera_info] = discovery.GetCameraInfo(x)
        camera_info_list.append(camera_info) 
        print("[",x,"]")
        print(camera_info_list[x])

    #Prompt user to select a camera
    user_input = input("Select camera: ")
    try:
        cam_id = int(user_input)
    except:
        ExitWithErrorPrompt("Error parsing input, not a number")

    #Check selected camera is within range
    if not 0 <= cam_id < num_discovered_cameras:
        ExitWithErrorPrompt("Selected camera out of range")

    selected_cam_info = camera_info_list[cam_id]
    #Call copy constructor
    #The camera info list elements are destroyed with PFDiscover
    if selected_cam_info.GetType() == pf.CameraType.CAMTYPE_GEV:
        copy_cam_info = pf.PFCameraInfoGEV(selected_cam_info)
    else:
        copy_cam_info = pf.PFCameraInfoU3V(selected_cam_info)

    return copy_cam_info


def ConnectAndConfigureCamera(pfCameraInfo):
    #Connect camera
    pfCam = pf.PFCamera()

    pfResult = pfCam.Connect(pfCameraInfo)
    #pfResult = pfCam.Connect(ip = "192.168.3.158")
    if pfResult != pf.Error.NONE:
        ExitWithErrorPrompt("Could not connect to the selected camera", pfResult)

    #Check camera has a color sensor
    if not pfCam.IsColorCamera():
        ExitWithErrorPrompt("This is a color camera sample but the selected camera does not have a color sensor")

    #Get enum parameters, we need the element count 
    pfResult, pfFeatureParam = pfCam.GetFeatureParams("PixelFormat")
    if pfResult != pf.Error.NONE:
        ExitWithErrorPrompt("Could not get PixelFormat parameters", pfResult)

    pixelFormatList = []
    #Iterate the enumeration elements
    for i in range(pfFeatureParam.EnumerationCount):
        [_, enumName] = pfCam.GetFeatureEnumName("PixelFormat", i)
        print(i, ":", enumName)
        pixelFormatList.append(enumName)

    #Try to set a pixel format which uses bayer pattern first, otherwise RGB
    bFormatSet = False
    colorFormatPattern = ["Bayer", "RGB"]
    for pattern in colorFormatPattern:
        for format in pixelFormatList:
            if(pattern in format):
                pfResult = pfCam.SetFeatureEnum("PixelFormat", format)
                if pfResult != pf.Error.NONE:
                    ExitWithErrorPrompt("Could not set PixelFormat", pfResult)
                bFormatSet = True
                break
        if bFormatSet:
            break

    #This sample assumes DR camera
    [pfResult, featureList] = pfCam.GetFeatureList()
    if pfResult != pf.Error.NONE:
        ExitWithErrorPrompt("Could not get feature list from camera", pfResult)

    #Check DoubleRate_Enable feature is present
    if any(elem.Name == "DoubleRate_Enable" for elem in featureList):
        print("DoubleRate_Enable feature found. Enabling feature.")
        pfResult = pfCam.SetFeatureBool("DoubleRate_Enable", True)
        if pfResult != pf.Error.NONE:
            ExitWithErrorPrompt("Failed to set DoubleRate_Enable", pfResult)
    else:
            ExitWithErrorPrompt("To run this sample select a DR camera", pfResult)

    #Set Width to maximum
    pfResult, pfFeatureParam = pfCam.GetFeatureParams("Width")
    if pfResult != pf.Error.NONE:
        ExitWithErrorPrompt("Could not get width feature parameters", pfResult)

    pfResult = pfCam.SetFeatureInt("Width", pfFeatureParam.Max)
    if pfResult != pf.Error.NONE:
        ExitWithErrorPrompt("Error setting width", pfResult)

    #Set Height to maximum
    pfResult, pfFeatureParam = pfCam.GetFeatureParams("Height")
    if pfResult != pf.Error.NONE:
        ExitWithErrorPrompt("Could not get Height feature parameters", pfResult)

    pfResult = pfCam.SetFeatureInt("Height", pfFeatureParam.Max)
    if pfResult != pf.Error.NONE:
        ExitWithErrorPrompt("Error setting Height", pfResult)

    return pfCam

def SetupStream(pfCam, pfCamInfo):
    #Create stream depending on camera type
    if pfCamInfo.GetType() == pf.CameraType.CAMTYPE_GEV:
        pfStream = pf.PFStreamGEV(False, True, True, True)
    else:
        pfStream = pf.PFStreamU3V()
    #Set ring buffer size to 100
    pfStream.SetBufferCount(100)

    pfResult = pfCam.AddStream(pfStream)
    if pfResult != pf.Error.NONE:
        ExitWithErrorPrompt("Error setting stream", pfResult)

    return pfStream

def InitializeHistogramPlot(bins):
    # Initialize plot.
    fig, ax = plt.subplots()

    ax.set_title('Histogram (RGB)')
    ax.set_xlabel('Bin')
    ax.set_ylabel('Frequency')

    # Initialize plot line object(s). Turn on interactive plotting and show plot.
    lw = 3
    alpha = 0.5

    lineR, = ax.plot(np.arange(bins), np.zeros((bins,)), c='r', lw=lw, alpha=alpha)
    lineG, = ax.plot(np.arange(bins), np.zeros((bins,)), c='g', lw=lw, alpha=alpha)
    lineB, = ax.plot(np.arange(bins), np.zeros((bins,)), c='b', lw=lw, alpha=alpha)

    ax.set_xlim(0, bins-1)
    ax.set_ylim(0, 1)
    plt.ion()
    plt.show(block=False)

    return [fig, lineR, lineG, lineB]

#https://www.pyimagesearch.com/2015/10/05/opencv-gamma-correction/
def adjust_gamma(image, gamma=1.0):
	# build a lookup table mapping the pixel values [0, 255] to
	# their adjusted gamma values
	invGamma = 1.0 / gamma
	table = np.array([((i / 255.0) ** invGamma) * 255
		for i in np.arange(0, 256)]).astype("uint8")

	# apply gamma correction using the lookup table
	return cv.LUT(image, table)


def DisplayFromStream(pfCam, pfStream):
    pfBuffer = 0
    pfImage = pf.PFImage()
    pfImageDR = pf.PFImage()
    pfImageColor = pf.PFImage()

    #Compute final image size (depending on DoubleRate being enabled)
    [_, width] = pfCam.GetFeatureInt("Window_W")
    [_, height] = pfCam.GetFeatureInt("Height")

    print("Image W:%d H:%d" % (width, height))

    #GetPixel type
    [pfResult, enumVal] = pfCam.GetFeatureEnum("PixelFormat")
    if pfResult != pf.Error.NONE:
        ExitWithErrorPrompt("Error querying pixel format: ", pfResult)
    #Check if its bayer
    bBayer =  "Bayer" in enumVal

    print("Pixel type: ", enumVal)

    #Allocate image for demodulation
    pfResult = pfImageDR.ReserveImage(pf.GetPixelType(enumVal), width, height)
    if pfResult != pf.Error.NONE:
        ExitWithErrorPrompt("Error allocating image: ", pfResult)

    #Allocate image for debayering
    pfResult = pfImageColor.ReserveImage(pf.PixelRGB8, width, height)
    if pfResult != pf.Error.NONE:
        ExitWithErrorPrompt("Error allocating image: ", pfResult)

    #Compute display window size
    aspectRatio = float(height) / float(width)
    displaySize = (600, int(600 * aspectRatio))

    imageSize = height * width
    bins = 64
    [fig, lineR, lineG, lineB] = InitializeHistogramPlot(bins)

    print("\n\n\nPress any key to stop capturing")
    print("\033[1A\033[1A\033[1A", end="")
	
    kb = KBHit()
    #Loop over stream frames
    while not kb.kbhit():
        #Fetch buffer
        [pfResult, pfBuffer] = pfStream.GetNextBuffer()
        if pfResult == pf.Error.NONE:
            #Get image object from buffer
            pfBuffer.GetImage(pfImage)
    
            #Demodulate double rate image
            pfResult = pfImage.DemodulateDR(pfImageDR, pfCam.IsColorCamera())
            if pfResult != pf.Error.NONE:
                ExitWithErrorPrompt("Error demodulating image: ", pfResult)

            #Debayer image
            if bBayer:
                pfImageDR.Debayer(pfImageColor, pf.BayerFilterBilinear)
                if pfResult != pf.Error.NONE:
                    ExitWithErrorPrompt("Error debayering image: ", pfResult)
            else:
                pfImageColor = pfImageDR

            #Do something with image data
            imageData = np.array(pfImageColor, copy = False)
            #Convert RGB to BGR
            imageData = cv.cvtColor(imageData, cv.COLOR_RGB2BGR)

            resizedImage = cv.resize(imageData, displaySize)
            cv.imshow("Captured frame", resizedImage)
            cv.waitKey(1)

            gammaAdjustedImage = adjust_gamma(resizedImage, 2.0)

            cv.imshow("Gamma adjust", gammaAdjustedImage)

            #Generate histogram
            #(b, g, r) = cv.split(imageData)
            histogramR = cv.calcHist([imageData], [0], None, [bins], [0, 255])/ imageSize
            histogramG = cv.calcHist([imageData], [1], None, [bins], [0, 255])/ imageSize
            histogramB = cv.calcHist([imageData], [2], None, [bins], [0, 255])/ imageSize
            lineR.set_ydata(histogramR)
            lineG.set_ydata(histogramG)
            lineB.set_ydata(histogramB)
        
            plt.pause(0.0001)

        #Release frame buffer, otherwise ring buffer will get full
        pfStream.ReleaseBuffer(pfBuffer)

        #Display stream statistics
        pfStreamStats = pfStream.GetStreamStatistics()
        print(pfStreamStats, end="\r")

def GrabAndDisplay(pfCam, pfStream):
    #Start grabbing images into stream
    pfResult = pfCam.Grab()
    if pfResult != pf.Error.NONE:
        ExitWithErrorPrompt("Could not start grab process", pfResult)

    #Get images from stream and display
    DisplayFromStream(pfCam, pfStream)

    #Stop frame grabbing
    pfResult = pfCam.Freeze()
    if pfResult != pf.Error.NONE:
        ExitWithErrorPrompt("Error stopping grab process", pfResult)

def DisconnectCamera(pfCam):
    #Disconnect camera
    pfResult = pfCam.Disconnect()
    if pfResult != pf.Error.NONE:
        ExitWithErrorPrompt("Error disconnecting", pfResult)

if __name__ == "__main__":
    colorama.init()

    pf.SetEventErrorReceiver(EventErrorCallback)

    pfCamInfo = DiscoverAndSelectCamera()
    pfCam = ConnectAndConfigureCamera(pfCamInfo)
    pfStream = SetupStream(pfCam, pfCamInfo)
    GrabAndDisplay(pfCam, pfStream)
    DisconnectCamera(pfCam)
    PromptEnterAndExit()
