#ifndef __RENDERLIB_RENDER_PIPELINEOUTPUTSTAGE_H__
#define __RENDERLIB_RENDER_PIPELINEOUTPUTSTAGE_H__

#include "render/AbstractRenderingStage.h"

#include "defaultimpl/shaders/OutputProgram.h"

namespace RenderLib
{
  namespace Render
  {
    class PipelineOutputStage
      : public PostProcessRenderStage<DefaultImpl::OutputProgram>
    {
    public:
      void
      initialize();
    };
  } // namespace Render
} // namespace RenderLib

#endif