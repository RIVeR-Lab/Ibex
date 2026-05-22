// *****************************************************************************
//
//     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <QMessageBox>


#define SAFE_DELETE( a ) \
	if ( ( a ) != NULL ) \
	{ \
        delete ( a ); \
		( a ) = NULL; \
	}


inline
void PvMessageBox( QWidget *aWidget, PvResult &aResult )
{
	QString lError = aResult.GetCodeString().GetAscii();
	QString lDescription = aResult.GetDescription().GetAscii();
	QMessageBox::critical( aWidget, "Error", lError + "\r\n\r\n" + lDescription + "\r\n" );
}
