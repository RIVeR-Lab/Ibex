// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <QDialog>
#include <QPushButton>
#include <QLabel>

class AboutBox  : public QDialog
{
    Q_OBJECT

public:

	AboutBox( QWidget *aParent, const QString &aAppName );
	virtual ~AboutBox();

private:

	void CreateLayout();

	QPushButton* mOKButton;
	QLabel *mBitmapLabel;
	QLabel *mEBUSPlayerLabel;
	QLabel *mPureGEVLabel;
	QLabel *mCopyrightLabel;
	QLabel *mPleoraLabel;
	QPixmap *mBackground;

	QString mAppName;
	
};

