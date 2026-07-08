import os
import sys

# Set up library paths before importing IMEC modules.
# LD_LIBRARY_PATH must be set before the process starts, so if it's not
# already configured, re-launch this script once with the correct environment.
_lib_paths = [
    '/opt/imec/hsi-mosaic/bin',
    '/opt/imec/hsi-mosaic/resources/installers/package/api/X64',
    '/opt/pleora/ebus_sdk/Ubuntu-22.04-x86_64/lib',
    '/opt/pleora/ebus_sdk/Ubuntu-22.04-x86_64/lib/genicam/bin/Linux64_x64',
    '/opt/XIMEA/CamTool',
    '/opt/PFSDK_2025.1.0_Linux64/lib',
]

if not os.environ.get('_IMEC_ENV_SET'):
    current = os.environ.get('LD_LIBRARY_PATH', '')
    os.environ['LD_LIBRARY_PATH'] = ':'.join(filter(None, _lib_paths + [current]))
    os.environ['GENICAM_ROOT_V3_1'] = '/opt/pleora/ebus_sdk/Ubuntu-22.04-x86_64/lib/genicam'
    os.environ['GENICAM_ROOT_V3_4'] = '/opt/pleora/ebus_sdk/Ubuntu-22.04-x86_64/lib/genicam'
    os.environ['_IMEC_ENV_SET'] = '1'
    # Re-execute this script with the updated environment
    os.execv(sys.executable, [sys.executable] + sys.argv)

import numpy as np
sys.path.append('/opt/imec/hsi-mosaic/python_apis')

import hsi_common as HSI_COMMON
import hsi_camera as HSI_CAMERA
import hsi_mosaic as HSI_MOSAIC

#CONTEXT_PATH = '/home/river/ibex_ws/src/hyper_drive/config/imec/context'
CONTEXT_PATH = '/home/river/imec_light_reference/context'
NUM_FRAMES = 30  # number of frames to average for each reference
INTEGRATION_TIME_MS = 60.0
FRAME_RATE_HZ = 15.0

def capture_averaged_frame(device, frame_template, num_frames):
    '''Capture multiple frames and return their average'''
    frames = []
    for i in range(num_frames):
        result, frame = HSI_CAMERA.AcquireFrame(device, frame=frame_template)
        arr = HSI_COMMON.FrameAsArray(frame)
        frames.append(arr.copy())
        print(f'Captured frame {i+1}/{num_frames}')
    
    averaged = np.mean(frames, axis=0)
    
    # Write averaged data back into a FrameFloat
    avg_frame = HSI_COMMON.AllocateFrame(
        HSI_CAMERA.GetOutputFrameDataFormat(device)
    )
    arr = HSI_COMMON.FrameAsArray(avg_frame)
    arr[:] = averaged
    return avg_frame

def main():
    # Initialize logger
    HSI_COMMON.InitializeLogger('/tmp', HSI_COMMON.LoggerVerbosity.LV_INFO)
    
    # Find and connect to IMEC camera
    dev_list = HSI_CAMERA.EnumerateConnectedDevices(
        manufacturer=HSI_CAMERA.Manufacturer.EM_IMEC
    )
    assert len(dev_list) > 0, 'No IMEC camera found'
    
    device = HSI_CAMERA.OpenDevice(dev_list[0])
    HSI_CAMERA.SetRegionOfInterestArray(
        device,
        [HSI_COMMON.RegionOfInterest(x=1, y=1, width=639, height=510)]
    )
    HSI_CAMERA.Initialize(device)
    
    # Set requested runtime parameters
    r_params = HSI_CAMERA.GetRuntimeParameters(device)
    r_params.frame_rate_hz = FRAME_RATE_HZ
    r_params.exposure_time_ms = INTEGRATION_TIME_MS
    r_params['flip_horizontal'] = True
    r_params['flip_vertical'] = False
    HSI_CAMERA.SetRuntimeParameters(device, r_params)
    HSI_CAMERA.Start(device)

    # Query what the camera actually applied
    r_params_actual = HSI_CAMERA.GetRuntimeParameters(device)
    ACTUAL_INTEGRATION_MS = r_params_actual.exposure_time_ms
    print(f'Requested: {INTEGRATION_TIME_MS}ms')
    print(f'Actual hardware integration time: {ACTUAL_INTEGRATION_MS}ms')
    print(f'Actual hardware frame rate: {r_params_actual.frame_rate_hz}Hz')
    
    # Allocate frame template
    dataformat = HSI_CAMERA.GetOutputFrameDataFormat(device)
    frame_template = HSI_COMMON.AllocateFrame(dataformat)
    
    # Load existing context
    context = HSI_MOSAIC.LoadContext(CONTEXT_PATH)
    
    # --- DARK REFERENCE: capture a single frame, no averaging ---
    input('\nCover the lens completely, then press Enter...')
    print('Capturing single dark reference frame...')

    # Acquire one frame directly into a fresh allocation
    dark_frame = HSI_COMMON.AllocateFrame(
        HSI_CAMERA.GetOutputFrameDataFormat(device)
    )
    result, dark_frame = HSI_CAMERA.AcquireFrame(device, frame=dark_frame)

    # Verify the frame
    arr = HSI_COMMON.FrameAsArray(dark_frame)
    print(f'Dark frame - min: {arr.min():.1f} max: {arr.max():.1f} mean: {arr.mean():.1f}')

    HSI_MOSAIC.ContextSetDarkFieldReferences(context, [dark_frame])
    print('Dark reference set')


    has_dark = HSI_MOSAIC.ContextIntegrationTimeInDark(context, 60.0)
    print(f'Dark reference for 60.0ms: {bool(has_dark)}')
    
    # White reference - single frame
    input('\nPoint camera at uniform white surface, then press Enter...')
    print('Capturing single white reference frame...')

    white_frame = HSI_COMMON.AllocateFrame(
        HSI_CAMERA.GetOutputFrameDataFormat(device)
    )
    result, white_frame = HSI_CAMERA.AcquireFrame(device, frame=white_frame)

    arr = HSI_COMMON.FrameAsArray(white_frame)
    print(f'White frame - min: {arr.min():.1f} max: {arr.max():.1f} mean: {arr.mean():.1f}')
    HSI_MOSAIC.ContextSetNonUniformity(context, dark_frame, white_frame)
    print('Nonuniformity correction captured')
    
    
    # --- SAVE CONTEXT ---
    HSI_MOSAIC.SaveContext(context, CONTEXT_PATH)
    print(f'Context saved to {CONTEXT_PATH}')
    
    # Verify
    status = HSI_MOSAIC.ContextGetStatus(context)
    print(f'Context status: {status}')
    
    # Check dark reference matches integration time
    has_dark = HSI_MOSAIC.ContextIntegrationTimeInDark(context, INTEGRATION_TIME_MS)
    print(f'Dark reference for {INTEGRATION_TIME_MS}ms: {bool(has_dark)}')

    print('Searching for stored dark reference integration time...')
    print('Searching for stored dark reference integration time...')
    found = False
    for t in [x * 0.1 for x in range(550, 650)]:
        if HSI_MOSAIC.ContextIntegrationTimeInDark(context, t):
            print(f'Dark reference found at: {t:.1f}ms')
            found = True
            break
    if not found:
        print('Dark reference not found in 55-65ms range')
    
    # Cleanup
    HSI_MOSAIC.DeallocateContext(context)
    HSI_CAMERA.Pause(device)
    HSI_CAMERA.Stop(device)
    HSI_CAMERA.CloseDevice(device)
    print('Done')

if __name__ == '__main__':
    main()