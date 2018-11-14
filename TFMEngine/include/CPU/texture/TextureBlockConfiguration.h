#ifndef __RENDERLIB_CPU_TEXTURE_TEXTUREBLOCKCONFIGURATION_H__
#define __RENDERLIB_CPU_TEXTURE_TEXTUREBLOCKCONFIGURATION_H__

#include "CPU/memory/PoolObjectBuilder.h"

namespace RenderLib
{
  namespace CPU
  {
    namespace Texture
    {
      class TextureBlockConfiguration : public Memory::BlockConfiguration
      {
      public:
        unsigned int width;
        unsigned int height;
        unsigned int depth;

        unsigned int pixelSize;

        size_t
        getSizeBytes();
      };
    } // namespace Texture
  } // namespace CPU
} // namespace RenderLib

#endif
