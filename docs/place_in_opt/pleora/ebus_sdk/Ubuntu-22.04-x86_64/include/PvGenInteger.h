// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVGENINTEGER_H__
#define __PVGENINTEGER_H__

#include <PvGenParameter.h>


///
/// \class PvGenInteger
/// \ingroup GenICam features
/// \brief GenICam feature: Integer
///
/// To use this class, see #PvGenParameter.
///
class PvGenInteger : public PvGenParameter
{
public:

    ///
    /// \brief Set the feature's new value.
    ///
    /// \param[in] aValue The feature's new value.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    ///
	PV_GENICAM_API PvResult SetValue( int64_t aValue );

    ///
    /// \brief Get the feature's value.
    ///
    /// \param[out] aValue  The feature's value.
    ///
    /// \return See #SetValue.
    ///
	PV_GENICAM_API PvResult GetValue( int64_t &aValue ) const;

    ///
    /// \brief Get the feature's limits (minimum).
    ///
    /// \param[out] aMin The feature's minimum allowable value.
    ///
    /// \return See #SetValue.
    ///
	PV_GENICAM_API PvResult GetMin( int64_t &aMin ) const;

    ///
    /// \brief Get the feature's limits (maximum).
    ///
    /// \param[out] aMax  The feature's maximum allowable value.
    ///
    /// \return See #SetValue.
    ///
	PV_GENICAM_API PvResult GetMax( int64_t &aMax ) const;

    ///
    /// \brief Get the feature's suggested increment.
    ///
    /// The #GetIncrement method suggests how much an
    /// up/down button in the UI should increment/decrement
    /// the current value. 
    ///
    /// For example, a feature with a range of 0 to 1000000
    /// might have an increment of 100000 to keep the user
    /// from having to click excessively.
    ///
    /// \param[out] aIncrement The feature's suggested increment.
    ///
    /// \return See #SetValue.
    ///
	PV_GENICAM_API PvResult GetIncrement( int64_t &aIncrement ) const;

    ///
    /// \brief Get the feature's suggested presentation format.
    ///
    /// See #PvGenFloat::GetRepresentation.
    ///
    /// \param[out] aRepresentation  See #PvGenFloat::GetRepresentation.
    ///
    /// \return See #SetValue.
    ///
	PV_GENICAM_API PvResult GetRepresentation( PvGenRepresentation &aRepresentation ) const;

    ///
    /// \brief Get the feature's measurement unit.
    ///
    /// The #GetUnit method returns a string with the units for the feature. For example, 
    /// the string might contain "pixels", "bytes", "ms", and so on. The string
    /// is informational and has no intrinsic value for converting to other units.
    ///
    /// \param[out] aUnit  The units for the feature.
    ///
    /// \return See #SetValue.
    ///
    PV_GENICAM_API PvResult GetUnit( PvString &aUnit ) const;

protected:

    ///
    /// \brief Constructor.
    ///
	PvGenInteger();

    ///
    /// \brief Destructor.
    ///
	virtual ~PvGenInteger();

private:

    // Not implemented
	PvGenInteger( const PvGenInteger & );
	const PvGenInteger &operator=( const PvGenInteger & );

};

#endif
