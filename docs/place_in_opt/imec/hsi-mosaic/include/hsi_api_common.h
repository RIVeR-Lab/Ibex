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

#ifndef HSI_API_COMMON_H
#define HSI_API_COMMON_H

// api includes
#include "hsi_api_lib.h"
#include "hsi_api_types.h"

#include <wchar.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    /*!
        \brief Function to retrieve the internal version numbers as specified inside the library.

        \param[out] o_p_major pointer to an integer to contain the major version number.
        \param[out] o_p_minor pointer to an integer to contain the minor version number.
        \param[out] o_p_patch pointer to an integer to contain the patch version number.
        \param[out] o_p_build pointer to an integer to contain the build version number.

        \return ReturnValue indicating the result of the call.
        \retval HSI_OK The function call was completed successfully.
    */
    HSI_API HSI_RETURN commonGetAPIVersion (int* o_p_major, int* o_p_minor, int* o_p_patch, int* o_p_build);

    /*!
    \brief Allocates a data cube from the given data format.

    \param[out] o_p_cube Pointer to the cube structure in which to allocate the data.
    \param[in] i_data_format Data format of the data to allocate.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_ARGUMENT_INVALID The pointer o_p_cube is invalid or the cube has already been allocated (\sa DeallocateCube).
    \retval HSI_ALLOCATION_ERROR The allocation failed. Verify the system has sufficient resources.
    */
    HSI_API HSI_RETURN commonAllocateCube (CubeFloat* o_p_cube, CubeDataFormat i_data_format);

    /*!
    \brief Allocates a spectrum from the given cube data format.

    \param[out] o_p_spectrum Pointer to the spectrum structure in which to allocate the data.
    \param[in] i_data_format Data format of the data to allocate.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_ARGUMENT_INVALID The pointer o_p_spectrum is invalid or the spectrum has already been allocated (see DeallocateSpectrum() ).
    \retval HSI_ALLOCATION_ERROR The allocation failed. Verify the system has sufficient resources.
    */
    HSI_API HSI_RETURN commonAllocateSpectrum (SpectrumFloat* o_p_spectrum, CubeDataFormat i_data_format);

    /*!
    \brief Allocates a data frame from the given data format.

    \param[out] o_p_frame Pointer to the frame structure in which to allocate the data.
    \param[in] i_data_format Data format of the data to allocate.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_ARGUMENT_INVALID The pointer o_p_frame is invalid or the frame has already been allocated (see DeallocateCube() ).
    \retval HSI_ALLOCATION_ERROR The allocation failed. Verify the system has sufficient resources.
    */
    HSI_API HSI_RETURN commonAllocateFrame (FrameFloat* o_p_frame, FrameDataFormat i_data_format);

    /*!
    \brief Stores a data cube to disk in the standard ENVI file format.

    \param[in] i_cube The cube data that must be stored to disk.
    \param[in] i_out_dir_path Path to a directory in which the data must be stored. May be both relative or absolute. Non-existing paths will be created recursively.
    \param[in] i_out_file_name Filename for the output cube without extension. Will be used to name both the ENVI header and ENVI data file.
    \param[in] i_out_file_format File format in which the image must be stored on disk.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_ARGUMENT_INVALID The cube is invalid.
    \retval HSI_FILE_SYSTEM_ERROR The specified directory doesn't exist and could not be created.
    \retval HSI_FILE_IO_ERROR Could not write the data into the specified directory. Verify write permissions and disk space.
    */
    HSI_API HSI_RETURN commonSaveCube (CubeFloat i_cube, char const* i_out_dir_path, char const* i_out_file_name, FileFormat i_out_file_format);

    /*!
    \brief Loads a data cube from disk.

    \param [in] o_p_cube The cube that will contain the loaded data.
    \param [in] i_in_file_path The system file's path to the desired cube. Path must refer to the header and the raw must be standing aside.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_ARGUMENT_INVALID o_p_cube is nullptr, already used or i_in_file_path is nullptr.
    \retval HSI_FILE_IO_ERROR i_in_file_path leads to a cube that cannot be read.
    */
    HSI_API HSI_RETURN commonLoadCube (CubeFloat* o_p_cube, char const* i_in_file_path);

    /*!
    \brief Stores a data frame to disk in the standard TIFF file format.

    \param[in] i_frame The frame data that must be stored to disk.
    \param[in] i_out_dir_path Path to a directory in which the data must be stored. May be both relative or absolute. Non-existing paths will be created recursively.
    \param[in] i_out_file_name Filename for the output cube without extension.
    \param[in] i_out_file_format File format in which the image must be stored on disk.

    \warning Only RAW file format can be loaded from disk again into a FrameFloat (see LoadFrame() ).

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_ARGUMENT_INVALID The frame is invalid.
    \retval HSI_FILE_SYSTEM_ERROR The specified directory doesn't exist and could not be created.
    \retval HSI_FILE_IO_ERROR Could not write the data into the specified directory. Verify write permissions and disk space.
    */
    HSI_API HSI_RETURN commonSaveFrame (FrameFloat i_frame, char const* i_out_dir_path, char const* i_out_file_name, FileFormat i_out_file_format);

    /*!
    \brief Loads a data frame from disk.

    \param[in] o_p_frame The frame that will contain the loaded data.
    \param[in] i_in_file_path The system file's path to the desired frame.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_ARGUMENT_INVALID o_p_frame is nullptr, already used or i_in_file_path is nullptr.
    \retval HSI_FILE_IO_ERROR i_in_file_path leads to a frame that cannot be read.
    */
    HSI_API HSI_RETURN commonLoadFrame (FrameFloat* o_p_frame, char const* i_in_file_path);
    
    /*!
    \brief Saves an extracted spectrum to disk.

    \param [in] i_p_spectrum the spectrum to save to disk
    \param [in] i_out_file_path the directory where to save this spectrum
    \param [in] i_out_file_name the filename (without extension) to save this spectrum into.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_ARGUMENT_INVALID o_p_frame is nullptr, already used or i_in_file_path is nullptr.
    \retval HSI_FILE_IO_ERROR i_in_file_path leads to a frame that cannot be read.
    */
    HSI_API HSI_RETURN commonSaveSpectrum (SpectrumFloat i_p_spectrum, char const* i_out_file_path, char const* i_out_file_name);

    /*!
    \brief Loads a spectrum from disk.

    \param [out] o_p_spectrum the spectrum object where to load the values from disk into.
    \param [in] i_in_file_path the filename where to load the spectrum on disk from

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_ARGUMENT_INVALID o_p_frame is nullptr, already used or i_in_file_path is nullptr.
    \retval HSI_FILE_IO_ERROR i_in_file_path leads to a frame that cannot be read.
    */
    HSI_API HSI_RETURN commonLoadSpectrum (SpectrumFloat* o_p_spectrum, char const* i_in_file_path);

    /*!
    \brief Deallocates the memory behind the given cube. Does not delete the pointer to the cube.

    \param[out] o_p_cube The cube of which to deallocate the data.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_ARGUMENT_INVALID The pointer o_p_cube is invalid.
    */
    HSI_API HSI_RETURN commonDeallocateCube (CubeFloat* o_p_cube);

    /*!
    \brief Deallocates the memory behind the given spectrum. Does not delete the pointer to the spectrum.

    \param[out] o_p_spectrum The spectrum of which to deallocate the data.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_ARGUMENT_INVALID The pointer o_p_spectrum is invalid.
    */
    HSI_API HSI_RETURN commonDeallocateSpectrum (SpectrumFloat* o_p_spectrum);

    /*!
    \brief Deallocates the memory behind the given frame. Does not delete the pointer to the frame.

    \param[out] o_p_frame The frame of which to deallocate the data.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_ARGUMENT_INVALID The pointer o_p_cube is invalid.
    */
    HSI_API HSI_RETURN commonDeallocateFrame (FrameFloat* o_p_frame);

    /*!
    \brief Deallocates the memory behind the given cube data format. Does not delete the pointer to the cube data format.

    \param[out] o_p_cube_data_format The cube data format of which to deallocate the data.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_ARGUMENT_INVALID The pointer o_p_cube_data_format is invalid.
    */
    HSI_API HSI_RETURN commonDeallocateCubeDataFormat (CubeDataFormat* o_p_cube_data_format);

    /*!
    \brief Deallocates the memory behind the given frame data format. Does not delete the pointer to the frame data format.

    \param[out] o_p_frame_data_format The frame data format of which to deallocate the data.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_ARGUMENT_INVALID The pointer o_p_cube_data_format is invalid.
    */
    HSI_API HSI_RETURN commonDeallocateFrameDataFormat (FrameDataFormat* o_p_frame_data_format);

    /*!
    \brief Initializes the logger to output log data to the given directory.

    \param[in] i_log_dir_path Path to a directory in which the log data must be stored. May be both relative or absolute. Non-existing paths will be created recursively.
    \param[in] i_verbosity_level Level of verbosity for the logging system.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_FILE_SYSTEM_ERROR The specified directory doesn't exist and could not be created.
    */
    HSI_API HSI_RETURN commonInitializeLogger (char const* i_log_dir_path, LoggerVerbosity i_verbosity_level);

    /*!
    \brief Convert a char array into wchar array.

    \param[out] o_dest Pointer to the wide character array to write to.
    \param[in] i_dest_size Size of the destination array.
    \param[in] i_source Pointer to the null-terminated byte string to convert.

    Convert a char array into its wchar equivalent. If i_dest_size is smaller than the length of i_source, the converted
    string is truncated.
    
    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_ARGUMENT_INVALID One of the provided pointers is null or the destination size is less or equal to zero.
    */
    HSI_API HSI_RETURN commonToWcharArray (wchar_t* o_dest, int i_dest_size, char const* i_source);

    /*! \brief Gets the maximum length of strings allocated on the stack. */
    HSI_API int commonGetMaxStringLength ();

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HSI_API_COMMON_H