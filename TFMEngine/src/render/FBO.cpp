#include "render/FBO.h"

#include "GPU/texture/GPUTexture2D.h"

namespace RenderLib
{
	namespace Render
	{
		FBO FBO::DEFAULTFRAMEBUFFER;
		unsigned int FBO::MAX_RENDER_TARGETS = 0;

		void FBO::initializeDefaultFBO()
		{
			DEFAULTFRAMEBUFFER.id = 0;
		}

		void FBO::queryMaxRenderTargets()
		{
			if (MAX_RENDER_TARGETS == 0)
			{
				GLint maxAttach = 0;
				glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxAttach);

				GLint maxDrawBuf = 0;
				glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxDrawBuf);

				MAX_RENDER_TARGETS = std::min<int>(maxAttach, maxDrawBuf);
			}
		}

		FBO::FBO()
			: id(-1)
			, width(0)
			, height(0)
		{
			indexedAttachments.resize(MAX_RENDER_TARGETS);
			for (auto & info : indexedAttachments)
			{
				info.attachment = NULL;
			}
		}

		FBO::~FBO()
		{
		}

		void FBO::generate()
		{

		}

		void FBO::setEngineInstance(EngineInstance * instance)
		{
			accessor = instance;
		}

		GPU::Texture::GPUTexture * FBO::addColorAttachment(const unsigned int & index, const std::string & name)
		{
			if (index > MAX_RENDER_TARGETS)
			{
				throw EngineException("FBO: Adding color attachment failed. Maximun attachments are " + std::to_string(MAX_RENDER_TARGETS));
				return NULL;
			}

			GPU::Texture::GPUTexture * colorAttachment = 
				accessor->getGPUTextureManager().createGPUOnlyTexture<GPU::Texture::GPUTexture2D>(name);

			attachmentTextures[name] = colorAttachment;

			AttachmentInfo info;
			info.attachment = colorAttachment;
			info.attachmentType = GL_COLOR_ATTACHMENT0 + index;

			indexedAttachments[index] = info;

			return colorAttachment;
		}

		GPU::Texture::GPUTexture * FBO::addDepthAttachment(const std::string & name)
		{
			depthAttachment.attachment = accessor->getGPUTextureManager()
				.createGPUOnlyTexture<GPU::Texture::GPUTexture2D>(name);
			depthAttachment.attachmentType = GL_DEPTH_ATTACHMENT;

			attachmentTextures[name] = depthAttachment.attachment;

			return depthAttachment.attachment;
		}

		GPU::Texture::GPUTexture * FBO::getAttachment(const std::string & name)
		{
			auto it = attachmentTextures.find(name);
			if (it != attachmentTextures.end())
			{
				return it->second;
			}

			return NULL;
		}

		GPU::Texture::GPUTexture * FBO::getDepthAttachment()
		{
			return depthAttachment.attachment;
		}

		void FBO::bind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, id);
		}

		void FBO::unBind()
		{
			DEFAULTFRAMEBUFFER.bind();
		}

		void FBO::setSize(const unsigned int & width, const unsigned int & height)
		{
			this->width = width;
			this->height = height;
		
			for (auto & info : indexedAttachments)
			{
				if (info.attachment != NULL)
				{
					info.attachment->upload(NULL, width, height);
				}
			}

			bool addDepthBuffer = false;
			if (depthAttachment.attachment == NULL)
			{
				depthAttachment.attachment = accessor->getGPUTextureManager()
					.createGPUOnlyTexture<GPU::Texture::GPUTexture2D>("depthattachment_FBO_UNUSED_"+std::to_string(id));
				depthAttachment.attachmentType = GL_DEPTH_ATTACHMENT;
			}
			else
			{
				addDepthBuffer = true;
			}

			depthAttachment.attachment->upload(NULL, width, height);

			bind();

			std::vector<GLenum> attachmentList;
			for (auto & info : indexedAttachments)
			{
				if (info.attachment != NULL)
				{
					glFramebufferTexture2D(GL_FRAMEBUFFER, info.attachmentType, GL_TEXTURE_2D, info.attachment->getId(), 0);
					attachmentList.push_back(info.attachmentType);
				}
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthAttachment.attachment->getId(), 0);

			if (attachmentList.size() > 0)
			{
				glDrawBuffers((GLsizei)attachmentList.size(), &attachmentList[0]);
			}
			else
			{
				glDrawBuffer(GL_NONE);
			}

			if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
			{
				exit(-1);
			}

			unBind();
		}

		void FBO::destroy()
		{
			glDeleteFramebuffers(1, &id);
		}
	}
}