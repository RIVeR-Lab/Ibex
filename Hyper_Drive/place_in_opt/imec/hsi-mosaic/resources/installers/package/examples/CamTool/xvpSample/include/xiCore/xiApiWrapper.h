#ifndef XIAPIWRAPPER_H
#define XIAPIWRAPPER_H

#include <xiCoreGlobal.h>
#include <xiApi.h>

struct SxCameraInfo;
class CxHsiCameraParams;
class CxValue;

enum ExXiApiConstrantType
{
    eXI_DOWNSAMPLING_VALUE_ITEMS,
    eXI_TEST_PATTERN_GENERATOR_ITEMS,
    eXI_VERSION_ITEMS,
    eXI_TEST_PATTERN_ITEMS,
    eXI_DEC_PATTERN_ITEMS,
    eXI_BIN_PATTERN_ITEMS,
    eXI_BIN_SELECTOR_ITEMS,
    eXI_BIN_MODE_ITEMS,
    eXI_DEC_SELECTOR_ITEMS,
    eXI_SENSOR_TAP_CNT_ITEMS,
    eXI_BIT_DEPTH_ITEMS,
    eXI_DEBUG_LEVEL_ITEMS,
    eXI_IMG_FORMAT_ITEMS,
    eXI_COLOR_FILTER_ARRAY_ITEMS,
    eXI_BP_ITEMS,
    eXI_TRG_SOURCE_ITEMS,
    eXI_TRG_SELECTOR_ITEMS,
    eXI_TRG_OVERLAP_ITEMS,
    eXI_ACQ_TIMING_MODE_ITEMS,
    eXI_TRANSPORT_DATA_TARGET_MODE_ITEMS,
    eXI_GPI_SEL_CB_ITEMS,
    eXI_GPO_SEL_CB_ITEMS,
    eXI_GPI_MODE_ITEMS,
    eXI_GPI_SELECTOR_ITEMS,
    eXI_GPO_MODE_ITEMS,
    eXI_GPO_SELECTOR_ITEMS,
    eXI_LED_MODE_ITEMS,
    eXI_LED_SELECTOR_ITEMS,
    eXI_COUNTER_SELECTOR_ITEMS,
    eXI_TS_RST_MODE_ITEMS,
    eXI_TS_RST_SOURCE_ITEMS,
    eXI_PRM_TYPE_ITEMS,
    eXI_SWITCH_ITEMS,
    eXI_TEMP_SELECTOR_ITEMS,
    eXI_TEMP_CTRL_MODE_SELECTOR_ITEMS,
    eXI_TEMP_ELEMENT_SELECTOR_ITEMS,
    eXI_OUTPUT_DATA_PACKING_TYPE_ITEMS,
    eXI_DOWNSAMPLING_TYPE_ITEMS,
    eXI_EXPOSURE_TIME_SELECTOR_TYPE_ITEMS,
    eXI_INTERLINE_EXPOSURE_MODE_TYPE_ITEMS,
    eXI_GAIN_SELECTOR_TYPE_ITEMS,
    eXI_SHUTTER_TYPE_ITEMS,
    eXI_CMS_MODE_ITEMS,
    eXI_CMS_INTENT_ITEMS,
    eXI_OPEN_BY_ITEMS,
    eXI_LENS_FEATURE_ITEMS,
    eXI_SENSOR_FEATURE_SELECTOR_ITEMS,
    eXI_SENSOR_MODE_ITEMS,
    eXI_IMAGE_AREA_SELECTOR_ITEMS,
    eXI_SENSOR_OUTPUT_CHANNEL_COUNT_ITEMS,
    eXI_SENS_DEFFECTS_CORR_LIST_SELECTOR_ITEMS,
    eXI_ACQUISITION_STATUS_SELECTOR_ITEMS,
    eXI_DP_UNIT_SELECTOR_ITEMS,
    eXI_DP_PROC_SELECTOR_ITEMS,
    eXI_DP_PARAM_SELECTOR_ITEMS,
    eXI_DP_PARAM_VALUE_ITEMS,
    eXI_USER_SET_SELECTOR_ITEMS,
    eXI_DUAL_ADC_MODE_ITEMS,
    eXI_PROBE_SELECTOR_ITEMS,
    eXI_TOF_READOUT_MODE_ITEMS,
    eXI_DATA_SM_ITEMS,

    eXI_RET_ITEMS,
    eXI_PROC_RET_ITEMS
};

uint XICORE_API xiApi_GetNumberDevices();

bool XICORE_API xiApi_GetDeviceInfo(uint uiDevIdx, SxCameraInfo &info);

bool XICORE_API xiApi_SetDeviceInfoString(uint uiDevIdx, const char* sParam, QString &sValue);

bool XICORE_API xiApi_GetDeviceName(uint uiDevIdx, QString &sName);

bool XICORE_API xiApi_GetSerialNumber(uint uiDevIdx, QString &sSerialNumber);

XICORE_HANDLE XICORE_API xiApi_OpenDevice(uint uiDevIdx);

bool XICORE_API xiApi_CloseDevice(XICORE_HANDLE hDevice);

bool XICORE_API xiApi_StartAcquisition(XICORE_HANDLE hDevice);

bool XICORE_API xiApi_StopAcquisition(XICORE_HANDLE hDevice);

bool XICORE_API xiApi_GetImage(XICORE_HANDLE hDevice, uint uiTimeout, XI_IMG *aXiImg, XI_RETURN *piRetVal = NULL, bool bNotifyFailed = true);

bool XICORE_API xiApi_SetParamInt(XICORE_HANDLE hDevice, const char *szParam, const int iVal, XI_RETURN *piRetVal = NULL);

bool XICORE_API xiApi_SetParamInt64(XICORE_HANDLE hDevice, const char *szParam, const quint64 uiVal, XI_RETURN *piRetVal = NULL);

bool XICORE_API xiApi_SetParamFloat(XICORE_HANDLE hDevice, const char *szParam, const float fVal, XI_RETURN *piRetVal = NULL);

bool XICORE_API xiApi_SetParamString(XICORE_HANDLE hDevice, const char *szParam, char *szVal, quint32 uiValSize, XI_RETURN *piRetVal = NULL);

bool XICORE_API xiApi_GetParamInt(XICORE_HANDLE hDevice, const char *szParam, int &iVal, XI_RETURN *piRetVal = NULL);

bool XICORE_API xiApi_GetParamInt64(XICORE_HANDLE hDevice, const char *szParam, quint64 &uiVal, XI_RETURN *piRetVal = NULL);

bool XICORE_API xiApi_GetParamFloat(XICORE_HANDLE hDevice, const char *szParam, float &fVal, XI_RETURN *piRetVal = NULL);

bool XICORE_API xiApi_GetParamString(XICORE_HANDLE hDevice, const char *szParam, QByteArray &sVal, XI_RETURN *piRetVal = NULL);

bool XICORE_API xiApi_GetParamUnknownType(XICORE_HANDLE hDevice, const char *szParam, CxValue *pRet, XI_RETURN *piRetVal = NULL);

bool XICORE_API xiApi_FindConstantDescription(ExXiApiConstrantType eType, int iVal, QString *psName = NULL, QString *psDescription = NULL);

bool XICORE_API xiApi_FindErrorDescription(int iErrorCode, QString *psName = NULL, QString *psDescription = NULL);

bool XICORE_API xiApi_ReadHsiParams(XICORE_HANDLE hDevice, const SxCameraInfo &info, CxHsiCameraParams *pHsiParams, const QString &sFallbackXmlFolder);

bool XICORE_API xiApiHsi_IsCameraHsi(XICORE_HANDLE hDevice);

bool XICORE_API xiApiWrapper_EnableUnsupportedParamErrorLogging(bool bEnable = true);

#endif // XIAPIWRAPPER_H
