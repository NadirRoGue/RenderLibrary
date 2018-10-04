#ifndef __RENDERLIB_CPU_TEXTURE_TEXTUREBLENDOPERATION_H__
#define __RENDERLIB_CPU_TEXTURE_TEXTUREBLENDOPERATION_H__

#include "CPU/texture/Texture.h"

namespace RenderLib
{
	namespace CPU
	{
		namespace Texture
		{
			class TextureBlendOperation
			{
			public:
				virtual void doBlend(std::vector<unsigned char> & firstAndDst, std::vector<unsigned char> & second, float blendStrength) = 0;
			};
		}
	}
}

#endif