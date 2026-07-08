/////////////////////////////////////////////////////
//
//  Date: 9 April, 2018
//
//  Authors:
//    Wouter Charle, Vincent Radelet
//
//  Copyright 2018 imec. All rights reserved.
//
////////////////////////////////////////////

#ifndef HSI_API_TYPES_H
#define HSI_API_TYPES_H


#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    // type definitions
    typedef int HSI_RETURN; ///< Type of return value of API function calls.
    typedef void* HANDLE;   ///< Handle type keeping track of device instances.
#ifndef __cplusplus
    typedef unsigned char bool;
    static const bool false = 0;
    static const bool true = 1;
#endif

    /*!
    \brief When acquiring either a cube or a frame, acquisition time is expressed as elapsed microseconds since epoch.
    Conversion to traditionnal format will require to add this time to the definition of epoch.
    Since it is system dependant, non hardcoded checks or conversion methods can relay on EPOCH_STR.
    January 1st 1970 has been chosen.
    */
    const char EPOCH_STR [] = "1970-1-1T0:0:0"; ///< YYYY-mm-ddTHH:MM:SS
    /*!
    \brief The maximum length of strings allocated on the stack.
    */
    const int c_MAX_STRING_LENGTH = 64;

    /*! 
    \brief Enumeration of all possible verbosity levels for the logging system.
    
    The logging system has multiple verbosity levels to filter the output.
    Starting from the most permissive (LL_VERBOSE) to the least (LL_ERROR), each severity step reduces the output.
    Therefore, LL_ERROR only outputs errors while LL_INFO ouputs infos, warnings and errors, etc.

    \sa InitializeLogger
    */
    typedef enum LoggerVerbosity
    {
          LV_ERROR      ///< Logs on error only.
        , LV_WARNING    ///< Logs on error and warning.
        , LV_INFO       ///< Logs on info and higher.
        , LV_DEBUG      ///< Logs on debug and higher.
        , LV_VERBOSE    ///< Logs on verbose and higher.
        , LV_NONE       ///< No logging.
    } LoggerVerbosity;

    /*!
    \brief Enumeration of all supported File Writing formats.

    \sa SaveFrame, SaveCube
    */
    typedef enum FileFormat
    {
          // Frame output formats
          // When cubes are saved in one of these formats, every band is saved in a separate file.
          FF_PGM      =  0    ///< The PGM file format. \see https://en.wikipedia.org/wiki/Netpbm_format
        , FF_TIFF     =  1    ///< The TIFF file format. \see https://en.wikipedia.org/wiki/Tagged_Image_File_Format
        , FF_PNG      =  2    ///< The PNG file format. \see https://en.wikipedia.org/wiki/Portable_Network_Graphics
        , FF_BMP      =  3    ///< The BMP file format. \see https://en.wikipedia.org/wiki/BMP_File_Format
        , FF_JPEG     =  4    ///< The JPEG file format. \see https://en.wikipedia.org/wiki/JPEG

          // Cube output formats. These formats cannot be used for frames.
        , FF_ENVI     = 10    ///< The ENVI file format. \see http://www.exelisvis.com/docs/enviheaderfiles.html
        , FF_NRRD     = 11    ///< The NRRD file format. \see http://teem.sourceforge.net/nrrd/

          // Generic output format: raw (with xml header).
        , FF_RAW      = 20    ///< A raw data format (binary dump of the array data).

    } FileFormat;

    /*! 
    \brief Enumeration of all possible return values of functions in imec API function calls.
    
    Each method is returning a feedback on its execution. 
    While successful calls return HSI_OK, others return values that provide an insight of the error cause.
    Further information are given in the corresponding methods' documentation.
    */
    typedef enum ReturnValue
    {
        HSI_OK                          = 0,    ///< Function call successful.
        HSI_HANDLE_INVALID              = 1,    ///< Invalid device handle specified.
        HSI_ARGUMENT_INVALID            = 2,    ///< Invalid argument provided in function call.
        HSI_CALL_ILLEGAL                = 3,    ///< Function call illegal given the current internal state.
        HSI_FILE_NOT_FOUND              = 10,   ///< A file could not be found.
        HSI_CALIBRATION_FILE_NOT_FOUND  = 11,   ///< Sensor calibration file could not be found.
        HSI_CONNECTION_FAILED           = 12,   ///< Camera could not be connected to the system.
        HSI_ALLOCATION_ERROR            = 20,   ///< Allocation of resources failed.
        HSI_ACQUISITION_TIMEOUT         = 21,   ///< An Acquisition timeout has occured.
        HSI_ACQUISITION_FAILED          = 30,   ///< Acquisition failed during operation.
        HSI_DATA_NOT_ALLOCATED          = 32,   ///< Provided data structure is not allocated.
        HSI_DATA_NOT_VALID              = 33,   ///< Data with valid flag false provided as input for operation, or as resulting output.
        HSI_DATA_NOT_COMPATIBLE         = 34,   ///< Data provided is not compatible.
        HSI_FILE_SYSTEM_ERROR           = 40,   ///< Specified directory doesn't exist and could not be created.
        HSI_FILE_IO_ERROR               = 41,   ///< Could not read or write data from the filesystem.
        HSI_INTERNAL_ERROR              = 100   ///< An unexpected internal error occurred.
    } ReturnValue;
    
    /*!
    \brief Enumeration to specify if a cube is internally specified as Band-SeQuential or as Band-Interleaved-Pixels

    \sa CubeDataFormat
    */
    typedef enum CubeKind
    {
      CK_HYPERCUBE_BSQ
    , CK_HYPERCUBE_BIP
    } CubeKind;

    /*! 
    \brief Structure describing the data format of Cube data 
    
    Once configured, the system is outputting cubes of a given format.
    Description of such format is hereby contained and must be used to allocate cube in memory.

    \warning Must be deallocated properly to prevent memory leaks.

    \sa CubeFloat, AllocateCube, AllocateSpectrum, DeallocateCubeDataFormat
    */
    typedef struct CubeDataFormat
    {
        int         width;              ///< Number of columns per band.
        int         height;             ///< Number of rows per band.
        CubeKind    cube_kind;          ///< kind of the cube: difference between AK_HYPERCUBE_BSQ and AK_HYPERCUBE_BIP
        int         nr_bands;           ///< Nuber of bands.
        double*     band_names;         ///< Bands are reffered by index at acquisition then by wavelength after correction. \sa ApplySpectralCorrection
        long long   size_bytes;         ///< The disk space required, in bytes.

        void* p_reserved { nullptr };   ///< Reserved pointer. Do not use.
    } CubeDataFormat;

    /*! 
    \brief Structure gathering runtime information about Cube data 
    
    Some info about the acquisition like integration time are meant to remain unchanged.
    Actions such applying white reference or spectral correction will impact others like flags and saturation value.

    \sa CubeFloat, SpectrumFloat
    */
    typedef struct CubeDataInfo
    {
        double analog_gain;             ///< Analog gain in dB. Automatically filled on acquisition. \sa RuntimeParameters
        double integration_time_ms;     ///< Integration time used during acquisition. Automatically filled on acquisition. \sa RuntimeParameters
        long long acquisition_time_us;  ///< Acquisition time in microsecond since January 1st 1970. Automatically filled on acquisition. \sa EPOCH_STR
        double value_max;               ///< Highest data value. Might vary through processes.
        double value_saturation;        ///< Threshold indicating saturated data. Might vary through processes.
        double value_no_data;           ///< Value indicating unacquired data. Might vary through processes.
        double value_min;               ///< Lowest data value. Might vary through processes.
        char   system_id [64];          ///< Identifier of the system. Automatically filled on acquisition.
        bool   valid;                   ///< Validity flag. Might vary through processes. Allocated yet unacquired cubes are not valid.
        bool   dark_field_corrected;    ///< Bias correction flag. Automatically filled on acquisition. Cubes are bias corrected by default at acquisition.
        bool   vignetting_corrected;    ///< Vignetting flag. Not yet implemented.
        bool   white_balanced;          ///< White balance flag. Modified when white reference is applied. \sa ApplyWhiteReference
        bool   spectral_corrected;      ///< Spectral correction flag. Modified when spectral correction is applied. \sa ApplySpectralCorrection
    } CubeDataInfo;

    /*! 
    \brief Structure representing cube data with single precision floating point data.
    
    Gather format, info, and data within one struct.

    Access to the continuous memory block of data can be obtained by float* p_data = &ppp_data[0][0][0];

    \sa AllocateCube, DeallocateCube, SaveCube, LoadCube
    */
    typedef struct CubeFloat
    {
        struct CubeDataFormat format;   ///< Data format of the cube.
        struct CubeDataInfo   info;     ///< Runtime information on the cube.

        float* const* const* ppp_data;  ///< 3D data pointer to the cube data, ordered as band, row, column (i.e., \code{.c}ppp_data[band_idx][row_idx][col_idx]\endcode). 

        void* p_reserved { nullptr };   ///< Reserved pointer. Do not use.
    } CubeFloat;

    /*!
    \brief Structure representing a single spectrum in single precision floating point.

    The spectrum consists of data samples at a number of specific wavelengths.
    Information is retained from the cube the spectrum is extracted from.

    \sa AllocateSpectrum, DeallocateSpectrum, SaveSpectrum, LoadSpectrum
    */
    typedef struct SpectrumFloat
    {
        struct CubeDataInfo   info; ///< Information on the specral data.

        int     nr_bands;           ///< Nuber of bands.
        double* p_wavelength_nm;    ///< Positions of the samples in wavelength (nm).
        float*  p_data;             ///< Data pointer to the sample data.
    } SpectrumFloat;

    /*! 
    \brief Structure describing the data format of Frame data.
        
    Once configured, the system is outputting frame of a given format.
    Description of such format is hereby contained and must be used to allocate frame in memory.

    \warning Must be deallocated properly to prevent memory leaks.

    \sa FrameFloat, AllocateFrame, DeallocateFrameDataFormat, GetOutputFrameDataFormat
    */
    typedef struct FrameDataFormat
    {
        int         width;              ///< Number of columns.
        int         height;             ///< Number of rows.
        long long   size_bytes;         ///< The disk space required, in bytes.
        
        void* p_reserved { nullptr };   ///< Reserved pointer. Do not use.
    } FrameDataFormat;

    /*! 
    \brief Structure gathering runtime information about Frame data.
    
    Contains information about the system settings when the frame was acquired.

    \sa FrameFloat
    */
    typedef struct FrameDataInfo
    {
        double analog_gain;             ///< Analog gain in dB. Automatically filled on acquisition. \sa RuntimeParameters
        double integration_time_ms;     ///< Integration time used during acquisition. Automatically filled on acquisition. \sa RuntimeParameters
        long long acquisition_time_us;  ///< Acquisition time in microsecond since January 1st 1970. Automatically filled on acquisition. \sa EPOCH_STR
        double value_max;               ///< Highest data value. Automatically filled on acquisition.
        double value_saturation;        ///< Threshold indicating saturated data. Automatically filled on acquisition.
        double value_no_data;           ///< Value indicating unacquired data. Automatically filled on acquisition.
        double value_min;               ///< Lowest data value. Automatically filled on acquisition.
        char   system_id [64];          ///< Identifier of the system. Automatically filled on acquisition.
        bool   valid;                   ///< Validity flag. Automatically filled on acquisition. Allocated yet unacquired cubes are not valid.
    } FrameDataInfo;

    /*! 
    \brief Structure representing frame data with single precision floating point data. 
     
    Gather format, info, and data within one struct.

    \sa AllocateFrame, DeallocateFrame, LoadFrame, SaveFrame, CubeDataFormat, CubeDataInfo, AcquireFrame, AllocateFrameForCamera
    */
    typedef struct FrameFloat
    {
        FrameDataFormat format;       ///< Data format of the frame.
        FrameDataInfo info;           ///< Runtime information on the frame.

        float* const* pp_data;        ///< 2D data pointer to the frame data, row, column (i.e., \code{.c}pp_data[row_idx][col_idx]\endcode). 

        void* p_reserved { nullptr }; ///< Reserved pointer. Do not use.
    } FrameFloat;

    /*!
    \brief Structure modeling a region of interest in the frame.

    \sa SetRegionOfInterestArray
    */
    typedef struct RegionOfInterest
    {
        int x;          ///< Offset along the frame's x-axis in number of columns from the frame's upper left pixel.
        int y;          ///< Offset along the frame's y-axis in number of rows from the frame's upper left pixel.
        int width;      ///< Size of the region of interest along the frame's x-axis in number of columns.
        int height;     ///< Size of the region of interest along the frame's y-axis in number of rows.
    } RegionOfInterest;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HSI_API_TYPES_H
