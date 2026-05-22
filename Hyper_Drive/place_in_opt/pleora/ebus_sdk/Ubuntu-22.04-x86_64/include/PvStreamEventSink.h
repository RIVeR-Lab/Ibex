// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVSTREAMEVENTSINK_H__
#define __PVSTREAMEVENTSINK_H__

#include <PvStreamLib.h>


class PvBuffer;


///
/// \class PvStreamEventSink
/// \brief Event sink used to receive buffer queue/unqueue notifications.
///
/// Inherit a class from this interface and register an instance of it
/// to a PvStream in order to receive
/// these notifications.
///
/// These notifications are fired when events of pushing buffers in the stream
/// object with QueueBuffer and RetrieveBuffer are called by user code
/// or another object like PvPipeline.
///
/// QueueBuffer and RetrieveBuffer still need to be called. This notificaiton 
/// mechanism cannot be used in place of QueueBuffer and RetrieveBuffer
/// to receive buffer delivery callbacks.
///
class PV_STREAM_API PvStreamEventSink
{
public:

    ///
    /// \brief Constructor.
    ///
    PvStreamEventSink();

    ///
    /// \brief Destructor.
    ///
    virtual ~PvStreamEventSink();

    ///
    /// \brief Override to get OnBufferQueued callback.
    ///
    /// Called when a buffer is queued in a PvStream through QueueBuffer.
    /// Sychronous call, can be used to perform operations on the buffer
    /// if required.
    ///
    /// Base implementation does not need to be called.
    ///
    /// \param[in] aBuffer A pointer to the buffer being queued in the stream object.
    ///
    virtual void OnBufferQueued( PvBuffer *aBuffer );

    ///
    /// \brief Override to get OnBufferRetrieved callback.
    ///
    /// Called when a buffer is retrieved from a PvStream through RetrieveBuffer.
    /// Sychronous call, can be used to perform operations on the buffer before it is delivered to the 
    /// RetrieveBuffer caller if required.
    ///
    /// Base implementation does not need to be called.
    ///
    /// \param[in] aBuffer A pointer to the buffer being retrieved from the stream object.
    ///
    virtual void OnBufferRetrieved( PvBuffer* aBuffer );

};

#endif
