// *****************************************************************************
//
//     Copyright (c) 2023, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVDECOMPRESSIONFILTER_H__
#define __PVDECOMPRESSIONFILTER_H__

#include <IPvFilter.h>


namespace PvBufferLib
{
    class DecompressionFilter;
}


///
/// \class PvDecompressionFilter
///
/// Filter used for image decompression.
///

class PV_BUFFER_API PvDecompressionFilter : public IPvFilter
{
public:

    ///
    /// \brief Constructor.
    ///

    PvDecompressionFilter();

    ///
    /// \brief Destructor.
    ///

    virtual ~PvDecompressionFilter();

    ///
    /// \brief Decompresses aIn into aOut.
    ///
    /// If aOut is not an image or does not match the required output pixel type, width or height
    /// it is automatically re-allocated to match the output requirements.
    ///
    /// \param aIn [in] Input buffer.
    /// \param aOut [in] Output buffer.
    ///
    /// \return Includes:
    /// - #PvResult::Code::OK
    /// - #PvResult::Code::INVALID_PARAMETER
    /// - #PvResult::Code::NOT_SUPPORTED if the compression algorithm is not supported by this version of the software.
    /// - #PvResult::Code::IMAGE_ERROR if the input buffer seems corrupted or contains unexpected information.
    ///
    PvResult Execute( const PvBuffer *aIn, PvBuffer *aOut );

    ///
    /// \brief Sets the maximum of threads to use for decompression.
    ///
    /// \param aCount [in] How many threads to use for decompression. Min: 1, Max: 64.
    ///
    /// \return Includes:
    /// - #PvResult::Code::OK
    /// - #PvResult::Code::INVALID_PARAMETER if aCount is not valid.
    /// - #PvResult::Code::NOT_ENOUGH_MEMORY if all threads could not be allocated.
    /// - #PvResult::Code::NOT_SUPPORTED if the filter does not support multi-threading.
    ///
    PvResult SetThreadCount( uint32_t aCount );

    ///
    /// \brief Returns how many threads are used for decompression.
    ///
    /// \return How many threads are used for decompression.
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
    /// The default is  3 - THREAD_PRIORITY_NORMAL, see PvPipeline::SetBufferHandlingThreadPriority().
    /// We only allow setting priority for the threads of one converter object as a group.
    /// By default one worker thread is created for half of the total number of logical CPUs of the system.
    ///
    /// If you change the thread priorities, it may alterate the stability of the system. For this reason,
    /// if you report a bug to Pleora, ensure to clearly mention that you change threads priorities.
    ///
    /// \return Includes:
    /// - #PvResult::Code::OK
    /// - #PvResult::Code::NOT_SUPPORTED if the filter does not support multi-threading.
    /// - #PvResult::Code::INVALID_PARAMETER if aPriority maps to nothing on the target system.
    ///
    PvResult SetThreadsPriority( uint32_t aPriority );

    ///
    /// \brief Returns true if the buffer can be decompressed using an object of this class.
    ///
    /// \param aIn [in] Buffer to inspect.
    ///
    /// \return True if the buffer can be decompressed.
    ///
    static bool IsCompressed( const PvBuffer *aIn );

    ///
    /// \brief Returns the output format for a compressed buffer.
    ///
    /// \param aIn [in] Buffer to inspect.
    /// \param aPixelType [out] Pixel type the buffer is uncompressed to.
    /// \param aWidth [out] Width of the uncompressed buffer, in pixels.
    /// \param aHeight [out] Height of the uncompressed buffer, in pixels.
    ///
    /// \return Includes:
    /// - #PvResult::Code::OK
    /// - #PvResult::Code::NOT_SUPPORTED if the buffer is not a compatible compressed buffer.
    ///
    static PvResult GetOutputFormatFor( const PvBuffer *aIn, PvPixelType &aPixelType, uint32_t &aWidth, uint32_t &aHeight );

protected:

private:

    // Not implemented
    PvDecompressionFilter( const PvDecompressionFilter & );
    const PvDecompressionFilter &operator=( const PvDecompressionFilter & );

    PvBufferLib::DecompressionFilter *mThis;

};


#endif // __PVDECOMPRESSIONFILTER_H__

