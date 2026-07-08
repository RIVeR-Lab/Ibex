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
import PFPy3DLib as pf3d

def PromptEnterAndExit(code = 0):
    input("\n\n\nPress enter to exit ...")
    colorama.deinit()
    os._exit(code)

def ExitWithErrorPrompt(errString, pfResult = None):
    print(errString)
    if pfResult is not None:
        print(pfResult)
    PromptEnterAndExit(-1)

def EventErrorCallback(cameraNumber, errorCode, errorMessage):
    print("[Communication error callback] Camera(",cameraNumber,") Error(", errorCode, ", ", errorMessage, ")\n")

def DiscoverAndSelectCamera():
    #Discover cameras in the network or connected to the USB port
    discovery = pf.PFDiscovery()
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


def ConnectCamera(pfCameraInfo):
    if pfCamInfo.GetModelName().find("3D07") == -1:
        ExitWithErrorPrompt("Unsupported camera, connect to 3D07 model.")

    #Connect camera
    pfCam = pf.PFCamera()
    pfResult = pfCam.Connect(pfCameraInfo)
    #pfResult = pfCam.Connect(ip = "192.168.3.158")
    if pfResult != pf.Error.NONE:
        ExitWithErrorPrompt("Could not connect to the selected camera", pfResult)

    return pfCam

def ConfigureCamera(pfCam):
    #Get feature list
    #[pfResult, featureList] = pfCam.GetFeatureList()
    #if pfResult != pf.Error.NONE:
    #    ExitWithErrorPrompt("Could not get feature list from camera", pfResult)

    #Set Width to maximum
    pfResult, pfFeatureParam = pfCam.GetFeatureParams("Width")
    if pfResult != pf.Error.NONE:
        ExitWithErrorPrompt("Could not get width feature parameters", pfResult)

    pfResult = pfCam.SetFeatureInt("Width", pfFeatureParam.Max)
    if pfResult != pf.Error.NONE:
        ExitWithErrorPrompt("Error setting width", pfResult)

    pfResult = pfCam.SetFeatureEnum("LineFinder_Mode", "Mode_2Dand3D")
    if pfResult != pf.Error.NONE:
        ExitWithErrorPrompt("Error setting LineFinder_Mode", pfResult)

    pfResult = pfCam.SetFeatureBool("LineFinder_PeakHeight_Enable", True)
    if pfResult != pf.Error.NONE:
        ExitWithErrorPrompt("Error setting LineFinder_PeakHeight_Enable", pfResult)

    pfResult = pfCam.SetFeatureBool("LineFinder_PeakWidth_Enable", True)
    if pfResult != pf.Error.NONE:
        ExitWithErrorPrompt("Error setting LineFinder_PeakWidth_Enable", pfResult)

    pfResult = pfCam.SetFeatureBool("LineFinder_StatusLine_Enable", False)
    if pfResult != pf.Error.NONE:
        ExitWithErrorPrompt("Error setting LineFinder_StatusLine_Enable", pfResult)

def SetupStream(pfCam, pfCamInfo):
    #Create stream depending on camera type
    if pfCamInfo.GetType() == pf.CameraType.CAMTYPE_GEV:
        pfStream = pf.PFStreamGEV(True, True, False, True)
    else:
        pfStream = pf.PFStreamU3V()
    #Set ring buffer size to 100
    pfStream.SetBufferCount(1000)

    pfResult = pfCam.AddStream(pfStream)
    if pfResult != pf.Error.NONE:
        ExitWithErrorPrompt("Error setting stream", pfResult)

    return pfStream

def Grab(pfCam, pfStream):

    #Prompt user to select a camera
    user_input = input("Number of profiles in range map: ")
    try:
        num_profiles = int(user_input)
    except:
        ExitWithErrorPrompt("Error parsing input, not a number")

    [pfResult, width] = pfCam.GetFeatureInt("Width")
    if pfResult != pf.Error.NONE:
        ExitWithErrorPrompt("Error getting width", pfResult)

    #Start grabbing images into stream
    pfResult = pfCam.Grab()
    if pfResult != pf.Error.NONE:
        ExitWithErrorPrompt("Could not start grab process", pfResult)

    #Get configuration string for a 3D buffer reader
    [pfResult, configString] = pf.PFCameraConfig.GetConfigString(pfCam, pf.PFCameraConfig.BUFFERREADER_3D)
    if pfResult != pf.Error.NONE:
        ExitWithErrorPrompt("Error configuring buffer reader", pfResult)
    else:
        print("Configuration string: ", configString)

    kb = KBHit()
    pfBuffer = 0
    pfBufferReader3D = pf3d.PFBufferReader3D(configString)
    pfRangeMap = pf3d.PFProfileMap(width, num_profiles)

    #Loop over stream frames
    while not kb.kbhit():
        while not pfRangeMap.isFull():
            #Fetch buffer
            [pfResult, pfBuffer] = pfStream.GetNextBuffer()
            if pfResult == pf.Error.NONE:
                pfBufferReader3D.notify(pfBuffer, pfBuffer.GetSizeX(), pfBuffer.GetSizeY())

                for frameIndex in range(pfBufferReader3D.getAvailable3DFrames()):

                    profile = pfRangeMap.getNewProfile()
                    pfBufferReader3D.getProfile(profile, frameIndex, 0)

            #Release frame buffer, otherwise ring buffer will get full
            pfStream.ReleaseBuffer(pfBuffer)

        #Display stream statistics
        pfStreamStats = pfStream.GetStreamStatistics()
        print(pfStreamStats)
        print("Press any key to stop capturing", end="\r")
        print("\033[1A", end="")

        pfRangeMap.saveAsPLYFile("python_save_rangemap.ply")
        pfRangeMap.reset()

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

    pfCam = ConnectCamera(pfCamInfo)

    ConfigureCamera(pfCam)

    pfStream = SetupStream(pfCam, pfCamInfo)

    Grab(pfCam, pfStream)

    DisconnectCamera(pfCam)

    PromptEnterAndExit()