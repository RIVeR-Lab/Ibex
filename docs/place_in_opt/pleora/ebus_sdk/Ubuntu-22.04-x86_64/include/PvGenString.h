// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVGENSTRING_H__
#define __PVGENSTRING_H__

#include <PvGenParameter.h>


///
/// \class PvGenString
/// \ingroup GenICam features
/// \brief GenICam feature: String
///
/// To use this class, see #PvGenParameter.
///
class PvGenString : public PvGenParameter
{
public:

    ///
    /// \brief Set the feature's value.
    ///
    /// \param[in] aValue The feature's new value.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    ///
	PV_GENICAM_API PvResult SetValue( const PvString &aValue );

    ///
    /// \brief Get the feature's value.
    ///
    /// \param[out] aValue The feature's value.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    ///
	PV_GENICAM_API PvResult GetValue( PvString &aValue ) const;

    ///
    /// \brief Returns the maximum length of the string.
    ///
    /// \param[out] aMaxLength   Maximum length of the register, in characters.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    ///
    PV_GENICAM_API PvResult GetMaxLength( int64_t &aMaxLength ) const;

protected:

    ///
    /// \brief Constructor.
    ///
	PvGenString();

    ///
    /// \brief Destructor.
    ///
	virtual ~PvGenString();
 
private:

    // Not implemented
    PvGenString( const PvGenString & );
	const PvGenString &operator=( const PvGenString & );
};

#endif
