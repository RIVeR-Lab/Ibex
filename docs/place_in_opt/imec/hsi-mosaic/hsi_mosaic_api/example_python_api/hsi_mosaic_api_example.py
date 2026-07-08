'''//////////////////////////
//
// Date: 16 November, 2020
//
// Authors:
//    Geert Vanmeerbeeck
//
//  Copyright 2020 imec. All rights reserved.
//
/////////////////'''

import os
# NOTE: put this as very first action.
#       DLL-loading Could be impacted by loading other libs first ...
os.environ['PATH'] += os.pathsep + r'../../bin'

## only needed for python 3.8 (and above?)
try:
    os.add_dll_directory(r'../../bin')
except: ...


import sys
import logging
import numpy as np
import time
from pathlib import Path

# API path for deployment
sys.path.append('../../python_apis')

# API path for imec development environment
#sys.path.append('../../../hsi_api/hsi_api_python/')
#sys.path.append('../../../hsi_camera_api/hsi_camera_api_python/')
#sys.path.append('../../hsi_mosaic_api_python')

import hsi_common as HSI_COMMON
import hsi_mosaic as HSI_MOSAIC
import hsi_camera as HSI_CAMERA

if __name__ == '__main__':

    dn_context = '../example_resources/ApplesBelgica/context'
    fname = dn_context + '/../Acquisition0/image_0.raw.xml'

    #####################################################################

    formatter = logging.Formatter(fmt='%(asctime)s %(levelname)-6s :: %(message)s',
                                  datefmt='%Y-%m-%d %H:%M:%S')
    logger = logging.getLogger()
    for h in logger.handlers: h.setFormatter(formatter)
    logger.setLevel(logging.DEBUG)

    logging.info ('START MAIN.')

    version = HSI_MOSAIC.GetAPIVersion()
    logging.info (f'VERSION :: {version}')

    logging.info ("Starting Logger.")
    HSI_COMMON.InitializeLogger('logs', HSI_COMMON.LoggerVerbosity.LV_DEBUG)

    logging.info ("Loading Context ...")
    logging.info(Path(dn_context).absolute())
    assert Path(dn_context).exists()
    context = HSI_MOSAIC.LoadContext(dn_context)
    logging.info (f'Context = {context}')

    status = HSI_MOSAIC.ContextGetStatus(context)
    logging.info(status)


    #####################################################################
    ### Creating the first pipeline
    #####################################################################
    pipeline = HSI_MOSAIC.Create(context)
    logging.info(pipeline)

    params = HSI_MOSAIC.GetConfigurationParameters(pipeline)
    logging.info(f'Configuration Params : {params}')
    params.spatial_resampling_width = 1020
    params.spatial_resampling_height = 540
    HSI_MOSAIC.SetConfigurationParameters(pipeline, params)

    logging.info('Initializing Pipeline')
    HSI_MOSAIC.Initialize(pipeline)

    outputdataformat = HSI_MOSAIC.GetOutputDataFormat(pipeline)
    logging.info(f'Output Data Format = {outputdataformat}')

    cube = HSI_COMMON.AllocateCube(outputdataformat)

    logging.info('Starting Pipeline')
    HSI_MOSAIC.Start(pipeline)

    logging.info(f'Loading Frame: {fname}')
    frame = HSI_COMMON.LoadFrame(fname)
    logging.info(f'FRAME = {frame.format} / {frame.info}')
    np_frame = HSI_COMMON.FrameAsArray(frame)
    logging.info(f'SHAPE = {np_frame.shape}')

    HSI_MOSAIC.PushFrame(pipeline, frame)
    HSI_MOSAIC.GetCube(pipeline, cube, timeout_ms=8000)

    HSI_MOSAIC.Pause(pipeline)
    HSI_MOSAIC.Stop(pipeline)

    logging.info(f'Output Cube: {cube.format} / {cube.info}')
    HSI_COMMON.SaveCube(cube, 'OUT', 'ref_cube_out', HSI_COMMON.FileFormat.FF_ENVI)

    py_cube = HSI_COMMON.CubeAsArray(cube)
    logging.info(f'Cube shape {py_cube.shape}, {(py_cube.min(), py_cube.mean(), py_cube.max())}')

    # select the white tile in the scene as reference spectrum
    roi = HSI_COMMON.RegionOfInterest(x=700, y=300, width=120, height=120)
    ref_spectrum = HSI_MOSAIC.ExtractSpectrumFromCube(cube, roi)

    logging.info(f'Ref-spectrum : {HSI_COMMON.SpectrumAsDict(ref_spectrum)}')
    HSI_COMMON.SaveSpectrum(ref_spectrum, "OUT", 'ref_spectrum')
    logging.info(f"Spectrum: {ref_spectrum.as_dict()}")

    HSI_COMMON.DeallocateCube(cube)

    #####################################################################
    ### the second pipeline
    #####################################################################

    logging.info('Setting Pipeline Reference Spectrum...')
    HSI_MOSAIC.SetReferenceSpectrum(pipeline, ref_spectrum, 0.75)
    HSI_COMMON.DeallocateSpectrum(ref_spectrum)

    logging.info('Initializing Pipeline')
    HSI_MOSAIC.Initialize(pipeline)

    outputdataformat = HSI_MOSAIC.GetOutputDataFormat(pipeline)
    logging.info(f'Output Data Format = {outputdataformat}')

    cube = HSI_COMMON.AllocateCube(outputdataformat)

    logging.info('Starting Pipeline')
    HSI_MOSAIC.Start(pipeline)
    for i in range(1000):
        time1 = time.time()
        HSI_MOSAIC.PushFrame(pipeline, frame)
        HSI_MOSAIC.GetCube(pipeline, cube, timeout_ms=8000)
        time2 = time.time()
        print (f'function took {(time2-time1)*1000.0} ms')
    logging.info(f'Output Cube: {cube.format} / {cube.info}')

    HSI_COMMON.SaveCube(cube, 'OUT', 'rad_cube_out', HSI_COMMON.FileFormat.FF_ENVI)

    HSI_MOSAIC.Pause(pipeline)
    HSI_MOSAIC.Stop(pipeline)
    HSI_MOSAIC.Destroy(pipeline)

    py_cube = HSI_COMMON.CubeAsArray(cube)
    logging.info(f'Cube shape {py_cube.shape}, {(py_cube.min(), py_cube.mean(), py_cube.max())}')

    #####################################################################

    logging.info("Cleanup ...")
    HSI_COMMON.DeallocateFrame(frame)
    HSI_COMMON.DeallocateCube(cube)
    HSI_MOSAIC.DeallocateContext(context)

    logging.info ('MAIN DONE.')
