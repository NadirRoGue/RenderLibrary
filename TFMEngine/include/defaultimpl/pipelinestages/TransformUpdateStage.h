#ifndef __RENDERLIB_DEFAULTIMPL_TRANSFORMUPDATESTAGE_H__
#define __RENDERLIB_DEFAULTIMPL_TRANSFORMUPDATESTAGE_H__

#include "pipeline/PipelineStage.h"
#include "Transform.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		class TransformUpdateStage : public Pipeline::ElementBasedStage<Transform>
		{
		public:
			void preRunStage();
			void runStage();
			void processElement(Transform * element);
		private:
			void buildLinearSceneTree();
		};
	}
}

#endif