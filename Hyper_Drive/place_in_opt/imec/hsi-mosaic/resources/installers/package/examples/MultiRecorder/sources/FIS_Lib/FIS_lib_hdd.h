#ifndef FISCOREHDD_H
#define FISCOREHDD_H

#include "FIS_lib.h"
#include <mutex>
#include <vector>
#include <list>

//----------------------------------------------------------------------------------------
// Implements circular buffer stored on hard drive on all platforms
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
class CxFisRecorderHDD : public IxFisRecorder
{
public:

    CxFisRecorderHDD();
    ~CxFisRecorderHDD() override;

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

    bool openFile(FisRecOperation iMode);
    void closeFile();

private:
    class CxChunkBuffer
    {
    public:
        void *m_pBuffer;
        int64_t m_iFileOffset;    // start of this chunk within its file
        int64_t m_iBytesToWrite;  // size we should write into the file

        CxChunkBuffer(void *pBuffer);   // takes ownership of the buffer deletes it on destructor
        ~CxChunkBuffer();
        void addData(int64_t iBufferOffset, const void *pData, int64_t iDataSize) noexcept;
        void writeToFile(FILE *hFile) noexcept;
    };

    std::vector<FILE*> m_hFiles;				//!< Files (on multiple drives) for record files
    std::vector<std::string> m_vecFileNames;	//!< Name of loop record files stored on HDD
    int		m_iFileCount;					//!? Number of files (on multiple drives) to write to
    std::vector<std::string> m_vecStoragePaths;
    uint32_t m_uiCircleSize;
    uint32_t m_iFramesCount;
    int32_t m_iLastFrame;
    bool m_bInitialized;
    FisRecOperation m_iOpenMode;
    std::vector<CxChunkBuffer*> m_vecChunks;    // chunk buffers for writing to each file (on multiple drives)
    uint64_t m_uiPreferredChunkSize;//the size of chunk in bytes from user (parameter "chunk_size")
    uint64_t m_uiChunkSize;
    bool m_bWritingFinished;

    uint64_t m_uiFrameSize;
    uint32_t m_uiStorageErrors;

};

//----------------------------------------------------------------------------------------

#endif //FISCOREHDD_H
