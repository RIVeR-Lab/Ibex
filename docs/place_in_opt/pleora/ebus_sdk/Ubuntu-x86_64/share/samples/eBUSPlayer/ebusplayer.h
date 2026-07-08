// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QMutex>
#include <QEvent>
#include <QTime>
#include <QTimer>

#include <Player.h>

#include <PvGenBrowserWnd.h>
#include <PvSerialTerminalWnd.h>


#define WM_STARTTIMER ( WM_USER + 0x7000 )
#define WM_STOPTIMER ( WM_USER + 0x7001 )


class SetupDlg;
class EventMonitorDlg;
class ActionCommandDlg;
class FilteringDlg;
class ImageSaveDlg;
class QWindowsEvent;

// forward declare (for now)
class eBUSPlayer;

class OpenRecent : public QObject
{
    Q_OBJECT

public:

	QString mFileName;
	QAction* mFileOpenRecentAction;
	eBUSPlayer* mEBUSPlayer;

public slots:

    void OnFileOpenRecentAction();

};


class eBUSPlayer : public QMainWindow, IPlayerController
{
    Q_OBJECT

public:

    eBUSPlayer( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
    virtual ~eBUSPlayer();
    
    void SetFileName( const QString &aFileName ) { mFileName = aFileName; }
    void OpenConfig( const QString &aLocation );

    virtual void CreateLayout();
    virtual QString GetAppName() const { return "eBUS Player"; }

protected slots:

    void OnConnectButton();
    void OnDisconnectButton();
    void OnStart();
    void OnStop();
    void OnCbnSelchangeMode( int aIndex );
    void OnFileMenuAboutToShow();
    void OnToolsMenuAboutToShow();
    void OnHelpMenuAboutToShow();
    void OnFileOpenAction();
    void OnFileSaveAction();
    void OnFileSaveAsAction();
    void OnFileExitAction();
    void OnToolsSetupAction();
    void OnToolsEventMonitorAction();
    void OnToolsSerialAction();
    void OnToolsActionCommandAction();
    void OnToolsFilteringAction();
    void OnToolsDisplayOptionsAction();
    void OnToolsBufferOptionsAction();
    void OnToolsResetStreamingStatisticsAction();
    void OnToolsSaveImagesAction();
    void OnToolsSaveCurrentImageAction();
    void OnHelpAboutAction();
    void OnShowCommParameters();
    void OnShowDeviceParameters();
    void OnShowStreamParameters();
    void OnTimer();
    void OnRegisterInterface();
    void OnToolsRestoredefaultpreferences();
    void OnToolsSavepreferences();
    void OnToolsSaveXML();
    void OnToolsDeinterlacingDisabledAction();
    void OnToolsDeinterlacingWeavingHalfAction();
    void OnToolsDeinterlacingWeavingFullAction();
    void OnToolsDeinterlacingBlendingAction();
    void OnToolsDeinterlacingLineDoublingAction();
    void OnCbnSelchangeSource( int aIndex );
    void OnToolsGEVCommunicationParametersAction();
    void OnToolsU3VCommunicationParametersAction();
    void OnToolsFileTransferAction();

protected:

    enum StatusColor { SCDefault, SCRed, SCYellow };

    QGroupBox *CreateConnectGroup();
    QGroupBox *CreateAcquisition();
    QGroupBox *CreateParameters();
    QGroupBox *CreateDisplay();
    void CreateMenu();

    bool event( QEvent *aEvent );

    void SetStatusColor( StatusColor aColor );
    void ShowGenWindow( PvGenBrowserWnd *aWnd, PvGenParameterArray *aArray, const QString &aTitle );
    void CloseGenWindow( PvGenBrowserWnd *aWnd );

    void EnableInterface();
    void EnableControls( bool aEnabled );
    void EnableGenBrowsers( bool aEnabled );

	virtual void Connect( const PvDeviceInfo *aDI, PvConfigurationReader *aCR = NULL );
	virtual void Disconnect();
    void PreDisconnectUI();

    void ReportMRU( const QString &aFileName );
    void SaveMRUToRegistry();
    void LoadMRUFromRegistry();
    void UpdateMRUMenu();

    // Events used to bring async code back in main thread context
    void OnLinkDisconnected( QWindowsEvent *aEvent );
    void OnImageDisplayed( QWindowsEvent *aEvent );
	void OnUpdateSource( QWindowsEvent *aEvent );
	void OnUpdateSources( QWindowsEvent *aEvent );
    void OnUpdateAcquisitionModes( QWindowsEvent *aEvent );
	void OnUpdateAcquisitionMode( QWindowsEvent *aEvent );
	void OnAcquisitionStateChanged( QWindowsEvent *aEvent );
	void OnStartTimer( QWindowsEvent *aEvent );
	void OnStopTimer( QWindowsEvent *aEvent );

    void SaveConfig( const QString &aLocation, bool aSaveConnectedDevice );

    void SetPreferences();
    QString GetDefaultPath();
    QString GetStickyPath();
    QString GetAppDataPath();

	QString GetErrorString();

    intptr_t SendMsgIfPossible( uint32_t Msg, uintptr_t wParam = 0, intptr_t lParam = 0 );

    // Player callback interface
    intptr_t PostMsg( uint32_t Msg, uintptr_t wParam = 0, intptr_t lParam = 0 );
    intptr_t SendMsg( uint32_t Msg, uintptr_t wParam = 0, intptr_t lParam = 0 );
	void StartTimer();
	void StopTimer();
	IProgress *CreateProgressDialog();
    void AddDeviceParameterArray( PvGenParameterArray *aArray, const PvString &aName ) {}
    void RemoveDeviceParameterArray( PvGenParameterArray *aArray ) {}

    void CreateEventMonitor();
    void UpdateDeviceAttributes();

    // UI
    QPushButton *mConnectButton;
    QPushButton *mDisconnectButton;
    QLineEdit *mIpAddressLine;
    QLineEdit *mMacAddressLine;
    QLineEdit *mGUIDLine;
    QLineEdit *mManufacturerLine;
    QLineEdit *mModelLine;
    QLineEdit *mNameLine;
    QComboBox *mModeCombo;
    QComboBox *mSourceCombo;
    QPushButton *mStartButton;
    QPushButton *mStopButton;
    QPushButton *mCommunicationButton;
    QPushButton *mDeviceButton;
    QPushButton *mStreamButton;
    IPvDisplayAdapter *mDisplay;

    QTextEdit *mStatusLine;
	QTimer *mTimer;

    // Menu
    QMenu *mFileMenu;
    QAction *mFileOpenAction;
    QMenu *mFileOpenRecentMenu;
    QAction *mFileSaveAction;
    QAction *mFileSaveAsAction;
    QAction *mFileExitAction;
    QMenu *mToolsMenu;
    QAction *mToolsSetupAction;
    QAction *mToolsEventMonitorAction;
    QAction *mToolsSerialAction;
    QAction *mToolsActionCommandAction;
    QAction *mToolsFilteringAction;
    QAction *mToolsDisplayOptionsAction;
    QAction *mToolsBufferOptionsAction;
    QAction *mToolsResetStreamingStatisticsAction;
    QAction *mToolsSaveImagesAction;
    QAction *mToolsSaveCurrentImageAction;
    QAction *mToolsSaveXMLAction;
    QAction *mToolsDeinterlacingDisabled;
    QAction *mToolsDeinterlacingWeavingHalf;
    QAction *mToolsDeinterlacingWeavingFull;
    QAction *mToolsDeinterlacingBlending;
    QAction *mToolsDeinterlacingLineDoubling;
    QAction *mToolsSavePreferencesAction;
    QAction *mToolsRestoreDefaultAction;
    QAction *mToolsGEVCommunicationParametersAction;
    QAction *mToolsU3VCommunicationParametersAction;
    QAction *mToolsFileTransferAction;
    QMenu *mHelpMenu;
    QAction *mHelpAboutAction;

    OpenRecent mOpenRecent[10];

    SetupDlg* mSetupDlg;
    EventMonitorDlg *mEventMonitorDlg;
    ActionCommandDlg *mActionCommandDlg;
    FilteringDlg* mFilteringDlg;
    StatusColor mStatusColor;
    PvSerialTerminalWnd mSerialTerminalWnd;

	PvGenBrowserWnd *mDeviceWnd;
	PvGenBrowserWnd *mCommunicationWnd;
	PvGenBrowserWnd *mStreamWnd;

	PvGenBrowserWnd mDefaultCommGEVParametersWnd;
    PvGenBrowserWnd mDefaultCommU3VParametersWnd;

	QString mFileName;
	std::list<QString> mRecentList;

	bool mShowAcquisitionWarning;

    bool mHasLayout;

    Player *mPlayer;

};
