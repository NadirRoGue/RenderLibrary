#include "defaultimpl/components/MeshRenderer.h"

#include "SceneObject.h"

#include "defaultimpl/components/MeshFilter.h"

#include "defaultimpl/shaders/StandardProgram.h"

#include "material/MaterialManager.h"

#include "logger/Log.h"

namespace RenderLib
{
  namespace DefaultImpl
  {
    MeshRenderer::MeshRenderer()
      : cpuToGpuSync(CPUToGPUSyncPolicy::CPU_SYNC_ONCE_AT_BEGINNING)
      , gpuToCpuSync(GPUToCPUSyncPolicy::GPU_DO_NOT_SYNC)
      , preferredRender(PreferredRenderer::AUTO_SELECT)
      , gpuMesh(NULL)
      , material(NULL)
      , shaderType(typeid(DefaultImpl::StandardProgram))
    {
    }

    void
    MeshRenderer::start()
    {
      MeshFilter * meshFilter = object->getComponent<MeshFilter>();
      if (meshFilter == NULL || meshFilter->mesh == NULL)
      {
        enabled = false;

        Logger::Log::getInstance().logWarning(
            "MeshRenderer: Object " + object->objectName
            + " has no MeshFilter component attached, MeshRenderer component "
              "is disabled");
      }
      else
      {
        CPU::Mesh::Mesh * cpuMesh = meshFilter->mesh;
        material = Material::MaterialManager::getInstance().getMaterial(
            cpuMesh->sourceFileName, cpuMesh->materialIndex);
      }
    }

    size_t
    MeshRenderer::getIndex()
    {
      CPU::Mesh::Mesh * cpuMesh = getCPUMesh();
      if (cpuMesh != NULL)
      {
        return cpuMesh->index;
      }

      return 0;
    }

    CPU::Mesh::Mesh *
    MeshRenderer::getCPUMesh()
    {
      MeshFilter * meshFilter = object->getComponent<MeshFilter>();
      if (meshFilter == NULL)
      {
        enabled = false;
        return NULL;
      }

      return meshFilter->mesh;
    }
  } // namespace DefaultImpl
} // namespace RenderLib