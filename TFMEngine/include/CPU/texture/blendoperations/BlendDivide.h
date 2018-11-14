#ifndef __RENDERLIB_CPU_TEXTURE_BLENDDIVIDE_H__
#define __RENDERLIB_CPU_TEXTURE_BLENDDIVIDE_H__

#include "CPU/texture/TextureBlendOperation.h"

namespace RenderLib
{
  namespace CPU
  {
    namespace Texture
    {
      class BlendDivide : public TextureBlendOperation
      {
      public:
        void
        doBlend(std::vector<unsigned char> & firstAndDst,
                std::vector<unsigned char> & second, float blendStrength);
      };
    } // namespace Texture
  } // namespace CPU
} // namespace RenderLib

#endif
