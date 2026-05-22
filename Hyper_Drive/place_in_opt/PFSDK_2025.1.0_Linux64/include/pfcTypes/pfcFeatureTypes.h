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
/// @file pfcFeatureTypes.h
/// @brief Definition of different data structs related to the camera features.
///
/// Further Description.
///
/// @note
/// @author TAH 
/// @version 0.1 
/// @date 2016/11/09
///
#ifndef PFCFEATURETYPES_H
#define PFCFEATURETYPES_H

#include <stdint.h>
#ifndef WIN32
#define WINAPI
#else
#include <Windows.h>
#endif

#define FEATNAMELENGTH	128			/// Maximum Feature name length
#define FEATVALLENGTH	256 ///

// Type of features nodes
#define NODETYPE_CATEGORY     0		/// Node is a Category, or Subcategory
#define NODETYPE_FEATURE      1		/// Node is a Feature
#define NODETYPE_INTEGER      2		/// Node is a Integer
#define NODETYPE_FLOAT        3		/// Node is a Float
#define NODETYPE_STRING       4		/// Node is a String
#define NODETYPE_ENUMERATION  5		/// Node is an Enumeration (list of values)
#define NODETYPE_COMMAND      6		/// Node is a Command
#define NODETYPE_BOOLEAN      7		/// Node is a Boolean (True or False)
#define NODETYPE_REGISTER     8		/// Node is a Register
#define NODETYPE_PORT         9		/// Node is a Port

#define REPRESENTATION_LINEAR       0		/// Slider with linear behaviour
#define REPRESENTATION_LOGARITHMIC  1		/// Slider with logarithmic behaviour
#define REPRESENTATION_BOOLEAN      2		/// Checkbox
#define REPRESENTATION_PURE_NUMBER  3	    /// Decimal number in an edit control
#define REPRESENTATION_HEX_NUMBER   4	    /// Hex number in an edit control
#define REPRESENTATION_IP_ADDRESS  5		/// IP Address

#define ACCESS_MODE_RO    0x524F	/// Feature Access mode ReadOnly
#define ACCESS_MODE_RW    0x5257	/// Feature Access mode Read and Write
#define ACCESS_MODE_WO    0x574F	/// Feature Access mode WriteOnly 

#define VISIBILITY_INVISIBLE 0		/// Feature is invisible
#define VISIBILITY_BEGINNER  1		/// Feature is visible in Beginner level
#define VISIBILITY_EXPERT    2		/// Feature is visible in Expert level
#define VISIBILITY_GURU      3		/// Feature is visible in Guru level

#define ACCESS_OK 0						/// Access Status Camera
#define ACCESS_ALREADY_OPENED 1			/// Access Status Already Opened
#define ACCESS_NOT_SAME_SUBNET 2		/// Access Status Not Same Subnet
#define ACCESS_CONTROL_OPENED 3			/// Access Status Control Opened
#define ACCESS_UNKNOWN 4				/// Access Status Unknown

#define ACQ_MODE_CONTINUOUS  0				/// Continuous Mode 
#define ACQ_MODE_SINGLEFRAME 1				/// Single Frame 
#define ACQ_MODE_MULTIFRAME 2				/// Multi Frame 
#define ACQ_MODE_CONT_RECORDING 3			/// Continuous Recording
#define ACQ_MODE_CONT_READOUT 4				/// Continuous Readout
#define ACQ_MODE_SINGLEFRAME_RECORDING 5	/// Single Frame Recording
#define ACQ_MODE_SINGLEFRAME_READOUT 6		/// Single Frame Readout

#define TSC_RESET 0x0001
#define TSC_LATCH 0x0002

///
///	@struct PFFeatureItemInfo
///
/// @brief Information of feature
///
typedef struct pf_feature_item_info_t{
	char Name[FEATNAMELENGTH];	///< Feature name
	uint8_t Type;					///< Feature type, values within NODETYPE_ values (see declarations above)
	uint8_t Level;					///< Feature level
	uint8_t Visibility;			///< Feature visibility
}PFFeatureItemInfo;

///
///	@struct PFFeatureParameters
///
/// @brief Feature parameters
///
typedef struct pf_feature_parameters_t
{
	uint8_t Type;					///< Feature type
	int64_t Min;					///< Feature min value
	int64_t Max;					///< Feature max value
	uint32_t OnValue;				///< Feature on value
	uint32_t OffValue;				///< Feature off value
	uint16_t AccessMode;			///< Feature access mode
	uint16_t Representation;		///< Feature representation
	uint32_t Inc;					///< Feature increment
	uint32_t CommandValue;			///< Feature command value
	uint32_t Length;				///< Feature length
	uint8_t EnumerationCount;		///< Feature enumeration counter
	uint8_t Visibility;			///< Feature visibility
	double FloatMin;			///< Feature float min value
	double FloatMax;			///< Feature float max value
	double FloatInc;			///< Feature float increment
	uint8_t IsImplemented;			///< Feature implemented {1, 0}
	uint8_t IsAvailable;			///< Feature availability {1, 0}
	uint8_t IsLocked;				///< Feature lock status {1, 0}
	uint8_t Sign;					///< Feature sign
	uint64_t Address;				///< Feature address
	uint8_t DisplayNotation;		///< Feature display notation
	uint8_t DisplayPrecision;		///< Feature display precision
	uint8_t InvalidatorCount;		///< Feature invalidators counter
	int64_t PollingTime;			///< Feature Polling time
} PFFeatureParameters;

///
///	@union PFFeatureValueUnion
///
/// @brief Union of possible feature value types
///
typedef union pf_feature_value_union_t
{
	int64_t				integer_value;  ///< Integer value
	double			float_value;    ///< Float value
	const char*		string_value;   ///< String value
	const char*		enum_value;     ///< Enum value
	uint32_t				bool_value;     ///< Boolean value
	uint32_t				command_value;  ///< Command value
} PFFeatureValueUnion;


///
///	@brief Callback function type "pf_feature_callback_fn"
///
/// @brief Callback function for feature changes
/// @param name Name of the feature
/// @param type Feature Type 
/// @param unionOf Union of Values
/// @param data Additional data (usually the pointer to the object that calls the function) 
///
typedef void(WINAPI *pf_feature_callback_t)(const char* name, uint8_t type, PFFeatureValueUnion unionOf, void* data);
typedef pf_feature_callback_t PFFeatureCallback;

#endif //PFCFEATURETYPES_H