#include "GPU/texture/GPUTexture3D.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Texture
		{
			GPUTexture3D::GPUTexture3D()
				:GPUTexture()
			{

			}

			GPUTexture3D::~GPUTexture3D()
			{

			}

			GLenum GPUTexture3D::getTexturType()
			{
				return GL_TEXTURE_3D;
			}

			void GPUTexture3D::uploadMutable(void * data)
			{
				glTexImage3D(GL_TEXTURE_3D, 0, internalFormat, width, height, depth, 0, format, pixelType, data);
			}

			void GPUTexture3D::uploadInmutable(void * data)
			{
				glTexStorage3D(GL_TEXTURE_3D, 1, internalFormat, width, height, depth);
				glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, width, height, depth, format, pixelType, data);
			}
		}
	}
}