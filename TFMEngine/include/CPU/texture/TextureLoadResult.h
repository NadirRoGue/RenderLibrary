#ifndef __RENDERLIB_CPU_TEXTURE_TEXTURELOADRESULT_H__
#define __RENDERLIB_CPU_TEXTURE_TEXTURELOADRESULT_H__

#include <vector>

#include "CPU/io/FileLoadResult.h"

namespace RenderLib
{
  namespace CPU
  {
    namespace Texture
    {
      class TextureLoadResult : public CPU::IO::AbstractLoadResult
      {
      public:
        std::vector<unsigned char> loadedPixels;
        unsigned int width;
        unsigned int height;
        unsigned int numPixelComponents; // RGBA = 4, RGB = 3,...

        void
        resizeBuffer();

        size_t
        getResultSizeBytes();
      };
    } // namespace Texture
  } // namespace CPU
} // namespace RenderLib

#endif
