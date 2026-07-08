#ifndef PFIMAGEGENERATOR_H
#define PFIMAGEGENERATOR_H

#include "PFCameraLib.h"
#include "PFImage.h"
#include "PFPixelCollectionTypes.h"
#include "PFPixelCollection.h"

namespace pfcameralib
{
	
/** 
* \class PFImageGenerator 
*
* \brief This static helper class is used to generate a full image based on PFPixelCollection
*/
  class PFCAMERALIB_API PFImageGenerator
  {
  public:
  
  /**
   *  \brief Generates a PFImage
   *
   *   It will fill with specific color the parts where there are no pixels
   *
   *   \param fill The color to fill the image
   *   \param collection The PFPixelCollection to use to generate the Image
   */

   static PFImage generateImage(PFPixelCollectionTypes::Color fill,PFPixelCollection &collection);

   /**
    * @brief Fills an existing PFImage
    * it superimposes the collection on the existing image.
    * The image must be at least the size of the collection
    *
    * @param image The image to superImpose to
    * @param collection The collection to superimpose from
    * @return if the Superimposition is sucessfull returns true, false otherwise
    */
   static bool fillImage(PFImage &image,PFPixelCollection &collection);

   /**
    * @brief Fills an raw image pointer (8bit) only
    * It does not allocate the memory, make sure there is enough memory allocated
    * @param image The raw pointer to superimpose the collection, make sure there is enough memory allocated
    * @param collection The collection to superimpose from
    * @return if the Superimposition is sucessfull returns true, false otherwise
    */
   static bool fillImage(uint8_t *image,PFPixelCollection &collection);
  private:
    PFImageGenerator();
  };
}

#endif // PFIMAGEGENERATOR_H
