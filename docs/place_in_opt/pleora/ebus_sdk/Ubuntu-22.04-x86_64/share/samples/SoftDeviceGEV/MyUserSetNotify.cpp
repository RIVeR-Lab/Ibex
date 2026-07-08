// *****************************************************************************
//
// Copyright (c) 2022, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "Defines.h"
#include "MyUserSetNotify.h"
#include "Utilities.h"


///
/// \brief callback method for UserSet state changes
///
PvResult MyUserSetNotify::UserSetStateNotify( PvUserSetState aUserSetState,
	                                      uint32_t       aUserSetIndex )
{
    switch ( aUserSetState )
    {
    case  PvUserSetState::PvUserSetStateSaveStart:
        std::cout << "UserSet " << aUserSetIndex << ": SaveStart" << std::endl;
	break;
    case  PvUserSetState::PvUserSetStateSaveCompleted:
        std::cout << "UserSet " << aUserSetIndex << ": SaveCompleted" << std::endl;
	break;
    case  PvUserSetState::PvUserSetStateLoadStart:
        std::cout << "UserSet " << aUserSetIndex << ": LoadStart" << std::endl;
	break;
    case  PvUserSetState::PvUserSetStateLoadCompleted:
        std::cout << "UserSet " << aUserSetIndex << ": LoadCompleted" << std::endl;
	break;
    }
    return PvResult::Code::OK;
}
