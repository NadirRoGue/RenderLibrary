#include "CPU/texture/TextureLoadResult.h"

namespace RenderLib
{
  namespace CPU
  {
    namespace Texture
    {
      void
      TextureLoadResult::resizeBuffer()
      {
        loadedPixels.resize(width * height * numPixelComponents);
      }

      size_t
      TextureLoadResult::getResultSizeBytes()
      {
        return sizeof(unsigned char) * width * height * numPixelComponents;
      }
    } // namespace Texture
  } // namespace CPU
} // namespace RenderLib