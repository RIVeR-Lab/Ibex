/* s2i_config.h -- Configurable items
 *
 * (C) 2021 by Sensor to Image GmbH
 *
 * Version: 1.0.0 Date: 28.10.2021
 *
 * History:
 * Version: 1.0.0 Date: 28.10.2021
 * - initial release
 */
#ifndef __S2I_CONFIG_H
#define __S2I_CONFIG_H

#define CAMERA_COUNT 50
#define CAMERA_COUNT_DISCOVERY 255

#ifdef WIN32
#define CAMERA_COUNT_INIT {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#else
#define CAMERA_COUNT_INIT {}
#endif

#define DETAILED_LOG_OFF       0
#define DETAILED_LOG_INFO      1
#define DETAILED_LOG_WARNING   2
#define DETAILED_LOG_ERROR     4
#define DETAILED_LOG_REGISTER  8
#define DETAILED_LOG_DEBUG     16
#define DETAILED_LOG_VERBOSE   32

#endif
