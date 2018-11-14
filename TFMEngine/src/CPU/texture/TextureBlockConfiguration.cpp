#include "CPU/texture/TextureBlockConfiguration.h"

#include <algorithm>

namespace RenderLib
{
  namespace CPU
  {
    namespace Texture
    {
      size_t
      TextureBlockConfiguration::getSizeBytes()
      {
        return std::max<unsigned int>(width, 1)
            * std::max<unsigned int>(height, 1)
            * std::max<unsigned int>(depth, 1) * pixelSize;
      }
    } // namespace Texture
  } // namespace CPU
} // namespace RenderLib