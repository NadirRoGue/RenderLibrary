#include "render/renderstages/DeferredRenderStage.h"

namespace RenderLib
{
	namespace Render
	{
		DeferredRenderStage::DeferredRenderStage()
		{

		}

		void DeferredRenderStage::initialize()
		{
			MeshRenderingStage::initialize();

			// Create g-buffers
			outputFBO = engineInstance->getFBOManager().createFBO("GBUFFER");

			// Set up pos and specular scale target
			GPU::Texture::GPUTexture * posAndSpecScale = outputFBO->addColorAttachment(0, "positionAndSpecScale");
			GPU::Texture::GPUTextureConfig & posConfig = posAndSpecScale->getConfig();
			posConfig.internalFormat = GL_RGB32F;


		}

		bool DeferredRenderStage::shouldRegisterRenderable(DefaultImpl::MeshRenderer * renderable)
		{

		}

		void DeferredRenderStage::runStage()
		{

		}
	}
}