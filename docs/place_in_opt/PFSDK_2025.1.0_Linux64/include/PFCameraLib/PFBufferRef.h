#ifndef PFBUFFERREF_H
#define PFBUFFERREF_H

#include <PFBuffer.h>
#include "PFCameraLib.h"

class BufferTest;
namespace pfcameralib
{
///
/// \brief PFBufferRef class
/// Class wrapping PFbuffer to provide easy explicit data sharing.
/// Made to remove the usage of PFbuffer pointers.
/// Changes made to data held by this Object will propagate to others that where created from the same PFBuffer or PFbufferRef.
///
    class PFCAMERALIB_API PFBufferRef
    {
    public:
        ///
        /// \brief PFBufferRef
        ///Default constructor
        PFBufferRef();
        ///
        /// \brief PFBufferRef Constructor that uses a PFBuffer using shallowCopy.
        /// \param buffer
        ///
        PFBufferRef(const PFBuffer &buffer);
        ///
        /// \brief PFBufferRef copy constructor
        /// \param buffer
        ///
        PFBufferRef(const PFBufferRef &buffer);
        ///
        /// \brief PFBufferRef copy constructor
        /// \param buffer
        ///
        PFBufferRef(PFBufferRef &&buffer);

        ///
        /// \brief operator = assign operator
        /// \param buffer
        /// \return
        ///
        PFBufferRef& operator=(const PFBufferRef &buffer);
        ///
        /// \brief operator = Move operator
        /// \param buffer
        /// \return
        ///
        PFBufferRef& operator=(PFBufferRef &&buffer);

        ///
        /// \brief operator -> Overload to access internal PFBuffer data.
        /// \return
        ///
        PFBuffer *operator ->();
        ///
        /// \brief operator -> Overload to access internal PFBuffer data.
        /// \return
        ///
        const PFBuffer *operator ->() const;

        ///
        /// \brief operator * Returns internal PFBuffer by reference
        /// \return
        ///
        PFBuffer& operator*();

    private:
        friend class BufferTest; //Friend class for internal unit testing.
        PFBuffer buff;
    };
}

#endif // PFBUFFERREF_H
