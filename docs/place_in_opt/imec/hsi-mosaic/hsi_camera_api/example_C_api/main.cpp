
#include "hsi_camera_api.h"

#include <string>
#include <iomanip>
#include <iostream>
#include <chrono>
// forward declarations
void DisplayResult (std::string const& i_message, HSI_RETURN i_result);

// example code
int main ()
{
    HSI_RETURN ret_val = HSI_OK;

    ret_val = commonInitializeLogger ("./logs/", LV_DEBUG);
    DisplayResult ("commonInitializeLogger", ret_val);

    // get the HSI Common API version
    int major = 0, minor = 0, patch = 0, build = 0;
    ret_val = commonGetAPIVersion (&major, &minor, &patch, &build);
    DisplayResult ("commonGetAPIVersion", ret_val);
    if (HSI_OK != ret_val)
    {
        system ("pause");
        return -1;
    }

    // print the version number
    std::cout << "*** HSI Common API version: " << major << "." << minor << "." << patch << "." << build << std::endl;

    // get the HSI Camera API version
    ret_val = cameraGetAPIVersion (&major, &minor, &patch, &build);
    DisplayResult ("cameraGetAPIVersion", ret_val);
    if (HSI_OK != ret_val)
    {
        system ("pause");
        return -1;
    }

    // print the version number
    std::cout << "*** HSI Camera API version: " << major << "." << minor << "." << patch << "." << build << std::endl;

    // allocate space for 5 CameraInfo's
    int const nr_preallocated_devices = 5;
    CameraInfo camera_infos[nr_preallocated_devices];

    // enumerate the connected devices
    int nr_devices = 0;
    ret_val = cameraEnumerateConnectedDevices (&camera_infos[0], &nr_devices, nr_preallocated_devices, { EM_ALL });
    DisplayResult ("cameraEnumerateConnectedDevices", ret_val);
    if (HSI_OK != ret_val)
    {
        system ("pause");
        return -1;
    }

    if (0 == nr_devices)
    {
        std::cout << "No connected devices found." << std::endl;
        system ("pause");
        return 0;
    }

    std::cout << "Number of connected devices: " << nr_devices << std::endl;

    for (int i = 0; i < nr_devices; ++i)
    {
        CameraInfo* p_camera_info = &camera_infos[i];

        std::cout << "Camera " << i << std::endl;
        std::cout << "    manufacturer:  " << p_camera_info->manufacturer << std::endl;
        std::cout << "    model:  " << p_camera_info->model << std::endl;
    }

    std::cout << std::endl;

    int selected_camera = 1;
    CameraInfo camera_info = camera_infos[selected_camera];

    std::cout << "Using camera " << selected_camera << std::endl;
    std::cout << "    manufacturer:  " << camera_info.manufacturer << std::endl;
    std::cout << "    model:  " << camera_info.model << std::endl;
    std::cout << "    serial_number:  " << camera_info.serial_number << std::endl;
    std::cout << "    identification_string:  " << camera_info.identification_string << std::endl;
    std::cout << std::endl;

    HANDLE camera = 0;
    ret_val = cameraOpenDevice (&camera, camera_info);
    DisplayResult ("cameraOpenDevice", ret_val);
    if (HSI_OK != ret_val)
    {
        system ("pause");
        return -1;
    }

    cameraConfigurationParameters config_parameters = {};
    cameraGetConfigurationParameters (camera, &config_parameters);
    // change configuration parameters here
    // note: also runtime parameters can be changed at this point
    ret_val = cameraSetConfigurationParameters (camera, config_parameters);
    DisplayResult ("cameraSetConfigurationParameters", ret_val);
    if (HSI_OK != ret_val)
    {
        cameraCloseDevice (&camera);
        system ("pause");
        return -1;
    }

    ret_val = cameraInitialize (camera);
    DisplayResult ("cameraInitialize", ret_val);
    if (HSI_OK != ret_val)
    {
        cameraCloseDevice (&camera);
        system ("pause");
        return -1;
    }

    // get output data format after initialization
    FrameDataFormat data_format = {};
    ret_val = cameraGetOutputFrameDataFormat (camera, &data_format);
    DisplayResult ("cameraGetOutputFrameDataFormat", ret_val);
    if (HSI_OK != ret_val)
    {
        cameraCloseDevice (&camera);
        system ("pause");
        return -1;
    }

    // allocate frame based on data format
    // alternative: AllocateFrameForCamera will first get data format and then allocate frame
    FrameFloat frame = {};
    ret_val = commonAllocateFrame (&frame, data_format);
    DisplayResult ("commonAllocateFrame", ret_val);
    if (HSI_OK != ret_val)
    {
        cameraCloseDevice (&camera);
        system ("pause");
        return -1;
    }

    cameraRuntimeParameters runtime_parameters = {};
    cameraGetRuntimeParameters (camera, &runtime_parameters);
    // change runtime parameters here
    // note: configuration parameters can NOT be changed at this point
    runtime_parameters.frame_rate_hz = 25;
    runtime_parameters.exposure_time_ms = 5.0;
    runtime_parameters.trigger_mode = TM_NoTriggering;
    ret_val = cameraSetRuntimeParameters (camera, runtime_parameters);
    DisplayResult ("cameraSetRuntimeParameters", ret_val);
    if (HSI_OK != ret_val)
    {
        cameraCloseDevice (&camera);
        system ("pause");
        return -1;
    }

    // start streaming
    ret_val = cameraStart (camera);
    DisplayResult ("cameraStart", ret_val);
    if (HSI_OK != ret_val)
    {
        cameraCloseDevice (&camera);
        system ("pause");
        return -1;
    }
    for (int i = 0; i < 100; i++) {
        using std::chrono::high_resolution_clock;
        using std::chrono::duration_cast;
        using std::chrono::duration;
        using std::chrono::milliseconds;

        auto t1 = high_resolution_clock::now();
        ret_val = cameraAcquireFrame (camera, &frame);
        DisplayResult ("cameraAcquireFrame", ret_val);
        auto t2 = high_resolution_clock::now();
        auto ms_int = duration_cast<milliseconds>(t2 - t1);
        std::cout << ms_int.count() << "ms\n";
    }
    
    
    if (HSI_OK != ret_val)
    {
        cameraCloseDevice (&camera);
        system ("pause");
        return -1;
    }

    ret_val = commonSaveFrame (frame, "./output/", "frame0", FF_PNG);
    DisplayResult ("commonSaveFrame", ret_val);
    if (HSI_OK != ret_val)
    {
        cameraCloseDevice (&camera);
        system ("pause");
        return -1;
    }

    cameraPause (camera);

    // now runtime parameters can be changed again
    cameraGetRuntimeParameters (camera, &runtime_parameters);
    // change runtime parameters here
    // note: configuration parameters can NOT be changed at this point
    runtime_parameters.frame_rate_hz = 25;
    runtime_parameters.exposure_time_ms = 2.5;
    runtime_parameters.trigger_mode = TM_NoTriggering;
    ret_val = cameraSetRuntimeParameters (camera, runtime_parameters);
    DisplayResult ("cameraSetRuntimeParameters", ret_val);
    if (HSI_OK != ret_val)
    {
        cameraCloseDevice (&camera);
        system ("pause");
        return -1;
    }

    // start streaming
    ret_val = cameraStart (camera);
    DisplayResult ("cameraStart", ret_val);
    if (HSI_OK != ret_val)
    {
        cameraCloseDevice (&camera);
        system ("pause");
        return -1;
    }

    ret_val = cameraAcquireFrame (camera, &frame);
    DisplayResult ("cameraAcquireFrame", ret_val);
    if (HSI_OK != ret_val)
    {
        cameraCloseDevice (&camera);
        system ("pause");
        return -1;
    }

    ret_val = commonSaveFrame (frame, "./output/", "frame1", FF_PNG);
    DisplayResult ("commonSaveFrame", ret_val);
    if (HSI_OK != ret_val)
    {
        cameraCloseDevice (&camera);
        system ("pause");
        return -1;
    }

    cameraPause (camera);

    cameraStop (camera);

    // no configuration parameters can be changed again
    // note: requires to reallocate your data!

    // cleanup
    commonDeallocateFrame (&frame);
    commonDeallocateFrameDataFormat (&data_format);

    // close connection
    cameraCloseDevice (&camera);

    system ("pause");

    return 0;
}

void DisplayResult (std::string const& i_message, HSI_RETURN i_result)
{
    std::cout << "*** " << std::setw(35) << i_message << ": " << i_result << " => ";
    
    switch (i_result)
    {
    case HSI_OK:                          std::cout << "Function call successful.\n"; break;
    case HSI_HANDLE_INVALID:              std::cout << "Invalid device handle specified.\n"; break;
    case HSI_ARGUMENT_INVALID:            std::cout << "Invalid argument provided in function call.\n"; break;
    case HSI_CALL_ILLEGAL:                std::cout << "Function call illegal given the current internal state.\n"; break;
    case HSI_FILE_NOT_FOUND:              std::cout << "A file could not be found.\n"; break;
    case HSI_CALIBRATION_FILE_NOT_FOUND:  std::cout << "Sensor calibration file could not be found.\n"; break;
    case HSI_CONNECTION_FAILED:           std::cout << "Camera could not be connected to the system.\n"; break;
    case HSI_ALLOCATION_ERROR:            std::cout << "Allocation of resources failed.\n"; break;
    case HSI_ACQUISITION_TIMEOUT:         std::cout << "An Acquisition timeout has occured.\n"; break;
    case HSI_ACQUISITION_FAILED:          std::cout << "Acquisition failed during operation.\n"; break;
    case HSI_DATA_NOT_ALLOCATED:          std::cout << "Provided data structure is not allocated.\n"; break;
    case HSI_DATA_NOT_VALID:              std::cout << "Data with valid flag false provided as input for operation.\n"; break;
    case HSI_DATA_NOT_COMPATIBLE:         std::cout << "Data provided is not compatible.\n"; break;
    case HSI_FILE_SYSTEM_ERROR:           std::cout << "Specified directory doesn't exist and could not be created.\n"; break;
    case HSI_FILE_IO_ERROR:               std::cout << "Could not read or write data from the filesystem.\n"; break;
    case HSI_INTERNAL_ERROR:              std::cout << "An unexpected internal error occurred.\n"; break;
    }
}

