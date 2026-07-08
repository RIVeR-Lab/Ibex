// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVGENPARAMETERARRAY_H__
#define __PVGENPARAMETERARRAY_H__

#include <PvGenICamLib.h>
#include <PvGenString.h>
#include <PvGenInteger.h>
#include <PvGenEnum.h>
#include <PvGenFloat.h>
#include <PvGenCommand.h>
#include <PvGenBoolean.h>
#include <PvGenRegister.h>
#include <PvGenCategory.h>


namespace PvGenICamLib
{
    class GenParameterArray;
    class GenParameterArrayManager;
}

namespace PvDeviceLib
{
    class Device;
}


class PvGenFile;
class PvConfigurationWriter;
class PvConfigurationReader;


///
/// \class PvGenParameterArray
/// \brief GenICam feature array
///
/// <h2 id="controlling">Controlling features (programmatically)</h2>
///
/// <b>To control features:</b>
///     - Get the list of features. Use one of the following:
///         - #PvDevice::GetParameters (Parameters used to control the GigE Vision device to which you are connected)
///         - #PvDevice::GetCommunicationParameters (Parameters used to define how PvDevice connects to and communicates with the device - see Device Communication Parameters (<a href="devicecommunicationGEV.html">GEV</a>/<a href="devicecommunicationU3V.html">U3V</a>)
///         - #PvStream::GetParameters (Parameters used to define how PvStream receives data from a GigE Vision transmitter - see Stream Parameters (<a href="streamparametersGEV.html">GEV</a>/<a href="streamparametersU3V.html">U3V</a>)
///     - Get a feature. Use #Get.
///     - Get the feature's type. Use #PvGenParameter::GetType.
///     - Optionally:
///         - Get/Set the feature's value. Use the GetValue/SetValue method for the feature's type (e.g. #PvGenInteger::GetValue, #PvGenFloat::SetValue, etc.)
///         - If the feature is a command, activate it. Use #PvGenCommand::Execute.
///
///
/// <h2 id="controllingGUI">Controlling features (using a GUI)</h2>
///
/// <b>To control features (using a GUI):</b>
///     - See GenICam browser dialog documentation.
///
/// <h2>Running code when a feature's value changes</h2>
///
/// In some cases, you may want to know when a feature value or attribute changes without using CPU-intensive polling
/// to regularly check (and re-check) the feature's value.  
///
/// <b>To create a callback that runs when the feature's value changes:</b>
///     - Get the feature. See <a href="#controlling">Controlling features</a>.
///     - Create a subclass of #PvGenEventSink.
///     - Call #PvGenParameter::RegisterEventSink to be notified when that parameter is updated.
///     - In your new class, override #PvGenEventSink::OnParameterUpdate.
///
class PvGenParameterArray
{
public:

    ///
    /// \brief Constructor.
    ///
	PV_GENICAM_API PvGenParameterArray();

    ///
    /// \brief Destructor.
    ///
	PV_GENICAM_API virtual ~PvGenParameterArray();

    ///
    /// \brief Get the number of parameters (#PvGenParameter objects) in the array.
    ///
    ///
    /// \return The number of parameters (#PvGenParameter objects) in the array.
    ///
	PV_GENICAM_API uint32_t GetCount() const;

    ///
    /// \brief Get a parameter (#PvGenParameter object) from the array (using array position).
    ///
    /// \param[in] aIndex  The parameter's position in the array. The valid range for <i>aIndex</i>
    ///                     is:  0 =< <i>aIndex</i> < #GetCount.
    ///
    /// \return The parameter (#PvGenParameter object), or NULL if the index is out of bounds.
    ///
	PV_GENICAM_API PvGenParameter *Get( uint32_t aIndex );

    ///
    /// \brief Get a parameter (#PvGenParameter object) from the array (using parameter name).
    ///
    /// \param[in] aName  The parameter's name. See #PvGenParameter::GetName.
    ///
    /// \return The parameter (#PvGenParameter object), or NULL if the name does not match any paramaters in the array.
    ///
	PV_GENICAM_API PvGenParameter *Get( const PvString &aName );

    ///
    /// \brief Get a parameter (#PvGenParameter object) from the array (using array position).
    ///
    /// \param[in] aIndex  The parameter's position in the array. The valid range for <i>aIndex</i>
    ///                     is:  0 =< <i>aIndex</i> < #GetCount.
    ///
    /// \return The parameter (#PvGenParameter object), or NULL if the index is out of bounds.
    ///
	PV_GENICAM_API PvGenParameter *operator[]( uint32_t aIndex );

    ///
    /// \brief Get a parameter (#PvGenParameter object) from the array (using parameter name).
    ///
    /// \param[in] aName  The parameter's name. See #PvGenParameter::GetName.
    ///
    /// \return The parameter (#PvGenParameter object), or NULL if the name does not match any paramaters in the array.
    ///
	PV_GENICAM_API PvGenParameter *operator[]( const PvString &aName );

    ///
    /// \brief Get the number of categories (#PvGenCategory objects) in the array.
    ///
    /// \return The number of categories (#PvGenCategory objects) in the array.
    ///
	PV_GENICAM_API uint32_t GetCategoryCount() const;

    ///
    /// \brief Get a category (#PvGenCategory object) from the array (using array position).
    ///
    /// \param[in] aIndex  The category's position in the array. The valid range for <i>aIndex</i>
    ///                     is:  0 =< <i>aIndex</i> < #GetCategoryCount.
    ///
    /// \return The parameter (#PvGenCategory object), or NULL if the index is out of bounds.
    ///
	PV_GENICAM_API PvGenCategory *GetCategory( uint32_t aIndex );

    ///
    /// \brief Get a category (#PvGenCategory object) from the array (using category name).
    ///
    /// \param[in] aName  The category's name. See PvGenCategory::GetName.
    ///
    /// \return The category (#PvGenCategory object), or NULL if the name does not match any categories in the array.
    ///
	PV_GENICAM_API PvGenCategory *GetCategory( const PvString &aName );

    ///
    /// \brief Invalidates the cache of the GenICam parameter array.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    ///
    PV_GENICAM_API PvResult InvalidateCache();

    ///
    /// \brief Returns the interface of the GenApi node map wrapped by this PvGenParameterArray
    ///
    /// \return Interface of the GenApi node map wrapped by this PvGenPArameterArray
    ///
    /// For advanced use only. 
    ///
    /// GenApi headers and link libraries are not provided
    /// with the eBUS SDK. In order to use the GenApi node map provided by this 
    /// method, install the GenApi SDK.
    ///
	PV_GENICAM_API PV_GENAPI_NS::INodeMap *GetNodeMap();

    ///
    /// \brief Helper method returning a PvGenParameter casted as a PvGenInteger.
    ///
    /// Returns NULL if the parameter does not exist in the node map or the
    /// parameter is not an integer.
    ///
    /// \param[in] aName Name of the parameter.
    ///
    /// \return PvGenInteger parameter pointer from the parameter array.
    ///
	PV_GENICAM_API PvGenInteger *GetInteger( const PvString &aName );

    ///
    /// \brief Helper method returning a PvGenParameter casted as a PvGenFloat.
    ///
    /// Returns NULL if the parameter does not exist in the node map or the
    /// parameter is not a float.
    ///
    /// \param[in] aName Name of the parameter.
    ///
    /// \return PvGenFloat parameter pointer from the parameter array.
    ///
	PV_GENICAM_API PvGenFloat *GetFloat( const PvString &aName );

    ///
    /// \brief Helper method returning a PvGenParameter casted as a PvGenEnum.
    ///
    /// Returns NULL if the parameter does not exist in the node map or the
    /// parameter is not an enumeration.
    ///
    /// \param[in] aName Name of the parameter.
    ///
    /// \return PvGenEnum parameter pointer from the parameter array.
    ///
	PV_GENICAM_API PvGenEnum *GetEnum( const PvString &aName );

    ///
    /// \brief Helper method returning a PvGenParameter casted as a PvGenBoolean.
    ///
    /// Returns NULL if the parameter does not exist in the node map or the
    /// parameter is not a boolean.
    ///
    /// \param[in] aName Name of the parameter.
    ///
    /// \return PvGenBoolean parameter pointer from the parameter array.
    ///
	PV_GENICAM_API PvGenBoolean *GetBoolean( const PvString &aName );

    ///
    /// \brief Helper method returning a PvGenParameter casted as a PvGenCommand.
    ///
    /// Returns NULL if the parameter does not exist in the node map or the
    /// parameter is not a command.
    ///
    /// \param[in] aName Name of the parameter.
    ///
    /// \return PvGenBoolean parameter pointer from the parameter array.
    ///
	PV_GENICAM_API PvGenCommand *GetCommand( const PvString &aName );

    ///
    /// \brief Helper method returning a PvGenParameter casted as a PvGenString.
    ///
    /// Returns NULL if the parameter does not exist in the node map or the
    /// parameter is not a string.
    ///
    /// \param[in] aName Name of the parameter.
    ///
    /// \return PvGenString parameter pointer from the parameter array.
    ///
	PV_GENICAM_API PvGenString *GetString( const PvString &aName );

    ///
    /// \brief Helper method returning a PvGenParameter casted as a PvGenRegister.
    ///
    /// Returns NULL if the parameter does not exist in the node map or the
    /// parameter is not a register.
    ///
    /// \param[in] aName Name of the parameter.
    ///
    /// \return PvGenRegister parameter pointer from the parameter array.
    ///
	PV_GENICAM_API PvGenRegister *GetRegister( const PvString &aName );

    ///
    /// \brief Helper method used to directly read an integer parameter value of this parameter array.
    ///
    /// \param[in]  aName  Name of the parameter to read the value from.
    /// \param[out] aValue Value read from the parameter.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_FOUND if the parameter does not exist or is not the right type
    ///     - Any other error code from #PvGenInteger::GetValue
    ///
	PV_GENICAM_API PvResult GetIntegerValue( const PvString &aName, int64_t &aValue );

    ///
    /// \brief Helper method used to directly write an integer parameter value of this parameter array.
    ///
    /// \param[in]  aName  Name of the parameter to write the value to.
    /// \param[out] aValue Value written to the parameter.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_FOUND if the parameter does not exist or is not the right type
    ///     - Any other error code from #PvGenInteger::SetValue
    ///
	PV_GENICAM_API PvResult SetIntegerValue( const PvString &aName, int64_t aValue );

    ///
    /// \brief Helper method used to directly read a float parameter value of this parameter array.
    ///
    /// \param[in]  aName  Name of the parameter to read the value from.
    /// \param[out] aValue Value read from the parameter.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_FOUND if the parameter does not exist or is not the right type
    ///     - Any other error code from PvGenFloat::GetValue
    ///
	PV_GENICAM_API PvResult GetFloatValue( const PvString &aName, double &aValue );

    ///
    /// \brief Helper method used to directly write a float parameter value of this parameter array.
    ///
    /// \param[in]  aName  Name of the parameter to write the value to.
    /// \param[out] aValue Value written to the parameter.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_FOUND if the parameter does not exist or is not the right type
    ///     - Any other error code from #PvGenFloat::SetValue
    ///
	PV_GENICAM_API PvResult SetFloatValue( const PvString &aName, double aValue );

    ///
    /// \brief Helper method used to directly read an enumeration parameter value (as a string) of this parameter array.
    ///
    /// \param[in]  aName  Name of the parameter to read the value from.
    /// \param[out] aValue Value read from the parameter, string representation.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_FOUND if the parameter does not exist or is not the right type
    ///     - Any other error code from #PvGenEnum::GetValue
    ///
	PV_GENICAM_API PvResult GetEnumValue( const PvString &aName, PvString &aValue );

    ///
    /// \brief Helper method used to directly read an enumeration parameter value (as an integer) of this parameter array.
    ///
    /// \param[in]  aName  Name of the parameter to read the value from.
    /// \param[out] aValue Value read from the parameter, integer representation.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_FOUND if the parameter does not exist or is not the right type
    ///     - Any other error code from #PvGenEnum::GetValue
    ///
	PV_GENICAM_API PvResult GetEnumValue( const PvString &aName, int64_t &aValue );

    ///
    /// \brief Helper method used to directly write an enumeration parameter value (as a string) of this parameter array.
    ///
    /// \param[in]  aName  Name of the parameter to write the value to.
    /// \param[in] aValue Value written to the parameter.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_FOUND if the parameter does not exist or is not the right type
    ///     - Any other error code from #PvGenEnum::SetValue
    ///
	PV_GENICAM_API PvResult SetEnumValue( const PvString &aName, const PvString &aValue );

    ///
    /// \brief Helper method used to directly write an enumeration parameter value (as an integer) of this parameter array.
    ///
    /// \param[in]  aName  Name of the parameter to write the value to.
    /// \param[in] aValue Value written to the parameter.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_FOUND if the parameter does not exist or is not the right type
    ///     - Any other error code from #PvGenEnum::SetValue
    ///
	PV_GENICAM_API PvResult SetEnumValue( const PvString &aName, int64_t aValue );

    ///
    /// \brief Helper method used to directly read a boolean parameter value of this parameter array.
    ///
    /// \param[in]  aName  Name of the parameter to read the value from.
    /// \param[out] aValue Value read from the parameter.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_FOUND if the parameter does not exist or is not the right type
    ///     - Any other error code from #PvGenBoolean::GetValue
    ///
	PV_GENICAM_API PvResult GetBooleanValue( const PvString &aName, bool &aValue );

    ///
    /// \brief Helper method used to directly write a boolean parameter value of this parameter array.
    ///
    /// \param[in]  aName  Name of the parameter to write the value to.
    /// \param[out] aValue Value written to the parameter.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_FOUND if the parameter does not exist or is not the right type
    ///     - Any other error code from #PvGenBoolean::SetValue
    ///
	PV_GENICAM_API PvResult SetBooleanValue( const PvString &aName, bool aValue );

    ///
    /// \brief Helper method used to directly read a string parameter value of this parameter array.
    ///
    /// \param[in]  aName  Name of the parameter to read the value from.
    /// \param[out] aValue Value read from the parameter.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_FOUND if the parameter does not exist or is not the right type
    ///     - Any other error code from #PvGenString::GetValue
    ///
	PV_GENICAM_API PvResult GetStringValue( const PvString &aName, PvString &aValue );

    ///
    /// \brief Helper method used to directly write a string parameter value of this parameter array.
    ///
    /// \param[in]  aName  Name of the parameter to write the value to.
    /// \param[out] aValue Value written to the parameter.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_FOUND if the parameter does not exist or is not the right type
    ///     - Any other error code from #PvGenString::SetValue
    ///
	PV_GENICAM_API PvResult SetStringValue( const PvString &aName, const PvString &aValue );

    ///
    /// \brief Helper method used to directly execute a command parameter of this parameter array.
    ///
    /// \param[in]  aName  Name of the parameter to execute.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_FOUND if the parameter does not exist or is not the right type
    ///     - Any other error code from #PvGenCommand::Execute
    ///
	PV_GENICAM_API PvResult ExecuteCommand( const PvString &aName );

    ///
    /// \brief Helper method used to directly retrieve the range of an integer parameter.
    ///
    /// \param[in]  aName  Name of the parameter.
    /// \param[out] aMin Minimum.
    /// \param[out] aMax Maximum.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_FOUND if the parameter does not exist or is not the right type
    ///     - Any other error code from #PvGenInteger::GetMin or #PvGenInteger::GetMax
    ///
    PV_GENICAM_API PvResult GetIntegerRange( const PvString &aName, int64_t &aMin, int64_t &aMax );

    ///
    /// \brief Helper method used to directly retrieve the range of a float parameter.
    ///
    /// \param[in]  aName  Name of the parameter.
    /// \param[out] aMin Minimum.
    /// \param[out] aMax Maximum.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_FOUND if the parameter does not exist or is not the right type
    ///     - Any other error code from #PvGenFloat::GetMin or #PvGenFloat::GetMax
    ///
    PV_GENICAM_API PvResult GetFloatRange( const PvString &aName, double &aMin, double &aMax );

    ///
    /// \brief Attaches a payload containing data chunks to a GenICam node map.
    ///
    /// Takes the content of a PvBuffer and attempts to parse the chunks from the end of the buffer. Chunk IDs
    /// from the buffer matching chunk IDs in the node map are available through the node map once the buffer
    /// is attached.
    ///
    /// Using GenICam instead of the PvBuffer raw chunk methods access allows to access the chunks as structured
    /// data, as long as the chunks are described in the GenICam XML file of the device.
    ///
    /// \param[in] aBuffer Pointer to the buffer data (usually #PvBuffer::GetRawData)
    /// \param[in] aBufferLength Size of the buffer without padding (usually #PvBuffer::GetPayloadSize)
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK: Success
    ///     - #PvResult::Code::INVALID_PARAMETER: aBuffer is NULL or aBufferLength is 0
    ///     - #PvResult::Code::INVALID_DATA_FORMAT: the chunk layout is invalid and has been rejected by GenApi
    ///     - #PvResult::Code::GENERIC_ERROR: if an error happened within GenApi. Use #PvResult::GetDescription to get more information
    ///
    PV_GENICAM_API PvResult AttachDataChunks( uint8_t *aBuffer, uint32_t aBufferLength );

    ///
    /// \brief Detach previously attached data chunks.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK: Success
    ///     - #PvResult::Code::GENERIC_ERROR: if an error happened within GenApi. Use #PvResult::GetDescription to get more information.
    ///
    PV_GENICAM_API PvResult DetachDataChunks();

    ///
    /// \brief Returns the access mode of the port of the GenICam node map.
    ///
    /// \return 
    ///    Includes:
    ///     - PvGenAccessModeReadOnly
    ///     - PvGenAccessModeReadWrite
    ///     - PvGenAccessModeNotAvailable
    ///
    PV_GENICAM_API PvGenAccessMode GetAccessMode() const;

    ///
    /// \brief Invalidates nodes having reach their defined polling time since last update.
    ///
    /// Goes through all parameters having polling times defined and invalidates those that need to 
    /// be invalidated based on the last time they were invalidated.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK: Success
    ///     - #PvResult::Code::GENERIC_ERROR: if an error happened within GenApi. Use #PvResult::GetDescription to get more information.
    ///
    PV_GENICAM_API PvResult Poll();

protected:

    PvGenICamLib::GenParameterArray *mThis;

private:

    friend class PvGenFile;
    friend class PvConfigurationWriter;
    friend class PvConfigurationReader;
    friend class PvGenICamLib::GenParameterArrayManager;
    friend class PvDeviceLib::Device;

	 // Not implemented
	PvGenParameterArray( const PvGenParameterArray & );
	const PvGenParameterArray &operator=( const PvGenParameterArray & );

};

#endif
