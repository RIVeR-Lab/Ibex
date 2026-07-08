#include "FIS_image_metadata.h"

#include "../tinyxml/tinyxml.h"

CxFisImageMetadata::CxFisImageMetadata()
{
    m_iCameraModelId = 0;
    m_bAutoExposure = false;
    m_iExposure = 0;
    m_fGain = 0.0f;
    m_bAutoWB = false;
    m_fWhiteR = m_fWhiteG = m_fWhiteB = 1.0f;
    memset(m_fColorCorrMatrix, 0, 16*sizeof(float));
    m_fGammaY = m_fGammaC = 0;
    m_fSharpness = 0.0f;
    m_iSensorTaps = -1;
    m_iDownsamplingType = -1;
    m_fLensAperture = 0.0f;
    m_fLensFocalLength = 0.0f;
    m_fTemp = 0.0f;
    m_uUserSet = 0;
    m_eColorFilterArray = XI_CFA_NONE;
    m_uTransportDataFormat = 0;
    m_uDataFormat = 0;
    m_timeAcqDateTime = 0;
}

//----------------------------------------------------------------------------------------
bool CxFisImageMetadata::isColorCorrMatrixEmpty() const
{
    float fOther[16];
    memset(fOther, 0, 16*sizeof(float));
    return memcmp(m_fColorCorrMatrix, fOther, 16*sizeof(float))==0;
}

//----------------------------------------------------------------------------------------
void CxFisImageMetadata::readFromCamera(HANDLE hCamera)
{
    if (hCamera == nullptr)
        return;

    char szStr[512] = {0,};
    if (xiGetParamString(hCamera, XI_PRM_DEVICE_NAME, szStr, (DWORD)sizeof(szStr)) == XI_OK)
        m_sCameraModel = szStr;
    if (xiGetParamString(hCamera, XI_PRM_DEVICE_SN, szStr, (DWORD)sizeof(szStr)) == XI_OK)
        m_sCameraSerialNumber = szStr;
    if (xiGetParamString(hCamera, XI_PRM_DEVICE_SENS_SN, szStr, (DWORD)sizeof(szStr)) == XI_OK)
        m_sSensorSerialNumber = szStr;
    if (xiGetParamString(hCamera, XI_PRM_DEVICE_USER_ID, szStr, (DWORD)sizeof(szStr)) == XI_OK)
        m_sCameraUserName = szStr;
    xiGetParamInt(hCamera, XI_PRM_DEVICE_MODEL_ID, &m_iCameraModelId);

    int iVal;
    if (xiGetParamInt(hCamera, XI_PRM_AEAG, &iVal) == XI_OK)
        m_bAutoExposure = (iVal != 0);
    xiGetParamInt(hCamera, XI_PRM_EXPOSURE, &m_iExposure);
    xiGetParamFloat(hCamera, XI_PRM_GAIN, &m_fGain);
    if (xiGetParamInt(hCamera, XI_PRM_AUTO_WB, &iVal) == XI_OK)
        m_bAutoWB = (iVal != 0);
    xiGetParamFloat(hCamera, XI_PRM_WB_KR, &m_fWhiteR);
    xiGetParamFloat(hCamera, XI_PRM_WB_KG, &m_fWhiteG);
    xiGetParamFloat(hCamera, XI_PRM_WB_KB, &m_fWhiteB);
    xiGetParamFloat(hCamera, XI_PRM_CC_MATRIX_00, &m_fColorCorrMatrix[0]);
    xiGetParamFloat(hCamera, XI_PRM_CC_MATRIX_01, &m_fColorCorrMatrix[1]);
    xiGetParamFloat(hCamera, XI_PRM_CC_MATRIX_02, &m_fColorCorrMatrix[2]);
    xiGetParamFloat(hCamera, XI_PRM_CC_MATRIX_03, &m_fColorCorrMatrix[3]);
    xiGetParamFloat(hCamera, XI_PRM_CC_MATRIX_10, &m_fColorCorrMatrix[4]);
    xiGetParamFloat(hCamera, XI_PRM_CC_MATRIX_11, &m_fColorCorrMatrix[5]);
    xiGetParamFloat(hCamera, XI_PRM_CC_MATRIX_12, &m_fColorCorrMatrix[6]);
    xiGetParamFloat(hCamera, XI_PRM_CC_MATRIX_13, &m_fColorCorrMatrix[7]);
    xiGetParamFloat(hCamera, XI_PRM_CC_MATRIX_20, &m_fColorCorrMatrix[8]);
    xiGetParamFloat(hCamera, XI_PRM_CC_MATRIX_21, &m_fColorCorrMatrix[9]);
    xiGetParamFloat(hCamera, XI_PRM_CC_MATRIX_22, &m_fColorCorrMatrix[10]);
    xiGetParamFloat(hCamera, XI_PRM_CC_MATRIX_23, &m_fColorCorrMatrix[11]);
    xiGetParamFloat(hCamera, XI_PRM_CC_MATRIX_30, &m_fColorCorrMatrix[12]);
    xiGetParamFloat(hCamera, XI_PRM_CC_MATRIX_31, &m_fColorCorrMatrix[13]);
    xiGetParamFloat(hCamera, XI_PRM_CC_MATRIX_32, &m_fColorCorrMatrix[14]);
    xiGetParamFloat(hCamera, XI_PRM_CC_MATRIX_33, &m_fColorCorrMatrix[15]);
    xiGetParamFloat(hCamera, XI_PRM_GAMMAY, &m_fGammaY);
    xiGetParamFloat(hCamera, XI_PRM_GAMMAC, &m_fGammaC);
    xiGetParamFloat(hCamera, XI_PRM_SHARPNESS, &m_fSharpness);
    xiGetParamInt(hCamera, XI_PRM_SENSOR_TAPS, &m_iSensorTaps);
    xiGetParamInt(hCamera, XI_PRM_DOWNSAMPLING_TYPE, &m_iDownsamplingType);
    xiGetParamFloat(hCamera, XI_PRM_LENS_APERTURE_VALUE, &m_fLensAperture);
    xiGetParamFloat(hCamera, XI_PRM_LENS_FOCAL_LENGTH, &m_fLensFocalLength);
    xiGetParamFloat(hCamera, XI_PRM_TEMP, &m_fTemp);
    xiGetParamInt(hCamera, XI_PRM_COLOR_FILTER_ARRAY, (int*)&m_eColorFilterArray);
    xiGetParamInt(hCamera, XI_PRM_TRANSPORT_PIXEL_FORMAT, &m_uDataFormat);
    xiGetParamInt(hCamera, XI_PRM_USER_SET_SELECTOR, &m_uUserSet);
}

//----------------------------------------------------------------------------------------
void CxFisImageMetadata::storeToXml(TiXmlElement *pRootElement)
{
    saveTag(pRootElement, "cameraModel", m_sCameraModel);
    saveTag(pRootElement, "cameraModelId", std::to_string(m_iCameraModelId));
    saveTag(pRootElement, "cameraSerialNumber", m_sCameraSerialNumber);
    saveTag(pRootElement, "sensorSerialNumber", m_sSensorSerialNumber);
    saveTag(pRootElement, "cameraUserName", m_sCameraUserName);
    saveTag(pRootElement, "apiContextList", m_sApiContextList);
    saveTag(pRootElement, "autoExposure", m_bAutoExposure ? "1" : "0");
    saveTag(pRootElement, "exposure", std::to_string(m_iExposure));
    saveTag(pRootElement, "gain", std::to_string(m_fGain));
    saveTag(pRootElement, "autoWB", m_bAutoWB ? "1" : "0");
    saveTag(pRootElement, "whiteR", std::to_string(m_fWhiteR));
    saveTag(pRootElement, "whiteG", std::to_string(m_fWhiteG));
    saveTag(pRootElement, "whiteB", std::to_string(m_fWhiteB));
    saveTag(pRootElement, "gammaY", std::to_string(m_fGammaY));
    saveTag(pRootElement, "gammaC", std::to_string(m_fGammaC));
    saveTag(pRootElement, "sharpness", std::to_string(m_fSharpness));
    saveTag(pRootElement, "sensorTaps", std::to_string(m_iSensorTaps));
    saveTag(pRootElement, "downsamplingType", std::to_string(m_iDownsamplingType));
    saveTag(pRootElement, "lensAperture", std::to_string(m_fLensAperture));
    saveTag(pRootElement, "lensFocalLength", std::to_string(m_fLensFocalLength));
    saveTag(pRootElement, "temp", std::to_string(m_fTemp));
    saveTag(pRootElement, "xi_cfa", std::to_string((int)m_eColorFilterArray));
    saveTag(pRootElement, "transportDataFormat", std::to_string(m_uTransportDataFormat));
    saveTag(pRootElement, "dataFormat", std::to_string(m_uDataFormat));
    saveTag(pRootElement, "userSet", std::to_string(m_uUserSet));
    saveTag(pRootElement, "iAcqDateTime", std::to_string(m_timeAcqDateTime));
    if (!isColorCorrMatrixEmpty())
    {
        for (int i = 0; i < 16; i++)
            saveTag(pRootElement, std::string("ccmtrx") + std::to_string(i), std::to_string(m_fColorCorrMatrix[i]));
    }
}

//-------------------------------------------------------------
void CxFisImageMetadata::saveTag(TiXmlElement *pRootElement, const std::string &sTag, const std::string &sVal)
{
    TiXmlElement *pEl = new TiXmlElement(sTag);
    pEl->LinkEndChild(new TiXmlText(sVal));
    pRootElement->LinkEndChild(pEl);
}

//----------------------------------------------------------------------------------------
void CxFisImageMetadata::loadFromXml(TiXmlElement *pRootElement)
{
    TiXmlElement *pElParam = pRootElement->FirstChildElement();
    while (pElParam != nullptr)
    {
        const char *szName = pElParam->Value();
        const char *szTextValue = pElParam->GetText();

        if (szName != nullptr && szTextValue != nullptr)
        {
            if (strcmp(szName, "cameraModel") == 0) m_sCameraModel = szTextValue;
            else if (strcmp(szName, "cameraModelId") == 0) m_iCameraModelId = (int)atoi(szTextValue);
            else if (strcmp(szName, "cameraSerialNumber") == 0) m_sCameraSerialNumber = szTextValue;
            else if (strcmp(szName, "sensorSerialNumber") == 0) m_sSensorSerialNumber = szTextValue;
            else if (strcmp(szName, "cameraUserName") == 0) m_sCameraUserName = szTextValue;
            else if (strcmp(szName, "apiContextList") == 0) m_sApiContextList = szTextValue;
            else if (strcmp(szName, "autoExposure") == 0) m_bAutoExposure = (strcmp(szTextValue, "1") == 0);
            else if (strcmp(szName, "exposure") == 0) m_iExposure = (int)atoi(szTextValue);
            else if (strcmp(szName, "gain") == 0) m_fGain = (float)atof(szTextValue);
            else if (strcmp(szName, "autoWB") == 0) m_bAutoWB = (strcmp(szTextValue, "1") == 0);
            else if (strcmp(szName, "whiteR") == 0) m_fWhiteR = (float)atof(szTextValue);
            else if (strcmp(szName, "whiteG") == 0) m_fWhiteG = (float)atof(szTextValue);
            else if (strcmp(szName, "whiteB") == 0) m_fWhiteB = (float)atof(szTextValue);
            else if (strcmp(szName, "gammaY") == 0) m_fGammaY = (float)atof(szTextValue);
            else if (strcmp(szName, "gammaC") == 0) m_fGammaC = (float)atof(szTextValue);
            else if (strcmp(szName, "sharpness") == 0) m_fSharpness = (float)atof(szTextValue);
            else if (strcmp(szName, "sensorTaps") == 0) m_iSensorTaps = (int)atoi(szTextValue);
            else if (strcmp(szName, "downsamplingType") == 0) m_iDownsamplingType = (int)atoi(szTextValue);
            else if (strcmp(szName, "lensAperture") == 0) m_fLensAperture = (float)atof(szTextValue);
            else if (strcmp(szName, "lensFocalLength") == 0) m_fLensFocalLength = (float)atof(szTextValue);
            else if (strcmp(szName, "temp") == 0) m_fTemp = (float)atof(szTextValue);
            else if (strcmp(szName, "xi_cfa") == 0) m_eColorFilterArray = (XI_COLOR_FILTER_ARRAY)atoi(szTextValue);
            else if (strcmp(szName, "transportDataFormat") == 0) m_uTransportDataFormat = (int)atoi(szTextValue);
            else if (strcmp(szName, "dataFormat") == 0) m_uDataFormat = (int)atoi(szTextValue);
            else if (strcmp(szName, "userSet") == 0) m_uUserSet = (int)atoi(szTextValue);
            else if (strcmp(szName, "iAcqDateTime") == 0)
            {
                char* end = nullptr;
                m_timeAcqDateTime = (time_t)std::strtoll(szTextValue, &end, 10);
            }
            else if (strlen(szName) > 6 && strncmp(szName, "ccmtrx", 6) == 0)
            {
                int idx = atoi(szName+6);
                if (idx >= 0 && idx < 16)
                    m_fColorCorrMatrix[idx] = (float)atof(szTextValue);
            }
        }
        pElParam = pElParam->NextSiblingElement();
    }
}

//----------------------------------------------------------------------------------------
std::string CxFisImageMetadata::cameraDisplayName()
{
    std::string sName = m_sCameraModel;
    if (!m_sCameraUserName.empty())
        sName += std::string(" (") + m_sCameraUserName + ")";
    return sName;
}
