#include "render/renderstages/ForwardRenderStage.h"

#include <GL/glew.h>

#include "EngineInstance.h"

namespace RenderLib
{
	namespace Render
	{
		bool ForwardRenderStage::shouldRegisterRenderable(DefaultImpl::MeshRenderer * renderable)
		{
			Material::Material * mat = renderable->material;
			if (mat->opacity() < (FLOAT)1.0 || renderable->preferredRender == DefaultImpl::FORWARD_RENDER)
			{
				return true;
			}

			return false;
		}

		void ForwardRenderStage::runStage()
		{
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
			
			Camera * cam = engineInstance->getSceneManager().getActiveScene()->getActiveCamera();

			GPU::Mesh::GPUBuffer * staticBuf = engineInstance->getGPUMeshManager().getStaticMeshBuffer();
			staticBuf->bind();
			staticBuf->bindDataBuffer();
			staticRenderables.renderMap(*cam);

			GPU::Mesh::GPUBuffer * dynamicBuf = engineInstance->getGPUMeshManager().getDynamicMeshBuffer();
			dynamicBuf->bind();
			dynamicBuf->bindDataBuffer();
			dynamicRenderables.renderMap(*cam);
		}
	}
}