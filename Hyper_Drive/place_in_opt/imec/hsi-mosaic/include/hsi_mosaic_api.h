/////////////////////////////////////////////////////
//
//  Date: April, 2020
//
//  Authors:
//    G. Vanmeerbeeck, Wouter Charle, Vincent Radelet
//
//  Copyright 2018 imec. All rights reserved.
//
/////////////////////////////////////////////////////

#ifndef HSI_MOSAIC_API_H
#define HSI_MOSAIC_API_H

// includes
#include "hsi_mosaic_api_lib.h"
#include "hsi_api_types.h"
#include "hsi_api_common.h"


#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


    /*!
    \brief Enumeration to specify the kernel for the Median-Filtering in the HSI Mosaic Pipeline Stage.

    */
    typedef enum KernelSize
    {
        KS_3X3
      , KS_5X5
    } KernelSize;

    /*!
    \brief Enumeration to specify the optical range inside the Optical setup Parameters of a camera.

    */
    typedef enum OpticalRange
    {
        OR_VIS
    ,   OR_NIR
    ,   OR_VISNIR
    ,   OR_SWIR
    } OpticalRange;

    /*!
    \brief Structure describing the Configuration Parameters of a Mosaic Camera 
    
    \warning Must be deallocated properly to prevent memory leaks.

    \sa GetConfigurationParameters, SetConfigurationParameters
    */
    typedef struct mosaicConfigurationParameters
    {
        size_t      spatial_resampling_width;           ///< width of spatial resampling kernel
        size_t      spatial_resampling_height;          ///< height of spatial resampling kernel
        bool        spatial_median_filter_enable;       ///< Flags whether a spatial median filter must be applied after demosaicing (true) or not (false)
        KernelSize  spatial_median_filter_kernel_size;  ///< size of spatial median filtering kernel
        bool        white_balance_enable;               ///< Flags whether the spectral radiance data must be white balanced to spectral reflectance using the reference spectrum (true) or not (false). Note: requires that a reference spectrum is set (\see SetReferenceSpectrum).
    }  mosaicConfigurationParameters;

    /*!
    \brief Structure containing a set of status flags with respect to a context.

    A context is all the data and files required to be able to convert raw frames into a cube: calibration file, 
    dark and bright reference images, a correction matrix and optical setup information.

    \sa ContextGetStatus
    */
    typedef struct ContextStatus
    {
        int context_ready;                       ///< overall context-ready flag
        int has_dark_reference;                  ///< flag indicating that dark references have been added to the context
        int has_sensor_calibration;              ///< flag indicating that dark references have been added to the context
        int has_nonuniformity;                   ///< flag indicating that non-uniformity frames have been added to the context
        int has_optical_setup;                   ///< flag indicating that the optical setup information has been added to the context
    } ContextStatus;

    /*!
    \brief Structure to describe the optical setup for a given camera.

    \sa ContextSetOpticalSetup
    */
    typedef struct OpticalSetup
    {
        double lens_f_number;       ///< The f-number for the lens used in front of the camera sensor
        double focal_length_mm;     ///< the focal length of the lens, specified in millimeters
        double exit_pupil_mm;       ///< the distance to the exit of the pupil, specified in millimeters
        char* manufacturer;         ///< an information string containing the manufacturer of the lens
        OpticalRange optical_range; ///< the optical range of the lens
    } OpticalSetup;

    /*!
    \brief Function to retrieve the internal version numbers as specified inside the library.

    \param[out] o_p_major pointer to an integer to contain the major version number.
    \param[out] o_p_minor pointer to an integer to contain the minor version number.
    \param[out] o_p_patch pointer to an integer to contain the patch version number.
    \param[out] o_p_build pointer to an integer to contain the build version number.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_pipeline_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer i_p_roi_array is invalid while i_nr_rois is strictly positive.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.
    */
    HSI_MOSAIC_API HSI_RETURN mosaicGetAPIVersion (int* o_p_major, int* o_p_minor, int* o_p_patch, int* o_p_build);

    /*!
    \brief Function to create new pipeline object that will be able to process raw HSI Mosaic images.

    This function will create all the required pipeline stages, according to what is present in the context and in the 
    pipeline configuration parameters.  This also means that buffers and temporary arrays will be allocated during this
    call.  Make sure to call Destroy() to free all these internally allocated memory.

    \param[out] o_p_pipeline_handle Handle to the HSI Mosaic pipeline object.
    \param[in] i_context_handle  Handle to the context object on how to build and configure the HSI Mosaic Pipelineapply

    \warning a pipeline needs to be deallocated properly to prevent memory leaks. (see Destroy() )

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_pipeline_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer i_p_roi_array is invalid while i_nr_rois is strictly positive.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.
    */
    HSI_MOSAIC_API HSI_RETURN mosaicCreate (HANDLE* o_p_pipeline_handle, HANDLE i_context_handle);

    /*!
    \brief Function that will clean up and deallocate a pipeline object to process raw HSI Mosaic images.

    This means that all internal pipeline stages and all their internally allocated buffers will be properly
    freed.  Make sure to call this function on every pipeline object to avoid memory leaks.

    \param[in] io_pipeline_handle Handle to the device.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_pipeline_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer i_p_roi_array is invalid while i_nr_rois is strictly positive.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.

    \sa Create
    */
    HSI_MOSAIC_API HSI_RETURN mosaicDestroy (HANDLE* io_pipeline_handle);

    /*!
    \brief Function that will initialize a pipeline object to process raw HSI Mosaic images.

    \param[in] i_pipeline_handle Handle to the device.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_pipeline_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer i_p_roi_array is invalid while i_nr_rois is strictly positive.
    \retval HSI_CALL_ILLEGAL The pipeline is not configured properly and cannot be initialized.
    */
    HSI_MOSAIC_API HSI_RETURN mosaicInitialize (HANDLE i_pipeline_handle);

    /*!
    \brief Function that will start a pipeline object so it can start to process raw HSI Mosaic images.

    \param[in] i_pipeline_handle Handle to the device.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_pipeline_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer i_p_roi_array is invalid while i_nr_rois is strictly positive.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.
    */
    HSI_MOSAIC_API HSI_RETURN mosaicStart (HANDLE i_pipeline_handle);

    /*!
    \brief Function that will put the internal state of a pipeline object in the Pause-State.

    \param[in] i_pipeline_handle Handle to the device.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_pipeline_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer i_p_roi_array is invalid while i_nr_rois is strictly positive.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.
    */
    HSI_MOSAIC_API HSI_RETURN mosaicPause (HANDLE i_pipeline_handle);

    /*!
    \brief Function that will stop a pipeline object, so it will no longer process raw HSI Mosaic images.

    \param[in] i_pipeline_handle Handle to the device.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_pipeline_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer i_p_roi_array is invalid while i_nr_rois is strictly positive.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.
    */
    HSI_MOSAIC_API HSI_RETURN mosaicStop (HANDLE i_pipeline_handle);

    /*!
    \brief Function to retrieve the data format of the output stage of the pipeline object
    that will process HSI Mosaic images.

    \param[in] i_pipeline_handle Handle to the pipeline object containing all processing stages.
    \param[in] o_p_cube_data_format data format that will be produced by the pipeline object's output stage.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_pipeline_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer i_p_roi_array is invalid while i_nr_rois is strictly positive.
    \retval HSI_CALL_ILLEGAL The pipeline is not in the correct state to handle this function call.
    */
    HSI_MOSAIC_API HSI_RETURN mosaicGetOutputDataFormat (HANDLE i_pipeline_handle, CubeDataFormat* o_p_cube_data_format);

    /*!
    \brief Function to retrieve the set of Configuration Parameters for all stages of a pipeline object 
    that will process (raw) HSI Mosaic images.

    \param[in] i_pipeline_handle Handle to the pipeline object containing all processing stages.
    \param[in] o_p_parameters

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_pipeline_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer i_p_roi_array is invalid while i_nr_rois is strictly positive.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.
    */
    HSI_MOSAIC_API HSI_RETURN mosaicGetConfigurationParameters (HANDLE i_pipeline_handle, mosaicConfigurationParameters* o_p_parameters);

    /*!
    \brief Function that will set all Configuration Parameters for all stages of a pipeline object
    that will process (raw) HSI Mosaic images.

    \param[in] i_pipeline_handle Handle to the pipeline object containing all processing stages.
    \param[in] i_parameters Configuration parameters that will be set onto the pipeline object

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_pipeline_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer i_p_roi_array is invalid while i_nr_rois is strictly positive.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.
    */
    HSI_MOSAIC_API HSI_RETURN mosaicSetConfigurationParameters (HANDLE i_pipeline_handle, mosaicConfigurationParameters i_parameters);

    /*!
    \brief Function to push an input (raw) frame into a pipeline object to convert it into a processed HSI Mosaic cube.

    \param[in] i_pipeline_handle Handle to the pipeline object containing all processing stages.
    \param[in] i_frame the input raw frame that needs to be processed by the pipeline

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_pipeline_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer i_p_roi_array is invalid while i_nr_rois is strictly positive.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.
    */
    HSI_MOSAIC_API HSI_RETURN mosaicPushFrame (HANDLE i_pipeline_handle, FrameFloat i_frame);

    /*!
    \brief Function to retrieve a processed output HSI Mosaic Cube from the pipeline object. If several images 
    are pushed into the pipeline object, this will act like a First-In-First-Out queue.  Processed cubes will
    come out in the same order they were pushed in. Processed cubes are always marked as valid in their CubeDataInfo .

    This call is a Blocking call with a timeout argument. Meaning it will wait (and block) until an output cube 
    is ready or until the timeout value (in milliseconds) is reached (in this case the output cube is marked as invalid).

    \param[in] i_pipeline_handle Handle to the pipeline object containing all processing stages.
    \param[in] o_p_cube the processed output HSI Mosaic Cube.
    \param[in] i_timeout_ms the timeout value to wait for a valid output cube

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_INTERNAL_TIMEOUT No valid output cube could be produced within the given timeout range.
    \retval HSI_HANDLE_INVALID The device handle i_pipeline_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer i_p_roi_array is invalid while i_nr_rois is strictly positive.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.
    */
    HSI_MOSAIC_API HSI_RETURN mosaicGetCube  (HANDLE i_pipeline_handle, CubeFloat* o_p_cube, int i_timeout_ms);

    /*!
    \brief This Function will load a HSI Mosaic pipeline context from disc into its according memory object.

    The organisation of how a context is saved to disc is documented in the user manual.  The names of files and
    the folder structure is fixed by the tool, both for saving as well as for loading from disk.

    \param[out] o_p_context_handle the handle to the internal context structure (in memory)
    \param[in] i_pathname the string from where to load the pipeline context, i.e. the name of the context folder.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_pipeline_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer i_p_roi_array is invalid while i_nr_rois is strictly positive.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.
    */
    HSI_MOSAIC_API HSI_RETURN mosaicLoadContext (HANDLE* o_p_context_handle, char const* i_pathname);

    /*!
    \brief Function to save the current active pipeline context to disk.

    The organisation of how a context is saved to disc is documented in the user manual.  The names of files and
    the folder structure is fixed by the tool, both for saving as well as for loading from disk.

    After loading a context from disk, you can use ContextGetStatus() to check the status of the loaded context.

    \param[in] i_context_handle handle to the context structure
    \param[in] i_pathname name of the context folder where to save the current context to disk.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_pipeline_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer i_p_roi_array is invalid while i_nr_rois is strictly positive.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.
    */
    HSI_MOSAIC_API HSI_RETURN mosaicSaveContext (HANDLE i_context_handle, char const* i_pathname);

    /*!
    \brief Function to allocate a Context object into memory.

    This context object will need to be deallocated to avoid memory leaks.

    \param[out] o_p_context_handle Handle to the object that will contain a pipeline context.
    \param[in] i_frame Frame to set the Meta-Data information for the pipeline.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_pipeline_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer i_p_roi_array is invalid while i_nr_rois is strictly positive.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.
    */
    HSI_MOSAIC_API HSI_RETURN mosaicAllocateContext (HANDLE* o_p_context_handle, FrameFloat i_frame);

    /*!
    \brief Function to deallocate and free up the memory related to a pipeline context object.

    \param[out] io_p_context_handle Handle to the object that contains a pipeline context.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_pipeline_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer i_p_roi_array is invalid while i_nr_rois is strictly positive.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.
    */
    HSI_MOSAIC_API HSI_RETURN mosaicDeallocateContext (HANDLE* io_p_context_handle);

    /*!
    \brief This function will erase all information from the context and will reset it as if it was a 
    newly created context object.

    \param[in] i_context_handle Handle to the object that needs to be reset.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_pipeline_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer i_p_roi_array is invalid while i_nr_rois is strictly positive.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.
    */
    HSI_MOSAIC_API HSI_RETURN mosaicContextReset (HANDLE i_context_handle);

    /*!
    \brief ...

    \param[in] i_context_handle Handle to the object that needs to be reset.
    \param[out] o_p_status_flags

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_pipeline_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer i_p_roi_array is invalid while i_nr_rois is strictly positive.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.
    */
    HSI_MOSAIC_API HSI_RETURN mosaicContextGetStatus (HANDLE i_context_handle, ContextStatus* o_p_status_flags);

    /*!
    \brief Function to test if for a certain integration time there is a dark-reference present in the context.

    \param[in] i_context_handle Handle to the object that needs to be reset.
    \param[out] o_p_status_flag pointer to the integer flag where to store the result.
    \param[in] i_integration_time_ms the integration time in milliseconds to test for.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_pipeline_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer i_p_roi_array is invalid while i_nr_rois is strictly positive.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.
    */
    HSI_MOSAIC_API HSI_RETURN mosaicContextIntegrationTimeInDark (HANDLE i_context_handle, int* o_p_status_flag, float i_integration_time_ms);

    /*!
    \brief Function to add (or set) a sensor calibration file to a context.

    \param[in] i_context_handle Handle to the object that needs to be reset.
    \param[in] i_in_file_path The file path to the sensor calibration file (in XML format).

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_pipeline_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer i_p_roi_array is invalid while i_nr_rois is strictly positive.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.
    */
    HSI_MOSAIC_API HSI_RETURN mosaicContextSetCalibrationFile (HANDLE i_context_handle, char const* i_in_file_path);

    /*!
    \brief Function to add multiple dark-field reference images to a context. These will be used for bias-compensation during processing.

    The frames will be referenced by their (internal) integration times.  So make sure to add a dark-reference image for every
    integration times present in your data-set.

    For all frames an internal copy will be made.  Hence, as a user you are still expected to call DeallocateFrame()
    for each and everyone of them.  You can call that DeallocateFrame() even directly after this call.

    \param[in] i_context_handle Handle to the object that needs to be reset.
    \param[in] i_p_dark_field_array Array of frames with dark-reference images.  Will be internally referenced by their integration time.
    \param[in] i_array_length the number of frames present in the argument array of dark-references.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_pipeline_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer i_p_roi_array is invalid while i_nr_rois is strictly positive.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.
    */
    HSI_MOSAIC_API HSI_RETURN mosaicContextSetDarkFieldReferences (HANDLE i_context_handle, FrameFloat* i_p_dark_field_array, int i_array_length);
    
    /*!
    \brief Function to add Optical Setup information to the context.

    \param[in] i_context_handle Handle to the object that needs to be reset.
    \param[in] i_optical_setup The optical setup to apply onto the context.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_pipeline_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer i_p_roi_array is invalid while i_nr_rois is strictly positive.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.
    */
    HSI_MOSAIC_API HSI_RETURN mosaicContextSetOpticalSetup (HANDLE i_context_handle, OpticalSetup i_optical_setup);

    /*!
    \brief Extracts a spectrum from an ROI in a cube. 

    The spectrum is the average spectrum over all pixels in the ROI in the cube.
    The spectrum must have length equal to the number of bands in the cube (see AllocateSpectrum() ).

    \param[out] o_p_spectrum the output reference spectrum extracted
    \param[in] i_cube the cube from where the reference spectrum needs to be extracted
    \param[in] i_roi the Region of Interest (roi) from where in the cube to extract the reference spectrum

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_ARGUMENT_INVALID One or more input arguments don't satisfy the pre-conditions.
    */
    HSI_MOSAIC_API HSI_RETURN mosaicExtractSpectrumFromCube (SpectrumFloat* o_p_spectrum, CubeFloat i_cube, RegionOfInterest i_roi);

    /*!
    \brief Sets the reference spectrum to use for balancing the radiance data.

    The reference spectrum must be in radiance. It is used by the pipeline to normalize any other radiance
    measurement into reflectance. Essentially, this step removes the light source spectrum from the data.

    \pre The pipeline must be in the STOPPED state.

    \param[in] i_pipeline_handle     Handle to the internal pipeline object.
    \param[in] i_balancing_spectrum  the spectrum used to balance the cube.
    \param[in] i_balancing_coefficient weight-coefficient to be used when doing the spectrum balancing.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_pipeline_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer i_p_roi_array is invalid while i_nr_rois is strictly positive.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.

    \sa ExtractSpectrumFromCube
    */
    HSI_MOSAIC_API HSI_RETURN mosaicSetReferenceSpectrum (HANDLE i_pipeline_handle, SpectrumFloat i_balancing_spectrum, double i_balancing_coefficient);

    /*!
    \brief Loads an optical setup file (XML) from disk into an OpticalSetup object.

    \param[in] i_in_file_path path to the filename to read in (must be XML file)
    \param[out] o_p_optical_setup output object with optical setup information

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_ARGUMENT_INVALID The path string is not pointing to an existing file, or the pointer to the output is invalid.

    \sa ContextSetOpticalSetup
    */
    HSI_MOSAIC_API HSI_RETURN mosaicLoadOpticalSetup (char const* i_in_file_path, OpticalSetup* o_p_optical_setup);

    /*!
    \brief Method to add Non-Uniformity data to a context.  This means adding one non-uniformity dark reference frame and 
    one non-uniformity white reference frame.

    For both dark and white reference frames an internal copy will be made.  Hence, as a user you are still expected to 
    call DeallocateFrame() for both of them.  You can call that DeallocateFrame() even directly after this call.

    \param[in] i_context_handle Handle to the object that needs to be reset.
    \param[in] i_p_dark_frame pointer to a single frame to be used as non-uniformity dark frame
    \param[in] i_p_white_frame pointer to a single frame to be used as non-uniformity white frame

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_ARGUMENT_INVALID The path string is not pointing to an existing file, or the pointer to the output is invalid.
    */
    HSI_MOSAIC_API HSI_RETURN mosaicContextSetNonUniformity (HANDLE i_context_handle, FrameFloat* i_p_dark_frame, FrameFloat* i_p_white_frame);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HSI_MOSAIC_API_H