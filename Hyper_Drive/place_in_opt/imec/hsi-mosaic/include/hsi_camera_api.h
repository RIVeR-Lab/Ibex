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

#ifndef HSI_CAMERA_H
#define HSI_CAMERA_H

// Generic api includes
#include "hsi_camera_api_lib.h"
#include "hsi_api_types.h"
#include "hsi_api_common.h"
#include "hsi_camera_api_types.h"


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
    \retval HSI_HANDLE_INVALID The device handle i_pipeline_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer i_p_roi_array is invalid while i_nr_rois is strictly positive.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.
    */
    HSI_CAMERA_API HSI_RETURN cameraGetAPIVersion (int* o_p_major, int* o_p_minor, int* o_p_patch, int* o_p_build);

    /*!
    \brief Enumerate all devices connected to the system.

    \param[out] o_device_info_list Array of CameraInfo in which device information will be returned.
    \param[out] o_nr_devices_found The number of devices connected to the system.
    \param[in] i_array_size The size of the array o_device_info_list
    \param[in] i_device_args Struct containing the manufacturer and/or model for the device to filter. May be empty struct, defaults to EM_ALL/MO_ALL

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_ARGUMENT_INVALID The pointer o_device_info_list or o_nr_devices_found is invalid or i_array_size is negative.
    */

    HSI_CAMERA_API HSI_RETURN cameraEnumerateConnectedDevices (
        CameraInfo* o_device_info_list,
        int* o_nr_devices_found,
        int i_array_size,
        DeviceArgs i_device_args);

    /*!
    \brief Opens the connection to a device and creates a device handle.

    \post If the call succeeds with HSI_OK, the camera is in the idle state.

    \param[out] o_p_device_handle Pointer to a camera device handle. Will be set if opening the device succeeded.
    \param[in]  i_device_info Description of the camera to open the connection to.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_CONNECTION_FAILED It was impossible to connect. Verify the connection.
    */
    HSI_CAMERA_API HSI_RETURN cameraOpenDevice (HANDLE* o_p_device_handle, CameraInfo i_device_info);

    /*!
    \brief Closes the connection to a camera device.

    \param[in,out] io_p_device_handle Pointer to a device handle. Will be set to NULL if closing the device succeeded.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle pointed to by io_p_device_handle is invalid.
    */
    HSI_CAMERA_API HSI_RETURN cameraCloseDevice (HANDLE* io_p_device_handle);

    /*!
    \brief Initializes the camera.

    \pre The camera must be in the idle state.
    \post If the call succeeds with HSI_OK, the camera is in the ready state. Else the state remains unchanged.

    During this function call, the configuration parameters are used to allocate all internal buffers.

    \param[in] i_device_handle Handle to the device.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_device_handle is invalid.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.
    \retval HSI_ALLOCATION_ERROR The allocation of internal buffers failed. Verify the system has sufficient resources.
    */
    HSI_CAMERA_API HSI_RETURN cameraInitialize (HANDLE i_device_handle);

    /*!
    \brief Starts the camera.

    \pre The camera must be in the ready state.
    \post If the call succeeds with HSI_OK, the camera is in the started state. Else the state remains unchanged.

    During this function call, all runtime parameters are applied after which the system is ready to acquire data.

    \param[in] i_device_handle Handle to the device.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_device_handle is invalid.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.
    */
    HSI_CAMERA_API HSI_RETURN cameraStart (HANDLE i_device_handle);

    /*!
    \brief Pauses the camera.

    \pre It is recommended the camera is in the started state.
    \post If the call succeeds with HSI_OK, the camera is in the ready state. Else the state remains unchanged.

    \param[in] i_device_handle Handle to the device.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_device_handle is invalid.
    */
    HSI_CAMERA_API HSI_RETURN cameraPause (HANDLE i_device_handle);

    /*!
    \brief Stops the camera.

    \pre It is recommended the camera is in the ready state.
    \post If the call succeeds with HSI_OK, the camera is in the stopped state. Else the state remains unchanged.

    During this function call, all internal buffers are deallocated.

    \param[in] i_device_handle Handle to the device.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_device_handle is invalid.
    */
    HSI_CAMERA_API HSI_RETURN cameraStop (HANDLE i_device_handle);

    /*!
    \brief Software-Trigger the camera device to capture a frame.

    \pre The camera must be in the running state. (After calling Start() ), and be configured to react on Software Triggers.

    \param[in] i_device_handle Handle to the device.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_device_handle is invalid.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.
    */
    HSI_CAMERA_API HSI_RETURN cameraTrigger (HANDLE i_device_handle);

    /*!
    \brief Gets the configuration parameters currently set.

    \pre The method can be called at any time.

    \param[in] i_device_handle Handle to the device.
    \param[out] o_p_configuration_parameters Pointer to a ConfigurationParameters structure. The fields will be set upon success.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_device_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer o_p_configuration_parameters is invalid.
    */
    HSI_CAMERA_API HSI_RETURN cameraGetConfigurationParameters (HANDLE i_device_handle, cameraConfigurationParameters* o_p_configuration_parameters);

    /*!
    \brief Sets the camera configuration parameters.

    \pre The camera must be in the ready state (before calling Initialize() ).

    \param[in] i_device_handle Handle to the device.
    \param[in] i_configuration_parameters The configuration parameters.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_device_handle is invalid.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.
    \retval HSI_ARGUMENT_INVALID The given configuration parameters are not compatible with the camera device.
    */
    HSI_CAMERA_API HSI_RETURN cameraSetConfigurationParameters (HANDLE i_device_handle, cameraConfigurationParameters i_configuration_parameters);

    /*!
    \brief Gets the runtime parameters currently set.

    \pre The method can be called at any time.

    \param[in] i_device_handle Handle to the device.
    \param[out] o_p_runtime_parameters Pointer to a RuntimeParameters structure.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_device_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer o_p_runtime_parameters is invalid.
    */
    HSI_CAMERA_API HSI_RETURN cameraGetRuntimeParameters (HANDLE i_device_handle, cameraRuntimeParameters* o_p_runtime_parameters);

    /*!
    \brief Sets the camera runtime parameters.

    \pre The camera must be in the stopped or ready state ( Pause() ).

    \param[in] i_device_handle Handle to the device.
    \param[in] i_runtime_parameters The runtime parameters. The parameters must be consistent with the allowed ranges.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_device_handle is invalid.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.
    \retval HSI_ARGUMENT_INVALID The given runtime parameters are not consistent with the allowed ranges.
    */
    HSI_CAMERA_API HSI_RETURN cameraSetRuntimeParameters (HANDLE i_device_handle, cameraRuntimeParameters i_runtime_parameters);

    /*!
    \brief Gets the data format of the frames generated by the camera.

    \pre This method can be called at any time.

    \param[in] i_device_handle Handle to the device.
    \param[in] o_p_data_format Pointer to a FrameDataFormat structure.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_device_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer o_p_data_format is invalid.
    */
    HSI_CAMERA_API HSI_RETURN cameraGetOutputFrameDataFormat  (HANDLE i_device_handle, FrameDataFormat* o_p_data_format);

    /*!
    \brief Allocates a data frame compatible with the return data format of the given camera device.
    \pre The camera must be in the ready state.

    \param[in] i_device_handle Handle to the device.
    \param[out] o_p_frame Pointer to the frame structure in which to allocate the data.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_ARGUMENT_INVALID The pointer o_p_frame is invalid or the frame has already been allocated.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.
    \retval HSI_ALLOCATION_ERROR The allocation failed. Verify the system has sufficient resources.
    */
    HSI_CAMERA_API HSI_RETURN cameraAllocateFrameForCamera (HANDLE i_device_handle, FrameFloat* o_p_frame);

    /*!
    \brief Gets a frame from the camera device.

    Waits for the next first frame data coming from the camera. The data is placed in o_p_frame.
    A timeout argument can be specified. In this case the call will either return with a success value
    (and a valid output frame), or with an internal-timeout value (and an invalid frame) after the timeout
    has expired.  In case the timeout value is zero or negative, this call is a blocking call, and hence
    will only return upon a valid captured frame condition.

    \pre The camera must be in the started state ( Start() ). The data behind the specified frame must be allocated ( AllocateFrame() ).

    \param[in] i_device_handle Handle to the device.
    \param[in] o_p_frame Pointer to a FrameFloat structure.
    \param[in] i_timeout_ms Timeout in ms to wait for a new acquired frame. If zero or negative, this is a blocking call.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_INTERNAL_TIMEOUT The function call returned due to an internal timeout without a captured frame.
    \retval HSI_HANDLE_INVALID The device handle i_device_handle is invalid.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.
    \retval HSI_DATA_NOT_ALLOCATED The referenced data structure o_p_frame is not allocated ( AllocateFrame() ).
    \retval HSI_ACQUISITON_FAILED. Something occured during the data acquisition. Verify the available bandwidth to the camera and the CPU usage and the physical RAM availability.
    */
    HSI_CAMERA_API HSI_RETURN cameraAcquireFrame (HANDLE i_device_handle, FrameFloat* o_p_frame, int i_timeout_ms=0);

    /*!
    \brief Sets ROIs on the sensor.

    \pre The camera must be stopped first.

    \param[in] i_device_handle Handle to the device.
    \param[in] i_p_roi_array Array with RegionOfInterest objects of length i_nr_rois
    \param[in] i_nr_rois The number of ROIs to set.

    \note The ROIs on the sensor are reset to full sensor size if the length of the array is 0.

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_device_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer i_p_roi_array is invalid while i_nr_rois is strictly positive.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.
    */
    HSI_CAMERA_API HSI_RETURN cameraSetRegionOfInterestArray (HANDLE i_device_handle, RegionOfInterest* i_p_roi_array, int i_nr_rois);

    /*!
    \brief Allocates a list of objects that contain High-Dynamic-Range (HDR) parameters for a camera.

    \param[in] width number of pixels on a single row of the camera sensor
    \param[in] height number of pixels on a single column of the camera sensor
    \param[in] num_entries the number of HDR parameter-sets to allocate as a list.
    \param[in] o_p_hdr_parameters Struct with all the HDR parameters

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_ARGUMENT_INVALID The pointer i_p_roi_array is invalid while i_nr_rois is strictly positive.
    */
    HSI_CAMERA_API HSI_RETURN cameraAllocateHDRParameters (int width, int height, int num_entries, HDRParameters* o_p_hdr_parameters);

    /*!
    \brief Deallocates the list of objects that contain HDR parameters for a camera.

    \param[in] io_p_hdr_parameters Struct with all the HDR parameters

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    */
    HSI_CAMERA_API HSI_RETURN cameraDeallocateHDRParameters (HDRParameters* io_p_hdr_parameters);

    /*!
    \brief given a list of HDR parameters, this function will apply these HDR parameters onto a camera.
    \pre The camera must be in the ready state.

    \param[in] i_device_handle Handle to the device.
    \param[in] i_p_hdr_parameters Struct with all the HDR parameters

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_device_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer i_p_roi_array is invalid while i_nr_rois is strictly positive.
    \retval HSI_CALL_ILLEGAL The camera is not in the correct state to handle this function call.
    */
    HSI_CAMERA_API HSI_RETURN cameraSetHdrSettings (HANDLE i_device_handle, HDRParameters* i_p_hdr_parameters);

    /*!
    \brief Change the status of the internal camera shutter to be either open or closed.

    \param[in] i_device_handle Handle to the device.
    \param[in] i_shutter_state The new state of the camera shutter

    \return ReturnValue indicating the result of the call.
    \retval HSI_OK The function call was completed successfully.
    \retval HSI_HANDLE_INVALID The device handle i_device_handle is invalid.
    \retval HSI_ARGUMENT_INVALID The pointer i_p_roi_array is invalid while i_nr_rois is strictly positive.
    */
    HSI_CAMERA_API HSI_RETURN cameraSetShutterState (HANDLE i_device_handle, ShutterState i_shutter_state);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HSI_CAMERA_API_H
