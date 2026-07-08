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

///
/// @file PFProfileMap.h
/// @brief Class containing a profile continer for 3D points.

#pragma once

#include "PF3DLib.h"
#include <stddef.h>

///
/// @namespace pf3DLib
/// Contains extractors and Data containers for 3D data.

namespace pf3DLib
{
    /// 
    /// @class PFProfileMap class. 
    ///
    /// A ProfileMap is a container for profiles, each of which containing characteristics of the projected laser line in a particular frame.
    /// A ProfileMap could be used to store a range map (each profile contains the laser peak positions) or a width map or any other qualifier of the laser line.
    ///
    class PF3DLIB_API PFProfileMap
    {
    public:
        /// 
        /// @brief Creates a new empty range map. 
        ///
        /// Both profilesLength and maxProfiles define the size of the ProfileMap. 
        /// profileLength is usually given by the width of the sensor's region of interest. maxProfiles defines the maximum number of Profiles that the ProfileMap is able to store.
        ///
        /// @param[in] profileLength    Lenght of the profiles contained in the ProfileMap.
        /// @param[in] maxProfiles      Maximum number of profiles that the ProfileMap is able to store.
        ///
        PFProfileMap(size_t profileLength, size_t maxProfiles);

        ///Copy contructor
        PFProfileMap(const PFProfileMap &profilemap);

        ///Default Contructor
        PFProfileMap();



        /// 
        /// @brief Public destructor
        ///
        ~PFProfileMap();
        
        /// 
        /// @brief  Gets the number of profiles the range map has.  
        ///
        /// When an empty range map is created, this value is set to zero. Each time the getNewProfile() method is called, this counter is increased. 
        ///
        /// @return The number of profiles the range map has.       
        size_t  getProfilesCount() const;
        
        /// 
        /// @brief  Gets the next empty map's profile. 
        ///
        /// This method is used to get successive profiles from an empty ProfileMap, to fill them in. 
        /// Each time it is called, the first empty profile is returned and the counter of profiles is increased, up to maxProfiles.
        ///
        /// @return The first empty profile in the profile map.
        float* getNewProfile();

        /// 
        /// @brief  Gets the Profile at position index.  
        ///
        /// @param[in] profileIndex The index of the profile to get. The allowed range is [0..getProfilesCount()-1]
        ///
        float* getProfile(size_t profileIndex) const;
        
        /// 
        /// @brief  Gets the length of the profiles in the ProfileMap.   
        ///
        /// @return The length of the profiles.  
        size_t getProfileLength () const;
        
        /// 
        /// @brief  Gets the maximum number of profiles the profile map can have.  
        ///
        /// When creating an empty profile map, this function returns the parameter that was passed to the constructor.
        ///
        /// @return The maximum number of profiles the profile map can have. 
        size_t getMaxProfiles () const;
        
        /// 
        /// @brief  Resets the profile count to 0
        ///
        /// Used to reuse the object to capture a new profile map
        /// Empties used profiles
        ///
        /// @return The maximum number of profiles the profile map can have. 
        void reset();

        /// 
        /// @brief  Indicates whether all available profiles have been used
        ///
        /// @return A boolean value. True when full, false otherwise.
        bool isFull() const;

        /// 
        /// @brief  Saves the profile map as a PLY file in ASCII format.
        ///
        /// PLY is a computer file format known as the Polygon File Format or the Stanford Triangle Format. 
        /// In the ASCII version of the format, the vertices and faces are each described one to a line with the numbers separated by white space.
        ///
        /// @return A boolean value. False if the file cannot be accessed, true otherwise.
        bool saveAsPLYFile(const char* fileName);


    private:
        class   PFProfileMapImpl;

        PFProfileMapImpl *m_pProfileMapImpl;
        
    };
}
