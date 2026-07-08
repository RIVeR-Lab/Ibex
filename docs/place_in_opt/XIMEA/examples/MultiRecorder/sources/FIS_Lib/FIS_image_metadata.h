#ifndef FISIMAGEMETADATA_H
#define FISIMAGEMETADATA_H

#include <string>
#include <xiApi.h>

class TiXmlElement;

class CxFisImageMetadata
{
public:
    CxFisImageMetadata();
    void readFromCamera(HANDLE hCamera);
    void storeToXml(TiXmlElement *pRootElement);
    void loadFromXml(TiXmlElement *pRootElement);
    bool isColorCorrMatrixEmpty() const;

    // camera properties
public:
    std::string m_sCameraModel;         //!< String got from XI_PRM_DEVICE_NAME
    int m_iCameraModelId;               //!< Integer got from XI_PRM_DEVICE_MODEL_ID
    std::string m_sCameraSerialNumber;  //!< String got from XI_PRM_DEVICE_SN
    std::string m_sSensorSerialNumber;  //!< String got from XI_PRM_DEVICE_SENS_SN
    std::string m_sCameraUserName;      //!< String got from XI_PRM_DEVICE_USER_ID
    std::string m_sApiContextList;      //!< String got from XI_PRM_API_CONTEXT_LIST

    std::string cameraDisplayName();

    // camera settings
public:
    bool m_bAutoExposure;   //!< Is auto exposuse switched on?
    int m_iExposure;        //!< In microseconds - as in xiApi
    float m_fGain;          //!< Gain in dB, as in xiApi
    bool m_bAutoWB;         //!< Is AWB switched on?
    float m_fWhiteR, m_fWhiteG, m_fWhiteB;  //!< White balance coefficients
    float m_fColorCorrMatrix[16];           //!< Current color correction matrix
    float m_fGammaY, m_fGammaC, m_fSharpness;   //!< Other color correction parameters
    int m_iSensorTaps;                      //!< Number of sensor taps used when capturing the image
    int m_iDownsamplingType;                //!< Value of XI_PRM_DOWNSAMPLING_TYPE
    float m_fLensAperture, m_fLensFocalLength;  //!< Lens parameters (when present, 0 for none)
    float m_fTemp;							//!< Temperature in deg Celsius
    XI_COLOR_FILTER_ARRAY m_eColorFilterArray;  //!< Type of Bayer matrix, excfaNone when not RAW image
    int m_uTransportDataFormat;             //!< The GenTL image format on transport layer (see \ref XI_GenTL_Image_Format_e in \ref xiApi.h).
    int m_uDataFormat;						//!< Current format of pixels. (see \ref XI_GenTL_Image_Format_e in \ref xiApi.h).
    int m_uUserSet;                         //!< User Set selected by XI_PRM_USER_SET_SELECTOR
    time_t m_timeAcqDateTime;               //!< Acquisition start

protected:
    void saveTag(TiXmlElement *pRootElement, const std::string &sTag, const std::string &sVal);
};

#endif // CXIMAGEMETADATA_H
