/*****************************************************************************/
/*
 *	trace.h -- Abstraction layer for internal logging system.
 *             Needed by SphinxLib and filter driver. Don't touch it.
 *
 *	(c) Copyright 2021 Sensor to Image GmbH
 *
 * Version: 1.0.0 Date: 13.01.2021
 *
 * History:
 * Version: 1.0.0 Date: 13.01.2021
 * - initial release 
*/
/*****************************************************************************/
#ifndef TRACE_GUARD
#define TRACE_GUARD
#define MEMENTO_GUARD

#ifndef TRACE_MACROS
#define TRACE_MACROS
#define M_CRITICAL "[CRITICAL] -"
#define M_ERROR "[ERROR] -"
#define M_WARNING "[WARNING] -"
#define M_NOTICE "[INFO] -"
#define M_INFO "[INFO] -"
#define M_DEBUG ""
#define M_VERBOSE ""
#endif

#ifndef TRACE_MAPPERS
#define TRACE_MAPPERS
#define TRACE_TO_PRINTFM_CRITICAL TRACE_TO_PRINTF
#define TRACE_TO_PRINTFM_ERROR TRACE_TO_PRINTF

#define TRACE_TO_PRINTFM_WARNING TRACE_TO_PRINTF
#define TRACE_TO_PRINTFM_NOTICE TRACE_TO_PRINTF
#define TRACE_TO_PRINTFM_INFO TRACE_TO_PRINTF
#define TRACE_TO_PRINTFM_VERBOSE TRACE_TO_PRINTFM_DEBUG

#define TRACEX_TO_PRINTFM_CRITICAL TRACEX_TO_PRINTF
#define TRACEX_TO_PRINTFM_ERROR TRACEX_TO_PRINTF
#define TRACEX_TO_PRINTFM_WARNING TRACEX_TO_PRINTF
#define TRACEX_TO_PRINTFM_NOTICE TRACEX_TO_PRINTF
#define TRACEX_TO_PRINTFM_INFO TRACEX_TO_PRINTF
#define TRACEX_TO_PRINTFM_VERBOSE TRACEX_TO_PRINTFM_DEBUG
#endif

#ifndef TRACE_EOL
#define TRACE_EOL ""
#endif

#define PROBE_START(kind, name)
#define PROBE_END(kind, name)
#define PROBE_RESET(kind, name)
#define PROBE_LEVEL(kind, name, val)
#define PROBE_NOLEVEL(kind, name)

#define TRACE_0(level, kind, format) TRACE_TO_PRINTF##level(level format TRACE_EOL)
#define TRACE_1(level, kind, format,a) TRACE_TO_PRINTF##level(level format TRACE_EOL,a)
#define TRACE_2(level, kind, format,a,b) TRACE_TO_PRINTF##level(level format TRACE_EOL,a,b)
#define TRACE_3(level, kind, format,a,b,c) TRACE_TO_PRINTF##level(level format TRACE_EOL,a,b,c)
#define TRACE_4(level, kind, format,a,b,c,d) TRACE_TO_PRINTF##level(level format TRACE_EOL,a,b,c,d)
#define TRACE_5(level, kind, format,a,b,c,d,e) TRACE_TO_PRINTF##level(level format TRACE_EOL,a,b,c,d,e)
#define TRACE_6(level, kind, format,a,b,c,d,e,f) TRACE_TO_PRINTF##level(level format TRACE_EOL,a,b,c,d,e,f)

// NOTE: TRACE_CONTEXT should be defined by files that will call these macros and evaluate to a number

#define TRACE_0x(level, kind, format) TRACEX_TO_PRINTF##level(TRACE_CONTEXT, TRACE_LOGLEVEL##level, level format TRACE_EOL)
#define TRACE_1x(level, kind, format,a) TRACEX_TO_PRINTF##level(TRACE_CONTEXT, TRACE_LOGLEVEL##level, level format TRACE_EOL,a)
#define TRACE_2x(level, kind, format,a,b) TRACEX_TO_PRINTF##level(TRACE_CONTEXT, TRACE_LOGLEVEL##level, level format TRACE_EOL,a,b)
#define TRACE_3x(level, kind, format,a,b,c) TRACEX_TO_PRINTF##level(TRACE_CONTEXT, TRACE_LOGLEVEL##level, level format TRACE_EOL,a,b,c)
#define TRACE_4x(level, kind, format,a,b,c,d) TRACEX_TO_PRINTF##level(TRACE_CONTEXT, TRACE_LOGLEVEL##level, level format TRACE_EOL,a,b,c,d)
#define TRACE_5x(level, kind, format,a,b,c,d,e) TRACEX_TO_PRINTF##level(TRACE_CONTEXT, TRACE_LOGLEVEL##level, level format TRACE_EOL,a,b,c,d,e)
#define TRACE_6x(level, kind, format,a,b,c,d,e,f) TRACEX_TO_PRINTF##level(TRACE_CONTEXT, TRACE_LOGLEVEL##level, level format TRACE_EOL,a,b,c,d,e,f)

#endif
