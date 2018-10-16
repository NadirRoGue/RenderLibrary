#include "render/AbstractRenderingStage.h"

#include "EngineInstance.h"

namespace RenderLib
{
	namespace Render
	{
		AbstractRenderingStage::AbstractRenderingStage()
			: outputFBO(NULL)
			, prevStage(NULL)
			, nextStage(NULL)
		{

		}

		GPU::Texture::GPUTexture * AbstractRenderingStage::getStageOutput()
		{
			return finalOutput;
		}

		void AbstractRenderingStage::initialize()
		{
		}

		void AbstractRenderingStage::finalize()
		{

		}

		void AbstractRenderingStage::tryRegisterElement(DefaultImpl::MeshRenderer * renderable)
		{

		}

		void AbstractRenderingStage::forceRegisterRenderable(DefaultImpl::MeshRenderer * renderable)
		{

		}

		void AbstractRenderingStage::setTargetFBO(FBO * fbo)
		{
			outputFBO = fbo;
		}

		// =============================================================================================

		RenderableMap & MeshRenderingStage::getStaticRenderables()
		{
			return staticRenderables;
		}

		RenderableMap & MeshRenderingStage::getDynamicRenderables()
		{
			return dynamicRenderables;
		}

		void MeshRenderingStage::initialize()
		{
			const std::string name = std::to_string(engineInstance->getInstanceID()) + "_coloroutput";
			outputFBO = engineInstance->getFBOManager().
				getFBO(name);
			if (outputFBO == NULL)
			{
				outputFBO = engineInstance->getFBOManager().
					createFBO(name);
				finalOutput = outputFBO->addColorAttachment(
					0,
					name,
					GL_RGBA32F,
					GL_RGBA,
					GL_FLOAT);
				outputFBO->setSize(500, 500);
			}
			else
			{
				finalOutput = outputFBO->getAttachment(name);
			}

			// Static meshes
			GPU::Mesh::GPUBuffer * staticBuffer = engineInstance->getGPUMeshManager().getStaticMeshBuffer();
			staticBuffer->bind();
			staticBuffer->bindDataBuffer();
			staticRenderables.initializeMap();
			staticBuffer->unBindDataBuffer();
			staticBuffer->unBind();

			// Dynamic meshes buffer 1
			GPU::Mesh::GPUBuffer * dynamicBuffer = engineInstance->getGPUMeshManager().getDynamicMeshBuffer();
			dynamicBuffer->bind();
			dynamicBuffer->bindDataBuffer();
			dynamicRenderables.initializeMap();
			dynamicBuffer->unBindDataBuffer();
			dynamicBuffer->unBind();

			engineInstance->getGPUMeshManager().swapDynamicBuffers();

			// Dynamic meshes buffer 2
			dynamicBuffer = engineInstance->getGPUMeshManager().getDynamicMeshBuffer();
			dynamicBuffer->bind();
			dynamicBuffer->bindDataBuffer();
			dynamicRenderables.initializeMap();
			dynamicBuffer->unBindDataBuffer();
			dynamicBuffer->unBind();
		}

		void MeshRenderingStage::tryRegisterElement(DefaultImpl::MeshRenderer * renderable)
		{
			if (shouldRegisterRenderable(renderable))
			{
				if (renderable->cpuToGpuSync == DefaultImpl::CPU_SYNC_CONTINOUSLY
					|| renderable->gpuToCpuSync == DefaultImpl::GPU_SYNC_CONTINOUSLY)
				{
					dynamicRenderables.addRenderable(engineInstance->getProgramManager(), renderable);
				}
				else
				{
					staticRenderables.addRenderable(engineInstance->getProgramManager(), renderable);
				}
			}
		}

		void MeshRenderingStage::forceRegisterRenderable(DefaultImpl::MeshRenderer * renderable)
		{
			if (renderable->cpuToGpuSync == DefaultImpl::CPU_SYNC_CONTINOUSLY
				|| renderable->gpuToCpuSync == DefaultImpl::GPU_SYNC_CONTINOUSLY)
			{
				dynamicRenderables.addRenderable(engineInstance->getProgramManager(), renderable);
			}
			else
			{
				staticRenderables.addRenderable(engineInstance->getProgramManager(), renderable);
			}
		}
	}
}