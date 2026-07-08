#include "FIS_lib_ram.h"
#include <cstring>
#include <ctime>

#ifdef FIS_PERSISTANCE
#include "../tinyxml/tinyxml.h"
#endif

//----------------------------------------------------------------------------------------
CxFisRecorderRAM::CxFisRecorderRAM() : IxFisRecorder(),
    m_uiCircleSize(0), m_iFramesCount(0), m_iLastFrame(-1), m_bInitialized(false)
{
    CxFisRecorderRAM::resetInternals();
}

//----------------------------------------------------------------------------------------
CxFisRecorderRAM::~CxFisRecorderRAM()
{
    CxFisRecorderRAM::finishBuffer();
}

//----------------------------------------------------
void CxFisRecorderRAM::setStoragePaths(const std::vector<std::string> &)
{
}

//----------------------------------------------------
std::vector<std::string> CxFisRecorderRAM::storagePaths()
{
    return std::vector<std::string>();
}

//----------------------------------------------------
void CxFisRecorderRAM::setChunkSize(int32_t)
{
}

//----------------------------------------------------
int32_t CxFisRecorderRAM::chunkSize()
{
    return 0;
}

//----------------------------------------------------
void CxFisRecorderRAM::setCircleSize(uint32_t iSize)
{
    m_uiCircleSize = iSize;
}

//----------------------------------------------------
void CxFisRecorderRAM::resetInternals()
{
    m_iFramesCount = 0;
    m_iLastFrame = -1;
    m_bInitialized = false;
}

//----------------------------------------------------
uint32_t CxFisRecorderRAM::circleSize() const
{
    return m_uiCircleSize;
}

//----------------------------------------------------
void CxFisRecorderRAM::initializeBuffer()
{
    if (m_bInitialized)
        return;

    finishBuffer();

    if (m_uiFrameSize == 0)
    {
        throw_fis_lib_api("Recorder file size was not set");
    }

    if (m_uiCircleSize == 0)
    {
        throw_fis_lib_api("Recorder file count was not set");
    }

    resetInternals();

    if (m_vecFrames.size() != circleSize() || !m_bInitialized)
    {
        // remove additional buffers from ring
        while (m_vecFrames.size() > circleSize())
        {
            void *pData = m_vecFrames.back();
            if (pData != nullptr)
            {
                free(pData);
            }
            m_vecFrames.pop_back();
        }

        // add and allocate new up to desired size (allocate all now, do not delay when we start)
        while (m_vecFrames.size() < circleSize())
        {
            void *pNewData = malloc(m_uiFrameSize);
            if (pNewData == nullptr)
            {
                FIS_logError("CxFisRecorderRAM::initializeBuffer(): Allocation of circ. buffer failed.");

                //free some pictures for system (100MB)
                int iDelCount = (int) ((100 * 1000 * 1000) / m_uiFrameSize);
                int iDelFrom = (int)m_vecFrames.size() - iDelCount;
                iDelFrom = iDelFrom < 0 ? 0 : iDelFrom;
                int iDelTo = m_vecFrames.size() == 0 ? -1 : (int)m_vecFrames.size() - 1;
                for (int i = iDelTo; i >= iDelFrom; i--)
                {
                    void *pData = m_vecFrames.back();
                    if (pData != nullptr)
                        free(pData);
                    m_vecFrames.pop_back();
                }
                break;
            }
            m_vecFrames.push_back(pNewData);
        }
        setCircleSize((uint32_t)m_vecFrames.size());
        m_bInitialized = circleSize() > 0;
    }
}

//----------------------------------------------------
bool CxFisRecorderRAM::isBufferInitialized() const
{
    return m_bInitialized;
}

//----------------------------------------------------
uint32_t CxFisRecorderRAM::framesCount() const
{
    return m_iFramesCount;
}

//----------------------------------------------------
uint32_t CxFisRecorderRAM::lastFrame() const
{
    return (uint32_t)m_iLastFrame;
}

//----------------------------------------------------
uint32_t CxFisRecorderRAM::storageErrors() const
{
    return 0;
}

//----------------------------------------------------
uint32_t CxFisRecorderRAM::pushFrame(const void* pData)
{
    if (pData == NULL)
    {
        throw_fis_lib_api("Invalid input parameter pData");
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

    memcpy(m_vecFrames[m_iLastFrame], pData, frameSize());
    return (uint32_t)m_iLastFrame;
}

//----------------------------------------------------
void CxFisRecorderRAM::framePart(uint32_t iIndex, void* pData, uint64_t uiPartOffset, uint64_t uiPartSize)
{
    if (iIndex >= m_iFramesCount)
    {
        throw_fis_lib_api("Invalid index value, too high");
    }

    if (framesCount() >= circleSize()) // not finished the 1st pass
        iIndex = (iIndex + lastFrame() + 1) % circleSize();

    const uint64_t uiFrameSize = frameSize();
    if (uiPartOffset + uiPartSize > uiFrameSize)
    {
        throw_fis_lib_api("Data part out of range");
    }

    memcpy(pData, (uint8_t*)m_vecFrames[iIndex] + uiPartOffset, uiPartSize);
}

//----------------------------------------------------
void CxFisRecorderRAM::finishBuffer(bool)
{
    for (size_t i = 0; i < m_vecFrames.size() ; i++)
        free(m_vecFrames[i]);
    m_vecFrames.clear();
    m_bInitialized = false;
}

//----------------------------------------------------------------------------------------
void CxFisRecorderRAM::setFrameSize(uint64_t size)
{
    if (m_bInitialized)
        throw_fis_lib_api("Cannot change frame size once initialized, use 'finishBuffer()' call to clear current instance first.");

    m_uiFrameSize = size;
}

//----------------------------------------------------
uint64_t CxFisRecorderRAM::frameSize()
{
    return m_uiFrameSize;
}

//----------------------------------------------------
void CxFisRecorderRAM::finishWriting()
{
}

//----------------------------------------------------
std::string CxFisRecorderRAM::className()
{
    return "CxFisRecorderRAM";
}

//----------------------------------------------------
#ifdef FIS_PERSISTANCE
void CxFisRecorderRAM::saveConfiguration(TiXmlElement *)
{
}
#endif

//----------------------------------------------------
#ifdef FIS_PERSISTANCE
void CxFisRecorderRAM::loadConfiguration(TiXmlElement *)
{
}
#endif
