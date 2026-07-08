#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <xiCoreGlobal.h>

#include "Chainable.h"
#include "DataLoader.h"
#include <ICircularBuffer.h>

#include <QElapsedTimer>

/*! Circular buffer has two modes:
    1. as a CxChainable, serves as a storage for frames (primary task)
    2. as a IxImageDataLoader, serves as a in-memory storage for CxVideoSource
*/
class XICORE_API CxCircularBufferCnbl : public CxImageProvider, public IxImageDataLoader, public IxCircularBuffer
{
    Q_OBJECT
public:
    CxCircularBufferCnbl();
    virtual ~CxCircularBufferCnbl();

    //CxChainable
public:
    virtual QString title() const;
    virtual bool acceptsDataFrom(CxChainable *pPrecedessor);
    virtual int buffersCountInMemoryPool() const;
    virtual CxChainable* clone();
protected:
    virtual bool init(QString *pErrMsg = NULL);
    virtual bool initialized();
    virtual void setupStart();
    virtual void setupStop();

protected slots:
    virtual void onDataAvailable(CxChainable *pSender, int iFrameNo);
    //CxImageProvider
public:
    virtual bool currentOutputImageInfo(SxPicBufInfo &picInfo, CxImageMetadata *pMetadata = NULL);
    virtual bool queryOutputImageInfo(const SxPicBufInfo &picInfoInput, SxPicBufInfo &picInfoOutput,
                                      const CxImageMetadata *pMetadataInput, CxImageMetadata *pMetadataOutput = NULL);

public:
    virtual qint32 sequenceSize();
    virtual IxChainData *loadedData(qint32 iSeqIdx = 0);
    virtual bool load();
    virtual QString inputFile();
    virtual void setInputFile(const QString &sFileName);

    // IxImageDataLoader interface
public:
    virtual double loadedFps();
    virtual TxImageFormatList supportedImageFormats();

    // IxCircularBuffer
public:
    virtual ExCircularBufferType bufferType() const;
    virtual void setCircleSize(qint32 iSize);
    virtual qint32 circleSize() const;
	virtual void resetInternals();
    virtual bool initializeBuffer(const SxPicBufInfo &aPicFormat, uint64_t uiFrameSize, int iConcatImgCount = 0, int iConcatImgNextOffset = 0);
    virtual bool isBufferInitialized() const;
    virtual qint32 framesCount(bool bUnrollConcatImgs = false) const;
    virtual qint32 storageErrors() const;
    virtual bool pushFrame(const CxImageData *pData);
    virtual CxImageData* frame(int iIndex, bool bUnrollConcatImgs = false);
    virtual bool bufferPicFormat(SxPicBufInfo &aPicFormat) const;
    virtual quint64 timestamp(qint32 iIndex, bool bUnrollConcatImgs = false) const;
    virtual void finishBuffer();
    virtual quint64 bytesAvailable();
    virtual quint32 requiredFrameSizeByteAligning();
    virtual quint64 frameSize(bool bUnrollConcatImgs = false) const;
    virtual void finishWriting();
    virtual bool setParam(const QString &paramName, const QVariant &value);
    virtual QVariant param(const QString &paramName);
    virtual quint64 currentDataFlow();
    virtual quint64 averageDataFlow();
    virtual quint64 minDataFlow();
    virtual bool canSaveRecordingSession() const;
    virtual bool saveRecordingSessionToFile(const QString &sFilename, int iOutputXiImgFormat);

	//Own members
public:
    void setCircularBufferType(ExCircularBufferType eBufType);
    static bool isCircularBufferTypeAvailable(ExCircularBufferType eBufType); //!< Returns true when selected writer is available at the moment (e.g. in case it depends on platform, admin rights)
    void setChainConsumerFlag(bool enable);
    void SetRunContinuous(bool bRunC);
    bool IsRunContinuous();
    bool AllFramesCaptured();
    void SetSpeedTestFlag(bool bTest);
    int framesLost() const;
    int famesCaptured() const;

protected:
    double  m_dFps;					//!< fps of the incoming images. Need to tell when providing the data further
    int     m_iPrevFrameNo;			//!< For lost frames detection.
    int     m_iFramesCountOK;		//!< For reseting the error state.
    IxCircularBuffer *m_pCircBuffer;
    int     m_iFramesCaptured;		//!< The total number of frames captured by the circ. buffer.
    qint64  m_iPushFrameMaxTimeNsec;//!< The max. time in nanoseconds needed by \p pushFrame() method.
    quint32 m_iLostFrames;
    QElapsedTimer m_timer;
    qint64  m_iStartCaptureNsecs;
    qint64  m_iStopCaptureNsecs;
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    QMutex  m_lockWrite;
#else
    QRecursiveMutex  m_lockWrite;
#endif
    bool    m_bSpeedWritingTest;
    SxPicBuf m_picSpeedTest;
    bool	m_bRunContinuous;		//!< True when loop record is running continuously, owervriting previosly captured data, false for single run
	bool	m_bAllFramesCaptured;	//!< True for single capture run, when all required images were captured
    int     m_iConcatImgCount;      //!< Info about image concatenation
};

#endif // CIRCULARBUFFER_H
