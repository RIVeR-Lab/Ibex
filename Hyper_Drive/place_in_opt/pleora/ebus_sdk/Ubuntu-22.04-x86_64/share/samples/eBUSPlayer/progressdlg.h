// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QMutex>
#include <QThread>
#include <QTimer>

#include <PvDevice.h>

#include <IProgress.h>
#include <Task.h>


class TaskThread;


class ProgressDlg : public QDialog, public IProgress
{
    Q_OBJECT

public:

    ProgressDlg( QWidget* aParent );

    virtual ~ProgressDlg();

    int exec();
	virtual void paintEvent(QPaintEvent * event);

	// IProgress interface
    int RunTask( Task *aTask );
    void SetStatus( const std::string &aStatus );
    void SetCancellable( bool aCancellable );
    void ShowWarning( const std::string &aStatus );
    void SetTitle( const PvString &aTitle ) { mTitle = aTitle.GetAscii(); }
    bool IsCancelled() const { return mCancelled; }

signals:

    void UpdateCancellableSignal( bool aCancellable );

protected slots:

	void OnTimer();
    void OnCancelClicked();
    void UpdateCancellableSlot( bool aCancellable );

protected:

    void CreateLayout();
    void CreateTimer();
    void Update();

	QLabel *mStatusLabel;
    QPixmap *mWheel;
	QTimer *mTimer;
    QPushButton *mCancelButton;

    unsigned long mWheelIndex;

private:

    TaskThread *mThread;
    Task *mTask;

	QString mStatus;
    QString mTitle;
    QMutex mMutex;

    bool mCancellable;
    bool mCancelled;
};

