#ifndef __RENDERLIB_GPU_TEXTURE_GPUTEXTUREINSTANCE_H__
#define __RENDERLIB_GPU_TEXTURE_GPUTEXTUREINSTANCE_H__

#include "GPU/texture/GPUTexture.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Texture
		{
			class GPUTextureInstance
			{
			private:
				static float anysotropicFilterSupport;

				static void queryMaxAnysotropyFiltering();
			private:
				GPUTexture * texture;

				int minificationFilter;
				int magnificationFilter;
				
				int tComponentWrapType;
				int sComponentWrapType;
				int rComponentWrapType;

				float anysotropyLevel;
			public:
				GPUTextureInstance(GPUTexture * base);
				~GPUTextureInstance();

				void setMinMaxFilter(int minification = GL_NEAREST, int magnification = GL_NEAREST);
				void setWrapType(int wrapT = GL_REPEAT, int wrapS = GL_REPEAT, int wrapR = GL_REPEAT);

				void setAnisotropyLevel(float val = 0.0f);

				void bind();
			};
		}
	}
}

#endif