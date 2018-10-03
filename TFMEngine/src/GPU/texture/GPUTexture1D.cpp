#include "GPU/texture/GPUTexture1D.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Texture
		{
			GPUTexture1D::GPUTexture1D()
				:GPUTexture()
			{

			}

			GPUTexture1D::~GPUTexture1D()
			{

			}

			GLenum GPUTexture1D::getTexturType()
			{
				return GL_TEXTURE_1D;
			}

			void GPUTexture1D::uploadMutable(void * data)
			{
				glTexImage1D(GL_TEXTURE_1D, 0, internalFormat, width, 0, format, pixelType, data);
			}

			void GPUTexture1D::uploadInmutable(void * data)
			{
				glTexStorage1D(GL_TEXTURE_1D, 1, internalFormat, width);
				glTexSubImage1D(GL_TEXTURE_1D, 0, 0, width, format, pixelType, data);
			}
		}
	}
}