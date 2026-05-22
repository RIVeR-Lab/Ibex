#ifndef UTIL_H
#define UTIL_H

#include <ImageMetadata.h>
#include <xiApi.h>

class CxImageMetadata;

XICORE_API uint xiCore_GetChannelsCountForXiImageFormat(XI_IMG_FORMAT eFormat);

XICORE_API ExImageDataType xiCore_GetImageDataTypeForXiImageFormat(XI_IMG_FORMAT eFormat, uint uiTransportFormatBpc = 0);

XICORE_API ExDataStorageFormat xiCore_GetDataStorageFormatForXiImageFormat(XI_IMG_FORMAT eFormat);

XICORE_API ExColorFilterArray xiCore_ColorFilterArrayFromXiCfa(XI_COLOR_FILTER_ARRAY eCfa);

XICORE_API XI_COLOR_FILTER_ARRAY xiCore_GetXiCfaFromColorFilterArray(ExColorFilterArray eCfa);

XICORE_API bool xiCore_IsRgbForXiImageFormat(XI_IMG_FORMAT eFormat);

XICORE_API XI_IMG_FORMAT xiCore_GetXiImgFormatForPicBufInfo(const SxPicBufInfo &picInfo);

XICORE_API XI_DATA_SM xiCore_GetXiDataSmFromDataType(ExImageDataType eDataType);

XICORE_API ExSequenceType xiCore_GetImageSeqTypeFromXiSequenceType(XI_IMAGE_SEQUENCE_TYPE eSequenceType);
XICORE_API XI_IMAGE_SEQUENCE_TYPE xiCore_GetXiImageSequenceTypeFromSeqType(ExSequenceType eSequenceType);

XICORE_API bool xiCore_SetXiImgFromPicBuf(XI_IMG &aXiImg, const SxPicBuf *pPic, const CxImageMetadata *pMetadata = nullptr);     //!< Helper function to get XI_IMG back from SxPicBuf

#endif // UTIL_H
