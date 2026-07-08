// *****************************************************************************
//
//     Copyright (c) 2008, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVBUFFERCONVERTERRGBFILTER_H__
#define __PVBUFFERCONVERTERRGBFILTER_H__

#include <PvBuffer.h>


namespace PvBufferLib
{
    class BufferConverterRGBFilter;
}


class PvBufferConverter;

///
/// \class PvBufferConverterRGBFilter
///
/// \brief Contains a RGB filter configuration that can be used with a PvBufferConverter.
///
/// <b>To configure an RGB filter (manual):</b>
///     - Create an RGB filter controller.
///     - Set the filter's values. Use SetGain* and SetOffset* methods. See #SetGainB.
///
/// <b>To configure a white balance filter (automatic):</b>
///     - Create an RGB filter object.
///     - Calculate and apply white-balance settings. Use #WhiteBalance.
///
class PV_BUFFER_API PvBufferConverterRGBFilter
{

public:

    ///
    /// \brief Constructor.
    ///
    PvBufferConverterRGBFilter();

    ///
    /// \brief Destructor.
    ///
    virtual ~PvBufferConverterRGBFilter();

    ///
    /// \brief Get the filter's gain (red).
    ///
    /// See #SetGainB.
    ///
    /// \return See #SetGainB.
    ///
    double GetGainR() const;

    ///
    /// \brief Get the filter's gain (green).
    ///
    /// See #SetGainB.
    ///
    /// \return See #SetGainB.
    ///
    double GetGainG() const;

    ///
    /// \brief Get the filter's gain (blue).
    ///
    /// See #SetGainB.
    ///
    /// \return See #SetGainB.
    ///
    double GetGainB() const;

    ///
    /// \brief Set the filter's gain (red)
    ///
    /// See #SetGainB.
    ///
    /// \param[in] aValue  See #SetGainB.
    ///
    void SetGainR( double aValue );

    ///
    /// \brief Set the filter's gain (green)
    ///
    /// See #SetGainB.
    ///
    /// \param[in]  aValue  See #SetGainB.
    ///
    void SetGainG( double aValue );

    ///
    /// \brief Set the filter's gain (blue).
    ///
    /// The SetGain* and SetOffset methods let you control the 
    /// transformation made by the filter, for the colors
    /// red, green, and blue.
    ///
    /// The transformation uses the form:
    /// <br><i>x'</i> = <i>kx</i> + y
    ///
    /// where:
    ///     - <i>x'</i> is the modified color level (for one of red, blue, or green)
    ///     - <i>x</i> is the original color level (for one of red, blue, or green)
    ///     - <i>k</i> is the level multiplier
    ///     - <i>y</i> is the offset
    ///
    /// To set a filter's values, use:
    ///     - #SetGainB
    ///     - #SetGainG
    ///     - #SetGainR
    ///     - #SetOffsetB
    ///     - #SetOffsetG
    ///     - #SetOffsetR
    ///
    /// To get a filter's values, use:
    ///     - #GetGainB
    ///     - #GetGainG
    ///     - #GetGainR
    ///     - #GetOffsetB
    ///     - #GetOffsetG
    ///     - #GetOffsetR
    ///
    ///
    /// \param[in] aValue  The new gain (blue).
    ///
    void SetGainB( double aValue );

    ///
    /// \brief Get the filter's offset (red).
    ///
    /// See #SetGainB.
    ///
    /// \return See #SetGainB.
    ///
    int32_t GetOffsetR() const;

    ///
    /// \brief Get the filter's offset (green).
    ///
    /// See #SetGainB.
    ///
    /// \return See #SetGainB.
    ///
    int32_t GetOffsetG() const;

    ///
    /// \brief Get the filter's offset (blue).
    ///
    /// See #SetGainB.
    ///
    /// \return See #SetGainB.
    ///
    int32_t GetOffsetB() const;
    
    ///
    /// \brief Set the filter's offset (red)
    ///
    /// See #SetGainB.
    ///
    /// \param[in] aValue  See #SetGainB.
    ///
    void SetOffsetR( int32_t aValue );

    ///
    /// \brief Set the filter's offset (green)
    ///
    /// See #SetGainB.
    ///
    /// \param[in]  aValue  See #SetGainB.
    ///
    void SetOffsetG( int32_t aValue );

    ///
    /// \brief Set the filter's offset (blue)
    ///
    /// See #SetGainB.
    ///
    /// \param[in]  aValue  See #SetGainB.
    ///
    void SetOffsetB( int32_t aValue );

    ///
    /// \brief Calculate white-balance gains.
    ///
    /// The #WhiteBalance method receives an image and calculates
    /// appropriate gains to white-balance the image. This method
    /// sets the gains as if having called #SetGainB, #SetGainG, and #SetGainR.
    ///
    /// Internally, the #WhiteBalance method uses the following procedure:
    ///     - Calculate the average colors in the image: Bavg, Gavg, Ravg.
    ///     - Select the maximum of the three averages: Max = max (Bavg, Gavg, Ravg).
    ///     - Set the new gains, where:
    ///         - Bgain = Max / Bavg
    ///         - Ggain = Max / Gavg
    ///         - Rgain = Max / Ravg
    ///
    /// The #WhiteBalance method produces the best results for images that have an even mix of dark and light tones,
    /// and an even mix of colors. 
    ///
    /// \param[in]  aBuffer   The sample image; the image upon which to base the 
    ///                        white-balance settings.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER, 
    ///
    PvResult WhiteBalance( PvBuffer *aBuffer );

    ///
    /// \brief Reset the RGB filter configuration.
    ///
    /// This method resets the RGB filter configuration (#PvBufferConverterRGBFilter) to its default values.
    ///
    /// Gains are set to 1; offsets, to 0. After a reset, applying the filter doesn't change the image.
    ///
    void Reset();

protected:

private:

    friend class PvBufferConverter;

    // Not implemented
	PvBufferConverterRGBFilter( const PvBufferConverterRGBFilter & );
	const PvBufferConverterRGBFilter &operator=( const PvBufferConverterRGBFilter & );

    PvBufferLib::BufferConverterRGBFilter *mThis;
};

#endif
