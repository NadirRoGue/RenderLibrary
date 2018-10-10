#include "GPU/texture/GPUTexture.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Texture
		{
			GPUTexture::GPUTexture()
				:id(-1)
			{
			}

			GPUTexture::~GPUTexture()
			{
			}

			void GPUTexture::generate()
			{
				glGenTextures(1, &id);
			}

			void GPUTexture::bind()
			{
				glBindTexture(getTexturType(), id);
			}

			void GPUTexture::unBind()
			{
				glBindTexture(getTexturType(), 0);
			}

			unsigned int GPUTexture::getId()
			{
				return id;
			}

			void GPUTexture::setSize(unsigned int width, unsigned int height, unsigned int depth)
			{
				this->width = width;
				this->height = height;
				this->depth = depth;
			}

			const unsigned int & GPUTexture::getWidth()
			{
				return width;
			}

			const unsigned int & GPUTexture::getHeight()
			{
				return height;
			}

			const unsigned int & GPUTexture::getDepth()
			{
				return depth;
			}

			GPUTextureConfig & GPUTexture::getConfig()
			{
				return config;
			}

			bool GPUTexture::isMutable()
			{
				return config.isMutable;
			}

			void GPUTexture::upload(void * data, unsigned int width, unsigned int height, unsigned int depth)
			{
				setSize(width, height, depth);

				if (id == -1)
				{
					generate();
				}

				bind();

				if (config.isMutable)
				{
					uploadMutable(data);
				}
				else
				{
					uploadInmutable(data);
				}

				GLenum type = getTexturType();
				glTexParameteri(type, GL_TEXTURE_MIN_FILTER, config.minificationFilter);
				glTexParameteri(type, GL_TEXTURE_MAG_FILTER, config.magnificationFilter);
				glTexParameteri(type, GL_TEXTURE_WRAP_T, config.tComponentWrapType);
				glTexParameteri(type, GL_TEXTURE_WRAP_S, config.sComponentWrapType);
				glTexParameteri(type, GL_TEXTURE_WRAP_R, config.rComponentWrapType);

				if (config.anysotropyLevel > 0.0f)
				{
					glTexParameterf(type, GL_TEXTURE_MAX_ANISOTROPY_EXT, config.anysotropyLevel);
				}
				
				if (config.generateMipMaps)
				{
					glGenerateMipmap(type);
				}

				unBind();
			}

			void GPUTexture::destroy()
			{
				glDeleteTextures(1, &id);
			}
		}
	}
}