// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QComboBox>
#include <QProgressBar>
#include <QTimer>

#include <PvDevice.h>


class TransferThread;


class FileTransferDlg : public QDialog, public PvDeviceEventSink
{
    Q_OBJECT

public:

    FileTransferDlg( PvDevice *aDevice, QWidget* aParent );
	virtual ~FileTransferDlg();

    int exec();

protected:

	void CreateLayout();
    void EnableInterface();
    void LoadFileCombo();

protected slots:

	void OnFileComboSelChanged( int aIndex );
	void OnDownloadClicked();
	void OnUploadClicked();
	void OnCancelClicked();
	void OnTimer();

	void accept();
	void reject();

    void OnLinkDisconnected( PvDevice *aDevice );

private:

	QGroupBox *mFileGroupBox;
	QComboBox *mFileComboBox;
	QGroupBox *mTransferGroupBox;
	QPushButton *mDownloadButton;
	QPushButton *mUploadButton;
	QProgressBar *mProgressBar;
	QLabel *mProgressLabel;
	QPushButton *mCancelButton;
	QTimer *mTimer;

    PvDevice *mDevice;
	PvGenParameterArray *mParameters;
	TransferThread *mThread;

};

