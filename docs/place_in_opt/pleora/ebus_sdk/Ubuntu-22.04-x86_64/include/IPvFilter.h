// *****************************************************************************
//
//     Copyright (c) 2015, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __IPVFILTER_H__
#define __IPVFILTER_H__

#include <PvBuffer.h>


///
/// \class IPvFilter
///
/// Base class (interface) of various processing filters provided by the eBUS SDK.
///
class PV_BUFFER_API IPvFilter
{
public:

    ///
    /// \brief Constructor
    ///
    IPvFilter();

    ///
    /// \brief Destructor
    ///
    virtual ~IPvFilter();

    ///
    /// \brief Executes the filter: performs processing on aIn, writing into aOut.
    ///
    /// \param[in] aIn  The input buffer where data is read from.
    /// \param[out] aOut The output buffer where data is written to. 
    ///
    virtual PvResult Execute( const PvBuffer *aIn, PvBuffer *aOut ) = 0;

    ///
    /// \brief Sets the thread count to be used by the filter.
    ///
    /// \param[in] aCount The maximum number of threads that can be used by this filter.
    ///
    /// \return Includes:
    /// - #PvResult::Code::OK
    /// - #PvResult::Code::NOT_SUPPORTED if the filter does not support multi-threading.
    ///
    virtual PvResult SetThreadCount( uint32_t aCount ) = 0;

    ///
    /// \brief Gets the maximum number of threads the filter can use.
    ///
    /// \return The maximum number of threads the filter can use.
    ///
    virtual uint32_t GetThreadCount() const = 0;


///
/// \brief Gets the thread priority of the filter's worker threads.
///
/// \param aPriority [out] Output reference to the worker threads priority.
///
/// \return Includes:
/// - #PvResult::Code::OK
/// - #PvResult::Code::NOT_SUPPORTED if the filter does not support multi-threading.
///
    virtual PvResult GetThreadsPriority( uint32_t &aPriority ) const = 0;

///
/// \brief Sets the thread priority of the filter's worker threads.
///
/// \return Includes:
/// - #PvResult::Code::OK
/// - #PvResult::Code::NOT_SUPPORTED if the filter does not support multi-threading.
/// - #PvResult::Code::INVALID_PARAMETER if aPriority maps to nothing on the target system.
///
    virtual PvResult SetThreadsPriority( uint32_t aPriority ) = 0;

protected:

private:

};

#endif
