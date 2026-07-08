# *****************************************************************************
#
# Copyright (c) 2023, Pleora Technologies Inc., All rights reserved.
#
# *****************************************************************************

BUFFERCOUNT = 16
DEFAULT_FPS = 30

WIDTH_MIN = 64
WIDTH_MAX = 1920
WIDTH_DEFAULT = 640
WIDTH_INC = 4

HEIGHT_MIN = 4
HEIGHT_MAX = 1080
HEIGHT_DEFAULT = 480
HEIGHT_INC = 1

BASE_ADDR = 0x20000000

# Custom parameters defines

SAMPLECATEGORY = "SampleCategory"

SAMPLEENUMCOUNT = 3
SAMPLEENUMADDR = 0x10000000
SAMPLEENUMNAME = "SampleEnum"
SAMPLEENUMDESCRIPTION = "Sample enum description. Selects both sample string and sample bool."
SAMPLEENUMTOOLTIP = "Sample enum."

SAMPLESTRINGADDR = 0x10000010
SAMPLESTRINGNAME = "SampleString"
SAMPLESTRINGDESCRIPTION = "Sample string description. Cleared by sample command, invalidated by sample enum."
SAMPLESTRINGTOOLTIP = "Sample string. Selected by sample enum."

SAMPLEBOOLEANADDR = 0x10000020
SAMPLEBOOLEANNAME = "SampleBoolean"
SAMPLEBOOLEANDESCRIPTION = "Sample Boolean description."
SAMPLEBOOLEANTOOLTIP = "Sample Boolean. Selected by sample enum."

SAMPLECOMMANDADDR = 0x10000030
SAMPLECOMMANDNAME = "SampleCommand"
SAMPLECOMMANDDESCRIPTION = "Sample command description. Clears sample string."
SAMPLECOMMANDTOOLTIP = "Sample command."

SAMPLEINTEGERADDR = 0x10000040
SAMPLEINTEGERNAME = "SampleInteger"
SAMPLEINTEGERDESCRIPTION = "Sample integer defined as milliseconds."
SAMPLEINTEGERTOOLTIP = "Sample integer."
SAMPLEINTEGERUNITS = "ms"

SAMPLEFLOATADDR = 0x10000050
SAMPLEFLOATNAME = "SampleFloat"
SAMPLEFLOATDESCRIPTION = "Sample float defined as inches."
SAMPLEFLOATTOOLTIP = "Sample float."
SAMPLEFLOATUNITS = "inches"

SAMPLEPVALUE = "SamplePValue"
SAMPLEPVALUEDISPLAYNAME = "pValue"
SAMPLEPVALUEDESCRIPTION = "Sample pValue pointing to integer sample feature."
SAMPLEPVALUETOOLTIP = "Sample pValue to sample integer."
SAMPLEPVALUEUNITS = "ms"

SAMPLEINTSWISSKNIFENAME = "SampleIntSwissKnife"
SAMPLEINTSWISSKNIFEDESCRIPTION = "Sample integer SwissKnife which allows reading the sample integer as nanoseconds."
SAMPLEINTSWISSKNIFETOOLTIP = "Sample integer SwissKnife."
SAMPLEINTSWISSKNIFEUNITS = "ns"

SAMPLEFLOATSWISSKNIFENAME = "SampleFloatSwissKnife"
SAMPLEFLOATSWISSKNIFEDESCRIPTION = "Sample float SwissKnife which allows reading the sample float as centimeters."
SAMPLEFLOATSWISSKNIFETOOLTIP = "Sample float SwissKnife."
SAMPLEFLOATSWISSKNIFEUNITS = "cm"

SAMPLEINTCONVERTERNAME = "SampleIntConverter"
SAMPLEINTCONVERTERDESCRIPTION = "Integer converter linked to sample integer. Exposes the millisecond sample integer as nanosecond."
SAMPLEINTCONVERTERTOOLTIP = "Sample integer converter."
SAMPLEINTCONVERTERUNITS = "ns"

SAMPLEFLOATCONVERTERNAME = "SampleFloatConverter"
SAMPLEFLOATCONVERTERDESCRIPTION = "Float converter linked to sample float . Exposes the inches sample float as centimeters."
SAMPLEFLOATCONVERTERTOOLTIP = "Sample float converter."
SAMPLEFLOATCONVETERUNITS = "cm"

SAMPLEHIDDENSWISSKNIFENAME = "SampleHiddenSwissKnife"

SAMPLEPISAVAILABLENAME = "SamplePIsAvailable"
SAMPLEPISAVAILABLEDISPLAYNAME = "pIsAvailable"
SAMPLEPISAVAILABLEDESCRIPTION = "Sample pIsAvailable example: points to sample enumeration (as integer) but is only available when sample integer is greater than 5 (through sample hidden SwissKnife)"
SAMPLEPISAVAILABLETOOLTIP = "Sample pIsAvailable example."

SOURCE0_BOOL_ADDR = ( BASE_ADDR + 0x100 )
SOURCE0_INT_ADDR  = ( BASE_ADDR + 0x104 )

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


#
# Event defines
#

EVENTID = 0x9006
EVENTDATAID = 0x9005

EVENTCATEGORY = "EventControl\\EventSample"

EVENTCOUNTNAME = "EventSampleCount"
EVENTCOUNTDESCRIPTION = "Counter keeping track of events generated."
EVENTCOUNTTOOLTIP = "Event count."

EVENTTIMENAME = "EventSampleTime"
EVENTTIMEDESCRIPTION = "String representation of the time when the event was generated."
EVENTTIMETOOLTIP = "Event time."


#
# File defines
#

FILESELECTORADDR = 0x18000000
FILEBUFFERSIZE = 0x4000

FILECATEGORYNAME = "FileAccessControl"
FILESELECTORNAME = "FileSelector"
FILEOPERATIONSELECTORNAME = "FileOperationSelector"
FILEOPERATIONEXECUTENAME = "FileOperationExecute"
FILEOPENMODENAME = "FileOpenMode"
FILEACCESSBUFFERNAME = "FileAccessBuffer"
FILEACCESSOFFSETNAME = "FileAccessOffset"
FILEACCESSLENGTHNAME = "FileAccessLength"
FILEOPERATIONSTATUSNAME = "FileOperationStatus"
FILEOPERATIONRESULTNAME = "FileOperationResult"
FILESIZENAME = "FileSize"
