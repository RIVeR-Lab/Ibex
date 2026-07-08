// *****************************************************************************
//
//     Copyright (c) 2008, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVPIPELINEEVENTSINK_H__
#define __PVPIPELINEEVENTSINK_H__

#include <PvStreamLib.h>


class PvPipeline;
class PvBuffer;


///
/// \class PvPipelineEventSink
/// \brief Observer interface for an observable PvPipeline object
///
/// #PvPipelineEventSink defines a set of callback methods from
/// a PvPIpeline object to any object implementing this interface.
/// 
/// <b>To receive PvPipeline notifications:</b>
///
///     - Create an instance of your MySink class.
///     - Register your MySink class with the #PvPipeline object. Use #PvPipeline::RegisterEventSink.
///     - Handle events when #PvPipeline calls your overriden methods.
///
class PV_STREAM_API PvPipelineEventSink
{
public:

    ///
    /// \brief PvPipeline event sink constructor.
    ///
    PvPipelineEventSink();

    ///
    /// \brief PvPipeline event sink destructor.
    ///
    virtual ~PvPipelineEventSink();

    ///
    /// \brief Called whenever a PvBuffer is allocated by the pipeline.
    ///
    /// Can be used to track memory usage or structures used to follow
    /// PvBuffer pipeline management.
    ///
    /// This method is invoked right after the PvBuffer has been created but before its payload has been allocated.
    ///
    /// \param[in] aPipeline A pointer to the #PvPipeline object.
    /// \param[in] aBuffer Pointer to the buffer that was just created.
    ///
    virtual void OnBufferCreated( PvPipeline *aPipeline, PvBuffer *aBuffer );

    ///
    /// \brief Called whenever a PvBuffer is deleted by the pipeline.
    ///
    /// Can be used to track memory usage or structures used to follow
    /// PvBuffer pipeline management.
    ///
    /// This method is invoked right before the PvBuffer is deleted.
    ///
    /// \param[in] aPipeline A pointer to the #PvPipeline object.
    /// \param[in] aBuffer Pointer to the buffer that was just deleted.
    ///
    virtual void OnBufferDeleted( PvPipeline *aPipeline, PvBuffer *aBuffer );

    ///
    /// \brief PvPipeline event sink OnStart event.
    ///
    /// Invoked when the pipeline just before the pipeline starts.
    ///
    /// This callback is invoked from the same thread that called #PvPipeline::Start.
    ///
    /// \param[in] aPipeline A pointer to the #PvPipeline object.
    ///
    virtual void OnStart( PvPipeline *aPipeline );

    ///
    /// \brief PvPipeline event sink OnStop event.
    ///
    /// Invoked right after the pipeline has been stopped.
    ///
    /// This callback is invoked from the same thread that called #PvPipeline::Stop.
    ///
    /// \param[in] aPipeline A pointer to the #PvPipeline object.
    ///
    virtual void OnStop( PvPipeline *aPipeline );

    ///
    /// \brief PvPipeline event sink OnReset event.
    ///
    /// Invoked after the pipeline has been reset.
    ///
    /// This callback is invoked from the same thread that called #PvPipeline::Reset.
    ///
    /// \param[in] aPipeline A pointer to the #PvPipeline object.
    ///
    virtual void OnReset( PvPipeline *aPipeline );

    ///
    /// \brief PvPipeline event sink OnBufferTooSmall event.
    ///
    /// Invoked when the pipeline retrieve a buffer from the stream object which failed
    /// with an operation result set to BUFFER_TOO_SMALL.
    ///
    /// The user can set aRealloc all to true in order to force a complete re-allocation
    /// of the buffers performed synchronously after the notification is invoked.
    ///
    /// If OnBufferTooSmall is not handled to aReallocAll is set to false, the buffer
    /// is re-allocated before being queued in the stream object but the other buffers
    /// are not affected until they too will fail on BUFFER_TOO_SMALL.
    ///
    /// The aResetStats parameter is only considered if aReallocAll is set to true. The
    /// statistics do not need to be reset if the buffers are not re-allocated.
    ///
    /// This callback is invoked from the pipeline thread.
    ///
    /// \param[in] aPipeline A pointer to the #PvPipeline object.
    /// \param[in] aReallocAll If set to true by at least one of the sinks, synchronous buffer re-allocation will be performed.
    /// \param[in] aResetStats If set to true by at least one of the sinks, stream statistics are reset.
    ///
    virtual void OnBufferTooSmall( PvPipeline *aPipeline, bool *aReallocAll, bool *aResetStats );

    ///
    /// \brief PvPipeline event sink OnBufferReady event.
    ///
    /// Invoked after a buffer has been retrieved from stream and is ready for retrieve
    ///
    /// \param[in] aPipeline A pointer to the #PvPipeline object.
    ///
    virtual void OnBufferReady( PvPipeline *aPipeline );

};

#endif
