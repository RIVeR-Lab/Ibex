// *****************************************************************************
//
// Copyright (c) 2023, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <stdlib.h>
#include <iostream>


#define BUFFERCOUNT ( 16 )
#define DEFAULT_FPS ( 30 )

// Don't allow any image data, just chunk data
#define WIDTH_MIN ( 4 )
#define WIDTH_MAX ( 1920 )
#define WIDTH_DEFAULT ( 640 )
#define WIDTH_INC ( 4 )

#define HEIGHT_MIN ( 1 )
#define HEIGHT_MAX ( 1080 )
#define HEIGHT_DEFAULT ( 480 )
#define HEIGHT_INC ( 1 )

#define BASE_ADDR 0x20000000

//
// Chunk defines
//

#define CHUNKID ( 0x4001 )
#define CHUNKLAYOUTID ( 0x12345678 )
#define CHUNKSIZE ( 64 )

#define CHUNKCATEGORY "ChunkDataControl"

#define CHUNKCOUNTNAME "ChunkSampleCount"
#define CHUNKCOUNTDESCRIPTION "Counter keeping track of images with chunks generated."
#define CHUNKCOUNTTOOLTIP "Chunk count."

#define CHUNKTIMENAME "ChunkSampleTime"
#define CHUNKTIMEDESCRIPTION "String representation of the time when the chunk data was generated."
#define CHUNKTIMETOOLTIP "Chunk time."

