// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <QDialog>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QLabel>
#include <QLineEdit>

#include <PvConfigurationReader.h>
#include <PvConfigurationWriter.h>

class Player;


class BufferOptionsDlg : public QDialog
{
    Q_OBJECT

public:

    BufferOptionsDlg( Player *aPlayer, QWidget* aParent );
	virtual ~BufferOptionsDlg();

    int exec();

protected:

	void CreateLayout();

	void accept();

protected slots:

private:

	QLineEdit *mBufferCountEdit;
	QLineEdit *mBufferSizeEdit;
	QCheckBox *mAutoResizeCheck;

	QLabel *mLabel1;
    QLabel *mLabel2;
    QLabel *mLabel3;

	QPushButton *mOKButton;
	QPushButton *mCancelButton;

	Player *mPlayer;

};
