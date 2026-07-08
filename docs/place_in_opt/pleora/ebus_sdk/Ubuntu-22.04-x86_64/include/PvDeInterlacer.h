// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVDEINTERLACER_H__
#define __PVDEINTERLACER_H__

#include <PvBuffer.h>


namespace PvBufferLib
{
    class DeInterlacer;
}

///
/// \class PvDeInterlacer
///
/// \brief De-interlacing filter
///
/// This class only supports non-destructive (non-in-place) filtering on buffers with an image payload type.
///
/// It is possible to deinterlace a buffer containing both fields using the #Apply
/// method only taking a single input #PvBuffer as parameter. With this method, 
/// field inversion can be controlled through #SetFieldInversion.
///
/// The other #Apply method takes two independent #PvBuffer objects each containing
/// a field. Even and Odd interlaced fields are required for reconstructions.
/// #SetFieldInversion is ignored when deinterlacing with this #Apply method, the
/// user can simply swap the parameters to obtain the same effect.
///
/// <b>To deinterlace an image:</b>
///     - Configure a deinterlacing object. 
///     - Apply the filter. Use #PvDeInterlacer::Apply.
///
///
class PV_BUFFER_API PvDeInterlacer
{

public:

    ///
    /// \brief Constructor.
    ///
    PvDeInterlacer();

    ///
    /// \brief Destructor.
    ///
    virtual ~PvDeInterlacer();

    ///
    /// \brief De-interlaces an image from a #PvBuffer containing both fields.
    ///
    /// \param [in]  aIn         #PvBuffer used as source.
    /// \param [out] aOut        #PvBuffer used as destination for the operation.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK,
    ///     - #PvResult::Code::INVALID_PARAMETER, if a buffer is NULL or both buffers points to the same data
    ///     - #PvResult::Code::INVALID_DATA_FORMAT if the operation cannot be performed on the buffers
    ///
    PvResult Apply( const PvBuffer *aIn, PvBuffer *aOut );

    ///
    /// \brief De-interlaces an image from a a pair of #PvBuffer, one Even and one Odd field.
    ///
    /// \param [in]  aInOdd      #PvBuffer used as Odd field source.
    /// \param [in]  aInEven     #PvBuffer used as Even field source.
    /// \param [out] aOut        #PvBuffer used as destination for the operation.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK,
    ///     - #PvResult::Code::INVALID_PARAMETER, if a buffer is NULL or both buffers points to the same data
    ///     - #PvResult::Code::INVALID_DATA_FORMAT if the operation cannot be performed on the buffers
    ///
    PvResult Apply( const PvBuffer *aInOdd, const PvBuffer *aInEven, PvBuffer *aOut );

    ///
    /// \brief De-interlaces an image from a #PvBuffer containing an odd field.
    ///
    /// \param [in]  aIn         #PvBuffer used as source.
    /// \param [out] aOut        #PvBuffer used as destination for the operation.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK,
    ///     - #PvResult::Code::INVALID_PARAMETER, if a buffer is NULL or both buffers points to the same data
    ///     - #PvResult::Code::INVALID_DATA_FORMAT if the operation cannot be performed on the buffers
    ///
    PvResult ApplyOdd( const PvBuffer *aIn, PvBuffer *aOut );

    ///
    /// \brief De-interlaces an image from a #PvBuffer containing an even field.
    ///
    /// \param [in]  aIn         #PvBuffer used as source.
    /// \param [out] aOut        #PvBuffer used as destination for the operation.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK,
    ///     - #PvResult::Code::INVALID_PARAMETER, if a buffer is NULL or both buffers points to the same data
    ///     - #PvResult::Code::INVALID_DATA_FORMAT if the operation cannot be performed on the buffers
    ///
    PvResult ApplyEven( const PvBuffer *aIn, PvBuffer *aOut );

    ///
    /// \brief Doubles lines of the field to the output field.
    ///
    /// \param [in]  aIn         #PvBuffer used as source.
    /// \param [out] aOut        #PvBuffer used as destination for the operation.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK,
    ///     - #PvResult::Code::INVALID_PARAMETER, if a buffer is NULL or both buffers points to the same data
    ///     - #PvResult::Code::INVALID_DATA_FORMAT if the operation cannot be performed on the buffers
    ///
    PvResult ApplyDoubling( const PvBuffer *aIn, PvBuffer *aOut );

    ///
    /// \brief Blends (averages) even and odd lines. The result is doubled into the output buffer.
    ///
    /// \param [in]  aInOdd      #PvBuffer used as Odd field source.
    /// \param [in]  aInEven     #PvBuffer used as Even field source.
    /// \param [out] aOut        #PvBuffer used as destination for the operation.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK,
    ///     - #PvResult::Code::INVALID_PARAMETER, if a buffer is NULL or both buffers points to the same data
    ///     - #PvResult::Code::INVALID_DATA_FORMAT if the operation cannot be performed on the buffers
    ///
    PvResult ApplyBlending( const PvBuffer *aInOdd, const PvBuffer *aInEven, PvBuffer *aOut );

    ///
    /// \brief Retrieves the deinterlacer configuration for field inversion.
    ///
    /// \return 
    ///    Includes:
    ///     - False for 1-2 field order (default)
    ///     - True for 2-1 field order
    ///
    bool GetFieldInversion() const;

    ///
    /// \brief Configures the deinterlacer for 1-2 or 2-1 fields order.
    ///
    /// \param [in]  aInvert    False for 1-2 field order (default) or true for 2-1.
    ///
    void SetFieldInversion( bool aInvert );

protected:

private:

    // Not implemented
	PvDeInterlacer( const PvDeInterlacer & );
	const PvDeInterlacer &operator=( const PvDeInterlacer & );

    PvBufferLib::DeInterlacer *mThis;
};

#endif
