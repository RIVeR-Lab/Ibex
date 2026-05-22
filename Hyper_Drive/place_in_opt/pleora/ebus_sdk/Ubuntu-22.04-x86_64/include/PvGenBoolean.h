// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVGENBOOLEAN_H__
#define __PVGENBOOLEAN_H__

#include <PvGenParameter.h>


///
/// \class PvGenBoolean
/// \ingroup GenICam features
///
/// \brief GenICam feature: Boolean. A feature which can either be true or false.
///
/// To use this class, see #PvGenParameter.
///
class PvGenBoolean : public PvGenParameter
{
public:

	///
	/// \brief Set the feature's value.
	///
	/// \param[in] aValue  The feature's new value.
	///
	/// \return Includes:
	/// 	- #PvResult::Code::OK
	/// 	- #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
	///
	PV_GENICAM_API PvResult SetValue( bool aValue );

	///
	/// \brief Get the feature's value.
	///
	/// \param[out] aValue  The feature's value.
	///
	/// \return See #SetValue.
	///
	PV_GENICAM_API PvResult GetValue( bool &aValue ) const;

protected:

	///
	/// \brief Constructor.
	///
	PvGenBoolean();

	///
	/// \brief Destructor.
	///
	virtual ~PvGenBoolean();

private:

    // Not implemented
	PvGenBoolean( const PvGenBoolean & );
	const PvGenBoolean &operator=( const PvGenBoolean & );
};

#endif
