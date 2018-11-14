#ifndef __RENDERLIB_CPU_TEXTURE_TEXTURE_H__
#define __RENDERLIB_CPU_TEXTURE_TEXTURE_H__

#include "CPU/memory/PoolAttribute.hpp"

namespace RenderLib
{
  namespace CPU
  {
    namespace Texture
    {
      enum class TextureType
      {
        TEXTURE1D,
        TEXTURE2D,
        TEXTURE3D
      };

      class Texture
      {
      public:
        Memory::ByteAttribute pixels;
        unsigned int width, height, depth;

        TextureType type;

        size_t index;

      public:
        Texture();
        ~Texture();

        /**
   * Returns a memory pool attribute that allows to read the texture
   * as wished
   */
        template <class T>
        Memory::PoolAttribute<T, 1>
        getTextureReader()
        {
          Memory::PoolAttribute<T, 1> reader;
          reader.setAttributeSource(pixels.getMemorySource(),
                                    pixels.getOffset(), sizeof(T),
                                    pixels.size());

          return reader;
        }
      };
    } // namespace Texture
  } // namespace CPU
} // namespace RenderLib

#endif
