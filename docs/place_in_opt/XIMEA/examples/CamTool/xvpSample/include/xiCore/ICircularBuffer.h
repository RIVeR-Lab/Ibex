#ifndef ICIRCULARBUFFER_H
#define ICIRCULARBUFFER_H

#include <xiCoreGlobal.h>
#include <PicBuf.h>
#include <QVariant>

class CxImageData;

enum ExCircularBufferType
{
    ecbtRAM    = 0,
    ecbtRAM_HS = 1,			// high speed recording to RAM using xiAPI buffer
    ecbtHDD    = 2,
    ecbtHDD_HS = 3,         // high speed lowlevel disk writer
};

//-----------------------------------------------------
#define CIRCBUFFER_PARAM_FILEPATH  "filepath"
#define CIRCBUFFER_PARAM_CACHESIZE "chunk_size"
#define STORE_LOC_SEPARATOR         '#'

//!The base class for circular buffers.
class XICORE_API IxCircularBuffer
{
public:
	IxCircularBuffer();
	virtual ~IxCircularBuffer();

	virtual ExCircularBufferType bufferType() const = 0;
	virtual void setCircleSize(qint32 iSize) = 0;								//!< Sets the number of frames in the circular buffer.
	virtual qint32 circleSize() const = 0;										//!< Number of frames in the circular buffer.
	virtual void resetInternals() = 0;											//!< Reset internal variable state of circular buffer
    virtual bool initializeBuffer(const SxPicBufInfo &aPicFormat, uint64_t uiFrameSize, int iConcatImgCount = 0, int iConcatImgNextOffset = 0) = 0; //!< Initializes (allocates) the circular buffer.
	virtual bool isBufferInitialized() const = 0;								//!< Returns true if the circular buffer is initialized.
    virtual qint32 framesCount(bool bUnrollConcatImgs = false) const = 0;		//!< Number of frames captured to the circular buffer.
    virtual qint32 storageErrors() const = 0;                                   //!< Returns the number of frames lost because of storage error
    virtual bool pushFrame(const CxImageData *pData) = 0;						//!< Pushes new frame to the circular buffer.
    virtual CxImageData* frame(int iIndex, bool bUnrollConcatImgs = false) = 0; //!< Returns a a copy of the frame with the given index which must be lesser than \ref framesCount().
	virtual bool bufferPicFormat(SxPicBufInfo &aPicFormat) const = 0;			//!< Gets the current image format of the circular buffer.
    virtual quint64 timestamp(qint32 iIndex, bool bUnrollConcatImgs = false) const = 0;	//!< Return a timestamp of a frame with the given index.
    virtual void finishBuffer() = 0;											//!< Finish (deallocate) the circular bufer.
    virtual quint64 bytesAvailable() = 0;										//!< Returns the maximum size of the circular buffer in bytes.
    virtual quint32 requiredFrameSizeByteAligning();                            //!< Returns the frame size byte aligning needed for circular buffer. Return 0 when not needed (default).
    virtual quint64 frameSize(bool bUnrollConcatImgs = false) const = 0;		//!< Size of single frame saved to the circular buffer.
	virtual void finishWriting() = 0;											//!< This method must finish all async. writing operations.
	virtual quint64 currentDataFlow() = 0;										//!< Data flow of the circular buffer in bytes per second (the speed of the data storing to the ciruclar buffer).
	virtual quint64 averageDataFlow() = 0;										//!< The average data flow of the circ. buffer in bytes per second (it makes sense to call this method after the chain was stopped).
	virtual quint64 minDataFlow() = 0;											//!< The minimum data flow in bytes per second detected in a single frame.

	virtual bool checkCircBufferCompatible(const SxPicBufInfo &aPicFormat);		//!< Returns true if the given \p aPicFormat is compatible with the circular buffer image format.
	virtual bool setParam(const QString &paramName, const QVariant &value);		//!? Set custom parameter for specific circular buffer implementation
	virtual QVariant param(const QString &paramName);							//!? Get custom parameter for specific circular buffer implementation
    virtual bool canSaveRecordingSession() const;                               //!< Returns true when the recording session can saved to (and later loaded from) file
    virtual bool saveRecordingSessionToFile(const QString &sFilename, int iOutputXiImgFormat);  //!< Saves the session
};

#endif // ICIRCULARBUFFER_H
