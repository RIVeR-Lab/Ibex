// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVTRUESENSECONVERTER_H__
#define __PVTRUESENSECONVERTER_H__

#include <PvBuffer.h>


namespace PvBufferLib
{
    class TruesenseConverter;
}

struct YotsubaParam_t;


///
/// \class PvTruesenseConverter
///
/// \brief Converts images from TRUESENSE(TM) to RGB.
///
/// This class can be used to convert Sparse Color Filter TRUESENSE(TM) images (SCF1)
/// into RGB8 or RGBa8. The image quality is expected to be much better than SCF1 conversion
/// performed using PvBufferConverter.
///
/// The intense image processing algorithms used to obtain this high quality yields much higher 
/// processing times. Various gain and filtering parameters can be used to tweak the image conversion.
/// 
/// SCF1 12-unpacked is recommended as input pixel type for slightly better processing times.
/// Other SCF1 pixel types are also supported but must internally be converted to 
/// 12-unpacked before being converted to RGB.
///
class PV_BUFFER_API PvTruesenseConverter
{
public:

    ///
    /// \brief Constructor
    ///
    PvTruesenseConverter();

    ///
    /// \brief Destructor
    ///
    virtual ~PvTruesenseConverter();

    ///
    /// \brief Returns true if this converter class can be used for a source-to-destination conversion.
    ///
    /// \param[in] aSource Pixel type of the source buffer.
    /// \param[out] aDestination Pixel type of the destination buffer.
    ///
    /// \return True if the conversion is supported, false if not.
    ///
    static bool IsConversionSupported( PvPixelType aSource, PvPixelType aDestination );

    ///
    /// \brief Convert a buffer from one format to another.
    ///
    /// \param[in] aSource The original image.
    /// \param[out] aDestination The new image.
    /// \param[in] aReallocIfNeeded Depending on the conversion, the memory footprint
    ///     of the new image may be greater than aDestination current size. By setting this parameter to true, the 
    ///     buffer is re-allocated if aDestination is too small. If aDestination memory buffer is not owned by the 
    ///     #PvBuffer and is too small, #Convert fails.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_DATA_FORMAT
    ///     - #PvResult::Code::GENERIC_ERROR
    ///
    PvResult Convert( const PvImage *aSource, PvImage *aDestination, bool aReallocIfNeeded = true );

    ///
    /// \brief Returns the dark floor of the raw image.
    ///
    /// \return Dark floor of the raw image.
    ///
    uint32_t GetDarkfloor() const;

    ///
    /// \brief Returns the red gain of white balance.
    ///
    /// \return Red gain of white balance.
    ///
    float GetRedGain() const;

    ///
    /// \brief Returns the green gain of white balance.
    ///
    /// \return Red green of white balance.
    ///
    float GetGreenGain() const;

    ///
    /// \brief Returns the blue gain of white balance.
    ///
    /// \return Blue gain of white balance.
    ///
    float GetBlueGain() const;

    ///
    /// \brief Returns the panchromatic gain of white balance.
    ///
    /// \return Panchromatic gain of white balance.
    ///
    float GetPanGain() const;

    ///
    /// \brief Returns the global gain of white balance.
    ///
    /// \return Global gain of white balance.
    ///
    float GetGlobalGain() const;

    ///
    /// \brief Returns the sharp parameter.
    ///
    /// \return Sharp parameter.
    ///
    float GetSharpenParam() const;

    ///
    /// \brief Returns the sharp maximal threshold.
    ///
    /// \return Sharp maximal threshold.
    ///
    float GetMaxSharpen() const;

    ///
    /// \brief Returns the high noise threshold.
    ///
    /// \return High noise threshold.
    ///
    float GetHighLumaNoise() const;

    ///
    /// \brief Returns the low noise threshold.
    ///
    /// \return Low noise threshold.
    ///
    float GetLowLumaNoise() const;

    ///
    /// \brief Sets the dark floor of the raw image.
    ///
    /// \param[in] aValue New value for the parameter.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///
    PvResult SetDarkfloor( uint32_t aValue );

    ///
    /// \brief Sets the red gain of white balance.
    ///
    /// \param[in] aValue New value for the parameter.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///
    PvResult SetRedGain( float aValue );

    ///
    /// \brief Sets the green gain of white balance.
    ///
    /// \param[in] aValue New value for the parameter.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///
    PvResult SetGreenGain( float aValue );

    ///
    /// \brief Sets the blue gain of white balance.
    ///
    /// \param[in] aValue New value for the parameter.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///
    PvResult SetBlueGain( float aValue );

    ///
    /// \brief Sets the panchromatic gain of white balance.
    ///
    /// \param[in] aValue New value for the parameter.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///
    PvResult SetPanGain( float aValue );

    ///
    /// \brief Sets the global gain of white balance.
    ///
    /// \param[in] aValue New value for the parameter.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///
    PvResult SetGlobalGain( float aValue );

    ///
    /// \brief Sets the sharp parameter.
    ///
    /// \param[in] aValue New value for the parameter.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///
    PvResult SetSharpenParam( float aValue );

    ///
    /// \brief Sets the sharp maximal threshold.
    ///
    /// \param[in] aValue New value for the parameter.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///
    PvResult SetMaxSharpen( float aValue );

    ///
    /// \brief Sets the high noise threshold.
    ///
    /// \param[in] aValue New value for the parameter.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///
    PvResult SetHighLumaNoise( float aValue );

    ///
    /// \brief Sets the low noise threshold.
    ///
    /// \param[in] aValue New value for the parameter.
    ///
    /// \return 
    ///    Includes:
    ///     - #PvResult::Code::OK
    ///     - #PvResult::Code::INVALID_PARAMETER
    ///
    PvResult SetLowLumaNoise( float aValue );

protected:

private:

    // Not implemented
    PvTruesenseConverter( const PvTruesenseConverter & );
    const PvTruesenseConverter &operator=( const PvTruesenseConverter & );

    PvBufferLib::TruesenseConverter *mThis;
    YotsubaParam_t *mParameters;

};

#endif
