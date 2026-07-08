# *****************************************************************************
#
# Copyright (c) 2023, Pleora Technologies Inc., All rights reserved.
#
# *****************************************************************************

BUFFERCOUNT = 16
DEFAULT_FPS = 30

WIDTH_MIN = 4
WIDTH_MAX = 1920
WIDTH_DEFAULT = 640
WIDTH_INC = 4

HEIGHT_MIN = 1
HEIGHT_MAX = 1080
HEIGHT_DEFAULT = 480
HEIGHT_INC = 1

BASE_ADDR = 0x20000000

#
# Chunk defines
#

CHUNKID = 0x4001
CHUNKLAYOUTID = 0x12345678
CHUNKSIZE = 64

CHUNKCATEGORY = "ChunkDataControl"

CHUNKCOUNTNAME = "ChunkSampleCount"
CHUNKCOUNTDESCRIPTION = "Counter keeping track of images with chunks generated."
CHUNKCOUNTTOOLTIP = "Chunk count."

CHUNKTIMENAME = "ChunkSampleTime"
CHUNKTIMEDESCRIPTION = "String representation of the time when the chunk data was generated."
CHUNKTIMETOOLTIP = "Chunk time."

