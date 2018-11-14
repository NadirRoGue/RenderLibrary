#include "pipeline/Pipeline.h"

namespace RenderLib
{
  namespace Pipeline
  {
    Pipeline::Pipeline()
    {
    }

    Pipeline::~Pipeline()
    {
    }

    std::vector<std::unique_ptr<PipelineStage>> &
    Pipeline::getAllStages()
    {
      return stages;
    }

    void
    Pipeline::execute()
    {
      for (auto & st : stages)
      {
        st.get()->runStage();
      }
    }
  } // namespace Pipeline
} // namespace RenderLib