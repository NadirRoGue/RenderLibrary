#include "render/renderstages/DeferredRenderStage.h"

#include <iostream>

namespace RenderLib
{
  namespace Render
  {
    DeferredRenderStage::DeferredRenderStage()
    {
    }

    void
    DeferredRenderStage::initialize()
    {
      MeshRenderingStage::initialize();

      // Create g-buffers
      gBuffers = engineInstance->getFBOManager().createFBO("GBUFFER");

      // Set up pos and specular scale target
      GPU::Texture::GPUTexture * posAndSpecScale = gBuffers->addColorAttachment(
          0, "position", GL_RGB32F, GL_RGBA, GL_UNSIGNED_BYTE);

      GPU::Texture::GPUTexture * diffuseAndOpacity
          = gBuffers->addColorAttachment(1, "diffuseAndOpacity", GL_RGBA16F,
                                         GL_RGBA, GL_FLOAT);

      GPU::Texture::GPUTexture * normals = gBuffers->addColorAttachment(
          2, "normals", GL_RGB32F, GL_RGBA, GL_UNSIGNED_BYTE);

      GPU::Texture::GPUTexture * specularAndShininess
          = gBuffers->addColorAttachment(3, "specularAndShininess", GL_RGBA16F,
                                         GL_RGBA, GL_FLOAT);

      GPU::Texture::GPUTexture * emissive = gBuffers->addColorAttachment(
          4, "emissiveAndSpecScale", GL_RGBA16F, GL_RGBA, GL_FLOAT);

      GPU::Texture::GPUTexture * ambient = gBuffers->addColorAttachment(
          5, "ambient", GL_RGBA16F, GL_RGBA, GL_FLOAT);

      gBuffers->setSize(500, 500);

      deferredShading = engineInstance->getProgramManager()
                            .getProgram<DefaultImpl::DeferredShadingProgram>(0);

      GPU::Mesh::GPUMesh * quad
          = engineInstance->getGPUMeshManager().getPostProcessQuad();

      GPU::Mesh::GPUBuffer * buffer
          = engineInstance->getGPUMeshManager().getPostProcessQuadBuffer();

      buffer->bind();

      buffer->bindDataBuffer();

      deferredShading->configureRenderQuad(quad);

      buffer->unBindDataBuffer();
    }

    bool
    DeferredRenderStage::shouldRegisterRenderable(
        DefaultImpl::MeshRenderer * renderable)
    {
      Material::Material * mat = renderable->material;
      if ((mat->opacity() >= (FLOAT)1.0
           && renderable->preferredRender
               == DefaultImpl::PreferredRenderer::AUTO_SELECT)
          || renderable->preferredRender
              == DefaultImpl::PreferredRenderer::DEFERRED_RENDER)
      {
        return true;
      }

      return false;
    }

    void
    DeferredRenderStage::runStage()
    {
      if (!outputFBO)
      {
        throw EngineException(
            "DeferredRenderStage: No FrameBuffer setted. Aborting");
      }

      gBuffers->bind();

      GLfloat bkColor[4];
      glGetFloatv(GL_COLOR_CLEAR_VALUE, bkColor);
      glClearColor(0.0, 0.0, 0.0, 0.0);
      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
      glClearColor(bkColor[0], bkColor[1], bkColor[2], 0.0f);
      //glEnable(GL_CULL_FACE);
      glEnable(GL_DEPTH_TEST);
      glDisable(GL_BLEND);

      Camera * cam = engineInstance->getSceneManager()
                         .getActiveScene()
                         ->getActiveCamera();

      GPU::Mesh::GPUBuffer * staticBuf
          = engineInstance->getGPUMeshManager().getStaticMeshBuffer();
      staticBuf->bind();
      staticBuf->bindDataBuffer();
      staticRenderables.renderMap(*cam, engineInstance);

      GPU::Mesh::GPUBuffer * dynamicBuf
          = engineInstance->getGPUMeshManager().getDynamicMeshBuffer();
      dynamicBuf->bind();
      dynamicBuf->bindDataBuffer();
      dynamicRenderables.renderMap(*cam, engineInstance);

      outputFBO->bind();

      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

			GLboolean cfenabled;
			glGetBooleanv(GL_CULL_FACE, &cfenabled);
      glDisable(GL_CULL_FACE);

      GPU::Mesh::GPUBuffer * postProcessBuf
          = engineInstance->getGPUMeshManager().getPostProcessQuadBuffer();
      postProcessBuf->bind();

      deferredShading->bind();
      deferredShading->onRender(engineInstance);

      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)0);

			if (cfenabled)
			{
				glEnable(GL_CULL_FACE);
			}
    }
  } // namespace Render
} // namespace RenderLib