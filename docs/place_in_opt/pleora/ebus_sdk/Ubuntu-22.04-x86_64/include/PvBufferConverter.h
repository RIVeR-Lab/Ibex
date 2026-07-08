// *****************************************************************************
//
//     Copyright (c) 2008, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVBUFFERCONVERTER_H__
#define __PVBUFFERCONVERTER_H__

#ifndef EBUS_ENUM
#define EBUS_ENUM(x) typedef enum
#endif

#include <PvBuffer.h>
#include <PvBufferConverterRGBFilter.h>


namespace PvBufferLib
{
    class BufferConverter;
}

/// \file PvBufferConverter.h
///

/// \enum PvBayerFilterType
/// \brief Filter settings for #PvBufferConverter::SetBayerFilter.
///

/// \var PvBayerFilterSimple
/// \brief 2x2 nearest neighbor used when converting from Bayer.
///

/// \var PvBayerFilter3X3
/// \brief 3x3 kernel considered when converting from Bayer.
///
EBUS_ENUM(PvBayerFilterType)
{
    PvBayerFilterSimple = 1,
    PvBayerFilter3X3 = 2

} PvBayerFilterType;

/// \enum PvBiColorFilterType
/// \brief Filter settings for #PvBufferConverter::SetBiColorFilter.
///

/// \var PvBiColorFilterSimple
/// \brief 2x2 nearest neighbor used when converting from BiColor.
///

/// \var PvBiColorFilter3X3
/// \brief 3x3 kernel considered when converting from BiColor.
///
EBUS_ENUM(PvBiColorFilterType)
{
    PvBiColorFilterSimple = 1,
    PvBiColorFilter3X3 = 2

} PvBiColorFilterType;


///
/// \class PvBufferConverter
///
/// \brief Used to convert a #PvBuffer to another pixel type.
///
/// <b>To change an image's format:</b>
///
///     - Acquire an image. See #PvStream::RetrieveBuffer.
///     - Create an image-format converter (#PvBufferConverter object). Use #PvBufferConverter::PvBufferConverter.
///     - Test if you can convert your image to your preferred pixel type. Use #IsConversionSupported.
///         For your source pixel type, use #PvImage::GetPixelType; for your destination, select an enumeration from
///         PvPixelType.h.
///     - Create a destination buffer. See #PvBuffer (you define the destination image's pixel type during this step).
///     - Optionally, configure options like Bayer filtering and BiColor filtering, use #SetBayerFilter, SetBiColorFilter.
///     - Convert the image to the new pixel type. Use #Convert.
///
/// Consult the #PvPixelType documentation in order to find which pixel types are supported as conversion destinations.
///
class PV_BUFFER_API PvBufferConverter
{
public:

    ///
    /// \brief Constructor.
    ///
    /// \param[in] aMaxNumberOfThreads Maximum number of threads the buffer converter
    ///                                 is allowed to use in order to speed up conversion.
    ///                                 If 0 or smaller than 0, one thread per processor
    ///                                 is assumed. Threads have no specific processor
    ///                                 affinity.
    ///
    PvBufferConverter( int32_t aMaxNumberOfThreads = -1 );
    
    ///
    /// \brief Destructor.
    ///
    virtual ~PvBufferConverter();

    ///
    /// \brief Test if a pixel type can be converted to another type.
    ///
    /// Converting from one pixel type to another isn't a trivial mathematical 
    /// calculation - all conversions require specialized, dedicated algorithms. 
    /// 
    /// The #IsConversionSupported method tests if
    /// the #Convert method has an available conversion algorithm.
    ///
    /// \param[in] aSource       The original pixel type. For available
    ///                           pixel types, see PvPixelType.h.
    /// \param[in] aDestination  The new pixel type. See <i>aSource</i>.
    ///
    /// \return True if #Convert has an available conversion algorithm; 
    ///         otherwise, false.
    ///
    ///
    static bool IsConversionSupported( PvPixelType aSource, PvPixelType aDestination );

    ///
    /// \brief Convert a buffer from one format to another.
    ///
    /// \param[in]  aSource            The original image/buffer.
    /// \param[out] aDestination       The new image/buffer.
    /// \param[in]  aReallocIfNeeded   Depending on the conversion, the memory footprint
    ///                                 of the new image may be greater than aDestination
    ///                                 current size. By setting this parameter to true, the 
    ///                                 buffer is re-allocated if aDestination is too small.
    ///                                 If aDestination memory buffer is not owned by the 
    ///                                 #PvBuffer and is too small, #Convert fails.
    /// \param[in] aFlipY              When performing the conversion, flips the image on its Y axis.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_DATA_FORMAT
    ///     - #PvResult::Code::GENERIC_ERROR
    ///
    PvResult Convert( const PvBuffer *aSource, PvBuffer *aDestination, bool aReallocIfNeeded = true, bool aFlipY = false );

    ///
    /// \brief Convert a buffer from one format to another.
    ///
    /// \param[in]  aSource            The original image.
    /// \param[out] aDestination       The new image/buffer.
    /// \param[in]  aReallocIfNeeded   Depending on the conversion, the memory footprint
    ///                                 of the new image may be greater than aDestination
    ///                                 current size. By setting this parameter to true, the 
    ///                                 buffer is re-allocated if aDestination is too small.
    ///                                 If aDestination memory buffer is not owned by the 
    ///                                 #PvBuffer and is too small, #Convert fails.
    /// \param[in] aFlipY              When performing the conversion, flips the image on its Y axis.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_DATA_FORMAT
    ///     - #PvResult::Code::GENERIC_ERROR
    ///
    PvResult Convert( const IPvImage *aSource, PvBuffer *aDestination, bool aReallocIfNeeded = true, bool aFlipY = false );

    ///
    /// \brief Retrieves current optional Bayer filtering.
    /// 
    /// See #SetBayerFilter
    ///
    /// \return The Bayer filter's type; a #PvBayerFilterType enumeration.
    ///
    PvBayerFilterType GetBayerFilter() const;

    ///
    /// \brief Set the optional Bayer filtering for this converter.
    ///
    /// Of course, only applies if the #Convert method uses a #PvBuffer having a 
    /// Bayer pixel type. Simply ignored otherwise.
    ///
    /// \param[in] aFilter   The new filter settings; a #PvBayerFilterType enumeration.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///
    PvResult SetBayerFilter( PvBayerFilterType aFilter );

    ///
    /// \brief Retrieves current optional BiColor filtering.
    /// 
    /// See #SetBiColorFilter
    ///
    /// \return The BiColor filter's type; a #PvBiColorFilterType enumeration.
    ///
    PvBiColorFilterType GetBiColorFilter() const;

    ///
    /// \brief Set the optional BiColor filtering for this converter.
    ///
    /// Of course, only applies if the #Convert method uses a #PvBuffer having a 
    /// BiColor pixel type. Simply ignored otherwise.
    ///
    /// \param[in] aFilter   The new filter settings; a #PvBiColorFilterType enumeration.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///
    PvResult SetBiColorFilter( PvBiColorFilterType aFilter );

    ///
    /// \brief Resets the RGB filter used for conversion (if any)
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///
    PvResult ResetRGBFilter();

    ///
    /// \brief Assigns a RGB filter to be used during conversion
    ///
    /// \param[in] aFilter   The RGB filter to be used during conversion.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///
    PvResult SetRGBFilter( PvBufferConverterRGBFilter &aFilter );

    ///
    /// \brief Get the priority of the threads doing the conversion in background
    ///
    /// See #SetConversionThreadsPriority for details about the meaning of the values returned
    /// 
    /// \return The priority of the thread
    ///
    uint32_t GetConversionThreadsPriority() const;

    ///
    /// \brief Set the priority of the threads doing the conversion
    ///
    /// By default one conversion thread is created for each processing core of the system. 
    /// We only allow setting priority for the threads of one converter object as a group.
    ///
    /// If you change the thread priorities, it may alterate the stability of the system. For this reason,
    /// if you report a bug to Pleora, ensure to clearly mention that you change threads priorities.
    ///
    /// See #GetConversionThreadsPriority
    /// 
    /// \param[in] aPriority   See #PvPipeline::SetBufferHandlingThreadPriority for description of the supported values
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///     - #PvResult::Code::THREAD_ERROR
    ///
    PvResult SetConversionThreadsPriority( uint32_t aPriority );

protected:

private:

    // Not implemented
	PvBufferConverter( const PvBufferConverter & );
	const PvBufferConverter &operator=( const PvBufferConverter & );

    PvBufferLib::BufferConverter *mThis;
};

#endif
