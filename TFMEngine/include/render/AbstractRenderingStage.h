#ifndef __RENDERLIB_RENDER_ABSTRACTRENDERINGSTAGE_H__
#define __RENDERLIB_RENDER_ABSTRACTRENDERINGSTAGE_H__

#include <memory>

#include "defaultimpl/components/MeshRenderer.h"

#include "GPU/program/PostProcessProgram.h"

#include "GPU/mesh/GPUMesh.h"

#include "render/RenderableMap.h"
#include "render/FBO.h"

namespace RenderLib
{
	class EngineInstance;

	namespace Render
	{
		class AbstractRenderingStage
		{
		public:
			EngineInstance * engineInstance;
		protected:
			FBO * outputFBO;
		public:
			AbstractRenderingStage();
			virtual void initialize();
			virtual void runStage() = 0;
			virtual void finalize();
			virtual void tryRegisterElement(DefaultImpl::MeshRenderer * renderable);
			virtual void forceRegisterRenderable(DefaultImpl::MeshRenderer * renderable);
			void setTargetFBO(FBO * fbo);
		};

		typedef std::unique_ptr<AbstractRenderingStage> AbstractRenderingStagePtr;

		class MeshRenderingStage : public AbstractRenderingStage
		{
		protected:
			RenderableMap staticRenderables;
			RenderableMap dynamicRenderables;
		public:
			virtual void initialize();
			void tryRegisterElement(DefaultImpl::MeshRenderer * renderable);
			void forceRegisterRenderable(DefaultImpl::MeshRenderer * renderable);

			virtual bool shouldRegisterRenderable(DefaultImpl::MeshRenderer * renderable) = 0;
		};

		template<class T>
		class PostProcessRenderStage : public AbstractRenderingStage
		{
		protected:
			GPU::Program::PostProcessProgram * postProcessProgram;
		public:
			PostProcessRenderStage()
			{

			}

			virtual void initialize()
			{
				postProcessProgram = engineInstance->getProgramManager()
					.getProgram<T>(0);

				postProcessProgram->initializeShader(engineInstance);

				GPU::Mesh::GPUMesh * quad =
					engineInstance->getGPUMeshManager().getPostProcessQuad();

				GPU::Mesh::GPUBuffer * buffer = engineInstance->getGPUMeshManager()
					.getPostProcessQuadBuffer();

				buffer->bind();
				buffer->bindDataBuffer();
				postProcessProgram->configureRenderQuad(quad);
				buffer->unBindDataBuffer();
				buffer->unBind();
			}

			virtual void runStage()
			{
				if (!outputFBO)
				{
					throw EngineException("DeferredRenderStage: No FrameBuffer setted. Aborting");
				}

				outputFBO->bind();

				GPU::Mesh::GPUBuffer * buffer = engineInstance->getGPUMeshManager()
					.getPostProcessQuadBuffer();

				buffer->bind();
				postProcessProgram->bind();

				postProcessProgram->onRender(engineInstance);

				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
			}
		};
	}
}

#endif