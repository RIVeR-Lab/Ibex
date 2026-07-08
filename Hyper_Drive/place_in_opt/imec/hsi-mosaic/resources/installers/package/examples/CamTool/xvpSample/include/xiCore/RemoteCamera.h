#ifndef REMOTECAMERA_H
#define REMOTECAMERA_H

#include <xiCoreGlobal.h>
#include <QString>
#include <QList>
#include <xiApi.h>

class CxRemoteCameraClient;
class CxHsiCameraParams;
struct SxCameraInfo;
class CxValue;
class TiXmlDocument;
class TiXmlElement;

class XICORE_API CxRemoteCamera
{
public:
    struct SxRemoteCameraDesc
    {
        QString m_sIPAddress;
        int m_iPort;
        QString m_sName;
        QString m_sSerial;
    };


    static QList<SxRemoteCameraDesc> listCamerasAtAddress(const QString &sIPAddress, int iPort, QString &sErrorCode);
    static CxRemoteCamera* openCamera(const QString &sIPAddress, int iPort, const QString &sDeviceName, const QString &sSerial, bool bLogErrors);

    // instances
protected:
    CxRemoteCameraClient *m_pCameraClient;
    bool m_bLogAllErrors;       // log all errors, even in all xiSetParam (default false)

    CxRemoteCamera();
    static void errorLogCallback(const std::string &sErrorMsg);

public:
    ~CxRemoteCamera();
    void remoteConnectionParameters(QString &sHostName, int &iPort) const;

    // function to mimic classic camera / xiApi commands implementation
public:
    bool getDeviceInfo(SxCameraInfo &aInfo);        // as in xiApi_GetDeviceInfo
    int closeDevice();
    int startAcquisition();
    int stopAcquisition();
    int getImage(unsigned uiTimeout, XI_IMG *pXiImg);
    int setParamInt(const char *szParam, const int iVal);
    int setParamInt64(const char *szParam, const quint64 uiVal);
    int setParamFloat(const char *szParam, const float fVal);
    int setParamString(const char *szParam, char *szVal, quint32 uiValSize);
    int getParamInt(const char *szParam, int &iVal);
    int getParamInt64(const char *szParam, quint64 &uiVal);
    int getParamFloat(const char *szParam, float &fVal);
    int getParamString(const char *szParam, QByteArray &sVal);
    int getParamUnknownType(const char *szParam, CxValue *pRet);
    int readHsiParams(const SxCameraInfo *pInfo, CxHsiCameraParams *pHsiParams);
    int isCameraHsi();

    // functions for LoopRecording
public:
    bool setLoopRecorderStorage(const QString &sFilePaths);
    bool loopRecordStorageCapacity(quint64 &uiSpace);
    bool loopRecordGoReady(int iFrameCount);
    bool loopRecordCancel();
    bool loopRecordedFrameData(int iIdx, void *pDstBuffer, quint64 uiDstBufferSize);   // just data in transport format
    bool loopRecordReleaseDataBuffers(bool bRemoveDataFiles);
};

#endif // REMOTECAMERA_H
