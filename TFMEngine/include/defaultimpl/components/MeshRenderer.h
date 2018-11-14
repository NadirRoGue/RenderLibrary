#ifndef __RENDERLIB_DEFAULTIMPL_MESHRENDERER_H__
#define __RENDERLIB_DEFAULTIMPL_MESHRENDERER_H__

#include "Component.h"

#include "CPU/memory/SortablePoolElement.h"

#include "CPU/mesh/Mesh.h"

#include "GPU/mesh/GPUMesh.h"

#include "material/Material.h"

namespace RenderLib
{
  namespace DefaultImpl
  {
    enum CPUToGPUSyncPolicy
    {
      CPU_SYNC_ONCE_AT_BEGINNING,
      CPU_SYNC_CONTINOUSLY,
      CPU_DO_NOT_SYNC
    };

    enum GPUToCPUSyncPolicy
    {
      GPU_SYNC_ONCE_AT_END,
      GPU_SYNC_CONTINOUSLY,
      GPU_DO_NOT_SYNC
    };

    enum PreferredRenderer
    {
      AUTO_SELECT,
      FORWARD_RENDER,
      DEFERRED_RENDER
    };

    class MeshRenderer : public Component,
                         public CPU::Memory::SortablePoolElement
    {
    public:
      CPUToGPUSyncPolicy cpuToGpuSync;
      GPUToCPUSyncPolicy gpuToCpuSync;
      PreferredRenderer preferredRender;
      GPU::Mesh::GPUMesh * gpuMesh;
      Material::Material * material;
      std::type_index shaderType;

    public:
      MeshRenderer();
      void
      start();
      size_t
      getIndex();
      CPU::Mesh::Mesh *
      getCPUMesh();

      template <class T>
      void
      setShaderType()
      {
        shaderType = typeid(T);
      }
    };
  } // namespace DefaultImpl
} // namespace RenderLib

#endif
