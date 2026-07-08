// *****************************************************************************
//
// Copyright (c) 2018, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <PvSoftDeviceGEVInterfaces.h>


void Time2UInt8( uint8_t *aBuffer, size_t aBufferSize );
void FireTestEvents( IPvMessageChannel *aMessageChannel );
void DumpRegisters( IPvRegisterMap *aRegisterMap );

