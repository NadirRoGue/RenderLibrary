#ifndef __RENDERLIB_DEFAULTIMPL_SHADERCOMPILATIONSTAGE_H__
#define __RENDERLIB_DEFAULTIMPL_SHADERCOMPILATIONSTAGE_H__

#include "pipeline/PipelineStage.h"

#include "defaultimpl/components/MeshRenderer.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		class ShaderCompilationStage : public Pipeline::ElementBasedStage<MeshRenderer>
		{
		public:
			void preRunStage();
			void runStage();
		private:
			void compileShaders();
		};
	}
}

#endif