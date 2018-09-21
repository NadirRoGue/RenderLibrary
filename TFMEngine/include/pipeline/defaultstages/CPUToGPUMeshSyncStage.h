#ifndef __RENDERLIB_PIPELINE_CPUTOGPUMESHSYNCSTAGE_H__
#define __RENDERLIB_PIPELINE_CPUTOGPUMESHSYNCSTAGE_H__

#include <vector>

#include "components/MeshRenderer.h"

#include "pipeline/PipelineStage.h"

namespace RenderLib
{
	namespace Pipeline
	{
		class CPUToGPUMeshSyncStage : public ElementBasedStage<Components::MeshRenderer>
		{
		private:
			std::vector<Components::MeshRenderer *> syncOnce;
			std::vector<Components::MeshRenderer *> syncContinously;
		public:
			void registerElement(Component * component);

			void preRunStage();
			void runStage();
		};
	}
}

#endif