#ifndef __RENDERLIB_DEFAULTIMPL_COMPONENTREGISTERSTAGE_H__
#define __RENDERLIB_DEFAULTIMPL_COMPONENTREGISTERSTAGE_H__

#include "pipeline/PipelineStage.h"

namespace RenderLib
{
  namespace DefaultImpl
  {
    class ComponentRegisterStage : public Pipeline::PipelineStage
    {
    public:
      void
      preRunStage();
      void
      runStage();

    private:
      void
      initAndRegister();
    };
  } // namespace DefaultImpl
} // namespace RenderLib

#endif
