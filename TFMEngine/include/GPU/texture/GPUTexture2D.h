#include "GPU/texture/GPUTexture.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Texture
		{
			class GPUTexture2D : public GPUTexture
			{
			public:
				GPUTexture2D();
				~GPUTexture2D();

				GLenum getTexturType();

				void uploadMutable(void * data);
				void uploadInmutable(void * data);
			};
		}
	}
}