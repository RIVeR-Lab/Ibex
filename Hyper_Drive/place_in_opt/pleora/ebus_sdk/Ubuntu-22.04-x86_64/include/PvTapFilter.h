// *****************************************************************************
//
//     Copyright (c) 2015, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVTAPFILTER_H__
#define __PVTAPFILTER_H__

#include <IPvFilter.h>
#include <PvTapGeometry.h>


class TapFilter;

///
/// \class PvTapFilter
///
/// Filter used for tap reconstruction.
///
class PV_BUFFER_API PvTapFilter : public IPvFilter
{
public:

    ///
    /// \brief Constructor
    ///
    PvTapFilter();

    ///
    /// \brief Destructor
    ///
    virtual ~PvTapFilter();

    ///
    /// \brief Executes the filter: performs tap reconstruction on aIn, writing into aOut.
    ///
    /// \param[in] aIn  The input buffer where data is read from. Must be a PvImage.
    /// \param[out] aOut The output buffer where data is written to. Can be resized if needed if the 
    ///                  buffer is allocated. If the buffer is attached, the buffer must be of the right
    ///                  image size and pixel type.
    ///
    /// \return Includes:
    /// - #PvResult::Code::OK
    /// - #PvResult::Code::INVALID_PARAMETER
    /// - #PvResult::Code::NOT_ENOUGH_MEMORY
    /// - #PvResult::Code::INVALID_DATA_FORMAT
    /// - #PvResult::Code::NOT_SUPPORTED
    /// - #PvResult::Code::GENERIC_ERROR
    ///
    PvResult Execute( const PvBuffer *aIn, PvBuffer *aOut );

    ///
    /// \brief Sets the thread count to be used by the filter.
    ///
    /// \param[in] aCount The maximum number of threads that can be used by this filter.
    ///
    /// \return Includes:
    /// - #PvResult::Code::OK
    /// - #PvResult::Code::NOT_SUPPORTED if the filter does not support multi-threading.
    ///
    PvResult SetThreadCount( uint32_t aCount );

    ///
    /// \brief Gets the maximum number of threads the filter can use.
    ///
    /// \return The maximum number of threads the filter can use.
    ///
    uint32_t GetThreadCount() const;

    PvResult GetThreadsPriority( uint32_t &aPriority ) const;
    PvResult SetThreadsPriority( uint32_t aPriority );

    ///
    /// \brief Sets the tap geometry for tap reconstruction.
    ///
    /// \param[in] aValue New tap geometry used by the filter.
    ///
    /// \return Includes:
    /// - #PvResult::Code::OK
    ///
    PvResult SetGeometry( PvTapGeometryEnum aValue );

    ///
    /// \brief Returns the tap geometry currently used by the filter.
    ///
    /// \return Tap geometry currently used by the filter.
    ///
    PvTapGeometryEnum GetGeometry() const;

    ///
    /// \brief Static method returning how many tap geometries are supported by the filter.
    ///
    /// Can be used with #GetSupportedGeometryValue and #GetSupportedGeometryName to populate 
    /// UI combo boxes allowing tap geometry selection.
    ///
    /// \return Count of supported tap geometries.
    ///
    static uint32_t GetSupportedGeometryCount();

    ///
    /// \brief Static method returning the value of a tap geometry indexed in a zero-based supported geometries array.
    ///
    /// Can be used with #GetSupportedGeometryCount and #GetSupportedGeometryName to populate 
    /// UI combo boxes allowing tap geometry selection.
    ///
    /// \param[in] aIndex Zero-based index, maximum is #GetSupportedGeometryCount - 1.
    ///
    /// \return PvTapGeometryEnum value.
    ///
    static PvTapGeometryEnum GetSupportedGeometryValue( uint32_t aIndex );

    ///
    /// \brief Static method returning the name of a tap geometry indexed in a zero-based supported geometries array.
    ///
    /// Can be used with #GetSupportedGeometryCount and #GetSupportedGeometryValue to populate 
    /// UI combo boxes allowing tap geometry selection.
    ///
    /// \param[in] aIndex Zero-based index, maximum is #GetSupportedGeometryCount - 1.
    ///
    /// \return Human understandable tap geometry name.
    ///
    static const PvString &GetSupportedGeometryName( uint32_t aIndex );

    ///
    /// \brief Static method used to query the output pixel type for an input image of a specific pixel type.
    ///
    /// This method can be used if the output buffer provided to Execute is attached and not allocated. The filter
    /// can re-allocate an allocated buffer if needed (mismatch on width, height or pixel format) but attached
    /// external buffers must be properly formatted before calling Execute.
    ///
    /// To properly prepare the output buffer, use the same width and height as the input buffer. For the pixel
    /// format, use this static method with the input buffer in aBuffer to query the required pixel type of the
    /// output buffer.
    ///
    /// \param[in] aBuffer 
    ///
    /// \return Required pixel type of the aOutput of #Execute if aBuffer is used as aInput. PvPixelUndefined is returned
    ///         if the filter is not compatible with aBuffer (unsupported pixel type, not a PvImage, etc.)
    ///
    static PvPixelType GetOutputPixelTypeFor( const PvBuffer *aBuffer );

    ///
    /// \brief Utility method returning how many taps are used to a specific tap geometry.
    ///
    /// \param[in] aValue Tap geometry enum.
    ///
    /// \return Tap count.
    ///
    static uint32_t GetTapCountFor( PvTapGeometryEnum aValue );

    ///
    /// \brief Utility method returning how many taps are used to a specific tap geometry.
    ///
    /// \param[in] aValue SFNC compliant tap geometry name.
    ///
    /// \return Tap count.
    ///
    static uint32_t GetTapCountFor( const PvString &aValue );

protected:

private:

    // Not implemented
    PvTapFilter( const PvTapFilter & );
    const PvTapFilter &operator=( const PvTapFilter & );

    TapFilter * mThis;

};

#endif
