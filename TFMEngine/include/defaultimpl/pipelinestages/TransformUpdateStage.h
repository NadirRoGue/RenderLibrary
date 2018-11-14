#ifndef __RENDERLIB_DEFAULTIMPL_TRANSFORMUPDATESTAGE_H__
#define __RENDERLIB_DEFAULTIMPL_TRANSFORMUPDATESTAGE_H__

#include "Transform.h"
#include "pipeline/PipelineStage.h"

#include <list>

namespace RenderLib
{
  namespace DefaultImpl
  {
    class TransformUpdateStage : public Pipeline::ElementBasedStage<Transform>
    {
    private:
      std::list<std::vector<Component *>> linearGraph;

    public:
      void
      preRunStage();
      void
      runStage();
      void
      processElement(Transform * element);

    private:
      void
      updateLinealGraph(); // UNUSED
    };
  } // namespace DefaultImpl
} // namespace RenderLib

#endif
