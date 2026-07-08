#ifndef PFSLAFACTORY_H
#define PFSLAFACTORY_H

#include "PFPixelCollection.h"
#include "PFPixelCollectionTypes.h"
#include "PFPixelCollectionLib.h"
#include <vector>

class PFPixelCollectionLib_API PFPixelCollectionFactory
{
public:
//  PFPixelCollectionFactory() = delete;
  static PFPixelCollection createPFSLA(const std::vector<std::pair<PFPixelCollectionTypes::Position, PFPixelCollectionTypes::Color> > &data, unsigned int width, unsigned int max, unsigned int min);
private:
  PFPixelCollectionFactory();
};

#endif // PFSLAFACTORY_H
