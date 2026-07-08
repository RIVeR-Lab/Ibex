#include "FIS_lib.h"

//#include <filesystem>
//#include <set>
#include <fstream>
#include <random>

#include "FIS_lib_hdd.h"
#include "FIS_lib_ram.h"
#ifdef WIN32
  #include "FIS_lib_impl.h"
#endif

#ifdef FIS_PERSISTANCE
#include "../tinyxml/tinyxml.h"

SxFisAdditionalPersistanceData::SxFisAdditionalPersistanceData()
{
    m_iOutputFmt = -1;
    m_iConcatImgCount = 0;
    m_iConcatImgNextOffset = 0;
}

#endif

//----------------------------------------------------------------------------------------
IxFisRecorder::IxFisRecorder()
{
}

//----------------------------------------------------------------------------------------
IxFisRecorder::~IxFisRecorder()
{
}

//----------------------------------------------------------------------------------------
uint64_t IxFisRecorder::storageBytesAvailable()
{
    //TODO: use this after all build servers support std::filesystem (failed on Linux ARM and macOS)
    /*
    std::vector<std::string> vecStoragePaths = storagePaths();
    std::set<std::string> setCheckedPaths;
    uint64_t uiTotalFreeSpace = 0;
    for (std::string &sPath : vecStoragePaths)
    {
        // path destinations can be checked only once
        if (setCheckedPaths.find(sPath) != setCheckedPaths.end())   // found
            continue;

        // get the free space
        std::error_code err;
        const std::filesystem::space_info si = std::filesystem::space(std::filesystem::path(sPath), err);
        if (err) continue;

        // remove 128 MBs from free space
        uint64_t u128M = 128 * 1000 * 1000;
        if (si.available > u128M)
            uiTotalFreeSpace += si.available - u128M;
        setCheckedPaths.insert(sPath);
    }
    return uiTotalFreeSpace;
    */
    return 0;
}

//----------------------------------------------------------------------------------------
void IxFisRecorder::setChunkSize(int32_t)
{
}

//----------------------------------------------------------------------------------------
int32_t IxFisRecorder::chunkSize()
{
    return 0;
}

//----------------------------------------------------------------------------------------
void IxFisRecorder::frame(uint32_t iIndex, void* pData, uint64_t dataSize)
{
    const uint64_t uiFrameSize = frameSize();
    if (dataSize < uiFrameSize)
    {
        throw_fis_lib_api("Insufficient data buffer");
    }
    framePart(iIndex, pData, 0, uiFrameSize);
}

//----------------------------------------------------------------------------------------
uint32_t IxFisRecorder::requiredFrameSizeByteAligning()
{
    return 0;
}

//----------------------------------------------------------------------------------------
void IxFisRecorder::abort()
{
}

//----------------------------------------------------
std::mutex g_RandLock;
std::string IxFisRecorder::makeUniqueFilename(const std::string &sFilePath, const std::string &sExt)
{
    static std::mt19937 mt;
    static bool s_bRandSeeded = false;
    const std::lock_guard<std::mutex> lock(g_RandLock);
    if (!s_bRandSeeded)
    {
        s_bRandSeeded = true;
        std::random_device rd;
        mt.seed(rd());
    }
    std::uniform_int_distribution<int> aRandomDistr(1, 99999);

    std::string sNewFilename;
    while (1)
    {
        sNewFilename = sFilePath + std::to_string(aRandomDistr(mt)) + sExt;
        // check if file already exists - else generate new random number
        std::ifstream infile(sNewFilename.c_str());
        if (!infile.good())
            break;
    }
    return sNewFilename;
}

#ifdef FIS_PERSISTANCE
//----------------------------------------------------
bool IxFisRecorder::saveConfiguration(const std::string &sFilename, SxFisAdditionalPersistanceData *pAddData)
{
    TiXmlDocument doc;
    TiXmlElement *pElRoot = new TiXmlElement("FisRecorder");
    pElRoot->SetAttribute("class", className());
    doc.LinkEndChild(pElRoot);

    TiXmlElement *pEl;
    pEl = new TiXmlElement("circleSize");
    pEl->LinkEndChild(new TiXmlText(std::to_string(circleSize())));
    pElRoot->LinkEndChild(pEl);

    pEl = new TiXmlElement("chunkSize");
    pEl->LinkEndChild(new TiXmlText(std::to_string(chunkSize())));
    pElRoot->LinkEndChild(pEl);

    pEl = new TiXmlElement("frameSize");
    pEl->LinkEndChild(new TiXmlText(std::to_string(frameSize())));
    pElRoot->LinkEndChild(pEl);

    if (pAddData != nullptr)
    {
        pEl = new TiXmlElement("outputFmt");
        pEl->LinkEndChild(new TiXmlText(std::to_string(pAddData->m_iOutputFmt)));
        pElRoot->LinkEndChild(pEl);

        pEl = new TiXmlElement("concatImgCount");
        pEl->LinkEndChild(new TiXmlText(std::to_string(pAddData->m_iConcatImgCount)));
        pElRoot->LinkEndChild(pEl);

        pEl = new TiXmlElement("concatImgNextOffset");
        pEl->LinkEndChild(new TiXmlText(std::to_string(pAddData->m_iConcatImgNextOffset)));
        pElRoot->LinkEndChild(pEl);

        if (!pAddData->m_vecTimeStamps.empty())
        {
            bool bAddFrameNo = (pAddData->m_vecTimeStamps.size() == pAddData->m_vecFrameNo.size());

            TiXmlElement *pElFrames = new TiXmlElement("frames");
            for (size_t i = 0; i < pAddData->m_vecTimeStamps.size(); i++)
            {
                pEl = new TiXmlElement("frame");
                pEl->SetAttribute("ts", std::to_string(pAddData->m_vecTimeStamps[i]));
                if (bAddFrameNo)
                    pEl->SetAttribute("no", std::to_string(pAddData->m_vecFrameNo[i]));
                pElFrames->LinkEndChild(pEl);
            }
            pElRoot->LinkEndChild(pElFrames);
        }

        pEl = new TiXmlElement("metadata");
        pAddData->m_aMetadata.storeToXml(pEl);
        pElRoot->LinkEndChild(pEl);
    }

    saveConfiguration(pElRoot);

    return doc.SaveFile(sFilename);
}
#endif

#ifdef FIS_PERSISTANCE
//----------------------------------------------------
IxFisRecorder* IxFisRecorder::loadConfiguration(const std::string &sFilename, SxFisAdditionalPersistanceData *pAddData)
{
    TiXmlDocument doc(sFilename);
    if (!doc.LoadFile())
        return nullptr;

    TiXmlElement *pElRoot = doc.FirstChildElement("FisRecorder");
    if (pElRoot == nullptr)
        return nullptr;

    const char *szClass = pElRoot->Attribute("class");
    if (szClass == nullptr)
        return nullptr;

    uint32_t uiCircleSize = 0;
    int32_t iChunkSize = 0;
    uint64_t uiFrameSize = 0;

    // load common parameters
    TiXmlElement *pElParam = pElRoot->FirstChildElement();
    while (pElParam != nullptr)
    {
        const char *szName = pElParam->Value();
        if (szName == nullptr)
        {
            pElParam = pElParam->NextSiblingElement();
            continue;
        }
        if (strcmp(szName, "metadata") == 0)
        {
            if (pAddData != nullptr)
                pAddData->m_aMetadata.loadFromXml(pElParam);
        }
        else if (strcmp(szName, "frames") == 0)
        {
            if (pAddData != nullptr)
            {
                TiXmlElement *pElFrame = pElParam->FirstChildElement("frame");
                while (pElFrame != nullptr)
                {
                    const char *szTs = pElFrame->Attribute("ts");
                    if (szTs != nullptr)
                    {
                        char* end = nullptr;
                        pAddData->m_vecTimeStamps.push_back((uint64_t)std::strtoull(szTs, &end, 10));
                    }
                    const char *szNo = pElFrame->Attribute("no");
                    if (szNo != nullptr)
                    {
                        pAddData->m_vecFrameNo.push_back((uint32_t)atoi(szNo));
                    }
                    pElFrame = pElFrame->NextSiblingElement("frame");
                }
            }
        }
        else
        {
            const char *szTextValue = pElParam->GetText();

            if (szTextValue == nullptr) {}
            else if (strcmp(szName, "circleSize") == 0) uiCircleSize = (uint32_t)atoi(szTextValue);
            else if (strcmp(szName, "chunkSize") == 0) iChunkSize = atoi(szTextValue);
            else if (strcmp(szName, "frameSize") == 0) uiFrameSize = (uint64_t)atol(szTextValue);
            else if (strcmp(szName, "outputFmt") == 0 && pAddData != nullptr) pAddData->m_iOutputFmt = atol(szTextValue);
            else if (strcmp(szName, "concatImgCount") == 0 && pAddData != nullptr) pAddData->m_iConcatImgCount = atol(szTextValue);
            else if (strcmp(szName, "concatImgNextOffset") == 0 && pAddData != nullptr) pAddData->m_iConcatImgNextOffset = atol(szTextValue);
        }
        pElParam = pElParam->NextSiblingElement();
    }
    if (uiCircleSize == 0 || uiFrameSize == 0)
        return nullptr;

    // create recorder and load its custom config
    IxFisRecorder *pRecorder = nullptr;
    if (strcmp(szClass, "CxFisRecorderHDD") == 0)
        pRecorder = new CxFisRecorderHDD();
#ifdef WIN32
    else if (strcmp(szClass, "CxFisRecorderHDDWin") == 0)
        pRecorder = new CxFisRecorderHDDWin();
#endif

    if (pRecorder == nullptr)
        return nullptr;

    pRecorder->setCircleSize(uiCircleSize);
    pRecorder->setChunkSize(iChunkSize);
    pRecorder->setFrameSize(uiFrameSize);

    pRecorder->loadConfiguration(pElRoot);
    return pRecorder;
}
#endif

//----------------------------------------------------------------------------------------
bool FIS_isHDDLowLevelRecorderAvailable()
{
#ifdef WIN32
    return true;
#else
    return false;
#endif
}

//----------------------------------------------------------------------------------------
IxFisRecorder* FIS_createHDDLowLevelRecorder()
{
#ifdef WIN32
    return new CxFisRecorderHDDWin();
#else
    return nullptr;
#endif
}

//----------------------------------------------------------------------------------------
IxFisRecorder* FIS_createHDDRecorder()
{
    return new CxFisRecorderHDD();
}

//----------------------------------------------------------------------------------------
IxFisRecorder* FIS_createRAMRecorder()
{
    return new CxFisRecorderRAM();
}

//----------------------------------------------------------------------------------------
TxFisLogCallback g_fnLogCallback = nullptr;
void FIS_setErrorLogFunction(TxFisLogCallback callback)
{
    g_fnLogCallback = callback;
}

//----------------------------------------------------------------------------------------
void FIS_logError(const std::string &msg)
{
    if (g_fnLogCallback != nullptr)
        g_fnLogCallback(std::string("FIS_Lib: ") + msg);
}
