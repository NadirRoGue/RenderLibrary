#ifndef __RENDERLIB_PIPELINE_COMPONENTREGISTERSTAGE_H__
#define __RENDERLIB_PIPELINE_COMPONENTREGISTERSTAGE_H__

#include "pipeline/PipelineStage.h"

namespace RenderLib
{
	namespace Pipeline
	{
		class ComponentRegisterStage : public PipelineStage
		{
		public:
			void runStage();
		};
	}
}

#endif