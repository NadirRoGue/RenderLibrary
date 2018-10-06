#ifndef __RENDERLIB_CPU_TEXTURE_BLENDADD_H__
#define __RENDERLIB_CPU_TEXTURE_BLENDADD_H__

#include "CPU/texture/TextureBlendOperation.h"

namespace RenderLib
{
	namespace CPU
	{
		namespace Texture
		{
			class BlendAdd : public TextureBlendOperation
			{
			public:
				void doBlend(
					std::vector<unsigned char> & firstAndDst,
					std::vector<unsigned char> & second,
					float blendStrength);
			};
		}
	}
}

#endif