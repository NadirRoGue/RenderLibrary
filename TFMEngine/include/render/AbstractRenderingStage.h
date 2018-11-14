#ifndef __RENDERLIB_RENDER_ABSTRACTRENDERINGSTAGE_H__
#define __RENDERLIB_RENDER_ABSTRACTRENDERINGSTAGE_H__

#include <memory>

#include "defaultimpl/components/MeshRenderer.h"

#include "GPU/program/PostProcessProgram.h"

#include "GPU/mesh/GPUMesh.h"

#include "render/FBO.h"
#include "render/RenderableMap.h"

namespace RenderLib
{
  class EngineInstance;

  namespace Render
  {
    class AbstractRenderingStage
    {
    public:
      EngineInstance * engineInstance;
      AbstractRenderingStage * prevStage;
      AbstractRenderingStage * nextStage;

    protected:
      FBO * outputFBO;
      GPU::Texture::GPUTexture * finalOutput;

    public:
      AbstractRenderingStage();

      GPU::Texture::GPUTexture *
      getStageOutput();

      virtual void
      initialize();
      virtual void
      runStage()
          = 0;
      virtual void
      finalize();
      virtual void
      tryRegisterElement(DefaultImpl::MeshRenderer * renderable);
      virtual void
      forceRegisterRenderable(DefaultImpl::MeshRenderer * renderable);
      void
      setTargetFBO(FBO * fbo);
    };

    typedef std::unique_ptr<AbstractRenderingStage> AbstractRenderingStagePtr;

    class MeshRenderingStage : public AbstractRenderingStage
    {
    protected:
      RenderableMap staticRenderables;
      RenderableMap dynamicRenderables;

    public:
      virtual void
      initialize();
      void
      tryRegisterElement(DefaultImpl::MeshRenderer * renderable);
      void
      forceRegisterRenderable(DefaultImpl::MeshRenderer * renderable);

      RenderableMap &
      getStaticRenderables();
      RenderableMap &
      getDynamicRenderables();

      virtual bool
      shouldRegisterRenderable(DefaultImpl::MeshRenderer * renderable)
          = 0;
    };

    template <class T>
    class PostProcessRenderStage : public AbstractRenderingStage
    {
    protected:
      GPU::Program::PostProcessProgram * postProcessProgram;
      GPU::Mesh::GPUBuffer * buffer;

    public:
      PostProcessRenderStage()
      {
      }

      virtual void
      initialize()
      {
        // Initialize screen quad
        buffer = engineInstance->getGPUMeshManager().getPostProcessQuadBuffer();

        // Initialize output FBO
        const std::string name = std::string(typeid(*this).name()) + "_output";
        outputFBO              = engineInstance->getFBOManager().getFBO(name);
        if (outputFBO == NULL)
        {
          outputFBO   = engineInstance->getFBOManager().createFBO(name);
          finalOutput = outputFBO->addColorAttachment(0, name, GL_RGBA32F,
                                                      GL_RGBA, GL_FLOAT);
          outputFBO->setSize(500, 500);
        }
        else
        {
          finalOutput = outputFBO->getAttachment(name);
        }

        // Initialize post process shader
        postProcessProgram
            = engineInstance->getProgramManager().getProgram<T>(0);
        postProcessProgram->initializeShader(engineInstance);

        // Configure everything
        GPU::Mesh::GPUMesh * quad
            = engineInstance->getGPUMeshManager().getPostProcessQuad();

        buffer->bind();
        buffer->bindDataBuffer();
        postProcessProgram->configureRenderQuad(quad);
        buffer->unBindDataBuffer();
        buffer->unBind();
      }

      virtual void
      runStage()
      {
        buffer->bind();
        outputFBO->bind();

        postProcessProgram->bind();

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);

        postProcessProgram->onRender(prevStage->getStageOutput(),
                                     engineInstance);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)0);
      }
    };
  } // namespace Render
} // namespace RenderLib

#endif