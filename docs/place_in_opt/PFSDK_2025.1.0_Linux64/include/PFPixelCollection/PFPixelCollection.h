#ifndef PFSLA_H
#define PFSLA_H

#include "PFPixelCollectionLib.h"
#include <memory>
#include "PFPixelCollectionTypes.h"
#include <vector>
#include <stdexcept>
//#include <PFImage.h>


class PFPixelCollectionFactory;
class PFPixelCollectionPrivate;

/**
 * @brief The PFPixelCollection class
 * A collection of Pixels, each pixel is represented by it's value and it's X and Y position.
 * It may or may not prepresent consecutive pixels, a collection of sparse pixels.
 * Access is done via iterators
 */

class PFPixelCollectionLib_API PFPixelCollection
{
public:

  typedef std::vector<std::pair<PFPixelCollectionTypes::Position,PFPixelCollectionTypes::Color>> container;
  typedef container::iterator iterator;

  PFPixelCollection();
  ~PFPixelCollection();
  PFPixelCollection(PFPixelCollection &other); //copy constructor
  PFPixelCollection(PFPixelCollection &&other); //Move

  /**
   * @brief size, get base image sie
   * @return
   */
  size_t size();
  /**
   * @brief getmin, get SLA min height
   * @return
   */
  uint32_t getmin();
  /**
   * @brief getmax get sla max height
   * @return
   */
  uint32_t getmax();

  /**
   * @brief width get total width
   * @return
   */
  uint32_t width();


  /**
   * @brief begin , iterator begin
   * @return
   */
  iterator begin();
  /**
   * @brief end. interator end
   * @return
   */
  iterator end();
//  PFCameraDLL::PFImage generateImage(pf3DLib::Color fill);

  /**
   * @brief deepcopy , creates a new copy of the collection
   * @return the new PFPixelCollection copy
   */
  PFPixelCollection deepcopy();
  PFPixelCollection& operator=(PFPixelCollection &&other);
private:
  friend PFPixelCollectionFactory;
#if _MSC_VER
  #pragma warning( push )
  #pragma warning( disable : 4251 )
#endif
  std::unique_ptr<PFPixelCollectionPrivate> d;
  #if _MSC_VER
  #pragma warning( pop )
#endif
};



#endif // PFSLA_H
