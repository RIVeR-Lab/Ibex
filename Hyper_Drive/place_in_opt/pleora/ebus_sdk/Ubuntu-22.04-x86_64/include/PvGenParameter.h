// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVGENPARAMETER_H__
#define __PVGENPARAMETER_H__

#include <PvGenICamLib.h>
#include <PvGenParameterList.h>
#include <PvGenTypes.h>
#include <PvGenApi.h>


class PvGenEventSink;
class PvGenParameterList;

namespace PvGenICamLib
{
    class GenParameterArray;
    class GenParameterInternal;
    class GenParameterArrayManager;
}


#ifdef PV_INTERNAL_HEADERS
    class PvGenBoolean;
    class PvGenCommand;
    class PvGenEnum;
    class PvGenFloat;
    class PvGenInteger;
    class PvGenRegister;
    class PvGenString;
#endif // PV_INTERNAL_HEADERS


///
/// \class PvGenParameter
/// \ingroup GenICam features
/// \brief Base class for all GenICam feature types.
///
class PvGenParameter
{
public:

    ///
    /// \brief Get the parameter's name.
    ///
    /// \return The parameter's name.
    ///
	PV_GENICAM_API PvString GetName() const;

    ///
    /// \brief Get the parameter's name.
    ///
    /// \param[out] aName  The parameter's name.
    ///
    /// \return See #GetCategory.
    ///
	PV_GENICAM_API PvResult GetName( PvString &aName ) const;

    ///
    /// \brief Get the parameter's type.
    ///
    /// \param[out]  aType  The parameter's type; a PvGenType enumeration.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///
	PV_GENICAM_API PvResult GetType( PvGenType &aType ) const;

    ///
    /// \brief Get the parameter's category.
    ///
    /// \param[out] aCategory  The parameter's category. Levels are separated by '\' characters.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    ///
	PV_GENICAM_API PvResult GetCategory( PvString &aCategory ) const;

    ///
    /// \brief Get the parameter's tool tip.
    ///
    /// \param aToolTip   The parameter's tool tip; typically a brief description of how to use the parameter.
    /// 
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    ///
	PV_GENICAM_API PvResult GetToolTip( PvString &aToolTip ) const;

    ///
    /// \brief Get the parameter's description.
    ///
    /// \param[out] aDescription   The parameter's description; typically a 
    ///                             long description of what the parameter is.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    /// 
    /// \sa GetToolTip
    ///
	PV_GENICAM_API PvResult GetDescription( PvString &aDescription ) const;

    ///
    /// \brief Get the parameter's display name.
    ///
    /// \param[out] aDisplayName   The parameter's display name; a name that 
    ///                             can be used to identify a parameter in a user-interface
    ///                             context instead of #GetName.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    /// 
    /// \sa GetToolTip
    ///
    PV_GENICAM_API PvResult GetDisplayName( PvString &aDisplayName ) const;

    ///
    /// \brief Get the parameter's name space.
    ///
    /// \param[out] aNameSpace The parameter's name space.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    /// 
    /// \sa GetToolTip
    ///
    PV_GENICAM_API PvResult GetNameSpace( PvGenNameSpace &aNameSpace ) const;

    ///
    /// \brief Test if the parameter's value is cached in the PC.
    ///
    /// \param[out] aCached True if the PC keeps a cached version of the parameter's value; 
    ///                      false if the value must be retrieved from the video interface each time
    ///                      it's requested.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    ///
	PV_GENICAM_API PvResult IsValueCached( bool &aCached ) const;

    ///
    /// \brief Test if the parameter is to be considered when saving the state of a GenICam interface to disk.
    ///
    /// The #IsPersistent tests if the feature's persistence attribute is true. 
    ///
    /// Features that might not be persistent:
    ///     - Features such as a clock's current value and others that are meaningless
    /// shortly after the value is retrieved. 
    ///     - Some network configuration settings.
    ///     - Etc.
    /// 
    /// \param[out] aPersistent  True if the parameter's value is persistent; otherwise, false.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    ///
	PV_GENICAM_API PvResult IsPersistent( bool &aPersistent ) const;

    ///
    /// \brief Test if the parameter is implemented.
    ///
    /// Typically the IsImplemented attribute does not change during the lifetime of a parameter.
    /// However some GenICam interfaces are using IsImplemented in order to activate hidden or
    /// advanced features.
    ///
    /// \param[out] aImplemented True if the parameter is implemented; otherwise, false.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    ///
	PV_GENICAM_API PvResult IsImplemented( bool &aImplemented ) const;

    ///
    /// \brief Test if the parameter is currently available, but may be available latter on.
    ///
    /// \param[out] aAvailable  True if the parameter is currently available; otherwise, false.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    ///
	PV_GENICAM_API PvResult IsAvailable( bool &aAvailable ) const;

    ///
    /// \brief Test if the parameter is writable.
    ///
    /// \param[out] aWritable  True if the parameter is currently writable; otherwise, false.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    ///
	PV_GENICAM_API PvResult IsWritable( bool &aWritable ) const;

    ///
    /// \brief Test if the parameter is readable.
    ///
    /// 'Readable' is an attribute in the GenICam specification; in practice most
    /// parameters are readable.
    ///
    /// \param[out] aReadable   True if the parameter is currently readable; otherwise, false.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    ///
	PV_GENICAM_API PvResult IsReadable( bool &aReadable ) const;

    ///
    /// \brief Test if the parameter is streamable.
    ///
    /// 'Streamable' is an attribute in the GenICam specification; If true, it means
    /// the parameter should be considered for persistence.
    ///
    /// \param[out] aStreamable True if the parameter to be considered for persistence; otherwise, false.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    ///
	PV_GENICAM_API PvResult IsStreamable( bool &aStreamable ) const;

    ///
    /// \brief Test if the parameter's value is cached in the PC.
    ///
    /// \return 
    ///    Includes:
    ///     - True if the PC keeps a cached version of the parameter's value; 
    ///     - false if the value must be retrieved from the video interface each time it's requested.
    ///
	PV_GENICAM_API bool IsValueCached() const;

    ///
    /// \brief Test if the parameter is to be considered when saving the state of a GenICam interface to disk.
    ///
    /// \return True if the parameter's value is persistent; otherwise, false.
    ///
	PV_GENICAM_API bool IsPersistent() const;

    ///
    /// \brief Test if the parameter is implemented.
    ///
    /// Typically the IsImplemented attribute does not change during the lifetime of a parameter.
    /// However some GenICam interfaces are using using IsImplemented in order to activate hidden or
    /// advanced features.
    ///
    /// \return True if the parameter is currently implemented; otherwise, false.
    ///
	PV_GENICAM_API bool IsImplemented() const;

    ///
    /// \brief Test if the parameter is currently available, but may be available latter on.
    ///
    /// \return True if the parameter is currently available; otherwise, false.
    ///
	PV_GENICAM_API bool IsAvailable() const;

    ///
    /// \brief Test if the parameter is writable.
    ///
    /// \return True if the parameter is currently writable; otherwise, false.
    ///
	PV_GENICAM_API bool IsWritable() const;

    ///
    /// \brief Test if the parameter is readable.
    ///
    /// 'Readable' is an attribute in the GenICam specification; in practice most
    /// parameters are readable.
    ///
    /// \return True if the parameter is currently readable; otherwise, false.
    ///
	PV_GENICAM_API bool IsReadable() const;

    ///
    /// \brief Test if the parameter is visible at the specified visibilty level.
    ///
    /// \param[in]  aCurrentVisibility  
    ///
    /// \return True if the parameter is visible at the specified visibilty level.
    ///
	PV_GENICAM_API bool IsVisible( PvGenVisibility aCurrentVisibility ) const;

    ///
    /// \brief Test if the parameter is streamable.
    ///
    /// 'Streamable' is an attribute in the GenICam specification; If true, it means
    /// the parameter should be considered for persistence.
    ///
    /// \return True if the parameter to be considered for persistence; otherwise, false.
    ///
	PV_GENICAM_API bool IsStreamable() const;

    ///
    /// \brief Get the parameter's recommended user level (visibility)
    ///
    /// See #PvGenVisibility.
    ///
    /// \param[out] aVisibility  The parameter's recommended user level; a #PvGenVisibility enumeration.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    ///
    /// \sa IsVisible
    ///
	PV_GENICAM_API PvResult GetVisibility( PvGenVisibility &aVisibility ) const;

    ///
    /// \brief Test if the parameter is visible at the specified visibilty level.
    ///
    /// See #PvGenVisibility.
    ///
    /// \param[in]  aCurrentVisibility  The current visibility setting; a #PvGenVisibility parameter.
    /// \param[out] aVisible            True if the parameter is visible; otherwise, false.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    ///
    /// \sa GetVisibility
    ///
	PV_GENICAM_API PvResult IsVisible( PvGenVisibility aCurrentVisibility, bool &aVisible ) const;

    ///
    /// \brief Queries whether this parameter is a selector or not.
    ///
    /// \param[out] aSelector True if the parameter is a selector. False if not.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    ///
    PV_GENICAM_API PvResult IsSelector( bool &aSelector ) const;

    ///
    /// \brief Returns all parameters selected by this parameter.
    ///
    /// The list used to output parameters is cleared by this method.
    ///
    /// \param[out] aList List (output) where this method copies selected parameter pointers.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    ///
    PV_GENICAM_API PvResult GetSelectedParameters( PvGenParameterList &aList ) const;

    ///
    /// \brief Returns all selectors for this parameter.
    ///
    /// The list used to output parameters is cleared by this method.
    ///
    /// \param[out] aList List (output) where this method copies selector parameter pointers.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    ///
    PV_GENICAM_API PvResult GetSelectingParameters( PvGenParameterList &aList ) const;

    ///
    /// \brief Register an event sink interface that will be used for callbacks on this parameter.
    ///
    /// Event sinks are automatically unregistered when the #PvGenParameter object is destroyed
    /// (even if #UnregisterEventSink wasn't called).
    ///
    /// \param[in] aEventSink  The event sink.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///     - #PvResult::Code::GENERIC_ERROR
    ///
    PV_GENICAM_API PvResult RegisterEventSink( PvGenEventSink *aEventSink );

    ///
    /// \brief Unregister an event sink.
    ///
    /// \param[in] aEventSink See #RegisterEventSink
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_FOUND
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///
	PV_GENICAM_API PvResult UnregisterEventSink( PvGenEventSink *aEventSink );

    ///
    /// \brief Gets the parameter value as a string.
    /// 
    /// \return Parameter value as a string.
    ///
	PV_GENICAM_API PvString ToString() const;

    ///
    /// \brief Gets the parameter value as a string.
    /// 
    /// \param[out] aValue  A string representing the parameter value.
    ///
    /// Typically used for UI, persistence.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    ///
    PV_GENICAM_API PvResult ToString( PvString &aValue ) const;

    ///
    /// \brief Sets the parameter value from a string.
    /// 
    /// \param[in] aValue  A string representing the new value to set the parameter to.
    ///
    /// Typically used for UI, persistence.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    ///
    PV_GENICAM_API PvResult FromString( const PvString &aValue );

    ///
    /// \brief Returns the GenApi node wrapped by this PvGenParameter
    ///
    /// \return GenApi node wrapped by this PvGenPArameter
    ///
    /// For advanced use only. 
    ///
    /// GenApi headers and link libraries are not provided
    /// with the eBUS SDK. In order to use the GenApi node provided by this 
    /// method, install the GenApi SDK.
    ///
	PV_GENICAM_API PV_GENAPI_NS::INode *GetNode();

    ///
    /// \brief Returns true if parameter update notifications are temporarily disabled.
    ///
    /// \return True if they are disabled, false if not.
    ///
	PV_GENICAM_API bool GetUpdatesEnabled();

    ///
    /// \brief Temporarily disable (or re-enable) parameter update notifications.
    ///
    /// \param[in] aEnabled False to disable, true to re-enable.
    ///
	PV_GENICAM_API void SetUpdatesEnabled( bool aEnabled );

protected:

    ///
    /// \brief Constructor.
    ///
	PvGenParameter();

    ///
    /// \brief Destructor.
    ///
	virtual ~PvGenParameter();

#ifndef PV_GENERATING_DOXYGEN_DOC

    PvGenICamLib::GenParameterInternal *mThis;

    friend class PvGenICamLib::GenParameterArray;
    friend class PvGenICamLib::GenParameterArrayManager;

#endif // PV_GENERATING_DOXYGEN_DOC 

private:

    // Not implemented
	PvGenParameter( const PvGenParameter & );
	const PvGenParameter &operator=( const PvGenParameter & );

};

///
/// \class PvGenEventSink
/// \brief Observer interface for an observable PvGenParameter object
/// 
/// To use #PvGenEventSink, see #PvGenParameter::RegisterEventSink.
///
class PvGenEventSink
{
public:

    ///
    /// \fn PvGenEventSink::OnParameterUpdate( PvGenParameter *aParameter ) = 0
    /// \brief Callback that is invoked when a feature's value is updated.
    ///
    /// \param[in] aParameter  The feature that was updated.
    ///
    ///
	virtual void OnParameterUpdate( PvGenParameter *aParameter ) = 0;

};

#endif
