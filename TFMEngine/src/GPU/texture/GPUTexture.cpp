#include "GPU/texture/GPUTexture.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Texture
		{
			GPUTexture::GPUTexture()
				:id(-1)
				, mutableTexture(false)
				, generateMipMaps(false)
				, internalFormat(GL_RGBA8)
				, format(GL_RGBA)
				, pixelType(GL_UNSIGNED_BYTE)
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

			void GPUTexture::setIsMutable(bool mutableT)
			{
				mutableTexture = mutableT;
			}

			bool GPUTexture::isMutable()
			{
				return mutableTexture;
			}

			void GPUTexture::setInternalFormat(const int & internalFormat)
			{
				this->internalFormat = internalFormat;
			}

			void GPUTexture::setDataFormat(const GLenum & format)
			{
				this->format = format;
			}

			void GPUTexture::setPixelFormat(const GLenum & pixelFormat)
			{
				this->pixelType = pixelFormat;
			}

			void GPUTexture::upload(void * data, unsigned int width, unsigned int height, unsigned int depth)
			{
				setSize(width, height, depth);

				if (id == -1)
				{
					generate();
				}

				bind();

				if (mutableTexture)
				{
					uploadMutable(data);
				}
				else
				{
					uploadInmutable(data);
				}

				if (generateMipMaps)
				{
					glGenerateMipmap(getTexturType());
				}
			}

			void GPUTexture::destroy()
			{
				glDeleteTextures(1, &id);
			}
		}
	}
}