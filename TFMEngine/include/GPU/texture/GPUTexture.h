#ifndef __RENDERLIB_GPU_TEXTURE_TEXTURE_H__
#define __RENDERLIB_GPU_TEXTURE_TEXTURE_H__

#include <GL/glew.h>
#include <cstdlib>

namespace RenderLib
{
	namespace GPU
	{
		namespace Texture
		{
			class GPUTexture
			{
			protected:
				unsigned int id;
				int internalFormat;	// How do we want to store the texture in the gpu memory?
				GLenum format;		// Whats the format of the image we got form disk (what does each element we read mean)?
				GLenum pixelType;	// Hows each element of the texture codified? (3 floats, 1 unsigned int, 1 unsigned char,...)
				bool generateMipMaps;

				bool mutableTexture;

				unsigned int width, height, depth;
			public:
				size_t index;
			public:
				GPUTexture();
				~GPUTexture();

				virtual GLenum getTexturType() = 0;

				void generate();
				void bind();
				void unBind();

				void setSize(unsigned int width, unsigned int height = 1, unsigned int depth = 1);
				const unsigned int & getWidth();
				const unsigned int & getHeight();
				const unsigned int & getDepth();

				void setIsMutable(bool isMutable);
				bool isMutable();

				void setInternalFormat(const int & internalFormat);
				void setDataFormat(const GLenum & format);
				void setPixelFormat(const GLenum & pixelFormat);
				
				void upload(void * data, unsigned int width, unsigned int height = 1, unsigned int depth = 1);

				void destroy();
			protected:
				virtual void uploadMutable(void * data) = 0;
				virtual void uploadInmutable(void * data) = 0;
			};
		}
	}
}

#endif