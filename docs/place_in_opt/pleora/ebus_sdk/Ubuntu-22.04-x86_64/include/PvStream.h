// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVSTREAM_H__
#define __PVSTREAM_H__

#ifndef EBUS_ENUM
#define EBUS_ENUM(x) typedef enum
#endif

#include <PvStreamLib.h>

#include <PvBuffer.h>
#include <PvStreamEventSink.h>
#include <PvGenParameterArray.h>


namespace PvStreamLib
{
    class Stream;
}


class PvDeviceInfo;
class PvConfigurationWriter;
class PvConfigurationReader;
class PvPipeline;


EBUS_ENUM(PvStreamType)
{
    PvStreamTypeUnknown = -1,
    PvStreamTypeGEV = 0,
    PvStreamTypeU3V = 1,
    PvStreamTypeRTP = 2

} PvStreamType;


///
/// \class PvStream
///
/// \brief Receive data from a GigE Vision or USB3 Vision transmitter.
///
/// This provides a means of opening a connection to the data receiver and obtaining data from it.
///
class PV_STREAM_API PvStream
{
public:

    ///
    /// \brief Destructor
    ///	
    virtual ~PvStream();

    ///
    /// \brief Get the number of buffers (#PvBuffer objects) waiting to be filled with images by the data receiver.
    ///
    /// Every call to #QueueBuffer increases the count by one; every call to #RetrieveBuffer reduces the count by one.
    ///
    /// \return The number of buffers (#PvBuffer objects). 
    ///
    uint32_t GetQueuedBufferCount() const;

    ///
    /// \brief Get the maximum number of buffers (#PvBuffer objects) that can be queued at a time.
    ///
    /// \return Maximum number of buffers (#PvBuffer objects).
    ///
    uint32_t GetQueuedBufferMaximum() const;

    ///
    /// \brief Dynamically allocates a PvStream of the right type.
    ///
    /// This static method takes a generic PvDeviceInfo, creates the right type of PvStream (PvStreamGEV
    /// or PvStreamU3V) and opens it.
    ///
    /// Objects allocated with CreateAndOpen should be freed with #PvStream::Free.
    ///
    /// \param[in] aDeviceInfo Pointer to the device information used to open the stream.
    /// \param[out] aResult Outcome of the operation. Includes:
    /// - #PvResult::Code::OK
    /// - #PvResult::Code::INVALID_PARAMETER    aInfo is not correct or default buffer configurations are not correct 
    /// - #PvResult::Code::NOT_ENOUGH_MEMORY    could not allocate class
    /// - #PvResult::Code::NOT_CONNECTED        device not connected
    /// - #PvResult::Code::GENERIC_ERROR        unexpected drive error
    /// - #PvResult::Code::BAD_VERSION          bad driver version
    /// - #PvResult::Code::NOT_FOUND            usb only
    /// - #PvResult::Code::CANNOT_OPEN_FILE     usb only
    /// - #PvResult::Code::NOT_SUPPORTED        usb only
    ///
    /// \return A pointer to the new object on success, NULL on failure. See aResult for more information on the failure.
    ///
    static PvStream *CreateAndOpen( const PvDeviceInfo *aDeviceInfo, PvResult *aResult );

    ///
    /// \brief Dynamically allocates a PvStream of the right type.
    ///
    /// This static method takes a string and creates the right type of PvStream (PvStreamGEV
    /// or PvStreamU3V) and connects it.
    ///
    /// Objects allocated with CreateAndOpen should be freed with #PvStream::Free.
    ///
    /// \param[in] aInfo Information that uniquely identifies the device. You may use one of the following:
    ///             - GigE Vision device's IP Address. Use the form <tt>192.168.1.114</tt>.
    ///             - USB3 Vision GUID.
    /// \param[out] aResult Outcome of the operation. Includes:
    /// - #PvResult::Code::OK
    /// - #PvResult::Code::INVALID_PARAMETER    aInfo is not correct or default buffer configurations are not correct 
    /// - #PvResult::Code::NOT_ENOUGH_MEMORY    could not allocate class
    /// - #PvResult::Code::NOT_CONNECTED        device not connected
    /// - #PvResult::Code::GENERIC_ERROR        unexpected drive error
    /// - #PvResult::Code::BAD_VERSION          bad driver version
    /// - #PvResult::Code::NOT_FOUND            usb only
    /// - #PvResult::Code::CANNOT_OPEN_FILE     usb only
    /// - #PvResult::Code::NOT_SUPPORTED        usb only
    ///
    /// \return A pointer to the new object on success, NULL on failure. See aResult for more information on the failure (unless failure was aResult being NULL)
    ///
    static PvStream *CreateAndOpen( const PvString &aInfo, PvResult *aResult );

    ///
    /// \brief Frees an object allocated with CreateAndOpen.
    ///
    /// Even though calling delete on a PvStream created with CreateAndOpen should
    /// work in most cases, sometime it is necessary to call "delete" in the same context where
    /// "new" was called to avoid false positives on memory leaks.
    ///
    /// It is a good practice to use this method to free a PvStream allocated with CreateAndOpen.
    ///
    /// \param[in] aStream Pointer to the device object to delete.
    ///
    static void Free( PvStream *aStream );

    virtual PvResult Open( const PvDeviceInfo *aDeviceInfo ) = 0;
    virtual PvResult Open( const PvString &aInfo ) = 0;

    ///
    /// \brief Close the stream channel receiver.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_CONNECTED
    ///
    /// \sa Open
    ///
    PvResult Close();

    ///
    /// \brief Returns whether the object is a GigE Vision or USB3 Vision stream receiver.
    ///
    /// \return Specific object type.
    ///
    virtual PvStreamType GetType() const;

    ///
    /// \brief Abort all pending block requests
    ///
    /// Once you've aborted pending block requests, they still need to be de-queued. Use #RetrieveBuffer.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK 
    ///     - #PvResult::Code::NOT_CONNECTED
    ///
    PvResult AbortQueuedBuffers();

    ///
    /// \brief Queue a buffer to receive a block.
    ///
    /// The #QueueBuffer method queues in a buffer (#PvBuffer object) to a stream (#PvStream object) so it
    /// can be filled by the data receiver.
    ///
    /// For each buffer you remove (using #RetrieveBuffer), you should queue another (using #QueueBuffer).
    /// <b>The maximum number of buffers that can be queued at a given time can be obtained by calling #GetQueuedBufferMaximum.</b>
    /// To reduce the risk of missing incoming data, it is recommended that several blocks remain queued and ready to go at any given time.
    /// Generally it is recommended that a larger number of buffers remain queued for higher frame rates. 
    /// Larger block sizes with blocks being streamed at a lower frame rate can afford to allocate fewer buffers to optimize memory usage and reduce potential latency.
    ///
    /// Once the stream fills the buffer with a block, retrieve the buffer by calling #RetrieveBuffer.
    ///
    /// \param[in] aBuffer  A pointer to the buffer to be filled.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///     - #PvResult::Code::NOT_CONNECTED
    ///     - #PvResult::Code::NO_MORE_ENTRY
    ///     - #PvResult::Code::NOT_ENOUGH_MEMORY
    ///     - #PvResult::Code::PENDING if the buffer has been properly queued
    ///     - #PvResult::Code::GENERIC_ERROR if the buffer could not be queued. See #PvResult::GetDescription for more information
    ///
    PvResult QueueBuffer( PvBuffer * aBuffer );

    ///
    /// \brief Attempt to retrieve a queued buffer.
    ///
    /// To use this method, see also #QueueBuffer.
    ///
    /// The #RetrieveBuffer method blocks until one of the following occurs:
    ///     - The buffer is filled with a block (image or raw data).
    ///     - The retrieve buffer operation times out: it exceeds <i>aTimeout</i>. 
    ///     - The acquisition fails (by being aborted, data receiver timeout as defined by ImageTimeout property, etc.)
    /// 
    /// Once the method returns, you can determine the ultimate success of the image acquisition by 
    /// testing the <i>aOperationResult</i> parameter. For example, #RetrieveBuffer may return PvResult::Code::OK,
    /// even if the operation was aborted (by a call to #AbortQueuedBuffers or #Close). 
    /// In this case, <i>aOperationResult</i> would be PvResult::Code::ABORTED.
    ///
    /// The return value contains the RetrieveBuffer operation results and <i>aOperationResult</i> contains
    /// the actual result (or failure) of the data receiver on this buffer. This method can
    /// succeed in providing you the oldest buffer but the result of the operation may still be 
    /// a failure - if acquisition was aborted, incomplete, etc.
    /// 
    /// \param[out] aBuffer           The buffer that holds the image.
    /// \param[out] aOperationResult  A pointer to the result of the acquisition operation on aBuffer, see #PvBuffer::GetOperationResult.
    /// \param[in]  aTimeout The maximum time, in ms, to wait for a buffer to be returned by the data receiver.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///     - #PvResult::Code::TIMEOUT
    ///     - #PvResult::Code::NOT_CONNECTED (not connected.)
    ///     - #PvResult::Code::NO_MORE_ENTRY
    ///     - #PvResult::Code::NO_MORE_ITEM (no PvBuffer queued.)
    ///     - #PvResult::Code::BUSY
    ///
    PvResult RetrieveBuffer( PvBuffer ** aBuffer, PvResult * aOperationResult, uint32_t aTimeout = 0xFFFFFFFF );

    ///
    /// \brief Get the stream's Channel property.
    ///
    /// \return The stream's Channel property.
    ///
    uint16_t GetChannel();

    ///
    /// \brief Test if the stream is open.
    ///
    /// \return True if the stream is opened; otherwise, false.
    ///
    bool IsOpen() const;

    ///
    /// \brief Register an event sink used for callbacks.
    ///
    /// \param[in] aEventSink  A pointer to the event sink.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///     - #PvResult::Code::GENERIC_ERROR
    ///
    PvResult RegisterEventSink( PvStreamEventSink *aEventSink );

    ///
    /// \brief Unregister an event sink
    ///
    /// \param[in] aEventSink  A pointer to the event sink.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///     - #PvResult::Code::NOT_FOUND
    ///
    /// \sa #RegisterEventSink
    ///
    PvResult UnregisterEventSink( PvStreamEventSink *aEventSink );

    ///
    /// \brief Get the parameters defining how data is received from a GigE Vision transmitter
    ///
    /// \return The parameters defining how data is received from a GigE Vision transmitter
    ///
    PvGenParameterArray *GetParameters();

protected:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              

    ///
    /// \brief Constructor.
    ///
	PvStream();

    PvStreamLib::Stream *mThis;

private:

	 // Not implemented
	PvStream( const PvStream & );
    const PvStream &operator=( const PvStream & );

    friend class PvPipeline;
};


#endif
