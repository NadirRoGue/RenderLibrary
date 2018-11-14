#ifndef __RENDERLIB_DEFAULTIMPL_CPUTOGPUTEXTURESYNCSTAGE_H__
#define __RENDERLIB_DEFAULTIMPL_CPUTOGPUTEXTURESYNCSTAGE_H__

#include "pipeline/PipelineStage.h"

#include "defaultimpl/components/MeshRenderer.h"

#include "material/Material.h"

namespace RenderLib
{
  namespace DefaultImpl
  {
    class CPUToGPUTextureSyncStage
      : public Pipeline::ElementBasedStage<MeshRenderer>
    {
    public:
      void
      preRunStage();
      void
      runStage();

    private:
      void
      syncTextures();

      void
      syncTexture(Material::MaterialTexture & textureParameter);
    };
  } // namespace DefaultImpl
} // namespace RenderLib

#endif
