// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVGENREGISTER_H__
#define __PVGENREGISTER_H__

#include <PvGenParameter.h>


/// \class PvGenRegister
/// \ingroup GenICam features
/// \brief GenICam feature: Register, or IRegister. A feature providing direct access to a section of the device's register map.
///
/// To use this class, see #PvGenParameter.
///
class PvGenRegister : public PvGenParameter
{
public:

    ///
    /// \brief Write to the register.
    ///
    /// Partial (less than #GetLength) writes are supported at the GenICam level,
    /// but the behaviour is defined by the device.
    ///
    /// GigE Vision enforces a mod-4 (can be divided by 4 bytes) length restriction
    /// on all read/write operations.
    ///
    /// \param[in] aBuffer   Data to write to the register.
    /// \param[in] aLength   Length of the buffer, in bytes.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
    ///
    PV_GENICAM_API PvResult Set( const uint8_t *aBuffer, int64_t aLength );

    ///
    /// \brief Reads from the register.
    ///
    /// Partial (less than #GetLength) reads are supported at the GenICam level,
    /// but the behaviour is defined by the device.
    ///
    /// GigE Vision enforces a mod-4 (can be divided by 4 bytes) length restriction
    /// on all read/write operations.
    ///
    /// \param[out] aBuffer   Data read from the register.
    /// \param[out] aLength   Length of the buffer, in bytes.
    ///
    /// \return See #Set.
    ///
    PV_GENICAM_API PvResult Get( uint8_t *aBuffer, int64_t aLength ) const;

    ///
    /// \brief Returns the length of the register mapped with this feature.
    ///
    /// \param[out] aLength   Length of the register, in bytes.
    ///
    /// \return See #Set.
    ///
    PV_GENICAM_API PvResult GetLength( int64_t &aLength ) const;

protected:

    ///
    /// \brief Constructor.
    ///
	PvGenRegister();

    ///
    /// \brief Destructor.
    ///
	virtual ~PvGenRegister();

private:

    // Not implemented
	PvGenRegister( const PvGenRegister & );
	const PvGenRegister &operator=( const PvGenRegister & );

};

#endif
