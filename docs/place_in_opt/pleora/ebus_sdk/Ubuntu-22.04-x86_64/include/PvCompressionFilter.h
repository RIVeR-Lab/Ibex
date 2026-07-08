// *****************************************************************************
//
//     Copyright (c) 2023, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVCOMPRESSIONFILTER_H__
#define __PVCOMPRESSIONFILTER_H__

#include <IPvFilter.h>
#include <PvEncoding.h>


namespace PvBufferLib
{
    class PTCEncoder;
}

///
/// \class PvCompressionFilter
///
/// Filter used for image compression.
///

class PV_BUFFER_API PvCompressionFilter : public IPvFilter
{
public:

    ///
    /// \brief Constructor.
    ///
    PvCompressionFilter();


    ///
    /// \brief Destructor.
    ///
    virtual ~PvCompressionFilter();

    ///
    /// \brief Compresses aIn into aOut.
    ///
    /// \param aIn [in] Input buffer.
    /// \param aOut [in] Output buffer.
    ///
    /// \return Includes:
    /// - #PvResult::Code::OK
    /// - #PvResult::Code::INVALID_PARAMETER
    /// - #PvResult::Code::NOT_SUPPORTED if aIn cannot be compressed (not an image, pixel type not supported, etc)
    ///
    PvResult Execute( const PvBuffer *aIn, PvBuffer *aOut );

    ///
    /// \brief Sets the maximum of threads to use for compression. Not supported for compression.
    ///
    /// \param aCount [in] How many threads to use for compression.
    ///
    /// \return Includes:
    /// - #PvResult::Code::NOT_SUPPORTED the compression filter does not support multi-threading.
    ///
    PvResult SetThreadCount( uint32_t aCount );

    ///
    /// \brief Returns how many threads are used for compression.
    ///
    /// \return How many threads are used for compression.
    ///
    uint32_t GetThreadCount() const;

    ///
    /// \brief Gets the thread priority of the filter's worker threads.
    ///
    /// \param aPriority [out] Output reference to the worker threads priority.
    ///
    /// \return Includes:
    /// - #PvResult::Code::OK
    /// - #PvResult::Code::NOT_SUPPORTED if the filter does not support multi-threading.
    ///
    PvResult GetThreadsPriority( uint32_t &aPriority ) const;

    ///
    /// \brief Sets the thread priority of the filter's worker threads.
    ///
    /// Only one worker thread is created.
    /// Setting thread priority isn't supported.
    ///
    /// \return Includes:
    /// - #PvResult::Code::OK
    /// - #PvResult::Code::NOT_SUPPORTED if the filter does not support multi-threading.
    /// - #PvResult::Code::INVALID_PARAMETER if aPriority maps to nothing on the target system.
    ///
    PvResult SetThreadsPriority( uint32_t aPriority );

    ///
    /// \brief Sets the encoding used when compressing the image.
    ///
    /// \param aValue [in] Encoding to be used by the filter.
    ///
    /// \return Includes:
    /// - #PvResult::Code::OK
    /// - #PvResult::Code::NOT_SUPPORTED if the requested encoding is not supported.
    ///
    PvResult SetEncoding( PvEncodingEnum aValue );

    ///
    /// \brief Returns the encoding used when compressing the image.
    ///
    PvEncodingEnum GetEncoding() const;

protected:

private:

    // Not implemented
    PvCompressionFilter( const PvCompressionFilter & );
    const PvCompressionFilter &operator=( const PvCompressionFilter & );

    PvBufferLib::PTCEncoder *mThis;

};


#endif // __PVCOMPRESSIONFILTER_H__

