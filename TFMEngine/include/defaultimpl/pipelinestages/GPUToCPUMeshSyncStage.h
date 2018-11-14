#ifndef __RENDERLIB_DEFAULTIMPL_GPUTOCPUMESHSYNCSTAGE_H__
#define __RENDERLIB_DEFAULTIMPL_GPUTOCPUMESHSYNCSTAGE_H__

#include "pipeline/PipelineStage.h"

#include <vector>

#include "defaultimpl/components/MeshRenderer.h"

namespace RenderLib
{
  namespace DefaultImpl
  {
    class GPUToCPUMeshSyncStage
      : public Pipeline::ElementBasedStage<DefaultImpl::MeshRenderer>
    {
    private:
      std::vector<GPU::Mesh::GPUMesh *> syncContinousMeshes;
      std::vector<GPU::Mesh::GPUMesh *> syncOnceMeshes;

    public:
      void
      registerElement(Component * comp);
      void
      runStage();
      void
      postRunStage();

    private:
      void
      updateCPUMeshes(std::vector<GPU::Mesh::GPUMesh *> & list);
    };
  } // namespace DefaultImpl
} // namespace RenderLib

#endif
