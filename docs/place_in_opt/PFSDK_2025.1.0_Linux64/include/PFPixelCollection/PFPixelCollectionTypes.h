#ifndef PFPIXELCOLLECTIONTYPES_H
#define PFPIXELCOLLECTIONTYPES_H

#include <cstdint>

namespace PFPixelCollectionTypes
{
  struct Color{
   uint8_t r,g,b;
  };
  struct Position{
    unsigned int x,y;
  };
}


#endif
