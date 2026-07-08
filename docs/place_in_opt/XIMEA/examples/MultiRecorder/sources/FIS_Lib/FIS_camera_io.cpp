#include "FIS_camera_io.h"

#ifndef XI_PRM_EXTENDED_FEATURE_SELECTOR
  #define XI_PRM_EXTENDED_FEATURE_SELECTOR  "ext_feature_selector"
  #define XI_PRM_EXTENDED_FEATURE           "ext_feature"
  #define XI_EXT_FEATURE_SEL_TRANSPORT_IMAGE_BUFFER_SIZE_ALIGNMENT_BYTES 21
#endif

bool FIS_setBufferAligningAndFrameSize(IxFisRecorder *pRecorder, HANDLE hCamera, std::string *psErrorMsg, int32_t *piFisDataAlign)
{
    if (pRecorder == nullptr || hCamera == nullptr)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = "Camera or FIS recorder not created.";
        return false;
    }

    XI_RETURN ret;
    int32_t iDataAlign = 0;
    int32_t iDataAlignRequest = 0;
    try
    {
        iDataAlignRequest = (int)pRecorder->requiredFrameSizeByteAligning();
    }
    catch (FIS_lib_Exception const &e)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = e.what();
        return false;
    }

    if (iDataAlignRequest > 0)
    {
        ret = xiSetParamInt(hCamera, XI_PRM_EXTENDED_FEATURE_SELECTOR, XI_EXT_FEATURE_SEL_TRANSPORT_IMAGE_BUFFER_SIZE_ALIGNMENT_BYTES);
        if (ret != XI_OK)
        {
            if (psErrorMsg != nullptr) *psErrorMsg = std::string("Error setting xiApi data alignment ext selector, xiApi returned error ") + std::to_string(ret);
        }
        else
        {
            int iMinAlignment = 4096;
            ret = xiGetParamInt(hCamera, XI_PRM_EXTENDED_FEATURE XI_PRM_INFO_MIN, &iMinAlignment);

            if (ret == XI_OK && iDataAlignRequest < iMinAlignment)
            {
                // align minimum to request (i.e. get next request multiple higher then min)
                iDataAlignRequest = ((iMinAlignment + iDataAlignRequest-1) / iDataAlignRequest) * iDataAlignRequest;
            }

            ret = xiSetParamInt(hCamera, XI_PRM_EXTENDED_FEATURE, iDataAlignRequest);
            if (ret == XI_OK)
                iDataAlign = iDataAlignRequest;
            else
            {
                if (psErrorMsg != nullptr) *psErrorMsg = std::string("Error setting xiApi data alignment, xiApi returned error ") + std::to_string(ret);
                //return false;
            }
        }
    }

    if (piFisDataAlign != nullptr) *piFisDataAlign = iDataAlign;

    // get the image data size
    int32_t iFrameSize = 0;
    ret = xiGetParamInt(hCamera, XI_PRM_IMAGE_PAYLOAD_SIZE, &iFrameSize);
    if (ret != XI_OK)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = std::string("Cannot get the xiApi payload size, xiApi returned error ") + std::to_string(ret);
        return false;
    }

    // we need to silently suppose the allocated buffers will be larger, as we requested from xiApi. Payload size may be smaller
    if (iDataAlign > 0 && (iFrameSize % iDataAlign) > 0)
    {
        iFrameSize = ((iFrameSize + iDataAlign-1) / iDataAlign) * iDataAlign;
    }

    try
    {
        pRecorder->setFrameSize((uint64_t)iFrameSize);
    }
    catch (FIS_lib_Exception const &e)
    {
        if (psErrorMsg != nullptr) *psErrorMsg = e.what();
        return false;
    }
    return true;
}
