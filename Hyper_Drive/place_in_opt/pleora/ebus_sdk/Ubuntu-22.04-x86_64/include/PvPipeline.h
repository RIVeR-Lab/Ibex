// *****************************************************************************
//
//     Copyright (c) 2008, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVPIPELINE_H__
#define __PVPIPELINE_H__

#include <PvStreamLib.h>
#include <PvStream.h>
#include <PvPipelineEventSink.h>


namespace PvStreamLib
{
    class Pipeline;
}


///
/// \class PvPipeline
///
/// \brief Helper class for receiving data from a GigE Vision or USB3 Vision transmitter.
///
/// The process of receiving data from a GigE Vision or USB3 Vision transmitter involves queuing buffers into the data receiver ensuring that there are always buffers available for the incoming data.
/// #PvPipeline partners with a #PvStream object to allocate and push buffers into the data receiver as needed.
///
/// For further information on how #PvPipeline fits in with the process of receiving data from a GigE Vision transmitter, refer to the 
/// <a href="receivingdata.html">receiving data from a transmitter</a> page.
///
class PV_STREAM_API PvPipeline
{

public:

    ///
    /// \brief Constructor.
    ///
    /// The PvPipeline is just a helper of a PvStream class. Your code
    /// is still the owner of the PvStream class.
    ///
    /// \param[in] aStream The object responsible for receiving data from a GEV transmitter; a #PvStream object.
    ///
    PvPipeline( PvStream *aStream );

    ///
    /// \brief Virtual destructor
    ///    
    virtual ~PvPipeline();

    ///
    /// \brief Get the size for all managed buffers in bytes.
    ///
    /// \return The size of managed buffers, in bytes.
    ///
    uint32_t GetBufferSize() const;

    ///
    /// \brief Get number of buffers owned and used by the pipeline.
    ///
    /// \return The number of buffers.
    ///
    uint32_t GetBufferCount() const;

    ///
    /// \brief Returns the number of #PvBuffer objects currently available in the output queue.
    ///
    /// #PvBuffer objects are pushed into the output queue as soon as they have been populated with incoming data.
    /// #PvBuffer objects are pulled out of the output queue by calling #RetrieveNextBuffer.
    /// The number of buffers available in the pool managed by #PvPipeline is limited to #GetBufferCount.
    /// If the calling application cannot retrieve buffers at the rate that data is arriving, buffers are silently dropped. 
    /// To obtain a count of dropped buffers, refer to the PipelineImagesDropped property of the #PvStream parameters.
    ///
    /// \return The number of #PvBuffer currently available in the output queue.
    ///
    uint32_t GetOutputQueueSize() const;

    ///
    /// \brief Returns if the pipeline is configured to handle BUFFER_TOO_SMALL operation results automatically.
    ///
    /// \return True if the pipeline handles BUFFER_TOO_SMALL operation results automatically.
    ///
    /// \sa #PvPipeline::SetHandleBufferTooSmall
    ///
    bool GetHandleBufferTooSmall() const;

    ///
    /// \brief Test if the pipeline has started.
    ///
    /// \return True if the pipeline has been started (using #Start); otherwise, false.
    ///
    bool IsStarted();

    ///
    /// \brief Set the optimal size of buffers being queued into the data receiver
    ///
    /// Each time a buffer is queued into the data receiver, it is re-allocated as necessary according to the following behavior:
    ///     - If the buffer size is less than the require buffer size (BUFFER_TOO_SMALL) it is re-allocated.
    ///     - If the buffer size is more than four times the required buffer size, it is re-allocated in order to save memory.
    ///
    /// Originally the buffer size can be specified using SetBufferSize. The pipeline automatically adjusts the buffer size
    /// based on incoming buffers. As buffers are returned to the pipeline and being queued by the data receiver they may be 
    /// re-allocated as described above.
    ///
    /// \param[in] aSize      The size of the buffer, in bytes.
    ///
    /// \sa #PvBuffer::Alloc
    ///
    void SetBufferSize( uint32_t aSize );

    ///
    /// \brief Set the number of buffers owned and used by the #PvPipeline.
    ///
    /// Buffers are added or removed as necessary until the actual buffer count matches the desired buffer count being set.
    /// During this time, the pipeline is paused temporarily.
    ///
    /// \param[in] aBufferCount   The total number of buffers.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_ENOUGH_MEMORY
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///
    PvResult SetBufferCount( uint32_t aBufferCount );

    ///
    /// \brief Configures whether the pipeline handles BUFFER_TOO_SMALL operation result or not.
    ///
    /// If the pipeline is configured to handle BUFFER_TOO_SMALL operation results the buffers are automatically
    /// re-allocated and re-queued for acquisition when this condition occurs. 
    ///
    /// If the pipeline is not configured to handled BUFFER_TOO_SMALL operation results the buffers are
    /// queued in the pipeline output queue just as any other buffer and what to do with the buffer is deferred
    /// to the application using PvPipeline.
    ///
    /// If the pipeline handles BUFFER_TOO_SMALL operation results, the #PvPipelineEventSink::OnBufferTooSmall
    /// callback is invoked in order to order to query how to handle the condition. If not implemented
    /// the default behaviour is applied. See #PvPipelineEventSink::OnBufferTooSmall for more information.
    ///
    /// #PvPipelineEventSink::OnBufferTooSmall is not invoked when the pipeline is not configured to handle
    /// BUFFER_TOO_SMALL operation results.
    ///
    /// \param[in] aValue If true, the pipeline handles BUFFER_TOO_SMALL operation results automatically. If
    ///                    false the pipeline queues these buffers in the output queue and defers handling of these
    ///                    buffers to the application.
    ///
    void SetHandleBufferTooSmall( bool aValue );

#ifdef EBUS_PYTHON_DOXYGEN
    tuple RetrieveNextBuffer(int aTimeout = 0xFFFFFFFF);
#else
    ///
    /// \brief Retrieve the next acquired block.
    ///
    /// \param[out] aBuffer  A pointer to the pointer to the buffer.
    /// \param[in]  aTimeout The maximum time, in ms, to wait for a buffer to be received. The default value is 0xFFFFFFFF milliseconds
    /// \param[out] aOperationResult A pointer to the result of the acquisition operation on aBuffer, see #PvBuffer::GetOperationResult.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::TIMEOUT
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///     - #PvResult::Code::NOT_CONNECTED -> not connected
    ///     - #PvResult::Code::NO_MORE_ITEM  -> no PvBuffer queued
    ///
    PvResult RetrieveNextBuffer(
        PvBuffer ** aBuffer,
        uint32_t aTimeout = 0xFFFFFFFF,
		PvResult * aOperationResult = NULL );
#endif

    ///
    /// \brief Return a buffer to the pipeline.
    ///
    /// <b>IMPORTANT</b>: all buffers retrieved through #RetrieveNextBuffer must be 
    /// released back to the #PvPipeline. A minimum of 4 buffers must be available to 
    /// the #PvPipeline at all times.
    ///
    /// \param[in] aBuffer A pointer to the buffer to return to the queue.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::NOT_ENOUGH_MEMORY (May occur if the buffer size needs to be increased but not enough memory is available.)
    ///
    PvResult ReleaseBuffer( PvBuffer * aBuffer );

    ///
    /// \brief Starts the #PvPipeline.
    ///
    /// In order to retrieve images from the stream and make them available
    /// to #RetrieveNextBuffer, the pipeline must be running. This 
    /// starts a thread that continually ensures that buffers are allocated and ready in the #PvStream so that incoming data is captured.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///
    PvResult Start();

    ///
    /// \brief Stops the #PvPipeline.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::GENERIC_ERROR (Errors occured when stopping the pipeline's thread.)
    ///
    PvResult Stop();

    ///
    /// \brief Resets the pipeline.
    ///
    /// Synchronously aborts the stream, waits for all buffers to be removed from the
    /// stream and queues in a fresh set of buffers. Call ONLY when it is running, otherwise
    /// it returns error
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::STATE_ERROR If already waiting for another reset to complete or if not running
    ///
    PvResult Reset();

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
    PvResult RegisterEventSink( PvPipelineEventSink *aEventSink );

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
    PvResult UnregisterEventSink( PvPipelineEventSink *aEventSink );

    ///
    /// \brief Get the priority of the pipeline thread
    ///
    /// See #SetBufferHandlingThreadPriority for details about the meaning of the values returned
    /// 
    /// \return The priority of the thread
    ///
    uint32_t GetBufferHandlingThreadPriority() const;

    ///
    /// \brief Set the priority of the pipeline thread
    ///
    /// This thread is pretty much the pipeline itself: it queues buffers in PvStream and 
    /// retrieves them when they are ready, making them available in an output queue.
    ///
    /// In Linux, the priorities supported are between 0 and 99, 99 been the highest priority as per the 
    /// SCHED_FIFO scheduler specifications. If the process is not running with root priviledge, all the 
    /// threads are run into the SCHED_OTHER ( scheduler 01 ) and have the same priority at 0.
    /// 
    /// In Windows, the priorities supported are between 0 and 6. The value will be translated into one of the 
    /// following scheduler defines:
    ///
    ///     0 - THREAD_PRIORITY_IDLE
    ///     1 - THREAD_PRIORITY_LOWEST
    ///     2 - THREAD_PRIORITY_BELOW_NORMAL
    ///     3 - THREAD_PRIORITY_NORMAL
    ///     4 - THREAD_PRIORITY_ABOVE_NORMAL
    ///     5 - THREAD_PRIORITY_HIGHEST
    ///     6 - THREAD_PRIORITY_TIME_CRITICAL
    ///
    /// If you change the thread priorities, it may alterate the stability of the system. For this reason,
    /// if you report a bug to Pleora, ensure to clearly mention that you change threads priorities.
    ///
    /// See #GetBufferHandlingThreadPriority
    /// 
    /// \param[in] aPriority   Priority to be set to the conversion threads.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///     - #PvResult::Code::THREAD_ERROR
    ///
    PvResult SetBufferHandlingThreadPriority( uint32_t aPriority );

protected:

private:

    PvStreamLib::Pipeline * mThis;

	 // Not implemented
	PvPipeline( const PvPipeline& );
	const PvPipeline &operator=( const PvPipeline & );

};

#endif
