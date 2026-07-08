#ifndef PFBUFFER_READER_3D_H_
#define PFBUFFER_READER_3D_H_

///
/// @file PFBufferReader3D.h
/// @brief This class extracts the 3D information from a raw image buffer coming from a Photonfocus 3D camera.


#include "PF3DLib.h"
#include <PFPixelCollection.h>

#include <cstdint>
#include <string>

///
/// @namespace pf3DLib
/// Contains extractors and Data containers for 3D data.

namespace pf3DLib
{
    using std::uint32_t;

    /// 
    /// @class PFBufferReader3D class. 
    ///
    /// This class extracts the 3D information from a raw image buffer coming from a Photonfocus 3D camera.
    /// Depending on the camera configuration, the information contained in 
    /// the frames is different:
    /// - In "Mode_2Donly" laser detection is turned off and camera behaves as a 
    /// normal area scan camera.
    ///
    /// - In "Mode2Dand3D" laser line detection is turned on. The sensor image (2D image) 
    /// is transmitted together with the 3D data. In this case, this class extracts 
    /// a 3D profile from each frame (or more than one is the camera allow multiple peak detection). 
    /// The 3D data contains also information about the laser width and quality. 
    ///
    /// - In "Mode_3Donly" laser line detection is turned on and only 3D data is transmitted. 
    /// The scan rate of this mode is considerably faster than the 2D&3D mode. 
    /// In "3Donly" mode the cameras allow frame combine operation, thus sending several profiles 
    /// together in a single frame. In that case, the PFBufferReader3D is able to extract each profile
    /// information separately.
    ///  
    /// In order to work properly, PFBufferReader3D needs to know the current camera configuration. 
    /// To do so, PFBufferReader3D must be configured with a proper configuration string, setting values to certain tags.
    /// Here you may find a couple of examples:
    /// "3D06#Width=1280;LineFinder_Mode=2;LineFinder_DataFormat3D=2;FrameCombine_NrOfFrames=10;"
    /// "3D07#Width=1280;LineFinder_Mode=1;LineFinder_NumberOfPeaks=4;LineFinder_PeakHeight_Enable=false;LineFinder_PeakWidth_Enable=true;FrameCombine_NrOfFrames=1;"
    ///
    /// In case of using the PFCameraLib in your project, the method PFCameraConfig::GetConfigString() 
    /// can be used to build the above mentioned configuration strings.
    /// By passing an already connected PFCamera instance and the PFCameraConfig::BUFFERREADER_3D parameter, 
    /// it will retrieve the current camera configuration and build the configuration string.
    ///
    class PF3DLIB_API PFBufferReader3D
    {
    public:
        /// 
        /// @brief Public constructor
        ///
        /// Initializes a new PFBufferReader3D
        ///
        /// @param[in] configString   /// String containing the current laser line detection and camera configuration 
        PFBufferReader3D(const char* configString);
		
		/// 
        /// @brief Destructor
        ///
        ~PFBufferReader3D();
		
        /// 
        /// @brief Notify the PFBufferReader3D that there is a new data buffer to be processed.
        ///
        ///
        /// @param[in] dataBuffer   /// raw data buffer coming from the camera 
        /// @param[in] frameWidth   /// camera frame width 
        /// @param[in] frameHeight  /// camera frame height 
        void notify(uint8_t * dataBuffer, uint32_t frameWidth, uint32_t frameHeight);
         
        /// 
        /// @brief  Gets the number of available frames in the processed buffer.
        ///
        /// In "3Donly" mode the cameras allow frame combine operation, thus sending several profiles together in a single frame.
        /// When the FrameCombine is finished by a timeout, then the remaining data in the combined frame will be filled with filler data.
        /// This method will return the number of valid frames that have been received, ignoring the filler data.
        ///
        /// @return The number of available frames in the processed buffer.       
        size_t getAvailable3DFrames();

        /// 
        /// @brief  Gets the number of laser lines to be detected by the camera
        ///
        /// @return The number of laser peaks to be detected.       
        size_t getAvailablePeaks(uint8_t algorithmNumber = 0);

        /// 
        /// @brief  Gets the laser line profile of the selected frame and peak.
        ///
        /// @return A float array containing the laser peak positions detected in every sensor column.
        bool getProfile(float *profile, size_t frameNumber = 0, size_t peakNumber = 0) const;

        /// 
        /// @brief  Gets the laser line qualifier (with or height) of the selected frame and peak.
        ///
        /// @return A float array containing the qualifier values, such as laser width (scattering) or height (intensity).
        bool getQualifier(uint8_t qualifierNumber, float *qualifier, size_t frameNumber = 0, size_t peakNumber = 0) const;

        /// 
        /// @brief  Gets a single line scan for every frame.
        ///
        /// @return An unsinged char array containing the grey values of the selected sensor line.
        bool getLinescan(uint8_t *linescan, size_t frameNumber = 0, size_t peakNumber = 0) const;

        bool getSLA(PFPixelCollection &pxCollection , size_t frameNumber = 0) const;

    private:
        class PFBufferReader3DImpl;

        PFBufferReader3DImpl* m_pfBufferReader3DImpl;
    };
}
#endif
