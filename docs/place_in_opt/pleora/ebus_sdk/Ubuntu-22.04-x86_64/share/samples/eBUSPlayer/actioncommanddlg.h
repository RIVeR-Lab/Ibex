// *****************************************************************************
//
//     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <QDialog>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QListWidget>
#include <QLabel>

#include <PvActionCommand.h>


class eBUSPlayer;


class ActionCommandDlg : public QDialog
{
    Q_OBJECT

public:

    ActionCommandDlg( eBUSPlayer *aEBUSPlayer );
	virtual ~ActionCommandDlg();

protected:

	void FillNetworkList();
	void CreateLayout();
	void EnableInterface();

	bool Configure();
	bool Send();
	void MonitorAcknowledgements();

	bool EditToInt( QLineEdit *aEdit, uint64_t aMax, uint64_t &aValue );

protected slots:

    void OnSendClick();
    void OnScheduledStateChanged( int aState );
    void OnItemChanged ( QListWidgetItem * item );

private:

	QListWidget *mNetworkList;
	QLineEdit *mDeviceKeyEdit;
	QLineEdit *mGroupKeyEdit;
	QLineEdit *mGroupMaskEdit;
	QCheckBox *mScheduledCheck;
	QLabel *mTimestampLabel;
	QLineEdit *mTimestampEdit;
	QCheckBox *mForceACKFlagCheck;
	QPushButton *mSendButton;
	QTextEdit *mAcknowledgementsEdit;

	PvActionCommand mActionCommand;
	bool mForceACKFlag;
};
