#ifndef __RENDERLIB_DEFAULTIMPL_LIGHTSYNCSTAGE_H__
#define __RENDERLIB_DEFAULTIMPL_LIGHTSYNCSTAGE_H__

#include "pipeline/PipelineStage.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		class LightSyncStage : public Pipeline::PipelineStage
		{
		public:
			void preRunStage();
			void runStage();
		private:
			void syncDirLights();
			void syncPLLights();
			void syncSLLights();
		};
	}
}

#endif