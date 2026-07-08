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
/// @file PFResult.h 
/// @brief Class containing the status and error codes that will return as a result most methods of the Photonfocus SDK.
///

#ifndef PFRESULT_H
#define PFRESULT_H

#include "pfcErrors.h"
#include "PFCameraLib.h"
#include <cstdint>

namespace pfcameralib
{
    /// \brief Get error string and description. 
    ///
    /// \param[in] nError See \ref pfcErrors.h for associated error codes
    /// \param[in] pErrorStr Associated error string
    /// \param[in] pErrorDescription Associated description
    void PFCAMERALIB_API PFGetErrorString(uint32_t nError, char *pErrorStr, char *pErrorDescription);
                    
    /// 
    /// @class PFResult PFResult.h
    ///
    /// Class containing the status and error codes that different methods of the PF classes return as result. 
    /// @see pfcErrors.h header file for full list of error codes supported.
    ///
    class PFCAMERALIB_API PFResult
    {
    public:

        ///
        /// @brief Public constructor
        ///
        PFResult(); 
        
        ///
        /// @brief Public destructor
        ///
        ~PFResult();
        
        ///
        /// @brief Copy constructor
        ///
        PFResult(const PFResult &result);

        ///
        /// @brief Move constructor
        ///
        PFResult(PFResult &&result);
        
        ///
        /// @brief Assignment operator
        ///
        PFResult& operator= (const PFResult &result);

        ///
        /// @brief Move assignment operator
        ///
        PFResult& operator= (PFResult&& result);

        /// \brief Checks the equality of this \ref PFResult with another \ref PFResult
        ///
        /// \param[in] resultB The \ref PFResult to check equality
        ///
        /// \return Whether this \ref PFResult is equal to \p resultB or not
        bool operator == (const PFResult & resultB) const;
        bool operator == (const uint32_t resultCode) const;
        bool operator != (const PFResult & resultB) const;
        bool operator != (const uint32_t resultCode) const;

        ///
        /// @brief Returns the result code.
        ///
        /// This method returns the high level error code. You have to check this value to verify the result of the last operation. 
        ///  @see pfcErrors.h to verify error code.
        ///
        /// @return  m_ErrorCode
        /// 
        uint32_t GetResultCode() const;
        
        ///
        /// @brief Retrieves the string of the current error.
        ///
        /// This method returns the result code in the form of a string. 
        ///
        /// @return  m_pErrorString
        /// 
        const char* GetResultString() const;

        ///
        /// @brief Retrieves a description of the current error.
        ///
        /// This method returns the result description.
        ///
        /// @return  m_pDescription
        /// 
        const char* GetDescription() const;

        ///
        /// @brief Returns the internal result code.
        ///
        /// This method returns the internal code of the result.
        /// The purpose of this internal code is only to provide further information to Photonfocus developers.
        ///
        /// @return  m_ErrorCode
        /// 
        uint32_t GetInternalCode() const;
        
        // Required in public section to be used in ImageUtils
        // Constructor
        PFResult(uint32_t errorCode);

    private:
        friend class PFCamera;
        friend class PFCameraInternal;
        friend class PFCameraConfig;
        friend class PFDiscovery;
        friend class PFImage;
        friend class PFBuffer;
        friend class PFStream;
        friend class PFStreamGEV;
        friend class PFStreamU3V;
        friend class PFResultFactory;
        class PFResultImpl;
        PFResultImpl* m_pImpl;
        
    };
}

/// @namespace PFCameraDLL
/// Alias for pfcameralib legacy code
namespace PFCameraDLL = pfcameralib;

#endif //PFRESULT_H