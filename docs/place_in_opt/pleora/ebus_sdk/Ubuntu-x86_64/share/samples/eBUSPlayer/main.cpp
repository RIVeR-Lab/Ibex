// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "ebusplayerfactory.h"
#include "splashscreen.h"

#include <QApplication>
#include <QTime>
#include <QTextCodec>

#include <signal.h>


void SignalHangler( int aSig )
{
	qApp->quit();
}

int main( int argc, char *argv[] )
{
	QLocale::setDefault( QLocale( QLocale::English, QLocale::UnitedStates ) );
	QApplication a(argc, argv);

    eBUSPlayerFactory lFactory;
    eBUSPlayer *lEBUSPlayer = lFactory.CreateDlg();
    lEBUSPlayer->CreateLayout();

	SplashScreen* lSplashScreen = new SplashScreen( lEBUSPlayer->GetAppName() );
	lSplashScreen->show();

    QTime lTimer;
    lTimer.start();

    while ( lTimer.elapsed() < 500 )
    {
    	a.processEvents();
    }

    QCoreApplication::setOrganizationName( "Pleora Technologies Inc" );
    QCoreApplication::setOrganizationDomain( "pleora.com" );
    QCoreApplication::setApplicationName( "eBUSPlayer" );
    
    while ( lTimer.elapsed() < 2500 )
    {
    	a.processEvents();
    }

#ifdef _LINUX_
    // When catching the signal, we will simulate the File\Exit menu usage to ensure a proper cleanup
    QObject::connect( &a, SIGNAL( aboutToQuit() ), lEBUSPlayer, SLOT( OnFileExitAction() ) );
    signal( SIGTERM, SignalHangler );
    signal( SIGINT, SignalHangler );
#endif // _LINUX_

    lEBUSPlayer->show();

    while ( lTimer.elapsed() < 3000 )
    {
    	a.processEvents();
    }

    lSplashScreen->close();
    PVDELETE( lSplashScreen );

    int lRetVal = a.exec();
    PVDELETE( lEBUSPlayer );

    return lRetVal;
}

