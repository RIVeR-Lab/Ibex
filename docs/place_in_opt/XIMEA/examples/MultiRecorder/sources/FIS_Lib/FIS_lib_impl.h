#ifndef FISCOREIMPL_H
#define FISCOREIMPL_H

#include "FIS_lib.h"
#include <mutex>
#include <vector>
#include <list>

//----------------------------------------------------------------------------------------
// Implements circular buffer stored on hard drive on windows
//----------------------------------------------------------------------------------------

#ifdef WIN32
#include <windows.h>

class FisDataChunk
{
public:
    FisDataChunk(uint64_t index, HANDLE hFile, DWORD offsetLow, DWORD offsetHi, DWORD buffSize) noexcept;

    uint64_t index() noexcept;
    HANDLE event() noexcept;
    HANDLE fileHandle() noexcept;
    DWORD dataOffsetLow() noexcept;
    DWORD dataOffsetHigh() noexcept;
    OVERLAPPED* olInfo() noexcept;
    DWORD bufferSize() noexcept;

private:
    HANDLE		_hFile;     // file handle we are writing to
    OVERLAPPED	_olInfo;    // Windows structure for async writing
    uint64_t	_uiDataSize;// data size to write
    uint64_t	_uiIndex;   // frame number
};

//----------------------------------------------------------------------------------------

class FisAsyncWriterWin
{
public:

	enum WriteState
	{
		Ready,			// ready for next write operation
        Writing,		// write operation currently being executed
        Finished,		// write operation finihsed successfully
        Error,			// error during write operation detected
		Timeout			// timeout during write operation detected
	};

    FisAsyncWriterWin();
    ~FisAsyncWriterWin();

    WriteState GetState();
    void WriteDataAsync(FisDataChunk* chunk, const void* buff);
    void CancelWriting();
    std::string LastErrorMsg();

private:
    void WriteDataAsyncThread(const void* buff);

private:
    FisDataChunk*	curr_item;
    WriteState		_eState;
	std::string		_sLastErrorMsg;
    std::thread*    _thread_obj;
};

//----------------------------------------------------------------------------------------

#define FIS_ALIGN_TO(length, padding) ((((length)+padding-1)/padding)*padding)

class CxFisRecorderHDDWin : public IxFisRecorder
{
public:

	CxFisRecorderHDDWin();
	~CxFisRecorderHDDWin() override;

	// IxFisRecorder
public:
    std::string className() override;
    void setStoragePaths(const std::vector<std::string> &vecFilePaths) override;
    std::vector<std::string> storagePaths() override;
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
    void finishBuffer(bool easeData = false) override;
    uint32_t requiredFrameSizeByteAligning() override;
	void setFrameSize(uint64_t size) override;
	uint64_t frameSize() override;
    void finishWriting() override;
	void abort() override;

protected:
#ifdef FIS_PERSISTANCE
    void saveConfiguration(TiXmlElement *pElRoot) override;
    void loadConfiguration(TiXmlElement *pElRoot) override;
#endif

private:

	bool openFiles(FisRecOperation iMode);
	void closeFiles();

    void initDataChunkOffsets();
    bool cleanupFinishedWriters();      // returns true all done

    std::string getDriverLetterFromPath(const std::string &path);

private:

	FisRecOperation					_eRecMode;			//!? current HDDWin recorder status
	std::mutex						_mLock;
	std::vector<HANDLE>				_vecFileHandles;	//!? File handles for record files
	std::vector<std::string>		_vecFileNames;		//!< Name of loop record files stored on HDD
	uint32_t						_uiFileCount;		//!? Number of files to write to
	uint32_t						_iLastFrame;
    std::vector<std::string>        _vecStoragePaths;

	std::vector<FisDataChunk*>		_vecDataChunks; //!? List of file data chunks
    std::list<FisAsyncWriterWin*>	_lstWriters;	//!? List of data writers
	std::vector<uint32_t>			_vecBytesPerSector;
    uint32_t                        _uiFrameSizeAligning;

	uint32_t						_iFramesCount;
	uint32_t						_uiCircleSize;
	uint64_t						_uiFrameSize;
	bool							_bInitialized;
    int32_t                         _iStorageErrors;

	std::string						_sLastErrorMsg;
};

#endif //WIN32

//----------------------------------------------------------------------------------------

#endif //FISCOREIMPL_H
