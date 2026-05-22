#ifndef IMAGEMETADATA_H
#define IMAGEMETADATA_H

#include <QDateTime>
#include <QString>
#include <xiCoreGlobal.h>
#include <PicBuf.h>
#include <HsiCameraParams.h>
#include <QMap>

class CxCameraParameters;
class QDomDocument;
class QDomElement;

enum ExSequenceType {
    exseqtypeDisabled   = 0,            //!< no sub-sequence
    exseqtypeGeneric    = 1,            //!< some generic sub-sequence of images
    exseqtypeExposures  = 2,            //!< HDR sequence, exponentially increasig exposure time
    exseqtypeExposuresWithReset = 3,    //!< HDR, but the 1st image in sequence is "reset image"
};

//! \brief Metadata stored next to image pixel data
class XICORE_API CxImageMetadata
{
    // camera properties
public:
    QString m_sCameraModel;         //!< String got from XI_PRM_DEVICE_NAME
    int m_iCameraModelId;           //!< Integer got from XI_PRM_DEVICE_MODEL_ID
    QString m_sCameraSerialNumber;  //!< String got from XI_PRM_DEVICE_SN
    QString m_sSensorSerialNumber;  //!< String got from XI_PRM_DEVICE_SENS_SN
    QString m_sCameraUserName;      //!< String got from XI_PRM_DEVICE_USER_ID
    QString m_sApiContextList;      //!< String got from XI_PRM_API_CONTEXT_LIST

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
    ExColorFilterArray m_eColorFilterArray; //!< Type of Bayer matrix, excfaNone when not RAW image
    QStringList m_lstChannelNames;          //!< Optional list of channel names to give the channel a description; used only in special cases (2 channels, float image,...)
    uint m_uTransportDataFormat;            //!< The GenTL image format on transport layer (see \ref XI_GenTL_Image_Format_e in \ref xiApi.h).
	uint m_uDataFormat;						//!< Current format of pixels. (see \ref XI_GenTL_Image_Format_e in \ref xiApi.h). 
	uint m_uUserSet;						//!< User Set selected by XI_PRM_USER_SET_SELECTOR
    CxHsiCameraParams m_aHsiParams;         //!< Hyper Spectral Image information
    QDateTime m_aAcqDateTime;               //!< Date and time of exposure start	
    float m_fModulationFrequency;           //!< Value of XI_PRM_TOF_MODULATION_FREQUENCY
    int m_iHorizontalSectionsCount;         //!< Number of logical sections (phases) inside one image along Y axis (O when not present)
    int m_iToFPhaseAngle;                   //!< ToF phase angle (in degrees)
    int m_iToFPhaseIdx;                     //!< ToF phase idx among m_iToFTotalPhasesCount (used to identify all available phases of same scene)
    int m_iToFTotalPhasesCount;             //!< Number of total ToF phases camera produced
    float m_fToFTapWidth;                   //!< Size of tap width for ToF cameras with X1X2X3 sensors (in seconds)
    int m_iSequenceIdx;                     //!< Number of this frame inside the sequence as defined in m_iSequenceLength, i.e. it is not a frame counter (see xiApi: sequence_image_id)
    int m_iSequenceLength;                  //!< Total number of frames in the sequence, e.g. repeated HDR sequence coming from camera (0 for N/A, see xiApi: sequence_length)
    ExSequenceType m_eSequenceType;         //!< Type of the repeated sequence coming from camera (e.g. HDR)

public:
    CxImageMetadata();
    bool isColorCorrMatrixEmpty() const;

    void fillFromCameraSettings(CxCameraParameters* pPars, bool bForceReadContextList = false);  //!< Reads metadata values from current (cached) camera parameters
    void updateFromPicBufInfo(const SxPicBufInfo *pInfo);
    void replaceContextListVariable(const QString &sVariable, quint32 uiNewValue);      //!< Replaces value of a given parameter (e.g. 'xiApiImg:ActiveAreaWidth') in \ref m_sApiContextList
    static bool genTLFormatFromParams(uint &uiGenTLFormat, ExColorFilterArray eCfa, quint32 uiBpc, bool bPacked);

    // storage
public:
    void saveToXml(QDomDocument *pXmlDoc, QDomElement *pRootElement) const;   //!< Saves all values to XML document
    bool loadFromXml(QDomElement *pRootElement);        //!< Loads all values from XML document

    QString saveToXmlString() const;                    //!< Calls saveToXml, and saves the XML doc to string
    bool loadFromXmlString(const QString &sString);     //!< Loads all values from XML doc stored in a string

    // XML helper
public:
    static void saveTag(QDomDocument *pXmlDoc, QDomElement *pRootElement, const QString &sTag, const QString &sVal);
};

#endif // IMAGEMETADATA_H
