// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <QDialog>
#include <QLabel>
#include <QPixmap>


class SplashScreen  : public QDialog
{
    Q_OBJECT

public:

    SplashScreen( const QString &aAppName );
	virtual ~SplashScreen();

protected:

	void paintEvent( QPaintEvent * event );

private:

	void CreateLayout();

	QLabel *mBitmapLabel;
	QLabel *mEBUSPlayerLabel;
	QLabel *mPureGEVLabel;
	QLabel *mCopyrightLabel;
	QLabel *mPleoraLabel;
	QPixmap *mBackground;

	QString mAppName;
	
};
