/*
******************************************************************************
* @attention
*
*<h2><center>&copy; COPYRIGHT(c) 2021 Photonfocus AG</center></h2>
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
* 1. Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
* 3. Neither the name of Photonfocus nor the names of its contributors
* may be used to endorse or promote products derived from this software
* without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*/

///
/// @file PFStream.h 
/// @brief This class defines a streaming strategy to access data acquired from the camera. 
///           The stream is a kind of virtual channel between the camera and the client application.
///           The chunks of data being captured (PFBuffer) are stored in a internal ring buffer.
///           The data stored in the ring data can be obtained afterwards by calling GetNextBuffer method.
///
#ifndef PFSTREAM_H
#define PFSTREAM_H

#include "pfcErrors.h"
#include "PFCameraLib.h"
#include "PFResult.h"
#include "PFBuffer.h"
#include "PFBufferRef.h"
#include "pfcCameraTypes.h"

namespace pfcameralib
{
    /// 
    /// @class StreamStatistics PFStream.h 
    ///
    /// Manages Stream Statistics calculated during acquisition of data. 
    ///
    class PFCAMERALIB_API StreamStatistics
    {
    public:
        ///
        /// Total frames captured while grabbing
        ///
        uint64_t m_totalFrames=0;              
        
        ///
        /// Number of frames per second 
        ///
        double m_fpsGrab=0;                   
        
        ///
        /// Network rate in Mbits per second
        ///
        double m_networkRate=0;           
        
        ///
        /// Lost frames while grabbing
        ///
        uint64_t m_lostFrames=0;               

        ///
        /// Frames that had an error.
        ///
        uint64_t m_errorFrames=0;

        ///
        /// Accumulated missing packets
        ///
        uint64_t m_missingPackets=0;

        /// 
        /// @brief  Initializes Statistics. 
        ///
        void clearStatistics()
        {
            m_lostFrames = 0;
            m_totalFrames = 0;
            m_fpsGrab = 0;
            m_networkRate = 0;
            m_errorFrames = 0;
            m_missingPackets = 0;
        }
    };

    class IPFEventStream;

    /// 
    /// @class PFStream PFStream.h 
    ///
    /// Manages Streams to acquire data from the Camera.
	  /// There are two modes of streaming:
	  ///   - shared pool mode : only a single pool of buffers is used, which are mapped to the underlaying driver (in the kernel if filter driver is used)
    ///   - standard mode: the driver has its own buffer pool (SetDriverBufferCount) and the PFStream keeps another buffer pool (SetBufferCount).
    /// 
    /// In shared mode, the driver reconstructs directly into the shared buffers. GetNextBuffer returns the approriate buffer.
    /// In standard mode, the driver grabs frames in its buffers and a copy into the PFStream pool is done during GetNextBuffer. 
    /// 
    /// The shared pool mode avoids performing one copy for each frame, which @ 10Gbit/s cameras can be significant.
    /// Pros:
    ///   - reduced memory bandwidth usage ~2GB/s
    ///   - reduced CPU usage
    ///   - reduced memory usage can be achieved in some circumstances.
    /// Cons:
    ///   - the user has to be more careful in not delaying returns of the buffers.
    /// 
    /// 
    /// Whenever possible, allocated buffers are reused between calls to Open (LockParams) and Close (UnlockParams).
    /// This requires grab parameters to remain constant, and all acquired buffers to be returned.
    ///
    class PFCAMERALIB_API PFStream
    {
    public:
        /// 
        /// @enum class Event 
        /// Type of Events that can be triggered while the stream is being accessed
        /// 
        enum class Event {
            Opened,         ///< triggered after the stream has been opened
            Closed,         ///< triggered after the stream has been closed
            ParamsLocked,   ///< triggered after the stream parameters have been locked
            ParamsUnlocked, ///< triggered after the stream parameters have been unlocked
            Opening,        ///< triggered after just before opening the stream
            Closing         ///< triggered after just before closing the stream
        };
        /// 
        /// @struct Control 
        /// @brief Parameters to configure stream control behaviour
        /// 
        struct Control {
          bool lock_params_on_open = true;      ///< Open will trigger LockParams
          bool unlock_params_on_close = true;   ///< Close will trigger UnlockParams
          bool shared_pool_mode = true;         ///< Buffer pool will be mapped in the driver (kernel or not) to avoid copy.
        };

        ///
        /// @brief Default constructor
        ///
        PFStream(); 

        ///
        /// @brief Destructor
        ///
        virtual ~PFStream();

        ///
        /// @brief Copy constructor
        ///
        PFStream(const PFStream &stream);
        
        ///
        /// @brief Asignment operator
        ///
        PFStream & operator= (const PFStream &stream);
        
        /// 
        /// @brief Lock parameters related to the acquisition process.
        /// 
        /// This involves TLParamsLocked = 1.
        /// Memory allocation of the buffers is done if required.
        /// 
        /// @return PFResult                
        /// Error Codes:
        /// PFSDK_NOERROR
        PFResult LockParams();

        /// 
        /// @brief Unlock parameters related to the acquisition process.
        /// 
        /// @return PFResult                
        /// Error Codes:
        /// PFSDK_NOERROR
        PFResult UnlockParams();

        /// 
        /// @brief Open the Stream Channel. Once the stream is opened you can start to acquire data. You have to maintain the stream opened during the entire grabbing process.
        /// @return PFResult                
        /// Error Codes:
        /// PFSDK_ERROR_CAMERA_NOT_CONNECTED
        /// PFSDK_ERROR_GRAB_READPAYLOADSIZE_FAILED
        /// PFSDK_NOERROR
        PFResult Open();
                
        ///
        /// @brief Close the Stream Channel. Once grabbing is freezed stream must be closed.
        /// @return PFResult                
        /// Error Codes:
        /// PFSDK_ERROR_CAMERA_NOT_CONNECTED
        /// PFSDK_NOERROR
        PFResult Close();
                
        /// @brief Get Frames per second
        /// @return PFResult                
        /// Error Codes:
        /// PFSDK_ERROR_CAMERA_NOT_CONNECTED
        /// PFSDK_NOERROR
        PFResult GetFPS(double &fps); 

        /// 
        /// @brief Set size of buffer pool.
        ///           
        ///           If Control::shared_pool_mode == true, then SetBufferCount also affects driver buffer count.

        ///           The ring buffer is pre-allocated before grabbing process starts. The ring is being reused during the entire grabbing process.
        ///           Note: Memory consumption can be high when you fix a long value. Typically (PayloadSize * buffer_count) 
        ///                 Fix a value depending on the amount of memory you wish to preallocate and the acquisition speed you need to achieve.
        ///                 Usually as higher is the rate more memory you will need.
        ///
        /// @return PFResult                
        /// Error Codes:
        ///      PFSDK_NOERROR
        ///
        /// 
        PFResult SetBufferCount(const uint64_t buffer_count);
        
        /// 
        /// @brief Get Count of buffer pool.
        ///
        /// Get the current size of the buffer pool.
        ///
        /// @return PFResult                
        /// Error Codes:
        ///      PFSDK_NOERROR
        ///
        /// 
        PFResult GetBufferCount(uint64_t &buffer_count);
                   
                
        /// 
        /// @brief Get streaming type.
        ///
        /// Returns the Streaming type used. Two streaming protocols are supported: GigE, USB3. 
        ///
        /// @return CameraType  
        ///                 CAMTYPE_GEV      --> GigE Camera protocol
        ///                 CAMTYPE_U3V      --> USB3 Camera protocol
        ///
        /// 
        virtual CameraType GetStreamType();
        
        /// 
        /// @brief Get next buffer from the camera.
        ///
        /// This function acquires a new buffer from the camera and assigns to available stream's buffer pool. 
        /// This method has to be called consecutively during entire acquisition process.
        /// For this buffer to be available to reuse, the user must call PFBuffer::UnlockBuffer(), other wise eventually PFSDK_ERROR_QUEUE_FULL will happen.
        /// 
        /// @param[in,out] **pfBuffer       Pointer to the the incoming data buffer. The pointer passed by reference 
        ///                                             will be assigned to an available buffer from the pool.    
        ///                                             Note: You don't need to preallocate or free this pointer, all the memory management is
        ///                                             done internally.
        ///
        /// @return PFResult                
        /// Error Codes:
        ///     PFSDK_NOERROR
        ///     PFSDK_ERROR_QUEUE_FULL
        ///     PFSDK_ERROR_STREAM_CLOSED
        ///     PFSDK_ERROR_WRONG_PARAMETERS
        ///     PFSDK_ERROR_UNKNOWN_BUFFER_SIZE
        ///     PFSDK_ERROR_GETIMAGE_GRAB_ERROR
        ///     PFSDK_ERROR_GETIMAGE_MISSING_PACKETS
        ///     PFSDK_ERROR_GETIMAGE_DIFFERENT_IMAGE_HEADER
        ///     PFSDK_ERROR_GETIMAGE_TIMEOUT
        ///     PFSDK_ERROR_GETIMAGE_NULL_POINTER
        ///     PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
        ///     PFSDK_ERROR_GETIMAGE_GEV_FAILED
        /// 
        PFCAMERALIB_DEPRECATED PFResult GetNextBuffer(PFBuffer *&pfBuffer);

        ///
        /// @brief Get next buffer from the camera.
        ///
        /// This function acquires a new buffer from the camera and assigns to available stream's buffer pool. 
        /// This method has to be called consecutively during entire acquisition process.
        /// For this buffer to be available to reuse, the user must call PFBuffer::UnlockBuffer(), other wise eventually PFSDK_ERROR_QUEUE_FULL will happen.
        ///
        /// @param[in,out] &pfBuffer                 	Incoming data buffer. The variable passed by reference
        ///                                             will be assigned to the last available internal ring buffer.
        ///                                             Note: Behaves internally as a smart pointer.
        ///
        ///
        /// @return PFResult
        /// Error Codes:
        ///     PFSDK_NOERROR
        ///     PFSDK_ERROR_QUEUE_FULL
        ///     PFSDK_ERROR_STREAM_CLOSED
        ///     PFSDK_ERROR_WRONG_PARAMETERS
        ///     PFSDK_ERROR_UNKNOWN_BUFFER_SIZE
        ///     PFSDK_ERROR_GETIMAGE_GRAB_ERROR
        ///     PFSDK_ERROR_GETIMAGE_MISSING_PACKETS
        ///     PFSDK_ERROR_GETIMAGE_DIFFERENT_IMAGE_HEADER
        ///     PFSDK_ERROR_GETIMAGE_TIMEOUT
        ///     PFSDK_ERROR_GETIMAGE_NULL_POINTER
        ///     PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
        ///     PFSDK_ERROR_GETIMAGE_GEV_FAILED
        ///
        PFResult GetNextBuffer(PFBufferRef &pfBuffer);
        
        /// 
        /// @brief Release buffer.
        ///
        /// Prefer using PFBuffer::UnlockBuffer() instead
        /// 
        /// Release (unlock) a buffer which was obtained with a call to GetNextBuffer. The buffer will be made available for reuse in the ring buffer.
        /// Note that this call is mandatory after finishing the consumption of the image received in order to reuse the memory assigned. 
        /// If you don't do that then you will receive  PFSDK_ERROR_QUEUE_FULL from GetNextBuffer() call indicating that the ring buffer is full.
        /// 
        /// Important Note: In order to avoid null pointer exceptions be aware if you have image operations pending before calling this method.
        /// After calling this method don't expect to use the pointer again before next iteration of GetNextBuffer().
        ///
        /// @param[in,out] **pfBuffer       Pointer to the the incoming data buffer. The internal state of the buffer will be modified.
        ///
        /// @return PFResult                
        /// Error Codes:
        ///      PFSDK_NOERROR
        /// 
        PFCAMERALIB_DEPRECATED PFResult ReleaseBuffer(PFBuffer *pfBuffer);

        ///
        /// @brief Release buffer.
        ///
        /// Prefer using PFBuffer::UnlockBuffer() instead
        /// 
        /// Release (unlock) a buffer which was obtained with a call to GetNextBuffer. The buffer will be made available for reuse in the ring buffer.
        /// Note that this call is mandatory after finishing the consumption of the image received in order to reuse the memory assigned. 
        /// If you don't do that then you will receive  PFSDK_ERROR_QUEUE_FULL from GetNextBuffer() call indicating that the ring buffer is full.
        ///
        /// @param[in,out] pfBuffer       Reference to PFBufferRef which will be set to point to the retrieved buffer.
        ///
        /// @return PFResult                
        /// Error Codes:
        ///      PFSDK_NOERROR
        /// 
        PFResult ReleaseBuffer(PFBufferRef &pfBuffer);
                        
        /// 
        /// @brief Get Acquisition Frame Count.
        /// 
        /// Deprecated. Use PFCamera::GetAcquisitionFrameCount()
        ///
        /// Returns the Number of frames for Multi frame Acquisition Mode. This method returns 0 for Continuous Acquisition Mode.
        ///
        /// @return uint64_t 
        ///                 0               --> Continuous mode
        ///                 1               --> Single frame  
        ///                 > 1             --> Multi frames
        ///
        PFCAMERALIB_DEPRECATED uint64_t GetAcquisitionFrameCount();
        
        /// 
        /// @brief Get stream statistics. The stream statistics are initialized when the stream is opened.
        /// The internal counters are recalculated when GetNextBuffer() is called.
        /// 
        /// @return StreamStatistics   Statistics calculated during capture
        StreamStatistics GetStreamStatistics();
        
        /// 
        /// @brief Filter driver version.
        /// @param[out] *out_version Previouly allocated buffer where version will be copied
        /// @return PFResult
        virtual PFResult GetFilterDriverVersion(char* out_version) = 0;

        /// 
        /// @brief Get buffer size. Usually corresponds to PayloadSize feature value for a single frame.
        /// 
        /// @return Buffer size  One frame buffer size
        uint64_t GetBufferSize();

        ///
        /// @brief Add an event listener
        ///
        /// The lifetime of the handler passed must be guaranteed to exceed the lifetime of the PFStream, 
        /// or until after it is removed with RemoveEventSink 
        ///
        /// @param[in]  e       Event type to which the handler is added.
        /// @param[in]  sink    Pointer to an instance of IPFEventStream interface.
        ///
        /// @return PFResult
        /// Error Codes:
        ///     PFSDK_NOERROR
        PFResult AddEventSink(Event e, IPFEventStream* sink);

        ///
        /// @brief Remove an event listener
        ///
        /// @param[in]  e       Event type from which the handler is removed.
        /// @param[in]  sink    Pointer to an instance of IPFEventStream interface which was previously added.
        ///
        /// @return PFResult
        /// Error Codes:
        ///     PFSDK_NOERROR
        PFResult RemoveEventSink(Event e, IPFEventStream* sink);

        ///
        /// @brief Set the timeout value (microseconds) for the wait for an available buffer in the pool.
        /// 
        /// The stream will wait for an available buffer up to the specified amount before returning PFSDK_ERROR_QUEUE_FULL.
        /// For instance, this is useful to avoid flooding the execution with error events when using PFAsyncGrabber.
        /// The default value is 0 (no wait).
        /// 
        /// @param[in]  timeout_us  Timeout in microseconds.
        ///
        void SetBufferPoolTimeout(uint64_t timeout_us);

        ///
        /// @brief Get the timeout value (microseconds) for the wait for an available buffer in the pool.
        /// 
        /// @return Timeout in microseconds.
        /// 
        uint64_t GetBufferPoolTimeout();

        ///
        /// @brief Enable or disable usage of filter driver
        /// 
        /// @param[in] use True to use filter driver, false otherwise
        ///
        /// @return PFResult
        /// Error Codes:
        ///     PFSDK_NOERROR
        /// 
        PFResult SetFilterDriverEnabled(bool use = true);

        ///
        /// @brief Get filter driver enabled status
        /// 
        /// @return true if enabled, false otherwise
        /// 
        bool GetFilterDriverEnabled() const;


        ///
        /// @brief Reset acquisition statistics. 
        /// 
        /// For instance, this may be called in case the stream is kept open while trigerring sequences which are spaced out.
        /// If the stats are not reset FPS computation will account for all elapsed time, including spacing.
        /// Since FPS are computed in intervals this may result in unreliable reports.
        ///
        /// @return PFResult
        ///   PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
        ///   PFSDK_NOERROR
        /// 
        PFResult ResetStatistics();

        /// 
        /// @brief Define the behaviour of the stream object.
        /// 
        /// Provides a means of overriding default behaviour. 
        /// Intended for use with advanced grabbing workflows.
        /// 
        /// i.e Keep the stream channel open and only issue AcquisitionStart and AcquisitionStop commands, reusing resources.
        /// or reset the stream channel to reset the frame count, but keep the parameters locked.
        ///
        /// @param[in] control            New configuration.
        ///
        /// @return  PFResult   
        /// 
        PFResult SetControl(const PFStream::Control& control);

        /// 
        /// @brief Get current control parameters
        /// 
        /// @return  PFStream::Control
        /// 
        PFStream::Control GetControl();

        /// 
        /// @brief Get size of internal pool
        /// 
        /// @return  PFStream::GetPoolSize
        /// 
        uint64_t GetPoolSize();
        
        /// 
        /// @brief Get size of payload size used in internal pool
        /// 
        /// @return  PFStream::GetPayloadSize
        ///
        uint64_t GetPayloadSize();
        
        /// 
        /// @brief Check grab parameters and configure internal pool and ring buffer 
        ///
        /// @return PFResult
        ///   PFSDK_NOERROR
        ///   PFSDK_ERROR_CAMERA_TYPE_NOT_SUPPORTED
        ///   PFSDK_ERROR_WRONG_PARAMETERS
        ///   PFSDK_ERROR_BUFFERCOUNT_INVALID
        ///   PFSDK_ERROR_QUEUE_EMPTY
        ///   PFSDK_ERROR_RING_BUFFER_INIT_FAILURE
        /// 
        PFResult FetchGrabParams();

    protected:
        friend class PFCameraInternal;

        /*! \cond PRIVATE */
        class Impl;
        Impl* m_pImpl;
        /*! \endcond */

        PFStream(Impl* impl);
    };


    /// 
    /// @interface IPFEventStream PFStream.h 
    ///
    /// @brief Declares stream listener function to capture events triggered during acquisition of data. 
    ///
    class IPFEventStream {
    public:
      virtual ~IPFEventStream() {};
      
      /// 
      /// @brief Callback for new Stream Event available.
      ///
      /// @param[in] Stream Event
      ///
      virtual void OnEvent(PFStream::Event e) = 0;
    };
}

#endif //PFSTREAM_H
