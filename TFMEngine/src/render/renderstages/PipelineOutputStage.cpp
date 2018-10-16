#include "render/renderstages/PipelineOutputStage.h"

namespace RenderLib
{
	namespace Render
	{
		void PipelineOutputStage::initialize()
		{
			// Initialize screen quad
			buffer = engineInstance->getGPUMeshManager()
				.getPostProcessQuadBuffer();

			// Initialize post process shader
			postProcessProgram = engineInstance->getProgramManager()
				.getProgram<DefaultImpl::OutputProgram>(0);
			postProcessProgram->initializeShader(engineInstance);

			// Configure everything
			GPU::Mesh::GPUMesh * quad =
				engineInstance->getGPUMeshManager().getPostProcessQuad();

			buffer->bind();
			buffer->bindDataBuffer();
			postProcessProgram->configureRenderQuad(quad);
			buffer->unBindDataBuffer();
			buffer->unBind();

			outputFBO = &(FBO::DEFAULTFRAMEBUFFER);
		}
	}
}