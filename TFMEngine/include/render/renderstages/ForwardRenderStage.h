#ifndef __RENDERLIB_RENDER_FORWARDRENDERSTAGE_H__
#define __RENDERLIB_RENDER_FORWARDRENDERSTAGE_H__

#include "render/AbstractRenderingStage.h"

#include "GPU/mesh/GPUBuffer.h"

#include "GPU/texture/GPUTextureManager.h"

#include <vector>

namespace RenderLib
{
	namespace Render
	{
		class ForwardRenderStage : public AbstractRenderingStage
		{
		private:
			std::vector<DefaultImpl::MeshRenderer*> staticRenderables;
			std::vector<DefaultImpl::MeshRenderer*> dynamicRenderables;

			GPU::Texture::GPUTextureManager manager;
			GPU::Texture::GPUTexture * texture;

			unsigned int programId;
			unsigned int mvp;
			unsigned int apos;
		public:
			void initialize();
			void tryRegisterElement(DefaultImpl::MeshRenderer * renderable);
			void runStage();
		private:
			void doRender(std::vector<DefaultImpl::MeshRenderer*> & renderables, GPU::Mesh::GPUBuffer * meshBuffer);
		};
	}
}

#endif