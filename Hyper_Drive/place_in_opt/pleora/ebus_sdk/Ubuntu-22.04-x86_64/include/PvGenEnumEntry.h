// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVGENENUMENTRY_H__
#define __PVGENENUMENTRY_H__

#include <PvGenICamLib.h>
#include <PvGenTypes.h>


namespace PvGenICamLib
{
    class GenEnumEntryInternal;
}


///
/// \class PvGenEnumEntry
/// \ingroup GenICam features
/// \brief GenICam feature: Enumeration entry
///
/// To use #PvGenEnumEntry, see #PvGenEnum.
///
class PvGenEnumEntry
{
public:

    ///
    /// \brief Get the enumeration entry's value.
    ///
    /// \param[out] aValue  The enumeration entry's value.
    ///
    /// \return See #GetDescription.
    ///
    /// \sa PvGenEnum::GetValue
    ///
	PV_GENICAM_API PvResult GetValue( int64_t &aValue ) const;

    ///
    /// \brief Get the enumeration entry's name.
    ///
    /// \param[out] aName  The enumeration entry's name
    /// 
    /// \return See #GetDescription.
    ///
    /// \sa PvGenEnum::GetValueString
    ///
	PV_GENICAM_API PvResult GetName( PvString &aName ) const;

    ///
    /// \brief Get a short description of the enum entry.
    ///
    /// \param[out] aToolTip  A short description of the enum entry.
    ///
    /// \return See #GetDescription.
    ///
	PV_GENICAM_API PvResult GetToolTip( PvString &aToolTip ) const;

    ///
    /// \brief Get a longer description of the enum entry.
    ///
    /// \param[out] aDescription  A longer description of the enum entry.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use PvResult::GetDescription to get more information.)
    ///
	PV_GENICAM_API PvResult GetDescription( PvString &aDescription ) const;

    ///
    /// \brief Get the enumeration entry's current visibility level.
    ///
    /// See PvGenParameter::GetVisibility.
    ///
    /// \param[out] aVisibility   See PvGenParameter::GetVisibility.
    ///
    /// \return See #GetDescription.
    ///
    /// \sa PvGenParameter::GetVisibility
    ///
	PV_GENICAM_API PvResult GetVisibility( PvGenVisibility &aVisibility ) const;

    ///
    /// \brief Get the enum entry's display name.
    ///
    /// \param[out] aDisplayName   The enum entry's display name; a name that 
    ///                             can be used to identify an enum entry in a user-interface
    ///                             context instead of #GetName.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use PvResult::GetDescription to get more information.)
    ///
    PV_GENICAM_API PvResult GetDisplayName( PvString &aDisplayName ) const;

    ///
    /// \brief Get the enum enty's name space.
    ///
    /// \param[out] aNameSpace The enum entry's name space.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use PvResult::GetDescription to get more information.)
    ///
    PV_GENICAM_API PvResult GetNameSpace( PvGenNameSpace &aNameSpace ) const;

    ///
    /// \brief Test if the enumeration entry is above a specific visibilty level.
    ///
    /// See PvGenParameter::IsVisible.
    ///
    /// \param[in]  aVisibility         See PvGenParameter::IsVisible.
    /// \param[out] aVisible            See PvGenParameter::IsVisible.
    ///
    /// \return See #GetDescription.
    ///
    /// \sa PvGenParameter::IsVisible
    ///
	PV_GENICAM_API PvResult IsVisible( PvGenVisibility aVisibility, bool &aVisible ) const;

    ///
    /// \brief Test if the enumeration entry is currently available.
    ///
    /// See PvGenParameter::IsAvailable.
    /// 
    /// \param[out] aAvailable See PvGenParameter::IsAvailable.
    ///
    /// \return See #GetDescription.
    ///
    /// \sa PvGenParameter::IsAvailable
    ///
	PV_GENICAM_API PvResult IsAvailable( bool &aAvailable ) const;

    ///
    /// \brief Test if the parameter is visible at the specified visibilty level.
    ///
    /// \param[in]  aCurrentVisibility  See PvGenParameter::IsVisible.
    ///
    /// \return True if the parameter is visible at the specified visibilty level.
    ///
	PV_GENICAM_API bool IsVisible( PvGenVisibility aVisibility ) const;

    ///
    /// \brief Test if the enumeration entry is currently available.
    ///
    /// \return True if the enumeration entry is currently available.
    ///
    /// \sa PvGenParameter::IsAvailable
    ///
	PV_GENICAM_API bool IsAvailable() const;

protected:

    ///
    /// \brief Constructor.
    ///
	PvGenEnumEntry();

    ///
    /// \brief Destructor.
    ///
	virtual ~PvGenEnumEntry();

    PvGenICamLib::GenEnumEntryInternal *mThis;

private:

};

#endif
