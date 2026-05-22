'''//////////////////////////
//
// Date: 16 November, 2020
//
// Authors:
//    Geert Vanmeerbeeck
//    Bart Masschelein
//
//  Copyright 2020 imec. All rights reserved.
//
/////////////////'''

import sys
import signal
import logging
import numpy as np
import matplotlib.pyplot as plt
import time
import multiprocessing

# API path for installers
sys.path.append('../../python_apis')

# API path for imec development environment
sys.path.append('../../hsi_camera_api_python')

# Load the HSI APIs
import hsi_camera as HSI_CAMERA
import hsi_common as HSI_COMMON


def single_camera_example():
    # look for connected cameras
    dev_list = HSI_CAMERA.EnumerateConnectedDevices(manufacturer=HSI_CAMERA.Manufacturer.EM_XIMEA)
    logging.info(f'return number of devices = {len(dev_list)}')

    # and connect to the first one
    logging.info(f'looking for device:: { dev_list[0] }')
    device = HSI_CAMERA.OpenDevice(dev_list[0])

    # Get/Set Camera Configuration Parameters (example)
    c_params = HSI_CAMERA.GetConfigurationParameters(device)
    logging.info(f"Camera Configuration Parameters: {c_params}")
    HSI_CAMERA.SetConfigurationParameters(device, c_params)

    # [Optional] Apply a Region of Interest to the camera
    roi = [HSI_COMMON.RegionOfInterest(x=0, y=0, width=640, height=480)]
    HSI_CAMERA.SetRegionOfInterestArray(device, roi)
    logging.info(f"Region-of-Intereset Set to: {roi}")

    # [Optional] Apply High-Dynamic-Range (HDR) to the camera
    HDR_parameters = HSI_CAMERA.AllocateHDRParameters(640, 480, 2)
    HDR_parameters.integration_time_ratios[0] = 0.8
    HDR_parameters.integration_time_ratios[1] = 0.4
    logging.info(f"Camera HDR Parameters: {HDR_parameters}")
    # HSI.SetHdrSettings(device, HDR_parameters)
    HSI_CAMERA.DeallocateHDRParameters(HDR_parameters)

    logging.info('Initializing Camera...')
    HSI_CAMERA.Initialize(device)

    # Get/Set Camera Runtime Parameters (example)
    r_params = HSI_CAMERA.GetRuntimeParameters(device)
    r_params.trigger_mode = HSI_CAMERA.TriggerMode.TM_NO_TRIGGERING
    r_params.exposure_time_ms = 15
    r_params.frame_rate_hz = 30
    logging.info(f"Camera Runtime Parameters: {r_params}")
    HSI_CAMERA.SetRuntimeParameters(device, r_params)

    if 1:
        # Explicit Allocate based on Output-Data-Format
        dfmt = HSI_CAMERA.GetOutputFrameDataFormat(device)
        frame = HSI_COMMON.AllocateFrame(dfmt)
        logging.info(f"Camera Output Data Format: {dfmt}")
    else:
        # Implicit Allocate for Camera
        frame = HSI_CAMERA.AllocateFrameForCamera(device)
        logging.info(f"Camera Output Data Format: {frame.format}")

    # the device is ready: we can start it now ...
    HSI_CAMERA.Start(device)

    # Send a Software Trigger to the camera and grab the Frame
    # HSI_CAMERA.Trigger(device)
    for i in range(100):
        start_time = time.time()
        HSI_CAMERA.AcquireFrame(device, frame=frame)
        logging.info(frame)
        print("--- %s miliseconds ---" % (round((time.time() - start_time)*1000)))
    HSI_CAMERA.AcquireFrame(device, frame=frame)
    logging.info(frame)
c
    tmp = HSI_COMMON.FrameAsArray(frame)  # internally convert frame to numpy array
    logging.info(f'Acquired Frame :: {tmp.shape}')

    HSI_CAMERA.Pause(device)
    # Get r-params
    r_params = HSI_CAMERA.GetRuntimeParameters(device)
    print(r_params)
    r_params.frame_rate_hz = 1            
    HSI_CAMERA.SetRuntimeParameters(device, r_params) 
    # HSI_CAMERA.Start(device)
    # #Step-2: Set integration time            
    # HSI_CAMERA.Pause(device)
    r_params = HSI_CAMERA.GetRuntimeParameters(device)
    r_params.exposure_time_ms = 15   
    HSI_CAMERA.SetRuntimeParameters(device, r_params)    
    # HSI_CAMERA.Start(device)
    # #Step-3: Set frame rate            
    # HSI_CAMERA.Pause(device)
    r_params = HSI_CAMERA.GetRuntimeParameters(device)
    r_params.frame_rate_hz = 30        
    HSI_CAMERA.SetRuntimeParameters(device, r_params)  
    HSI_CAMERA.Start(device)
    # HSI_CAMERA.Trigger(device)
    HSI_CAMERA.AcquireFrame(device, frame=frame)
    logging.info(frame)

    tmp = HSI_COMMON.FrameAsArray(frame)  # internally convert frame to numpy array
    logging.info(f'Acquired Frame :: {tmp.shape}')
    # And Cleanup ...
    HSI_CAMERA.Pause(device)
    HSI_CAMERA.Stop(device)
    HSI_CAMERA.CloseDevice(device)
    HSI_COMMON.DeallocateFrame(frame)


def multiple_camera_example(num_cams=2, num_frames=20,
                            manufacturer=HSI_CAMERA.Manufacturer.EM_XIMEA,
                            path=r"C:\imec\camera_api"):
    # look for connected cameras
    dev_list = HSI_CAMERA.EnumerateConnectedDevices(manufacturer=manufacturer)
    logging.info(f'Return number of devices = {len(dev_list)}')

    if manufacturer != HSI_CAMERA.Manufacturer.EM_IMEC:
        num_cams = len(dev_list)
    else:
        # extend the dev_list with requested number of dummy cameras
        dev_list = dev_list * num_cams

    # and connect to the first one
    logging.info(f'looking for device:: { dev_list[0] }')
    device = [None] * num_cams
    frame_list = [[None] * num_cams for i in range(num_frames)]
    for c in range(num_cams):
        logging.info(f'Opening Camera-{c}')
        device[c] = HSI_CAMERA.OpenDevice(dev_list[c])

        logging.info(f'Initializing Camera-{c}')
        HSI_CAMERA.Initialize(device[c])

        logging.info(f'Setting parameters-{c}')
        runtime_params = HSI_CAMERA.GetRuntimeParameters(device[c])
        runtime_params.exposure_time_ms = 1.0
        runtime_params.frame_rate_hz = 15.0
        runtime_params.trigger_mode = HSI_CAMERA.TriggerMode.TM_NO_TRIGGERING
        HSI_CAMERA.SetRuntimeParameters(device[c], runtime_params)

        logging.info(f'Allocating Frame for Camera-{c}')
        for f in range(num_frames):
            frame_list[f][c] = HSI_CAMERA.AllocateFrameForCamera(device[c])
        logging.info(f"Camera-{c} Output Data Format: {frame_list[0][c].format}")

        logging.info(f'Starting Camera-{c}')
        HSI_CAMERA.Start(device[c])

    # Capture a number of images for each camera
    # This block is isolated from other operations not to impact frame rate calculation
    frame_rates = [None] * num_cams
    for c in range(num_cams):
        start_time = time.time()
        logging.info(f'Start time is {start_time}')
        for f in range(num_frames):
            sys.stdout.write('\r[{0}] {1}%'.format('#' * int((f+1) * 10 / num_frames), (f+1) * 100 / num_frames))
            HSI_CAMERA.AcquireFrame(device[c], frame=frame_list[f][c])
        print('')
        end_time = time.time()
        logging.info(f'End time is {end_time}')
        logging.info(f'For Camera-{c} it took {end_time-start_time} seconds to capture {num_frames} images')
        frame_rates[c] = num_frames / (end_time - start_time)
        logging.info(f'Frame rate for Camera-{c} is {frame_rates[c]}')

    # Convert frames to numpy arrays and save them to disk
    frame_array_list = [[None] * num_cams for i in range(num_frames)]
    logging.info(f'Saving images to {path}')
    for f in range(num_frames):
        for c in range(num_cams):
            # internally convert frame to numpy array
            frame_array_list[f][c] = HSI_COMMON.FrameAsArray(frame_list[f][c])
            # save the images
            HSI_COMMON.SaveFrame(frame_list[f][c], path, f"Camera{c}_{f}")

    # Display the images after all the acquisitions complete
    ax = [None] * num_cams
    im = [None] * num_cams
    for c in range(num_cams):
        ax[c] = plt.subplot(1, num_cams, c + 1)
        ax[c].title.set_text(f'Camera-{c} Image1')
        im[c] = ax[c].imshow(frame_array_list[0][c], cmap='gray', vmin=0, vmax=1023)

    plt.ion()

    for f in range(1, num_frames):
        for c in range(num_cams):
            ax[c].title.set_text(f'Camera-{c} Image{f+1}\nFrameRate:{int(frame_rates[c])}')
            im[c].set_data(frame_array_list[f][c])
        plt.pause(0.01)

    for c in range(num_cams):
        # And Cleanup ...
        HSI_CAMERA.Pause(device[c])
        HSI_CAMERA.Stop(device[c])
        HSI_CAMERA.CloseDevice(device[c])
        for f in range(num_frames):
            HSI_COMMON.DeallocateFrame(frame_list[f][c])


def _graph_worker_process(image_q, frame_rate_q, num_cams):
    sys.stdout.write("Process STARTED!!!!\n")
    sys.stdout.flush()
    num_frames = 0
    num_queue_items = 0
    which_cam = 0
    frame_rate = 0
    # Prepare visualization data structures
    ax = [None] * num_cams
    im = [None] * num_cams
    for c in range(num_cams):
        ax[c] = plt.subplot(1, num_cams, c + 1)
        ax[c].title.set_text(f'Camera-{c}')

    while True:
        try:
            image_array = image_q.get()
            num_queue_items = num_queue_items + 1

            if not frame_rate_q.empty():
                frame_rate = frame_rate_q.get()

            if num_frames == 0:
                im[which_cam] = ax[which_cam].imshow(image_array, cmap='gray', vmin=0, vmax=1023)
                plt.ion()
            else:
                ax[which_cam].title.set_text(f'Camera-{which_cam}\nFrame Rate: {int(frame_rate)}')
                im[which_cam].set_data(image_array)

            plt.pause(0.001)

            num_frames = num_queue_items // num_cams
            which_cam = num_queue_items % num_cams
        except:
            pass


# Global vars for process and signal handling
worker = None
device = None
frame_list = None
image_queue = None
sample_rate_queue = None
total_cams = 0


def multiple_camera_livestream_example(num_cams=2,
                                       manufacturer=HSI_CAMERA.Manufacturer.EM_IMEC):
    # look for connected cameras
    dev_list = HSI_CAMERA.EnumerateConnectedDevices(manufacturer=manufacturer)
    logging.info(f'Return number of devices = {len(dev_list)}')

    if manufacturer != HSI_CAMERA.Manufacturer.EM_IMEC:
        num_cams = len(dev_list)
    else:
        # extend the dev_list with requested number of dummy cameras
        dev_list = dev_list * num_cams
    global total_cams
    total_cams = num_cams

    # Start graph worker process and init queue
    global image_queue
    image_queue = multiprocessing.Queue()
    global sample_rate_queue
    sample_rate_queue = multiprocessing.Queue()
    global worker
    worker = multiprocessing.Process(target=_graph_worker_process, args=(image_queue, sample_rate_queue, num_cams))
    worker.daemon = True

    # and connect to the first one
    logging.info(f'Looking for device:: { dev_list[0] }')
    global device
    device = [None] * num_cams
    global frame_list
    frame_list = [None] * num_cams
    for c in range(num_cams):
        logging.info(f'Opening Camera-{c}')
        device[c] = HSI_CAMERA.OpenDevice(dev_list[c])

        logging.info(f'Initializing Camera-{c}')
        HSI_CAMERA.Initialize(device[c])

        logging.info(f'Setting parameters-{c}')
        runtime_params = HSI_CAMERA.GetRuntimeParameters(device[c])
        runtime_params.exposure_time_ms = 1.0
        runtime_params.frame_rate_hz = 15.0
        runtime_params.trigger_mode = HSI_CAMERA.TriggerMode.TM_NO_TRIGGERING
        HSI_CAMERA.SetRuntimeParameters(device[c], runtime_params)

        logging.info(f'Allocating Frame for Camera-{c}')
        frame_list[c] = HSI_CAMERA.AllocateFrameForCamera(device[c])
        logging.info(f"Camera-{c} Output Data Format: {frame_list[0].format}")

        logging.info(f'Starting Camera-{c}')
        HSI_CAMERA.Start(device[c])

    # Start worker process for visualization
    worker.start()

    # Capture images for each camera continuously and display livestream
    frame_array_list = [None] * num_cams
    num_frames = 0
    last_frame_num = 0
    frame_rate = 0
    start_time = time.time()
    while True:
        for c in range(num_cams):
            HSI_CAMERA.AcquireFrame(device[c], frame=frame_list[c])
            end_time = time.time()
            frame_array_list[c] = HSI_COMMON.FrameAsArray(frame_list[c])
            image_queue.put(frame_array_list[c])

        num_frames = num_frames + 1
        end_time = time.time()
        if end_time - start_time > 1:
            frame_rate = (num_frames - last_frame_num) / (end_time - start_time)
            sample_rate_queue.put(frame_rate)
            last_frame_num = num_frames
            start_time = time.time()
        logging.info(f'To stop the execution, first close the video screen and then press Ctrl+C on the console.')


def _signal_handler(signum, frame):
    logging.info("Ctrl+C is pressed!") 

    global total_cams
    global device
    global frame_list
    for c in range(total_cams):
        # And Cleanup ...
        HSI_CAMERA.Pause(device[c])
        HSI_CAMERA.Stop(device[c])
        HSI_CAMERA.CloseDevice(device[c])
        HSI_COMMON.DeallocateFrame(frame_list[c])

    sys.exit(0)



if __name__ == '__main__':

    # Set up the logging
    formatter = logging.Formatter(fmt='%(asctime)s %(levelname)-6s :: %(message)s',
                                  datefmt='%Y-%m-%d %H:%M:%S')
    logger = logging.getLogger()
    for h in logger.handlers:
        h.setFormatter(formatter)
    logger.setLevel(logging.INFO)

    logging.info('START MAIN.')

    # Set up the internal HSI logging infrastructure
    HSI_COMMON.InitializeLogger('./logs', HSI_COMMON.LoggerVerbosity.LV_VERBOSE)

    signal.signal(signal.SIGINT, _signal_handler)

    # For a single IMEC Dummy Camera
    single_camera_example()
    # For 2 IMEC Dummy Cameras
    # multiple_camera_example()
    # For 2 XIMEA Cameras to capture 50 frames
    # multiple_camera_example(2, 50, HSI_CAMERA.Manufacturer.EM_XIMEA)
    # For 3 IMEC Dummy Cameras in livestream
    # multiple_camera_livestream_example(3, HSI_CAMERA.Manufacturer.EM_IMEC)
    # For 2 XIMEA Cameras in livestream
    # multiple_camera_livestream_example(2, HSI_CAMERA.Manufacturer.EM_XIMEA)

    logging.info('DONE.')
