#ifndef FISCAMERAIO_H
#define FISCAMERAIO_H

//----------------------------------------------------------------------------------------
#include "FIS_lib.h"
#include <xiApi.h>

/*
 * This function takes the data alignment required by FIS_Lib (lowlevel implementations may need to
 * frame size to be multiple of the HDD sector size), and sets this value to xiAPI to allocate
 * larger buffers. The final frame size is calculated from the xiAPI payload size, and is set to FIS
 * recorder. You can access it later from IxFisRecorder::frameSize().
 */
bool FIS_setBufferAligningAndFrameSize(IxFisRecorder *pRecorder, HANDLE hCamera, std::string *psErrorMsg = nullptr, int32_t *piFisDataAlign = nullptr);

#endif //FISCAMERAIO_H
