#ifndef __RENDERLIB_GPU_MESH_GPUMESHMANAGER_H__
#define __RENDERLIB_GPU_MESH_GPUMESHMANAGER_H__

#include <memory>
#include <unordered_map>

#include "GPU/mesh/GPUBuffer.h"
#include "GPU/mesh/GPUMesh.h"

namespace RenderLib
{
  namespace GPU
  {
    namespace Mesh
    {
      class GPUMeshManager
      {
      private:
        std::unordered_map<size_t, GPUMeshPtr> staticMeshes;
        std::unordered_map<size_t, GPUMeshPtr> dynamicMeshes;
        GPUMeshPtr postProcessQuadMesh;

        GPUBufferPtr staticBuffer;

        GPUBufferPtr dynamicFrontBuffer;
        GPUBufferPtr dynamicBackBuffer;

        GPUBufferPtr postProcessQuadBuffer;

      public:
        GPUMeshManager();
        ~GPUMeshManager();

        GPUBuffer *
        initializeStaticBuffer();
        GPUBuffer *
        initializeDynamicBuffer();
        GPUBuffer *
        initializePostProcessQuadBuffer();

        GPUBuffer *
        getStaticMeshBuffer();
        GPUBuffer *
        getDynamicMeshBuffer();
        GPUBuffer *
        getSyncDynamicBuffer();
        GPUBuffer *
        getPostProcessQuadBuffer();

        GPUMesh *
        getPostProcessQuad();

        void
        generatePostProcessQuad();

        void
        swapDynamicBuffers();

        bool
        meshAlreadyExists(size_t index, bool staticMesh);
        GPUMesh *
        getGPUMesh(size_t index, bool staticMesh);
        GPUMesh *
        createGPUMesh(size_t index, bool staticMesh);
      };
    } // namespace Mesh
  } // namespace GPU
} // namespace RenderLib

#endif