#ifndef _IPF_ASYNC_GRAB_HANDLER_H_
#define _IPF_ASYNC_GRAB_HANDLER_H_

#include "PFCameraLib.h"
#include "PFBufferRef.h"

namespace pfcameralib {

    /// 
    /// @interface IPFAsyncGrabHandler IPFAsyncGrabHandler.h 
    ///
    /// Receive notifications related to acquisition process. Either for new buffers or errors.
    ///
    class PFCAMERALIB_API IPFAsyncGrabHandler {
    public:
      
      virtual ~IPFAsyncGrabHandler() {};

      /// 
      /// @brief Callback for new buffer available.
      ///
      /// The buffer is locked. The user MUST unlock the buffer once it is processed.
      ///
      /// @param[in] buffer Reference object to PFBuffer.
      ///
      virtual void OnNewBuffer(PFBufferRef &buffer) = 0;

      /// 
      /// @brief Callback for error while grabbing.
      ///
      /// @param[in] error  PFResult of the error that was produced while grabbing.
      ///
      virtual void OnGrabError(const PFResult &error) = 0;
    };
}

#endif