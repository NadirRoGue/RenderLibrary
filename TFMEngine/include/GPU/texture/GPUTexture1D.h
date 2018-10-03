#include "GPU/texture/GPUTexture.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Texture
		{
			class GPUTexture1D : public GPUTexture
			{
			public:
				GPUTexture1D();
				~GPUTexture1D();

				GLenum getTexturType();

				void uploadMutable(void * data);
				void uploadInmutable(void * data);
			};
		}
	}
}