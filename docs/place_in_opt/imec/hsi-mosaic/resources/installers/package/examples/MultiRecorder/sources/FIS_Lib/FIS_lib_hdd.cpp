#include "FIS_lib_hdd.h"
#include <cstring>
#include <ctime>
#include <memory>
#include <iostream>
#include <iomanip>
#include <math.h>

#ifdef FIS_PERSISTANCE
#include "../tinyxml/tinyxml.h"
#endif

#define CIRCBUFFER_CHUNK_DEFAULT_SIZE (32 * 1000 * 1000)

//--------------------------------------------------------
#ifdef WIN32
#define FIS_snprintf _snprintf
#define FIS_fseek64 _fseeki64
#else
#define FIS_snprintf snprintf
#define FIS_fseek64 fseeko
#endif

template<typename ... Args>
std::string FormatErrMsg(const std::string& format, Args ... args)
{
	std::string msg;
    int size_s = FIS_snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	if (size_s <= 0)
	{
		throw_fis_lib_api("FormatErrMsg: Error during formatting.");
	}
	else
	{
		auto size = static_cast<size_t>(size_s);
		std::unique_ptr<char[]> buf(new char[size]);
        FIS_snprintf(buf.get(), size, format.c_str(), args ...);
		msg = std::string(buf.get(), buf.get() + size - 1);
	}
	return msg;
}

CxFisRecorderHDD::CxChunkBuffer::CxChunkBuffer(void *pBuffer)
{
    m_pBuffer = pBuffer;
    m_iBytesToWrite = 0;
    m_iFileOffset = 0;
}
CxFisRecorderHDD::CxChunkBuffer::~CxChunkBuffer()
{
    if (m_pBuffer != nullptr)
    {
        free(m_pBuffer);
        m_pBuffer = nullptr;
    }
}
void CxFisRecorderHDD::CxChunkBuffer::addData(int64_t iBufferOffset, const void *pData, int64_t iDataSize) noexcept
{
    if (m_pBuffer != nullptr && pData != nullptr && iDataSize > 0)
        memcpy((uint8_t*)m_pBuffer + iBufferOffset, pData, (size_t)iDataSize);
    m_iBytesToWrite += iDataSize;
}
void CxFisRecorderHDD::CxChunkBuffer::writeToFile(FILE *pFile) noexcept
{
    if (m_pBuffer != nullptr && pFile != nullptr && m_iBytesToWrite > 0)
    {
        FIS_fseek64(pFile, m_iFileOffset, SEEK_SET);
        fwrite((const char*)m_pBuffer, (size_t)m_iBytesToWrite, 1, pFile);
        m_iBytesToWrite = 0;
    }
}

//----------------------------------------------------------------------------------------
//										CxFisRecorderHDD
//----------------------------------------------------------------------------------------
CxFisRecorderHDD::CxFisRecorderHDD() : IxFisRecorder(),
    m_iFileCount(0), m_uiCircleSize(0), m_iFramesCount(0), m_iLastFrame(-1), m_bInitialized(false),
    m_iOpenMode(FisRecOperation::Available), m_uiPreferredChunkSize(CIRCBUFFER_CHUNK_DEFAULT_SIZE), m_uiChunkSize(0),
    m_bWritingFinished(true), m_uiStorageErrors(0)
{
    CxFisRecorderHDD::resetInternals();
}

//----------------------------------------------------------------------------------------
CxFisRecorderHDD::~CxFisRecorderHDD()
{
    CxFisRecorderHDD::finishBuffer();
}

//----------------------------------------------------
void CxFisRecorderHDD::setStoragePaths(const std::vector<std::string> &vecFilePaths)
{
    if (m_bInitialized)
        throw_fis_lib_api("Cannot change recording destinations once initialized, use 'finishBuffer()' call to clear current instance first.");

    m_vecStoragePaths = vecFilePaths;
}

//----------------------------------------------------
std::vector<std::string> CxFisRecorderHDD::storagePaths()
{
    return m_vecStoragePaths;
}

//----------------------------------------------------
void CxFisRecorderHDD::setChunkSize(int32_t iChunkSize)
{
    uint64_t uiChunkSizeMB;
    if (iChunkSize <= 0)
    {
        uiChunkSizeMB = (uint64_t)(CIRCBUFFER_CHUNK_DEFAULT_SIZE / (1000 * 1000));
    }
    else
        uiChunkSizeMB = (uint64_t)iChunkSize;

    m_uiPreferredChunkSize = (uiChunkSizeMB) * ((uint64_t)1000) * ((uint64_t)1000);
}

//----------------------------------------------------
int32_t CxFisRecorderHDD::chunkSize()
{
    return (int32_t)(m_uiPreferredChunkSize / (((uint64_t)1000) * ((uint64_t)1000)));
}

//----------------------------------------------------
void CxFisRecorderHDD::setCircleSize(uint32_t iSize)
{
    m_uiCircleSize = iSize;
}

//----------------------------------------------------
void CxFisRecorderHDD::resetInternals()
{
    m_iFramesCount = 0;
    m_iLastFrame = -1;
    m_uiStorageErrors = 0;
    m_bInitialized = false;
}

//----------------------------------------------------
uint32_t CxFisRecorderHDD::circleSize() const
{
    return m_uiCircleSize;
}

//----------------------------------------------------
void CxFisRecorderHDD::initializeBuffer()
{
    if (m_bInitialized)
        return;

    finishBuffer();

    if (m_vecStoragePaths.empty())
    {
        throw_fis_lib_api("Recorder file paths are not set");
    }

    if (m_uiFrameSize == 0)
    {
        throw_fis_lib_api("Recorder file size was not set");
    }

    if (m_uiCircleSize == 0)
    {
        throw_fis_lib_api("Recorder file count was not set");
    }

    resetInternals();

    //init chunk
    m_uiFrameSize = frameSize();
    uint64_t uiFramesPerChunk = 1;
    if (m_uiFrameSize < m_uiPreferredChunkSize)
    {
        uiFramesPerChunk = ((uint64_t)m_uiPreferredChunkSize / m_uiFrameSize);
        m_uiChunkSize = uiFramesPerChunk * m_uiFrameSize;
    }
    else
    {
        m_uiChunkSize = m_uiFrameSize;
    }

    if (!openFile(FisRecOperation::Write))
    {
        throw_fis_lib_api(std::string("Failed to prepare file for writing"));
    }

    m_vecChunks.reserve((size_t)m_iFileCount);
    for (int i = 0; i < m_iFileCount; i++)
    {
        void *pChunk = malloc((size_t)m_uiChunkSize);
        if (pChunk == nullptr)
        {
            throw_fis_lib_api("Failed to allocate chunk item");
        }
        m_vecChunks.push_back(new CxChunkBuffer(pChunk));
    }

    //set file size, round up to chunk size
    const int32_t iFramesPerFile = (int32_t)ceil(m_uiCircleSize / (double)m_iFileCount);
    const int64_t iSingleFileSize = iFramesPerFile * (int64_t)m_uiFrameSize;
    for (size_t i = 0; i < (size_t)m_iFileCount; i++)
    {
        FILE* pFile = m_hFiles.at(i);
        FIS_fseek64(pFile, iSingleFileSize, SEEK_SET);
        fwrite("", 1, 1, pFile);
        FIS_fseek64(pFile, 0, SEEK_SET);
    }

    m_bInitialized = true;
}

//----------------------------------------------------
bool CxFisRecorderHDD::isBufferInitialized() const
{
    return m_bInitialized;
}

//----------------------------------------------------
uint32_t CxFisRecorderHDD::framesCount() const
{
    return m_iFramesCount;
}

//----------------------------------------------------
uint32_t CxFisRecorderHDD::lastFrame() const
{
    return (uint32_t)m_iLastFrame;
}

//----------------------------------------------------
uint32_t CxFisRecorderHDD::storageErrors() const
{
    return m_uiStorageErrors;
}

//----------------------------------------------------
uint32_t CxFisRecorderHDD::pushFrame(const void* pData)
{
    if (pData == NULL)
    {
        throw_fis_lib_api("Invalid input parameter pData");
    }

    if (m_iOpenMode != FisRecOperation::Write)//Should not never happen...
    {
        if (!openFile(FisRecOperation::Write))
        {
            throw_fis_lib_api(std::string("Failed to prepare file for writing"));
        }
    }

    // store the data here
    if (m_iFramesCount < circleSize())
    {
        // first pass
        m_iLastFrame = (int32_t)m_iFramesCount;
        m_iFramesCount++;
    }
    else
    {
        m_iLastFrame = (m_iLastFrame + 1) % (int32_t)circleSize();
    }

    // write data to our chunk and output file
    const uint64_t uiFrameSize = frameSize();
    const int sel = m_iLastFrame % m_iFileCount;                 // file number (on a ceratin drive)
    const int32_t iSplitIndex = m_iLastFrame / m_iFileCount;      // index within this file (on a certain drive)

    const int iFramesPerChunk = (int)(m_uiChunkSize / uiFrameSize);
    const int iFrameInChunk = iSplitIndex % iFramesPerChunk;
    const int64_t iChunkFileOffset = (iSplitIndex / iFramesPerChunk) * (int64_t)m_uiChunkSize;

    //copy image buffer to the chunk
    m_bWritingFinished = false;
    CxChunkBuffer* pChunk = m_vecChunks.at((size_t)sel);
    if (pChunk == nullptr)
    {
        throw_fis_lib_api("Invalid FIS list initialization");
    }
    if (pChunk->m_iFileOffset != iChunkFileOffset)    // write old data, we need to switch to a new chunk
    {
        pChunk->writeToFile(m_hFiles[(size_t)sel]);
        pChunk->m_iFileOffset = iChunkFileOffset;
    }
    pChunk->addData((int64_t)iFrameInChunk * (int64_t)uiFrameSize, pData, (int64_t)uiFrameSize);

    //if the chunk is full or if it is the last frame then we have to save it
    if (iFrameInChunk == (iFramesPerChunk-1) || m_iLastFrame == ((int32_t)m_uiCircleSize-1))
    {
        //write to file
        pChunk->writeToFile(m_hFiles[(size_t)sel]);
    }
    return (uint32_t)m_iLastFrame;
}

//----------------------------------------------------
void CxFisRecorderHDD::framePart(uint32_t iIndex, void* pData, uint64_t uiPartOffset, uint64_t uiPartSize)
{
    if (iIndex >= m_iFramesCount)
    {
        throw_fis_lib_api("Invalid index value, too high");
    }

    if (framesCount() >= circleSize()) // not finished the 1st pass
        iIndex = (iIndex + lastFrame() + 1) % circleSize();

    if (m_iOpenMode != FisRecOperation::Read)
    {
        if (!openFile(FisRecOperation::Read))
        {
            throw_fis_lib_api(std::string("Failed to open file for reading."));
        }
    }

    const uint64_t uiFrameSize = frameSize();
    if (uiPartOffset + uiPartSize > uiFrameSize)
    {
        throw_fis_lib_api("Data part out of range");
    }

    const int sel = (int)iIndex % m_iFileCount;
    if (sel >= (int)m_hFiles.size())
    {
        throw_fis_lib_api("Insuffisient data buffer");
    }

    const int32_t iSplitIndex = (int)iIndex / m_iFileCount;
    const int64_t iOffset = (int64_t)iSplitIndex * (int64_t)uiFrameSize;
    FILE* hFile = m_hFiles.at((size_t)sel);
    FIS_fseek64(hFile, iOffset + (int64_t)uiPartOffset, SEEK_SET);
    fread(pData, uiPartSize, 1, hFile);
}

//----------------------------------------------------
void CxFisRecorderHDD::finishBuffer(bool eraseData)
{
    closeFile();
    for (size_t i = 0; i < m_vecChunks.size() ; i++)
    {
        CxChunkBuffer* pChunk = m_vecChunks.at(i);
        if (pChunk != nullptr)
            delete pChunk;
    }
    m_vecChunks.clear();

    // erase written data files if requested
    if (eraseData)
    {
        for (size_t i = 0; i < m_vecFileNames.size(); i++)
        {
            std::string fName = m_vecFileNames.at(i);
            remove(fName.c_str());
        }
    }

    m_vecFileNames.clear();

    m_bInitialized = false;
}

//----------------------------------------------------------------------------------------
void CxFisRecorderHDD::setFrameSize(uint64_t size)
{
    if (m_bInitialized)
        throw_fis_lib_api("Cannot change frame size once initialized, use 'finishBuffer()' call to clear current instance first.");

    m_uiFrameSize = size;
}

//----------------------------------------------------
uint64_t CxFisRecorderHDD::frameSize()
{
    return m_uiFrameSize;
}

//----------------------------------------------------
void CxFisRecorderHDD::finishWriting()
{
    if (m_bWritingFinished)
        return;

    for (size_t i = 0; i < m_vecChunks.size() && i < m_hFiles.size(); i++)
    {
        CxChunkBuffer* pChunk = m_vecChunks.at(i);
        if (pChunk != nullptr && pChunk->m_iBytesToWrite > 0)
            pChunk->writeToFile(m_hFiles[i]);
    }

    m_bWritingFinished = true;
}

//----------------------------------------------------
bool CxFisRecorderHDD::openFile(FisRecOperation iMode)
{
    if (iMode == FisRecOperation::Write && m_iOpenMode != iMode)
    {
        closeFile();

        m_iFileCount = (int32_t)m_vecStoragePaths.size();
        m_vecFileNames.clear();
        for (size_t i = 0; i < m_vecStoragePaths.size(); i++)
        {
            const auto t = std::time(nullptr);
            const auto tm = *std::localtime(&t);
            std::stringstream ssFileName;
            ssFileName << m_vecStoragePaths.at(i) << "/";
            ssFileName << std::put_time(&tm, "%Y_%m_%d_%H_%M_%S_");
            std::string newFilename = IxFisRecorder::makeUniqueFilename(ssFileName.str(), ".dat");

            const char *filename = newFilename.c_str();

#ifdef WIN32
            FILE *pFile = fopen(filename, "wb");
#else
            FILE *pFile = fopen(filename, "w");
#endif
            if (pFile == NULL)
            {
                FIS_logError(FormatErrMsg("Failed to open file %s", filename));
                return false;
            }
            m_hFiles.push_back(pFile);
            m_vecFileNames.push_back(newFilename);
        }
        m_iOpenMode = iMode;
        return true;
    }
    else if (iMode == FisRecOperation::Read && m_iOpenMode != iMode)
    {
        closeFile();

        for (size_t i = 0; i < m_vecFileNames.size(); i++)
        {
            std::string sNativeFile = m_vecFileNames.at(i);
            const char *filename = sNativeFile.c_str();
#ifdef WIN32
            FILE *pFile = fopen(filename, "rb");
#else
            FILE *pFile = fopen(filename, "r");
#endif
            if (pFile == NULL)
            {
                FIS_logError(FormatErrMsg("Failed to open file %s", filename));
                return false;
            }
            m_hFiles.push_back(pFile);
        }
        m_iOpenMode = iMode;
        return true;
    }
    return false;
}

//----------------------------------------------------
void CxFisRecorderHDD::closeFile()
{
    for (size_t i = 0; i < m_hFiles.size(); i++)
    {
        fclose(m_hFiles[i]);
    }
    m_hFiles.clear();
    m_iOpenMode = FisRecOperation::Available;
}

//----------------------------------------------------
std::string CxFisRecorderHDD::className()
{
    return "CxFisRecorderHDD";
}

//----------------------------------------------------
#ifdef FIS_PERSISTANCE
void CxFisRecorderHDD::saveConfiguration(TiXmlElement *pElRoot)
{
    TiXmlElement *pEl;
    std::string sFilesJoined;
    for (auto &sSegment: m_vecFileNames) {
        if (!sFilesJoined.empty())
            sFilesJoined += "|";
        sFilesJoined += sSegment;
    }
    pEl = new TiXmlElement("vecFileNames");
    pEl->LinkEndChild(new TiXmlText(sFilesJoined));
    pElRoot->LinkEndChild(pEl);

    pEl = new TiXmlElement("lastFrame");
    pEl->LinkEndChild(new TiXmlText(std::to_string(m_iLastFrame)));
    pElRoot->LinkEndChild(pEl);

    pEl = new TiXmlElement("framesCount");
    pEl->LinkEndChild(new TiXmlText(std::to_string(m_iFramesCount)));
    pElRoot->LinkEndChild(pEl);

    pEl = new TiXmlElement("chunkSize");
    pEl->LinkEndChild(new TiXmlText(std::to_string(m_uiChunkSize)));
    pElRoot->LinkEndChild(pEl);

    pEl = new TiXmlElement("writingFinished");
    pEl->LinkEndChild(new TiXmlText(m_bWritingFinished ? "0" : "1"));
    pElRoot->LinkEndChild(pEl);
}
#endif

//----------------------------------------------------
#ifdef FIS_PERSISTANCE
void CxFisRecorderHDD::loadConfiguration(TiXmlElement *pElRoot)
{
    closeFile();

    TiXmlElement *pElParam = pElRoot->FirstChildElement();
    while (pElParam != nullptr)
    {
        const char *szName = pElParam->Value();
        const char *szTextValue = pElParam->GetText();

        if (szName != nullptr && szTextValue != nullptr)
        {
            if (strcmp(szName, "vecFileNames") == 0)
            {
                // split by '|'
                m_vecFileNames.clear();
                std::stringstream strStream(szTextValue);
                std::string sSegment;
                while(std::getline(strStream, sSegment, '|'))
                    m_vecFileNames.push_back(sSegment);
            }
            else if (strcmp(szName, "lastFrame") == 0) m_iLastFrame = atoi(szTextValue);
            else if (strcmp(szName, "framesCount") == 0) m_iFramesCount = (uint32_t)atoi(szTextValue);
            else if (strcmp(szName, "chunkSize") == 0) m_uiChunkSize = (uint32_t)atol(szTextValue);
            else if (strcmp(szName, "writingFinished") == 0) m_bWritingFinished = (atoi(szTextValue) == 1);
        }
        pElParam = pElParam->NextSiblingElement();
    }

    // now initialize all others
    m_iFileCount = (int32_t)m_vecFileNames.size();
}
#endif
