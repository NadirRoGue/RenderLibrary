#include "GPU/texture/GPUTexture2D.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Texture
		{
			GPUTexture2D::GPUTexture2D()
				:GPUTexture()
			{

			}

			GPUTexture2D::~GPUTexture2D()
			{

			}

			GLenum GPUTexture2D::getTexturType()
			{
				return GL_TEXTURE_2D;
			}

			void GPUTexture2D::uploadMutable(void * data)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, config.internalFormat, width, height, 0, config.format, config.pixelType, data);
			}

			void GPUTexture2D::uploadInmutable(void * data)
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, config.internalFormat, width, height);
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, config.format, config.pixelType, data);
			}
		}
	}
}