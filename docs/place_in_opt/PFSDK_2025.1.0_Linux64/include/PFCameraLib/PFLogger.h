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

// PFLogger.h
#ifndef PFLOGGER_H
#define PFLOGGER_H

#include <string>

#ifndef WIN32
#define sprintf_s(buf, ...) snprintf((buf), sizeof(buf), __VA_ARGS__)
#endif // WIN32

#include "PFCameraLib.h" 

namespace pfcameralib
{
	/// 
	/// @namespace PFLogger
	/// @brief Logger declarations
	/// 
	namespace PFLogger
	{
		/// Log file name for PFBASE
		const std::string PFBASE_LOG = "pfBase";
		/// Log file name for PFViewer
		const std::string PFVIEWER_LOG = "pfViewer";

		///
		/// @brief Enable logging and create logging file
		/// @param[in] name Name for Log file
		/// @param[in] filename	Filename 	
		/// 
		void PFCAMERALIB_API CreateLogger(const std::string& name="", const std::string& filename = "");
				
		///
		/// @brief Close log file
		/// @param[in] name Name for Log file
		/// 
		void PFCAMERALIB_API CloseLogger(const std::string& name);
		
		///
		/// @brief Print Trace message in log file
		/// @param[in] name Name for Log file
		/// @param[in] msg Message to print
		/// 
		void PFCAMERALIB_API Trace(const std::string& name, const std::string &msg);

		///
		/// @brief Print Debug message in log file
		/// @param[in] name Name for Log file
		/// @param[in] msg Message to print
		/// 
		void PFCAMERALIB_API Debug(const std::string& name, const std::string &msg);

		///
		/// @brief Check if logging is enabled
		/// 
		bool PFCAMERALIB_API IsLogEnabled();
		

		///
		/// @brief Print Debug with variadic parameters
		/// @param[in] name Name for Log file
		/// @param[in] format Message format
		/// @param[in] value Value to output
		/// @param[in] args Variadic parameters
		/// 
		template<typename T, typename ... Args>
		inline void PFCAMERALIB_API DebugWithParams(const std::string& name, const char* format, T value, Args ... args) {   // (4)
			if (!IsLogEnabled())
				return;
			
			char msg[1024];

			sprintf_s(msg, format, value, args...);
			Debug(name, std::string(msg));
		}
				
		///
		/// @brief Print Info message in log file
		/// @param[in] name Name for Log file
		/// @param[in] msg Message to print
		/// 
		void PFCAMERALIB_API Info(const std::string& name, const std::string &msg);

		///
		/// @brief Print Warn message in log file
		/// @param[in] name Name for Log file
		/// @param[in] msg Message to print
		/// 
		void PFCAMERALIB_API Warn(const std::string& name, const std::string &msg);

		///
		/// @brief Print Error message in log file
		/// @param[in] name Name for Log file
		/// @param[in] msg Message to print
		/// 
		void PFCAMERALIB_API Error(const std::string& name, const std::string &msg);

		///
		/// @brief Print Critical message in log file
		/// @param[in] name Name for Log file
		/// @param[in] msg Message to print
		/// 
		void PFCAMERALIB_API Critical(const std::string& name, const std::string &msg);
	}
}

#endif