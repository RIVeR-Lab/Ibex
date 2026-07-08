#ifndef FISCORE_H
#define FISCORE_H

//----------------------------------------------------------------------------------------

#include <cstdint>
#include <string>
#include <sstream>
#include <vector>

#ifdef FIS_PERSISTANCE
#include "FIS_image_metadata.h"
class TiXmlElement;

struct SxFisAdditionalPersistanceData
{
    int m_iOutputFmt;   // XI_IMG_FORMAT, set to -1 when not present in data
    int m_iConcatImgCount;          // number of images inside one buffer (see XI_PRM_CONCAT_IMG_COUNT)
    int m_iConcatImgNextOffset;     // offset to next image inside one concatenated buffer (see XI_PRM_CONCAT_IMG_TRANSPORT_IMG_OFFSET)
    std::vector<uint64_t> m_vecTimeStamps;      // in microseconds
    std::vector<uint32_t> m_vecFrameNo;
    CxFisImageMetadata m_aMetadata;

    SxFisAdditionalPersistanceData();
};
#endif

//----------------------------------------------------------------------------------------
// !The base class for circular buffers.
class IxFisRecorder
{
public:

    IxFisRecorder();
    virtual ~IxFisRecorder();
    virtual std::string className() = 0;

    virtual void setStoragePaths(const std::vector<std::string> &vecFilePaths) = 0; //!< Paths to be used for write to HDD, can be more paths on different HDDs to increase the speed
    virtual std::vector<std::string> storagePaths() = 0;                        //!< Returns paths to be used for writing to HDD
    virtual uint64_t storageBytesAvailable();                                   //!< Returns total free space in starages defined as StoragePath
    virtual void setChunkSize(int32_t iChunkSize);                              //!< Set recommended cache size in MB (optional, should be frameSize multiple, set -1 to reset to default)
    virtual int32_t chunkSize();                                                //!< Returns current chunk size in MB, returns 0 when not defined
    virtual void setCircleSize(uint32_t iSize) = 0;								//!< Sets the number of frames in the circular buffer.
	virtual uint32_t circleSize() const = 0;									//!< Number of frames in the circular buffer.
	virtual void resetInternals() = 0;											//!< Reset internal variable state of circular buffer
	virtual void initializeBuffer() = 0;										//!< Initializes (allocates) the circular buffer.
	virtual bool isBufferInitialized() const = 0;								//!< Returns true if the circular buffer is initialized.
	virtual uint32_t framesCount() const = 0;									//!< Number of frames captured to the circular buffer.
	virtual uint32_t lastFrame() const = 0;										//!< Returns an index of the last captured frame.
    virtual uint32_t storageErrors() const = 0;									//!< Returns the number of frames lost during the acquisition.
    virtual uint32_t pushFrame(const void* pData) = 0;                          //!< Pushes new data to the circular buffer and returns frame index within circular buffer
    virtual void frame(uint32_t iIndex, void* pData, uint64_t dataSize);        //!< Returns a copy of the frame with the given index which must be lesser than framesCount().
    virtual void framePart(uint32_t iIndex, void* pData, uint64_t uiPartOffset, uint64_t uiPartSize) = 0;	//!< Returns a copy of the frame part (offset, size) within frameSize. Useful for concatenated images.
    virtual void finishBuffer(bool eraseData = false) = 0;						//!< Finish (deallocate) the circular buffer.
    virtual uint32_t requiredFrameSizeByteAligning();                           //!< Returns the frame size byte aligning needed for circular buffer. Return 0 when not needed (default).
    virtual void setFrameSize(uint64_t size) = 0;                               //!< Expect image buffers with given size (set before initializeBuffer, cannot change later).
	virtual uint64_t frameSize() = 0;											//!< Size of single frame saved to the circular buffer.
    virtual void finishWriting() = 0;											//!< This method must finish all asynchronous writing operations. Blocking call.
    virtual void abort();                                                       //!< Stop any pending write operations

#ifdef FIS_PERSISTANCE
    bool saveConfiguration(const std::string &sFilename, SxFisAdditionalPersistanceData *pAddData = nullptr);                       //!< Save recorder configuration so that it can be later reinitialized later (for reading data files)
    static IxFisRecorder* loadConfiguration(const std::string &sFilename, SxFisAdditionalPersistanceData *pAddData = nullptr);      //!< Load recorder configuration (for reading data files)
#endif

protected:
    enum FisRecOperation
    {
        Available = 0,
        Write,
        Read
    };
    static std::string makeUniqueFilename(const std::string &sFilePath, const std::string &sExt);
#ifdef FIS_PERSISTANCE
    virtual void saveConfiguration(TiXmlElement *pElRoot) = 0;
    virtual void loadConfiguration(TiXmlElement *pElRoot) = 0;
#endif

};

//-------------------------------------------------------------------------
// implementation instances

bool FIS_isHDDLowLevelRecorderAvailable();      //!< Returns true when optimized HDD writer is available at the moment (e.g. in case it depends on platform, admin rights)
IxFisRecorder* FIS_createHDDLowLevelRecorder(); //!< Create optimized HDD writer for Windows using lowlevel functions
IxFisRecorder* FIS_createHDDRecorder();         //!< Create generic HDD writer for all platforms
IxFisRecorder* FIS_createRAMRecorder();         //!< Create RAM recorder, data is stored only in RAM

//-------------------------------------------------------------------------
// global exception for handler that reports file name and line number

class FIS_lib_Exception : public std::exception
{
public:
    FIS_lib_Exception(const std::string &arg, const char *file, int line, const char *func)
	{
		std::ostringstream o;
		o << file << " line " << line << " , " << func << ": " << arg;
		msg_ = o.str();
	}

    FIS_lib_Exception(const std::string &arg)
	{
		msg_ = arg;
	}

	~FIS_lib_Exception() noexcept {}

    const char *what() const noexcept override
	{
		return msg_.c_str();
	}

private:
	std::string						msg_;
};

#define throw_fis_lib_api(arg) throw FIS_lib_Exception(arg, __FILE__, __LINE__, __FUNCTION__);

//----------------------------------------------------------------------------------------
// error logging
typedef void (*TxFisLogCallback)(const std::string &);

void FIS_setErrorLogFunction(TxFisLogCallback callback);    //!< Application should set its error logging function here

void FIS_logError(const std::string &msg);                  //!< FIS_lib calls this function to send the error messages to application

#endif //FISCORE_H
