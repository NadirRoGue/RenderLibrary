#include "GPU/texture/GPUTexture.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Texture
		{
			class GPUTexture3D : public GPUTexture
			{
			public:
				GPUTexture3D();
				~GPUTexture3D();

				GLenum getTexturType();

				void uploadMutable(void * data);
				void uploadInmutable(void * data);
			};
		}
	}
}