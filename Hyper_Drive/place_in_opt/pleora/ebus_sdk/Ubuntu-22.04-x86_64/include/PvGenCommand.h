// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVGENCOMMAND_H__
#define __PVGENCOMMAND_H__

#include <PvGenParameter.h>


///
/// \class PvGenCommand
/// \ingroup GenICam features
/// \brief GenICam feature: Command. A feature which can only be activated.
///
/// To use this class, see #PvGenParameter.
class PvGenCommand : public PvGenParameter
{
public:

	///
	/// \brief Activate the feature.
	///
	/// \return 
    ///    Includes:
	/// 	- #PvResult::Code::OK
	/// 	- #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
	///
	PV_GENICAM_API PvResult Execute();
#ifdef EBUS_PYTHON_DOXYGEN
	bool IsDone();
#else

	///
	/// \brief Query whether the command is executed.
	///
	/// \param[in] aDone   True if the command has completed its execution.
	///
	/// \return 
    ///    Includes:
	/// 	- #PvResult::Code::OK
	/// 	- #PvResult::Code::GENERIC_ERROR (Use #PvResult::GetDescription to get more information.)
	///
	PV_GENICAM_API PvResult IsDone( bool &aDone );
#endif

protected:

	///
	/// \brief Constructor.
	///
	PvGenCommand();

	///
	/// \brief Destructor.
	///
	virtual ~PvGenCommand();

private:

    // Not implemented
	PvGenCommand( const PvGenCommand & );
	const PvGenCommand&operator=( const PvGenCommand & );

};

#endif
