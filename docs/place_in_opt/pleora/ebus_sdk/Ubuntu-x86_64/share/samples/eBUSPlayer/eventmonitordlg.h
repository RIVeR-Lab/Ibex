// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <map>

#include <QDialog>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QString>
#include <QTimer>

#include <LogBuffer.h>


class EventMonitorDlg : public QDialog
{
    Q_OBJECT

public:

	EventMonitorDlg( QWidget *aParent, LogBuffer *aLogBuffer );
	virtual ~EventMonitorDlg();

	void LoadData();

protected slots:

	void OnBnClickedSaveLogCheckBox();
	void OnBnClickedSaveLogButton();
	void OnBnClickedClearButton();
	void OnBnClickedGenICamCheckBox();
	void OnBnClickedEventsCheckBox();
	void OnBnClickedBufferAllCheckBox();
	void OnBnClickedChunkCheckBox();
	void OnBnClickedBufferErrorCheckBox();
	void OnBnClickedLogSinkCheckBox();
	void OnBnClickedSerialComLogCheckBox();
	void OnTimer();

protected:

	void CreateLayout();
	void EnableInterface();

private:

	QCheckBox* mSaveLogCheckBox;
	QLineEdit* mSaveLogLineEdit;
	QPushButton* mSaveLogButton;
	QTextEdit* mLogEdit;
	QPushButton* mClearLogButton;
	QCheckBox* mGenICamCheckBox;
	QCheckBox* mEventsCheckBox;
	QCheckBox* mBufferErrorCheckBox;
	QCheckBox* mBufferAllCheckBox;
	QCheckBox* mChunkCheckBox;
	QCheckBox* mLogSinkCheckBox;
	QCheckBox* mSerialComLogCheckBox;

	QTimer *mTimer;

	int mLogEditTextLength;
	int mLastUpdateTime;

	bool mNeedInit;

    LogBuffer *mLogBuffer;

};

