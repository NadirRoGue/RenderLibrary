#ifndef __RENDERLIB_RENDER_FBO_H__
#define __RENDERLIB_RENDER_FBO_H__

#include <vector>
#include <unordered_map>
#include <string>

#include "GPU/texture/GPUTexture.h"

namespace RenderLib
{
	class EngineInstance;

	namespace Render
	{
		typedef struct AttachmentInfo
		{
			GPU::Texture::GPUTexture * attachment;
			GLenum attachmentType;
		} AttachmentInfo;

		class FBO
		{
		private:
			static unsigned int MAX_RENDER_TARGETS;
		public:
			static FBO DEFAULTFRAMEBUFFER;
		public:
			static void queryMaxRenderTargets();
			static void initializeDefaultFBO();
		private:
			std::unordered_map<std::string, GPU::Texture::GPUTexture *> attachmentTextures;
			std::vector<AttachmentInfo> indexedAttachments;
			EngineInstance * accessor;
			unsigned int id;
			AttachmentInfo depthAttachment;
			unsigned int width, height;
		public:
			FBO();
			~FBO();

			void setEngineInstance(EngineInstance * instance);

			void generate();
			void bind();
			void unBind();
			void setSize(
				const unsigned int & width, 
				const unsigned int & height);

			const unsigned int &
				getWidth();
			const unsigned int &
				getHeight();

			GPU::Texture::GPUTexture * addColorAttachment(
				const unsigned int & index, 
				const std::string & name,
				GLenum internalFormat,
				GLenum format,
				GLenum pixelType);

			GPU::Texture::GPUTexture * addDepthAttachment(
				const std::string & name);

			GPU::Texture::GPUTexture * getAttachment(
				const std::string & name);

			GPU::Texture::GPUTexture * getDepthAttachment();

			void destroy();
		};
	}
}

#endif