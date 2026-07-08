// *****************************************************************************
//
//     Copyright (c) 2010, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <QMessageBox>


inline
void PvMessageBox( QWidget *aWidget, PvResult &aResult )
{
	QString lError = aResult.GetCodeString().GetAscii();
	QString lDescription = aResult.GetDescription().GetAscii();
	QMessageBox::critical( aWidget, "Error", lError + "\r\n\r\n" + lDescription + "\r\n" );
}
