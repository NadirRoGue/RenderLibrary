#ifndef __RENDERLIB_DEFAULTIMPL_CPUTOGPUMESHSYNCSTAGE_H__
#define __RENDERLIB_DEFAULTIMPL_CPUTOGPUMESHSYNCSTAGE_H__

#include <vector>

#include "defaultimpl/components/MeshRenderer.h"

#include "pipeline/PipelineStage.h"

#include "GPU/mesh/GPUBuffer.h"
#include "GPU/mesh/GPUMesh.h"

namespace RenderLib
{
  namespace DefaultImpl
  {
    typedef struct SyncData
    {
      CPU::Mesh::Mesh * cpuMesh;
      GPU::Mesh::GPUMesh * gpuMesh;
      size_t facesSize;
      size_t dataSize;

    } SyncData;

    typedef std::vector<DefaultImpl::MeshRenderer *> SourceMeshList;
    typedef std::vector<SyncData> GPUMeshList;

    class CPUToGPUMeshSyncStage
      : public Pipeline::ElementBasedStage<DefaultImpl::MeshRenderer>
    {
    private:
      GPUMeshList syncOnceData;
      GPUMeshList syncContinouslyData;

    public:
      void
      preRunStage();
      void
      runStage();

    private:
      void
      registerMeshes(SourceMeshList & staticMeshes,
                     SourceMeshList & dynamicMeshes);
      void
      createGPUMeshes(SourceMeshList & src, GPUMeshList & dst,
                      bool staticMeshes);
      void
      synchronizeData(GPU::Mesh::GPUBuffer * buffer,
                      GPUMeshList & sourceMeshes);
      GPU::Mesh::GPUMesh *
      buildGPUMeshFromCPUMesh(CPU::Mesh::Mesh * cpuMesh, size_t faceOffsetFix,
                              bool staticMesh);
    };
  } // namespace DefaultImpl
} // namespace RenderLib

#endif
