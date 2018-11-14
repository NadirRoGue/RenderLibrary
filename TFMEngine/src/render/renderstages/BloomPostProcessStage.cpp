#include "render/renderstages/BloomPostProcessStage.h"

namespace RenderLib
{
  namespace Render
  {
    void BloomPostProcessStage::initialize()
    {
      PostProcessRenderStage<DefaultImpl::BloomProgram>::initialize();

      outputFBO = &(FBO::DEFAULTFRAMEBUFFER);
    }
  } // namespace Render
} // namespace RenderLib