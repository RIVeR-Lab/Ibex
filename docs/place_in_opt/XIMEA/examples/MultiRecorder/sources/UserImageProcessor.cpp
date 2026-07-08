#include "UserImageProcessor.h"

//----------------------------------------------------------------------------------------
CxUserImageProcessor::CxUserImageProcessor()
{
}

//----------------------------------------------------------------------------------------
CxUserImageProcessor::~CxUserImageProcessor()
{
}

//----------------------------------------------------------------------------------------
bool CxUserImageProcessor::command(TiXmlElement *, std::string &sResponse, std::string &sResponseContentType)
{
    sResponse = "Unsupported";
    sResponseContentType = "text/plain";
    return false;
}

//----------------------------------------------------------------------------------------
bool CxUserImageProcessor::processImage(XI_IMG *, std::string &sResponse, std::string &sResponseContentType)
{
    sResponse = "Unsupported";
    sResponseContentType = "text/plain";
    return false;
}

//----------------------------------------------------------------------------------------
void CxUserImageProcessor::setCameraHandle(HANDLE)
{
}

//----------------------------------------------------------------------------------------
void CxUserImageProcessor::processRawData(const std::string &sData, std::string *psResponse, std::string *psResponseContentType)
{
    if (psResponse != nullptr)
        *psResponse = "Unsupported";
    if (psResponseContentType != nullptr)
        *psResponseContentType = "text/plain";
}
