#ifndef __RENDERLIB_DEFAULTIMPL_ITERATIONENDSTAGE_H__
#define __RENDERLIB_DEFAULTIMPL_ITERATIONENDSTAGE_H__

#include "pipeline/PipelineStage.h"

namespace RenderLib
{
  namespace DefaultImpl
  {
    class IterationEndStage : public Pipeline::PipelineStage
    {
    public:
      void
      preRunStage();
      void
      runStage();
      void
      postRunStage();
    };
  } // namespace DefaultImpl
} // namespace RenderLib

#endif
