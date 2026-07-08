#ifndef FISCORERAM_H
#define FISCORERAM_H

#include "FIS_lib.h"
#include <vector>

//----------------------------------------------------------------------------------------
// Implements circular buffer stored on RAM (no presistence)
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
class CxFisRecorderRAM : public IxFisRecorder
{
public:

    CxFisRecorderRAM();
    ~CxFisRecorderRAM() override;

	// IxFisRecorder
public:
    std::string className() override;
    void setStoragePaths(const std::vector<std::string> &vecFilePaths) override;
    std::vector<std::string> storagePaths() override;
    void setChunkSize(int32_t iChunkSize) override;
    int32_t chunkSize() override;
    void setCircleSize(uint32_t iSize) override;
	uint32_t circleSize() const override;
	void resetInternals() override;
	void initializeBuffer() override;
	bool isBufferInitialized() const override;
	uint32_t framesCount() const override;
	uint32_t lastFrame() const override;
    uint32_t storageErrors() const override;
    uint32_t pushFrame(const void* pData) override;
    void framePart(uint32_t iIndex, void* pData, uint64_t uiPartOffset, uint64_t uiPartSize) override;
    void finishBuffer(bool eraseData = false) override;
	void setFrameSize(uint64_t size) override;
	uint64_t frameSize() override;
    void finishWriting() override;

protected:
#ifdef FIS_PERSISTANCE
    void saveConfiguration(TiXmlElement *pElRoot) override;
    void loadConfiguration(TiXmlElement *pElRoot) override;
#endif

private:
    uint32_t m_uiCircleSize;
    uint32_t m_iFramesCount;
    int32_t m_iLastFrame;
    bool m_bInitialized;
    std::vector<void*> m_vecFrames;    // chunk buffers for writing to each file (on multiple drives)
    uint64_t m_uiFrameSize;
};

//----------------------------------------------------------------------------------------

#endif //FISCORERAM_H
