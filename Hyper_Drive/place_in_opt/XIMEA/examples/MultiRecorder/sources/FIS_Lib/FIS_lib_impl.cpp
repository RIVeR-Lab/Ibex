#ifdef WIN32

#include "FIS_lib_impl.h"
#include <chrono>
#include <cstring>
#include <ctime>
#include <memory>
#include <iostream>
#include <iomanip>

#ifdef FIS_PERSISTANCE
#include "../tinyxml/tinyxml.h"
#endif

//--------------------------------------------------------

template<typename ... Args>
std::string FormatErrMsg(const std::string& format, Args ... args)
{
	std::string msg;
	int size_s = _snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	if (size_s <= 0)
	{
		throw_fis_lib_api("FormatErrMsg: Error during formatting.");
	}
	else
	{
		auto size = static_cast<size_t>(size_s);
		std::unique_ptr<char[]> buf(new char[size]);
		_snprintf(buf.get(), size, format.c_str(), args ...);
		msg = std::string(buf.get(), buf.get() + size - 1);
	}
	return msg;
}

//----------------------------------------------------
//  FisDataChunk
//----------------------------------------------------
FisDataChunk::FisDataChunk(uint64_t index, HANDLE hFile, DWORD offsetLow, DWORD offsetHi, DWORD buffSize) noexcept
{
    _hFile = hFile;
    memset(&_olInfo, 0, sizeof(OVERLAPPED));
    _olInfo.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    _olInfo.Offset = offsetLow;
    _olInfo.OffsetHigh = offsetHi;
    _uiDataSize = buffSize;
    _uiIndex = index;
}

uint64_t FisDataChunk::index() noexcept { return _uiIndex; }
HANDLE FisDataChunk::event() noexcept { return _olInfo.hEvent; }
HANDLE FisDataChunk::fileHandle() noexcept { return _hFile; }
DWORD FisDataChunk::dataOffsetLow() noexcept { return _olInfo.Offset; }
DWORD FisDataChunk::dataOffsetHigh() noexcept { return  _olInfo.OffsetHigh; }
OVERLAPPED* FisDataChunk::olInfo() noexcept { return &_olInfo; }
DWORD FisDataChunk::bufferSize() noexcept { return (DWORD)_uiDataSize; }

//----------------------------------------------------
//  AsyncWriter
//----------------------------------------------------
#define MAX_WRITERS_CNT 32		// max. number of parallel write operations
#define MAX_TIMEOUTS_MS 5000    //5s timeout for each write operation

//----------------------------------------------------
FisAsyncWriterWin::FisAsyncWriterWin()
{
    curr_item = nullptr;
    _thread_obj = nullptr;
    _eState = FisAsyncWriterWin::Ready;
}

//----------------------------------------------------
FisAsyncWriterWin::~FisAsyncWriterWin()
{
    if (_thread_obj != nullptr)
    {
        _thread_obj->join();
        delete _thread_obj;
    }
}

//----------------------------------------------------
std::string FisAsyncWriterWin::LastErrorMsg(){ return _sLastErrorMsg; }

//----------------------------------------------------
FisAsyncWriterWin::WriteState FisAsyncWriterWin::GetState(/*int timeout, bool waitForFinish*/)
{
    return _eState;
}

//----------------------------------------------------------------------------------------
void FisAsyncWriterWin::WriteDataAsync(FisDataChunk* it, const void* buff)
{
    curr_item = it;
    _eState = WriteState::Writing;
    _thread_obj = new std::thread(&FisAsyncWriterWin::WriteDataAsyncThread, this, buff);
}

//----------------------------------------------------
void FisAsyncWriterWin::WriteDataAsyncThread(const void* buff)
{
	BOOL bRet = TRUE;
	DWORD numBytesWritten = 0;

    if (buff == nullptr)
	{
		_sLastErrorMsg = FormatErrMsg("WriteDataAsync invalid data pointer.");
        FIS_logError(_sLastErrorMsg);
		_eState = WriteState::Error;
        return;
	}
    if (curr_item == nullptr)
	{
		_sLastErrorMsg = FormatErrMsg("WriteDataAsync write item not set.");
        FIS_logError(_sLastErrorMsg);
        _eState = WriteState::Error;
        return;
	}
    if (!curr_item->fileHandle())
	{
		_sLastErrorMsg = FormatErrMsg("WriteDataAsync file handle not initialized.");
        FIS_logError(_sLastErrorMsg);
        _eState = WriteState::Error;
        return;
	}
    if (!curr_item->event())
	{
		_sLastErrorMsg = FormatErrMsg("WriteDataAsync overlapped event handle not initialized.");
        FIS_logError(_sLastErrorMsg);
        _eState = WriteState::Error;
        return;
	}

    // write image data
    if (!ResetEvent(curr_item->event()))
	{
		_sLastErrorMsg = FormatErrMsg("WriteDataAsync failed to reset overlapped event with error %d.", GetLastError());
        FIS_logError(_sLastErrorMsg);
        _eState = WriteState::Error;
        return;
	}
    bRet = WriteFile(curr_item->fileHandle(), buff, curr_item->bufferSize(), &numBytesWritten, curr_item->olInfo());
	const DWORD lError = GetLastError();
	if (!bRet && lError != ERROR_IO_PENDING)
	{
		_sLastErrorMsg = FormatErrMsg("WriteDataAsync failed with error %d.", lError);
        FIS_logError(_sLastErrorMsg);
        _eState = WriteState::Error;
        return;
	}

    const DWORD stat = WaitForSingleObject(curr_item->event(), MAX_TIMEOUTS_MS);
    switch (stat)
    {
    case WAIT_ABANDONED:
    {
        _sLastErrorMsg = FormatErrMsg("WriteDataAsync ERROR: WaitForSingleObject (frame %i) was abandoned", curr_item->index());
        FIS_logError(_sLastErrorMsg);
        CancelWriting();
        _eState = WriteState::Error;
        break;
    }
    case WAIT_OBJECT_0:
    {
        DWORD numBytesTransferred = 0;
        const BOOL bResRet = GetOverlappedResult(curr_item->fileHandle(), curr_item->olInfo(), &numBytesTransferred, FALSE);
        const DWORD lastErr = GetLastError();
        if (bResRet == TRUE)
        {
            //FIS_logError("Written ok");
            _eState = WriteState::Finished;
        }
        else
        {
            _sLastErrorMsg = FormatErrMsg("WriteDataAsync ERROR: GetOverlappedResult (frame %i) failed with error %d", curr_item->index(), lastErr);
            FIS_logError(_sLastErrorMsg);
            _eState = WriteState::Error;
        }
        break;
    }
    case WAIT_TIMEOUT:
    {
        _sLastErrorMsg = FormatErrMsg("WriteDataAsync ERROR: WaitForSingleObject (frame %i) timeout", curr_item->index());
        FIS_logError(_sLastErrorMsg);
        _eState = WriteState::Timeout;
        break;
    }
    case WAIT_FAILED:
    {
        _sLastErrorMsg = FormatErrMsg("WriteDataAsync ERROR: WaitForSingleObject (frame %i) failed with error %d", curr_item->index(), GetLastError());
        FIS_logError(_sLastErrorMsg);
        CancelWriting();
        _eState = WriteState::Error;
        break;
    }
    default:
    {
        _sLastErrorMsg = FormatErrMsg("WriteDataAsync ERROR: WaitForSingleObject (frame %i) failed with unknown error %d", curr_item->index(), GetLastError());
        FIS_logError(_sLastErrorMsg);
        CancelWriting();
        _eState = WriteState::Error;
        break;
    }
    }
}

//----------------------------------------------------
void FisAsyncWriterWin::CancelWriting()
{
    std::string logMsg = FormatErrMsg("Cancelling frame %i", curr_item->index());
    FIS_logError(logMsg);

    const BOOL bRet = CancelIoEx(curr_item->fileHandle(), curr_item->olInfo());
    if (bRet == FALSE)
    {
        const DWORD lErr = GetLastError();
        if (lErr != ERROR_NOT_FOUND && lErr != ERROR_OPERATION_ABORTED)
        {
            logMsg = FormatErrMsg("CancelIoEx failed with error %d", lErr);
            FIS_logError(logMsg);
        }
    }
}

//----------------------------------------------------------------------------------------
//										CxFisRecorderHDDWin
//----------------------------------------------------------------------------------------
CxFisRecorderHDDWin::CxFisRecorderHDDWin()
{
    _eRecMode = Available;
    CxFisRecorderHDDWin::resetInternals();
	_uiCircleSize = 0;
	_uiFrameSize = 0;
}

//----------------------------------------------------------------------------------------
CxFisRecorderHDDWin::~CxFisRecorderHDDWin()
{
    CxFisRecorderHDDWin::finishBuffer();
}

//----------------------------------------------------
void CxFisRecorderHDDWin::setStoragePaths(const std::vector<std::string> &vecFilePaths)
{
    if (_bInitialized)
        throw_fis_lib_api("Cannot change recording destinations once initialized, use 'finishBuffer()' call to clear current instance first.");

    _vecStoragePaths = vecFilePaths;
}

//----------------------------------------------------
std::vector<std::string> CxFisRecorderHDDWin::storagePaths()
{
    return _vecStoragePaths;
}

//----------------------------------------------------------------------------------------
void CxFisRecorderHDDWin::setCircleSize(uint32_t iSize)
{
	if (_bInitialized)
		throw_fis_lib_api("Cannot change circle size once initialized, use 'finishBuffer()' call to clear current instance first.");

	_uiCircleSize = iSize;
}

//----------------------------------------------------------------------------------------
uint32_t CxFisRecorderHDDWin::circleSize() const
{
	return _uiCircleSize;
}

//----------------------------------------------------------------------------------------
void CxFisRecorderHDDWin::resetInternals()
{
	_uiFileCount = 0;
	_iLastFrame = 0;
	_iFramesCount = 0;	
	_bInitialized = false;
    _iStorageErrors = 0;
    _uiFrameSizeAligning = 0;
}

//----------------------------------------------------------------------------------------
void CxFisRecorderHDDWin::initializeBuffer()
{
	if (_bInitialized)
	{
		return;
	}

	// clear previous session
	finishBuffer();

    if (_vecStoragePaths.empty())
	{
		throw_fis_lib_api("Recorder file paths are not set");
	}

	if (_uiFrameSize == 0)
	{
		throw_fis_lib_api("Recorder file size was not set");
	}

	if (_uiCircleSize == 0)
	{
		throw_fis_lib_api("Recorder file count was not set");
	}

	const std::lock_guard<std::mutex> lock(_mLock);
	

	resetInternals();
    _uiFrameSizeAligning = requiredFrameSizeByteAligning();

	//Create file for buffer
	if (!openFiles(Write))
	{
		throw_fis_lib_api(_sLastErrorMsg);
	}
    initDataChunkOffsets();
    _bInitialized = true;
}

//----------------------------------------------------------------------------------------
void CxFisRecorderHDDWin::initDataChunkOffsets()
{
	// store offsets for respective files	
	std::vector<uint64_t> offset_values;
	for (uint32_t i = 0; i < _uiFileCount; i++)
	{
		offset_values.push_back(0);
	}
	
	for (uint32_t i = 0; i < _uiCircleSize; i++)
	{
        const int sel = i % _uiFileCount;

        LARGE_INTEGER offset;
        offset.QuadPart = offset_values[sel];

        FisDataChunk *pItem = new FisDataChunk(i, _vecFileHandles[sel], offset.LowPart, offset.HighPart, (DWORD)_uiFrameSize);
		if (pItem == NULL)
		{
			throw_fis_lib_api("Failed to allocate chunk item");
		}
		_vecDataChunks.push_back(pItem);
		offset_values[sel] += _uiFrameSize;
	}
}

//----------------------------------------------------------------------------------------
bool CxFisRecorderHDDWin::isBufferInitialized() const
{
	return _bInitialized;
}

//----------------------------------------------------------------------------------------
uint32_t CxFisRecorderHDDWin::framesCount() const
{
	return _iFramesCount;
}

//----------------------------------------------------------------------------------------
uint32_t CxFisRecorderHDDWin::lastFrame() const
{
	return _iLastFrame;
}

//----------------------------------------------------------------------------------------
uint32_t CxFisRecorderHDDWin::storageErrors() const
{
    return _iStorageErrors;
}

//----------------------------------------------------------------------------------------
uint32_t CxFisRecorderHDDWin::pushFrame(const void* pData)
{
	const std::lock_guard<std::mutex> lock(_mLock);

	if (pData == NULL)
	{
		throw_fis_lib_api("Invalid input parameter pData");
	}

	// prepare files for writing
	if (_eRecMode != Write)
    {
        //Should never happen...
        if (!openFiles(Write))
		{
			throw_fis_lib_api(_sLastErrorMsg);
		}
	}

	// store the data here
	if (_iFramesCount < circleSize())
	{
		// first pass
		_iLastFrame = _iFramesCount;
		_iFramesCount++;
	}
	else
	{
		_iLastFrame = (_iLastFrame + 1) % _uiCircleSize;
	}

    while (true)
	{
        // clean up finished writers
        cleanupFinishedWriters();

        // create new writer
        if (_lstWriters.size() < MAX_WRITERS_CNT)
        {
            FisAsyncWriterWin* writer_ptr = new FisAsyncWriterWin();
            writer_ptr->WriteDataAsync(_vecDataChunks.at(_iLastFrame), pData);
            _lstWriters.push_back(writer_ptr);
            break;
        }
        // when reached max writer count, let them work
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return _iLastFrame;
}

//----------------------------------------------------------------------------------------
bool CxFisRecorderHDDWin::cleanupFinishedWriters()
{
    for (auto it = _lstWriters.begin(); it != _lstWriters.end();)
    {
        auto itThis = it;
        it++;
        FisAsyncWriterWin* pWriter = *itThis;
        const FisAsyncWriterWin::WriteState eState = pWriter->GetState();
        if (eState != FisAsyncWriterWin::Writing)
        {
            std::string sWriterError;
            if (eState != FisAsyncWriterWin::Finished)
                sWriterError = pWriter->LastErrorMsg();

            delete pWriter;
            _lstWriters.erase(itThis);

            if (!sWriterError.empty())
            {
                _iStorageErrors++;
                FIS_logError(sWriterError);     // duplicate?
            }
        }
    }
    return _lstWriters.empty();
}

//----------------------------------------------------------------------------------------
void CxFisRecorderHDDWin::framePart(uint32_t iIndex, void* pData, uint64_t uiPartOffset, uint64_t uiPartSize)
{
    const std::lock_guard<std::mutex> lock(_mLock);
    if (iIndex >= _iFramesCount)
    {
        throw_fis_lib_api("Invalid index value, too high");
    }

    if (_iFramesCount >= circleSize())
    {
        iIndex = (iIndex + lastFrame() + 1) % circleSize();
    }

    if (_eRecMode != FisRecOperation::Read)
    {
        if (!openFiles(FisRecOperation::Read))
        {
            throw_fis_lib_api(_sLastErrorMsg);
        }
    }

    const uint64_t uiFrameSize = frameSize();
    if (uiPartOffset + uiPartSize > uiFrameSize)
    {
        throw_fis_lib_api("Data part out of range");
    }

    if (iIndex >= _vecDataChunks.size())
    {
        throw_fis_lib_api("Data chunks not initialized for read operation.");
    }
    LARGE_INTEGER offset;
    offset.LowPart = _vecDataChunks.at(iIndex)->dataOffsetLow();
    offset.HighPart = _vecDataChunks.at(iIndex)->dataOffsetHigh();

    offset.QuadPart += (int64_t)uiPartOffset;

    const uint32_t sel = iIndex % _uiFileCount;
    BOOL iOk = SetFilePointerEx(_vecFileHandles.at(sel), offset, NULL, FILE_BEGIN);
    if (iOk == FALSE)
    {
        throw_fis_lib_api("Failed to set offset for read operation.");
    }

    DWORD uiRead = 0;
    iOk = ReadFile(_vecFileHandles.at(sel), pData, (DWORD)uiPartSize, &uiRead, NULL);
    if (iOk == FALSE || (DWORD)uiPartSize != uiRead)
    {
        DWORD err = GetLastError();
        throw_fis_lib_api(FormatErrMsg("ReadFile error, code %d.", err));
    }
}

//----------------------------------------------------------------------------------------
void CxFisRecorderHDDWin::finishBuffer(bool eraseData)
{
    // delete all writers (should be empty, they are alread cleared in chain.stop (chainable.setupStop -> circbuff.finishWriting)
    CxFisRecorderHDDWin::finishWriting();

    const std::lock_guard<std::mutex> lock(_mLock);

    // clear all data chunk definitions
    for (size_t i = 0; i < _vecDataChunks.size(); i++)
	{
		FisDataChunk* pItem = _vecDataChunks.at(i);
		if (pItem != nullptr)
		{
			CloseHandle(pItem->event());
			//pItem->m_ol.hEvent = NULL;
			delete pItem;
		}
	}
	_vecDataChunks.clear();
	closeFiles();

	// erase written data files if requested
	if (eraseData)
	{
        for (size_t i = 0; i < _vecFileNames.size(); i++)
		{
			std::string fName = _vecFileNames.at(i);
			remove(fName.c_str());
		}
	}

	_vecFileNames.clear();
	_bInitialized = false;
}

//----------------------------------------------------------------------------------------
void CxFisRecorderHDDWin::setFrameSize(uint64_t size)
{
	if (_bInitialized)
		throw_fis_lib_api("Cannot change frame size once initialized, use 'finishBuffer()' call to clear current instance first.");

	_uiFrameSize = size;
}

//----------------------------------------------------------------------------------------
uint64_t CxFisRecorderHDDWin::frameSize()
{
    return _uiFrameSize;
}

//----------------------------------------------------------------------------------------
void CxFisRecorderHDDWin::finishWriting()
{
    bool bAllDone = false;
    while (!bAllDone)
    {
        {
            const std::lock_guard<std::mutex> lock(_mLock);
            bAllDone = cleanupFinishedWriters();
        }
        if (!bAllDone)
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // let other (writer) threads do their work
    }
}

//----------------------------------------------------------------------------------------
// TODO: this function is never used
void CxFisRecorderHDDWin::abort()
{
    const std::lock_guard<std::mutex> lock(_mLock);
    for (auto pWriter : _lstWriters)
    {
        if (pWriter != nullptr && pWriter->GetState() == FisAsyncWriterWin::Writing)
        {
            pWriter->CancelWriting();
        }
    }
}

//----------------------------------------------------------------------------------------
uint32_t CxFisRecorderHDDWin::requiredFrameSizeByteAligning()
{
    if (_uiFrameSizeAligning > 0)
        return _uiFrameSizeAligning;

    _vecBytesPerSector.clear();

    for (size_t iToken = 0; iToken < _vecStoragePaths.size(); iToken++)
	{
		BOOL bRet = TRUE;
		std::string logicalDrive = "\\\\.\\";
        std::string sDrive = getDriverLetterFromPath(_vecStoragePaths.at(iToken));
		DWORD iBytesPerSector = 512;  // used in case of an error

		// prepare path, use only drive letter
		logicalDrive.append(sDrive);

        HANDLE hddDevice = CreateFileA(logicalDrive.c_str(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		if (hddDevice == NULL || hddDevice == INVALID_HANDLE_VALUE)
		{
            const DWORD lError = GetLastError();
            std::string errMsg = FormatErrMsg("CreateFileA on drive %s failed with error %d.", sDrive.c_str(), lError);
            FIS_logError(errMsg);
        }
		else
		{
			// Now that we have the device handle for the disk, let us get the disk metadata
			DWORD outsize = 0;
			STORAGE_PROPERTY_QUERY storageQuery;
			memset(&storageQuery, 0, sizeof(STORAGE_PROPERTY_QUERY));
			storageQuery.PropertyId = StorageAccessAlignmentProperty;
			storageQuery.QueryType = PropertyStandardQuery;

			STORAGE_ACCESS_ALIGNMENT_DESCRIPTOR diskAlignment = { 0 };
			memset(&diskAlignment, 0, sizeof(STORAGE_ACCESS_ALIGNMENT_DESCRIPTOR));

			bRet = DeviceIoControl(hddDevice, IOCTL_STORAGE_QUERY_PROPERTY, &storageQuery, sizeof(STORAGE_PROPERTY_QUERY), &diskAlignment, sizeof(STORAGE_ACCESS_ALIGNMENT_DESCRIPTOR), &outsize, NULL);
			if (bRet == FALSE)
			{
                const DWORD lError = GetLastError();
                std::string errMsg = FormatErrMsg("DeviceIoControl on drive %s failed with error %d.", sDrive.c_str(), lError);
                FIS_logError(errMsg);
            }
			else
			{
				// store HDD physical sector size info
                iBytesPerSector = diskAlignment.BytesPerPhysicalSector;
			}
			CloseHandle(hddDevice);
		}
        _vecBytesPerSector.push_back(iBytesPerSector);
    }

	//To be able to use FILE_FLAG_NO_BUFFERING we have to align buffer size to fit the sector size.
    // get maximum
    uint32_t uiByteAligning = 0;
    for (size_t i = 0; i < _vecBytesPerSector.size(); i++)
    {
        if (uiByteAligning < _vecBytesPerSector.at(i))
            uiByteAligning = _vecBytesPerSector.at(i);
    }
    return uiByteAligning;
}

//----------------------------------------------------------------------------------------
//#define DX_FAIL_WHEN_NOT_ADMIN
bool CxFisRecorderHDDWin::openFiles(FisRecOperation iMode)
{
    if (iMode == Write && _eRecMode != iMode)
    {
		HANDLE h_Process = GetCurrentProcess();
		HANDLE h_Token;
		BOOL bRet = OpenProcessToken(h_Process, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &h_Token);
		if (bRet == FALSE)
		{
            _sLastErrorMsg = FormatErrMsg("OpenProcessToken failed with error %d", GetLastError());
            FIS_logError(_sLastErrorMsg);
#ifdef DX_FAIL_WHEN_NOT_ADMIN
            return false;
#endif
        }

		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		bRet = LookupPrivilegeValue(NULL, SE_MANAGE_VOLUME_NAME, &tp.Privileges[0].Luid);
        if (bRet == FALSE)
		{
			_sLastErrorMsg = FormatErrMsg("LookupPrivilegeValue failed with error %d", GetLastError());
            FIS_logError(_sLastErrorMsg);
#ifdef DX_FAIL_WHEN_NOT_ADMIN
            CloseHandle(h_Token);
            return false;
#endif
		}
        else
        {
            bRet = AdjustTokenPrivileges(h_Token, FALSE, &tp, 0, NULL, NULL);
            if (!bRet || GetLastError() != ERROR_SUCCESS)
            {
                const DWORD lError = GetLastError();
                _sLastErrorMsg = FormatErrMsg("AdjustTokenPrivileges failed with error %d", lError);
                if (GetLastError() == 1300)
                    _sLastErrorMsg += ", running without admin privileges.";
                FIS_logError(_sLastErrorMsg);
#ifdef DX_FAIL_WHEN_NOT_ADMIN
                CloseHandle(h_Token);
                return false;
#endif
            }
        }
        CloseHandle(h_Token);

        // clear previously opened handles
        closeFiles();

        _uiFileCount = (uint32_t)_vecStoragePaths.size();

		_vecFileNames.clear();
        for (uint32_t i = 0; i < _uiFileCount; i++)
		{
            // generate new file name form current datetime
			const auto t = std::time(nullptr);
			const auto tm = *std::localtime(&t);
			std::stringstream ssFileName;
            ssFileName << _vecStoragePaths.at(i) << "/";
			ssFileName << std::put_time(&tm, "%Y_%m_%d_%H_%M_%S_");
            std::string newFilename = IxFisRecorder::makeUniqueFilename(ssFileName.str(), ".dat");
            FIS_logError(newFilename);

			const char *filename = newFilename.c_str();
			// first create file for writing
			HANDLE hFile = CreateFileA(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH | FILE_FLAG_OVERLAPPED, NULL);
			if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
			{
                _sLastErrorMsg = FormatErrMsg("Failed to open file handle for %s, error %d", filename, GetLastError());
                FIS_logError(_sLastErrorMsg);
				return false;
			}

			// pre-allocate file for writing
			{
				BOOL bret = TRUE;
				DWORD dwret = 0;
				LARGE_INTEGER req_file_size;
				const uint64_t currFrameSize = frameSize();

                req_file_size.QuadPart = (currFrameSize * _uiCircleSize) / _uiFileCount;
                if (_uiCircleSize % _uiFileCount > 0)
				{
					// add one more payload to make sure there is enough space
					req_file_size.QuadPart += currFrameSize;
				}

                // file size must be bigger then sector size
                const uint32_t pathBytesPerSector = _vecBytesPerSector.size()>i ? _vecBytesPerSector.at(i) : 512;
				if (pathBytesPerSector > req_file_size.QuadPart)
				{
					req_file_size.QuadPart = pathBytesPerSector;
				}

				// file size must be rounded to sector size
				if (req_file_size.QuadPart % (int)pathBytesPerSector > 0)
				{
					req_file_size.QuadPart += pathBytesPerSector - (req_file_size.QuadPart % pathBytesPerSector);
				}

				dwret = SetFilePointer(hFile, req_file_size.LowPart, &req_file_size.HighPart, FILE_BEGIN);
				if (dwret == INVALID_SET_FILE_POINTER)
				{
					_sLastErrorMsg = FormatErrMsg("Failed to move file pointer in %s, error %d", filename, GetLastError());
                    FIS_logError(_sLastErrorMsg);
                    CloseHandle(hFile);
					return false;
				}
				bret = SetEndOfFile(hFile);
				if (bret == FALSE)
				{
					_sLastErrorMsg = FormatErrMsg("Failed to set end of file %s, error %d", filename, GetLastError());
                    FIS_logError(_sLastErrorMsg);
                    CloseHandle(hFile);
					return false;
				}
				bret = SetFileValidData(hFile, req_file_size.QuadPart);
				if (bret == FALSE)
				{
                    const DWORD lError = GetLastError();
                    _sLastErrorMsg = FormatErrMsg("Failed to set valid data in file %s, error %d", filename, lError);
                    if (lError == 1314)
                        _sLastErrorMsg += ", running without admin privileges, writing speed can be a bit slower.";
                    FIS_logError(_sLastErrorMsg);
#ifdef DX_FAIL_WHEN_NOT_ADMIN
                    CloseHandle(hFile);
                    return false;
#endif
				}
			}
			// re-open file
			CloseHandle(hFile);
			hFile = CreateFileA(filename, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH, NULL);
			if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
			{
                _sLastErrorMsg = FormatErrMsg("Failed to re-open file %s, error %d", filename, GetLastError());
                FIS_logError(_sLastErrorMsg);
                return false;
			}
			// store file handle and name for later use
			_vecFileNames.push_back(newFilename);
			_vecFileHandles.push_back(hFile);
		}
		_eRecMode = FisRecOperation::Write;
		return true;
	}
    else if (iMode == Read && _eRecMode != iMode)
    {
        // clear previously opened handles
        closeFiles();

        for (size_t i = 0; i < _vecFileNames.size(); i++)
		{
			std::string sNativeFile = _vecFileNames.at(i);
			const char *filename = sNativeFile.c_str();
			HANDLE hFile = CreateFileA(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING, NULL);
			if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
			{
                _sLastErrorMsg = FormatErrMsg("Failed to open file %s, error %d", filename, GetLastError());
                FIS_logError(_sLastErrorMsg);
				return false;
			}
			_eRecMode = FisRecOperation::Read;
			_vecFileHandles.push_back(hFile);
		}

        if (_vecDataChunks.empty())
            initDataChunkOffsets(); // only done after loading from a sesssion (offsets are kept after Writing)
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------------------
void CxFisRecorderHDDWin::closeFiles()
{
	for (HANDLE hFile : _vecFileHandles)
	{
        if (hFile != NULL)
        {
            const BOOL bRet = CancelIoEx(hFile, NULL);
            if (bRet == FALSE)
            {
                const DWORD lErr = GetLastError();
                if (lErr != ERROR_NOT_FOUND && lErr != ERROR_OPERATION_ABORTED)
                {
                    _sLastErrorMsg = FormatErrMsg("CancelIoEx failed with error %d", lErr);
                    FIS_logError(_sLastErrorMsg);
                }
            }
            CloseHandle(hFile);
        }
	}
	_vecFileHandles.clear();
	_eRecMode = FisRecOperation::Available;
}

//----------------------------------------------------------------------------------------
std::string CxFisRecorderHDDWin::getDriverLetterFromPath(const std::string &path)
{
	std::string drive;
	int pos = (int)path.find(':', 0);
    if (pos > 0)
    {
        drive = path.substr(pos - 1, 1);
        drive.append(":");
    }
	return drive;
}

//----------------------------------------------------
std::string CxFisRecorderHDDWin::className()
{
    return "CxFisRecorderHDDWin";
}

//----------------------------------------------------
#ifdef FIS_PERSISTANCE
void CxFisRecorderHDDWin::saveConfiguration(TiXmlElement *pElRoot)
{
    TiXmlElement *pEl;
    std::string sFilesJoined;
    for (auto &sSegment: _vecFileNames) {
        if (!sFilesJoined.empty())
            sFilesJoined += "|";
        sFilesJoined += sSegment;
    }
    pEl = new TiXmlElement("vecFileNames");
    pEl->LinkEndChild(new TiXmlText(sFilesJoined));
    pElRoot->LinkEndChild(pEl);

    pEl = new TiXmlElement("lastFrame");
    pEl->LinkEndChild(new TiXmlText(std::to_string(_iLastFrame)));
    pElRoot->LinkEndChild(pEl);

    pEl = new TiXmlElement("frameSizeAligning");
    pEl->LinkEndChild(new TiXmlText(std::to_string(_uiFrameSizeAligning)));
    pElRoot->LinkEndChild(pEl);

    pEl = new TiXmlElement("framesCount");
    pEl->LinkEndChild(new TiXmlText(std::to_string(_iFramesCount)));
    pElRoot->LinkEndChild(pEl);
}
#endif

//----------------------------------------------------
#ifdef FIS_PERSISTANCE
void CxFisRecorderHDDWin::loadConfiguration(TiXmlElement *pElRoot)
{
    closeFiles();

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
                _vecFileNames.clear();
                std::stringstream strStream(szTextValue);
                std::string sSegment;
                while(std::getline(strStream, sSegment, '|'))
                    _vecFileNames.push_back(sSegment);
            }
            else if (strcmp(szName, "lastFrame") == 0) _iLastFrame = atoi(szTextValue);
            else if (strcmp(szName, "frameSizeAligning") == 0) _uiFrameSizeAligning = atol(szTextValue);
            else if (strcmp(szName, "framesCount") == 0) _iFramesCount = atoi(szTextValue);
        }
        pElParam = pElParam->NextSiblingElement();
    }

    _uiFileCount = (uint32_t)_vecFileNames.size();
}
#endif

#endif //WIN32
