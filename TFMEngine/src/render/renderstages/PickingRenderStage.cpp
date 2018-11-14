#include "render/renderstages/PickingRenderStage.h"

#include "logger/Log.h"

#include "defaultimpl/components/PickArrowMovement.h"

namespace RenderLib
{
  namespace Render
  {
    void
    PickingRenderStage::initialize()
    {
      // Static meshes
      GPU::Mesh::GPUBuffer * staticBuffer
          = engineInstance->getGPUMeshManager().getStaticMeshBuffer();
      staticBuffer->bind();
      staticBuffer->bindDataBuffer();
      staticRenderables.initializeMap();
      staticBuffer->unBindDataBuffer();
      staticBuffer->unBind();

      // Dynamic meshes buffer 1
      GPU::Mesh::GPUBuffer * dynamicBuffer
          = engineInstance->getGPUMeshManager().getDynamicMeshBuffer();
      dynamicBuffer->bind();
      dynamicBuffer->bindDataBuffer();
      dynamicRenderables.initializeMap();
      dynamicBuffer->unBindDataBuffer();
      dynamicBuffer->unBind();

      engineInstance->getGPUMeshManager().swapDynamicBuffers();

      // Dynamic meshes buffer 2
      dynamicBuffer
          = engineInstance->getGPUMeshManager().getDynamicMeshBuffer();
      dynamicBuffer->bind();
      dynamicBuffer->bindDataBuffer();
      dynamicRenderables.initializeMap();
      dynamicBuffer->unBindDataBuffer();
      dynamicBuffer->unBind();

      outputFBO = &(FBO::DEFAULTFRAMEBUFFER);

      pickProgram = engineInstance->getProgramManager()
                        .getProgram<DefaultImpl::PickingProgram>(0);

      engineInstance->getPickManager().setWorkingScene(
          engineInstance->getSceneManager().getActiveScene());
    }

    bool
    PickingRenderStage::shouldRegisterRenderable(
        DefaultImpl::MeshRenderer * renderable)
    {
      return true;
    }

    void
    PickingRenderStage::runStage()
    {
      Scene * scene = engineInstance->getSceneManager().getActiveScene();

      if (scene->getInputManager().mouseButtonDown(0))
      {
        pickTargets.clear();

        outputFBO->bind();
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        pickProgram->bind();

        const Camera & cam = *(scene->getActiveCamera());

        GPU::Mesh::GPUBuffer * staticBuf
            = engineInstance->getGPUMeshManager().getStaticMeshBuffer();
        drawList(staticBuf, staticRenderables.getAllRenderables(), cam);

        GPU::Mesh::GPUBuffer * dynamicBuf
            = engineInstance->getGPUMeshManager().getDynamicMeshBuffer();
        drawList(dynamicBuf, dynamicRenderables.getAllRenderables(), cam);

        glFlush();
        glFinish();

        unsigned int x = scene->getInputManager().getLastMouseX();
        unsigned int y = engineInstance->getWindow()->getHeight()
            - scene->getInputManager().getLastMouseY();

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        unsigned char readColor[4];
        glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, readColor);

        unsigned int pickedID
            = readColor[0] + readColor[1] * 256 + readColor[2] * 256 * 256;

        auto it = pickTargets.find(pickedID);

        if (it != pickTargets.end())
        {
          engineInstance->getPickManager().setLastPick(it->second->object);
        }
        else
        {
          engineInstance->getPickManager().setLastPick(nullptr);
        }
      }
    }

    void
    PickingRenderStage::drawList(
        GPU::Mesh::GPUBuffer * buffer,
        const std::vector<DefaultImpl::MeshRenderer *> & renderables,
        const Camera & cam)
    {
      if (renderables.size() > 0)
      {
        buffer->bind();
        buffer->bindDataBuffer();

        bool currentlyPicked
            = engineInstance->getPickManager().getLastPick() != nullptr;

        for (auto r : renderables)
        {
          if (!r->enabled)
          {
            continue;
          }

          unsigned int tempID = (unsigned int)pickTargets.size() + 1;

          float red   = (float)(tempID & 0xff);
          float green = (float)((tempID >> 8) & 0xff);
          float blue  = (float)((tempID >> 16 & 0xff));

          pickTargets[tempID] = r;

          GPU::Mesh::GPUMesh * mesh = r->gpuMesh;

          pickProgram->configureMeshParameters(*mesh);
          pickProgram->sendTransformParameters(*(r->object), cam);
          pickProgram->setUniform3F("pickColor", red / 255.0f, green / 255.0f,
                                    blue / 255.0f);

          if (currentlyPicked
              && r->object->getComponent<DefaultImpl::PickArrowMovement>()
                  != NULL)
          {
            pickProgram->setUniformI("zeroDepth", 1);
          }
          else
          {
            pickProgram->setUniformI("zeroDepth", 0);
          }

          glDrawElements(
              GL_TRIANGLES,
              (GLsizei)(mesh->faces.numElements * mesh->vertices.elementCount),
              GL_UNSIGNED_INT, (void *)mesh->faceIndexOffset);
        }
      }
    }
  } // namespace Render
} // namespace RenderLib