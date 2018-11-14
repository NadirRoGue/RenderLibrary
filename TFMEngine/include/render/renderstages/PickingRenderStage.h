#ifndef __RENDERLIB_RENDER_PICKINGRENDERSTAGE_H__
#define __RENDERLIB_RENDER_PICKINGRENDERSTAGE_H__

#include <unordered_map>

#include "render/AbstractRenderingStage.h"

#include "defaultimpl/shaders/PickingProgram.h"

namespace RenderLib
{
  namespace Render
  {
    class PickingRenderStage : public MeshRenderingStage
    {
    private:
      std::unordered_map<unsigned int, DefaultImpl::MeshRenderer *> pickTargets;
      DefaultImpl::PickingProgram * pickProgram;

    public:
      void
      initialize();
      bool
      shouldRegisterRenderable(DefaultImpl::MeshRenderer * renderable);
      void
      runStage();

    private:
      void
      drawList(GPU::Mesh::GPUBuffer * buffer,
               const std::vector<DefaultImpl::MeshRenderer *> & renderables,
               const Camera & cam);
    };
  } // namespace Render
} // namespace RenderLib

#endif