/////////////////////////////////////////////////////
//
//  Date: March, 2022
//
//  Authors:
//    G. Vanmeerbeeck
//    Wouter Charle
//    Vincent Radelet
//    Bart Masschelein
//
//  Copyright 2018 imec. All rights reserved.
//
/////////////////////////////////////////////////////

#ifndef HSI_CAMERA_API_TYPES_H
#define HSI_CAMERA_API_TYPES_H

// Generic api includes
#include "hsi_api_types.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    /*!
    \brief Enumeration of manufacturers of supported cameras in this API.
    */
    typedef enum Manufacturer
    {
        EM_ALL = 0,
        EM_XIMEA,
        EM_PHOTONFOCUS,
        EM_IMEC
    } Manufacturer;

    /*!
    \brief Enumeration of camera models of supported cameras in this API.
    */
    typedef enum Model
    {
        MO_ALL = 0,
        MO_SNAPSHOT_SWIR,   // EM_IMEC model
        MO_DUMMY            // EM_IMEC model
    } Model;

    /*!
    \brief Enumeration of all supported camera trigger modes.
    */
    typedef enum TriggerMode
    {
        TM_NoTriggering,       ///< External triggering is disabled.
        TM_SoftwareTriggered,  ///< The camera captures frames on receiving a software trigger signal.
        TM_HardwareTriggered   ///< The camera captures frames on receiving a hardware trigger signal.
    } TriggerMode;

    /*!
    \brief Enumeration of camera shutter states.
    */
    typedef enum ShutterState
    {
        SS_CLOSE,   ///< the shutter is in closed state
        SS_OPEN    ///< The shutter is in the open state
    } ShutterState;

    /*!
    \brief Structure gathering information to uniquely identify a camera.
    */
    typedef struct CameraInfo
    {
        Manufacturer manufacturer;                        ///< Manufacturer of the camera
        char model[c_MAX_STRING_LENGTH];                  ///< Device model.
        char serial_number[c_MAX_STRING_LENGTH];          ///< The device's serial number.
        char identification_string[c_MAX_STRING_LENGTH];  ///< The connection identifier of the device.
    } CameraInfo;

    /*!
    \brief Structure gathering all camera configuration parameters.

    Configuration parameters determine the data output size. They impact the allocation of both internal and external buffers.
    These parameters can be set only when the camera is stopped and will be used to correctly initialize the camera device.

    \sa CubeDataFormat, Stop, Initialize
    */
    typedef struct cameraConfigurationParameters
    {
        unsigned int bit_depth;                  ///< [i] The bit-depth of the camera
        int          nr_me_frames;               ///< The number of multi-exposure frames
        bool         multi_exposure_scaling;     ///< Scale the multi-exposure frames
    } cameraConfigurationParameters;

    /*!
    \brief Structure gathering all camera runtime parameters.

    Runtime parameters determine the acquisition characteristics. They impact the sensor readout and scanning speed.
    These parameters can always be set except when the camera is started and will be set on the device when starting.

    \sa CubeDataInfo, Pause, Start
    */
    typedef struct cameraRuntimeParameters
    {
        int    black_level_offset;         ///< [r] The black level offset in DN
        double exposure_time_ms;           ///< [r] The image exposure time in milliseconds.
        bool   flip_horizontal;            ///< [r] Aka. up-down
        bool   flip_vertical;              ///< [r] Aka. left-right
        double frame_rate_hz;              ///< [r] The frame rate at which images are created by the camera or 0 for maximum possible framerate. The maximum frame rate is limited by the exposure time. \see exposure_time_ms
        double requested_temperature_dc;   ///<?[r] requested temperature to TEC
        bool   tec_enabled;                ///< [r] TEC
        TriggerMode trigger_mode;          ///< [r] The trigger mode of the camera.
        bool   hdr_scaling;                ///< [r] Scale the HDR image according to the integration time ratios
    } cameraRuntimeParameters;

    /*!
    \brief Structure gathering all settings needed to apply HDR (High Dynamic Range).
    */
    typedef struct HDRParameters
    {
        int width;                         ///< Number of columns in the map.
        int height;                        ///< Number of rows in the map.
        int num_entries;                   ///< Number of entries in the integration_time_ratios list.
        double* integration_time_ratios;   ///< list of integration time ratios to apply for HDR
        int* p_data;                       ///< pointer to the HDR map values : indicating which ratio (by index in the list) for every pixel
        int** pp_rows;                     ///< pointer to the rows for the HDR map.
    } HDRParameters;

    /*!
    \brief Structure to define which devices should be looked for in cameraEnumerateConnectedDevices.
    */
    typedef struct DeviceArgs {
        Manufacturer manufacturer;
        Model model;
    } DeviceArgs;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HSI_CAMERA_API_TYPES_H
