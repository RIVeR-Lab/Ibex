#ifndef _POLARIZEDDEFS_H
#define _POLARIZEDDEFS_H
#include <stdint.h>

const uint32_t MAXVAL_u8 = 255;
const uint32_t MAXVAL_u10 = 1023;
const uint32_t MAXVAL_u12 = 4095;
const uint32_t MAXVAL_u16 = 65535;

#define PATH_BUFFER_LEN 512
#define ERROR_BUFFER_LEN 512

// Check available Pixel Types for Polarized Cameras on header "pfcPixelTypes.h" included in folder "pfcTypes"

typedef enum
{
	BGR = 0,
	BGRPacked,
	RGB,
	RGBPacked
} pfColorImageFormat;

typedef enum : uint16_t
{
	Red = 0,
	Green = 1,
	Blue = 2,
	Mono = 0,

}PFPolarizedColor;

typedef enum : uint16_t
{
	deg_90 = 0,
	deg_45 = 1,
	deg_135 = 2,
	deg_0 = 3,

}PFPolarizedAngle;

#endif