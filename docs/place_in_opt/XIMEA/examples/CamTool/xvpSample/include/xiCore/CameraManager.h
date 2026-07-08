#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include <QObject>
#include <QMap>
#include <QSet>
#include <QMutex>
#include <QSettings>
#include <xiCoreGlobal.h>
#include <PicBuf.h>
#include <CameraParameters.h>

class CxRemoteCamera;

/*!
 \brief The class that provides all necessary methods to access cameras and
 to set up camera parameters.
*/
class XICORE_API CxCameraManager : public QObject
{
    Q_OBJECT

private:
    //! Private constructor (the class is singleton).
    CxCameraManager();


public:

    //! Gets the instance if this object.
    static CxCameraManager* instance();

    //! Destructor.
    ~CxCameraManager();

    //! Sets API parameters even before any camera is opened
    void setGeneralApiParameters() const;

    //! Gets the number of available cameras connected to PC.
    uint cameraCount() const;

    typedef enum
    {
        SENSOR_INIT_EN               =0, // sensor init enabled
        SENSOR_INIT_DIS_ALL          =1, // sensor init disabled for all cameras
        SENSOR_INIT_DIS_SPECSN       =2, // sensor init disabled for specified cameras

    } SENSOR_INIT;

    /*!
     \brief Gets the info about camera with given index \p uiCameraIdx.

     \remarks The index \p uiCameraIdx is related to cameras count returned
     by the \ref cameraCount() method (NOT by the \ref openedCameraCount() method!).
    */
    bool cameraInfo(uint uiCameraIdx, SxCameraInfo &aInfo) const;
    bool cameraName(uint uiCameraIdx, QString &sName) const;
    bool cameraSerialNumber(uint uiCameraIdx, QString &sSerialNumber) const;
    bool overrideCameraSNto(uint uiCameraIdx, QString &sNewSerialNumber);

    //!Opens a camera with given index and returns its handle.
    XICORE_HANDLE openCamera(uint uiCameraIdx, SxCameraInfo *pPreloadedInfo = NULL);

    //! Gets the number of opened cameras.
    uint openedCameraCount();

    //! Gets a list of handles to the opened cameras.
    QList<XICORE_HANDLE> openedCameras();

    bool isCameraOpened(XICORE_HANDLE hCamera);

    bool closeCamera(XICORE_HANDLE hCamera);
    bool startAcquisition(XICORE_HANDLE hCamera);
    bool stopAcquisition(XICORE_HANDLE hCamera);

    uint runningCameraCount();
    bool isCameraRunning(XICORE_HANDLE hCamera);
    QList<XICORE_HANDLE> runningCameras();

    //!  Gets the camera parameters.
    CxCameraParameters* cameraParameters(XICORE_HANDLE hCamera);

    //! Reads HSI calibration XML from camera
    bool readCameraHsiParams(XICORE_HANDLE hCamera, const SxCameraInfo &info, CxHsiCameraParams *pHsiParams) const;

    /*!
     \brief Call this method to notify the rest of the application that the camera
     format (image dimensions, components, ...) are to be changed.
    */
    void notifyCameraFormatChanging(XICORE_HANDLE hCamera);

    /*!
     \brief Call this method to notify rest of the application that the camera format
     (image dimensions, components, ...) has been changed.
    */
    void notifyCameraFormatChanged(XICORE_HANDLE hCamera);


    //! \brief Call this method to notify rest of the application that the camera just disconnected    */
    void notifyCameraLost(XICORE_HANDLE hCamera);

    //! \brief Call this method to notify rest of the application that the camera acquisition is failing  */
    void notifyCameraAcqError(XICORE_HANDLE hCamera, int iErrorCode, const QString &sInFunction);

    //! For internal use, subscribe to \ref imageHeaderReceived signal
    void notifyCameraImageHeaderReceived(XICORE_HANDLE hCamera, const QByteArray &baXiImgHeader);

    //! Gets the picbuf info currently produced by the given camera.
    bool cameraPicbufInfo(XICORE_HANDLE hCamera, SxPicBufInfo &picInfo);

    //! Gets the camera handle by the serial id of the camera.
    XICORE_HANDLE cameraHandleBySerialId(const QString &sSerialId);

    //! State of XI_PRM_DEBUG_LEVEL
    int apiDebugLevel() const;
    void setApiDebugLevel(int iLevel);
    //! State of XI_PRM_AUTO_BANDWIDTH_CALCULATION
    bool autoBandwidthCalculationEnabled() const;
    void enableAutoBandwidthCalculation(bool bEnable);
	//! State of XI_PRM_CAM_SENSOR_INIT_DISABLED
    int sensorInitDisabled() const;
    void setSensorInitDisabled(SENSOR_INIT eSensDisable);
    QString sensDisableForSN() const;
    void setSensDisableForSN(QString sCamSN);
	//! State of XI_PRM_TEST_PATTERN
	bool testPatternEnabled() const;
	void setTestPattern(bool bEnable);
    void enableTestPattern(XICORE_HANDLE hCamera);
	void enableTestPatternCam(XICORE_HANDLE hCamera);

    void saveSettings(QSettings *pSettings);    //!< store some options to app settings
    void loadSettings(QSettings *pSettings);    //!< load some options from app settings

public:
    void createCameraSimulator();
    void createCameraSimulator(int iResWidth, int iResHeight);
    void disconnectCameraSimulator();

    void createHsiCameraSimulator();
    void createRgbNirCameraSimulator();
    void createToFCameraSimulator();

public:
    XICORE_HANDLE openRemoteCamera(const QString &sIPAddress, int iPort, const QString &sDeviceName, const QString &sSerial, bool bLogErrors = true);
    bool isRemoteCamera(XICORE_HANDLE hCamera);                 //!< Returns true when handle belons to remotely opened camera
    CxRemoteCamera* remoteCameraPtr(XICORE_HANDLE hCamera);     //!< Do not store the pointer, use Handle

public slots:
    /*!
     \brief Stops all cameras and closes them. This method should be called
     on appliction exit.
    */
    void finishAll();

signals:
    //! The signal is emitted when the acquistion on the given camera was started.
    void acquisitionStarted(XICORE_HANDLE hCamera);

    //! The signal is emitted when the acquistion on the given camera was finished.
    void acquisitionFinished(XICORE_HANDLE hCamera);

    //! The signal is emitted when camera is opened.
    void cameraOpened(XICORE_HANDLE hCamera);

    /*!
     \brief The signal is emitted when the camera was closed.

     \remarks The camera handle \p hCamera is invalid in the moment of emitting of this signal,
     so any attempt to communitation with the camera fails.
    */
    void cameraClosed(XICORE_HANDLE hCamera);

    //! The signal is emitted when camera disconnected during app session.
    void cameraLost(XICORE_HANDLE hCamera);

    //! The signal is emitted when camera acquisition fails.
    void cameraAcqError(XICORE_HANDLE hCamera, int iErrorCode, const QString &sInFunction);

    //! The signal is emitted when format of camera image (dimension, components..) is to be changed.
    void imageFormatChanging(XICORE_HANDLE hCamera);

    //! The signal is emitted when format of the camera image (dimension, components..) changed.
    void imageFormatChanged(XICORE_HANDLE hCamera);

    //! The signal is emitted when new XI_IMG was received (XI_IMG passed in QByteArray, pointers in XI_IMG members are not valid).
    void imageHeaderReceived(XICORE_HANDLE hCamera, const QByteArray &baXiImgHeader);

private:
    QMap<XICORE_HANDLE, CxCameraParameters*> m_mapCameras;   /*!< Camera parameters by the camera handle */
    QSet<XICORE_HANDLE> m_setRunningCameras;                 /*!< Set of camera where the acquistion is running. */
    QMap<XICORE_HANDLE, CxRemoteCamera*> m_mapRemoteCameras; /*!< List of remote cameras with artificial camera handle. */
    int m_iLastRemoteCameraIdx;
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    QMutex m_lock;                                           /*!< The lock of the manager. */
#else
    QRecursiveMutex m_lock;                                  /*!< The lock of the manager. */
#endif
    XICORE_HANDLE m_hLastCameraLost;                         /*!< Do not emit the cameraLost signal more then once */
    XICORE_HANDLE m_hLastCameraAcqError;                     /*!< Do not emit the cameraAcqError signal more then once */
    QString m_sLastCameraAcqErrorFunc;
    QStringList m_lSensDisabledForSNs;

    // xiAPI options
    int m_iSensorInitDisabled;
	bool m_bEnableTestPattern;
    int m_iXiApiAutoBandwidthCalculation;   //!< Set XI_PRM_AUTO_BANDWIDTH_CALCULATION to ON
    int m_iXiApiDebugLevel;                 //!< Settings for XI_PRM_DEBUG_LEVEL

    bool initCameraAfterOpen(XICORE_HANDLE hCam, SxCameraInfo *pPreloadedInfo);
};

#endif // CAMERAMANAGER_H
