// *****************************************************************************
//
//     Copyright (c) 2018, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include "ebusplayer.h"


class eBUSPlayerFactory
{
public:

    eBUSPlayerFactory();
    virtual ~eBUSPlayerFactory();

    eBUSPlayer *CreateDlg();
    PvGenBrowserWnd *CreateDeviceBrowser();

};
