// *****************************************************************************
//
// Copyright (c) 2018, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <stdlib.h>
#include <iostream>


#define BUFFERCOUNT ( 16 )
#define DEFAULT_FPS ( 30 )

#define WIDTH_MIN ( 64 )
#define WIDTH_MAX ( 1920 )
#define WIDTH_DEFAULT ( 640 )
#define WIDTH_INC ( 4 )

#define HEIGHT_MIN ( 4 )
#define HEIGHT_MAX ( 1080 )
#define HEIGHT_DEFAULT ( 480 )
#define HEIGHT_INC ( 1 )

// Custom parameters defines

#define SAMPLECATEGORY "SampleCategory"

#define SAMPLEENUMADDR ( 0x10000000 )
#define SAMPLEENUMNAME "SampleEnum"
#define SAMPLEENUMDESCRIPTION "Sample enum description. Selects both sample integer and sample float."
#define SAMPLEENUMTOOLTIP "Sample enum."

#define SAMPLESTRINGADDR ( 0x10000010 )
#define SAMPLESTRINGNAME "SampleString"
#define SAMPLESTRINGDESCRIPTION "Sample string description. Invalidated by sample command."
#define SAMPLESTRINGTOOLTIP "Sample string."

#define SAMPLEBOOLEANADDR ( 0x10000020 )
#define SAMPLEBOOLEANNAME "SampleBoolean"
#define SAMPLEBOOLEANDESCRIPTION "Sample Boolean description."
#define SAMPLEBOOLEANTOOLTIP "Sample Boolean."

#define SAMPLECOMMANDADDR ( 0x10000030 )
#define SAMPLECOMMANDNAME "SampleCommand"
#define SAMPLECOMMANDDESCRIPTION "Sample command description. Invalidates sample string."
#define SAMPLECOMMANDTOOLTIP "Sample command."

#define SAMPLEINTEGERADDR ( 0x10000040 )
#define SAMPLEINTEGERNAME "SampleInteger"
#define SAMPLEINTEGERDESCRIPTION "Sample integer defined as milliseconds. Selected by sample enum."
#define SAMPLEINTEGERTOOLTIP "Sample integer."
#define SAMPLEINTEGERUNITS "ms"

#define SAMPLEFLOATADDR ( 0x10000050 )
#define SAMPLEFLOATNAME "SampleFloat"
#define SAMPLEFLOATDESCRIPTION "Sample float defined as inches. Selected by sample enum."
#define SAMPLEFLOATTOOLTIP "Sample float."
#define SAMPLEFLOATUNITS "inches"

#define SAMPLEPVALUE "SamplePValue"
#define SAMPLEPVALUEDISPLAYNAME "pValue"
#define SAMPLEPVALUEDESCRIPTION "Sample pValue pointing to integer sample feature."
#define SAMPLEPVALUETOOLTIP "Sample pValue to sample integer."
#define SAMPLEPVALUEUNITS "ms"

#define SAMPLEINTSWISSKNIFENAME "SampleIntSwissKnife"
#define SAMPLEINTSWISSKNIFEDESCRIPTION "Sample integer SwissKnife which allows reading the sample integer as nanoseconds."
#define SAMPLEINTSWISSKNIFETOOLTIP "Sample integer SwissKnife."
#define SAMPLEINTSWISSKNIFEUNITS "ns"

#define SAMPLEFLOATSWISSKNIFENAME "SampleFloatSwissKnife"
#define SAMPLEFLOATSWISSKNIFEDESCRIPTION "Sample float SwissKnife which allows reading the sample float as centimeters."
#define SAMPLEFLOATSWISSKNIFETOOLTIP "Sample float SwissKnife."
#define SAMPLEFLOATSWISSKNIFEUNITS "cm"

#define SAMPLEINTCONVERTERNAME "SampleIntConverter"
#define SAMPLEINTCONVERTERDESCRIPTION "Integer converter linked to sample integer. Exposes the millisecond sample integer as nanosecond."
#define SAMPLEINTCONVERTERTOOLTIP "Sample integer converter."
#define SAMPLEINTCONVERTERUNITS "ns"

#define SAMPLEFLOATCONVERTERNAME "SampleFloatConverter"
#define SAMPLEFLOATCONVERTERDESCRIPTION "Float converter linked to sample float . Exposes the inches sample float as centimeters."
#define SAMPLEFLOATCONVERTERTOOLTIP "Sample float converter."
#define SAMPLEFLOATCONVETERUNITS "cm"

#define SAMPLEHIDDENSWISSKNIFENAME "SampleHiddenSwissKnife"

#define SAMPLEPISAVAILABLENAME "SamplePIsAvailable"
#define SAMPLEPISAVAILABLEDISPLAYNAME "pIsAvailable"
#define SAMPLEPISAVAILABLEDESCRIPTION "Sample pIsAvailable example: points to sample enumeration (as integer) but is only available when sample integer is greater than 5 (through sample hidden SwissKnife)"
#define SAMPLEPISAVAILABLETOOLTIP "Sample pIsAvailable example."


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


//
// Event defines
//

#define EVENTID ( 0x9006 )
#define EVENTDATAID ( 0x9005 )

#define EVENTCATEGORY "EventControl\\EventSample"

#define EVENTCOUNTNAME "EventSampleCount"
#define EVENTCOUNTDESCRIPTION "Counter keeping track of events generated."
#define EVENTCOUNTTOOLTIP "Event count."

#define EVENTTIMENAME "EventSampleTime"
#define EVENTTIMEDESCRIPTION "String representation of the time when the event was generated."
#define EVENTTIMETOOLTIP "Event time."


