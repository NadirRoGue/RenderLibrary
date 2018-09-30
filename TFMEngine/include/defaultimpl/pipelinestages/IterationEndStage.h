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
			void runStage();
			void postRunStage();
		};
	}
}

#endif