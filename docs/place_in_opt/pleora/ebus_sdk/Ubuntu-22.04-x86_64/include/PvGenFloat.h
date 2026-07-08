// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVGENFLOAT_H__
#define __PVGENFLOAT_H__

#include <PvGenParameter.h>


///
/// \class PvGenFloat
/// \ingroup GenICam features
/// \brief GenICam feature: Float
///
/// To use this class, see #PvGenParameter.
///
class PvGenFloat : public PvGenParameter
{
public:

    ///
    /// \brief Set the feature's value.
    ///
    /// \param[in] aValue The feature's new value.
    ///
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    ///
	PV_GENICAM_API PvResult SetValue( double aValue );

    ///
    /// \brief Get the feature's value.
    ///
    /// \param[out] aValue The feature's value.
    ///
    /// \return See #SetValue.
    ///
	PV_GENICAM_API PvResult GetValue( double &aValue ) const;

    ///
    /// \brief Get the feature's limits (minimum).
    ///
    /// \param[out] aMin The feature's minimum allowable value.
    ///
    /// \return See #SetValue.
    ///
	PV_GENICAM_API PvResult GetMin( double &aMin ) const;

    ///
    /// \brief Get the feature's limits (maximum).
    ///
    /// \param[out] aMax The feature's maximum allowable value.
    ///
    /// \return See #SetValue.
    ////
	PV_GENICAM_API PvResult GetMax( double &aMax ) const;

    ///
    /// \brief Get the feature's suggested presentation format.
    ///
    /// \param[out] aRepresentation  The feature's suggested presentation format; a #PvGenRepresentation.
    ///
    /// \return See #SetValue.
    ///
    /// \sa #PvGenRepresentation
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
	PvGenFloat();

    ///
    /// \brief Destructor.
    ///
	virtual ~PvGenFloat();

private:

    // Not implemented
	PvGenFloat( const PvGenFloat & );
	const PvGenFloat &operator=( const PvGenFloat & );
};

#endif
