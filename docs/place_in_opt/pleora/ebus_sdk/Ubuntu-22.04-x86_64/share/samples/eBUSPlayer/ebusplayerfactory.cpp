// *****************************************************************************
//
//     Copyright (c) 2018, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "ebusplayerfactory.h"


///
/// \brief Constructor
///

eBUSPlayerFactory::eBUSPlayerFactory()
{
}


///
/// \brief Destructor
///

eBUSPlayerFactory::~eBUSPlayerFactory()
{
}


///
/// \brief Factory create method
///

eBUSPlayer *eBUSPlayerFactory::CreateDlg()
{
    // In this factory, we simply create the default eBUSPlayerDlg
    return new eBUSPlayer;
}


///
/// \brief Creates a device GenICam browser
///

PvGenBrowserWnd *eBUSPlayerFactory::CreateDeviceBrowser()
{
    return new PvGenBrowserWnd;
}
