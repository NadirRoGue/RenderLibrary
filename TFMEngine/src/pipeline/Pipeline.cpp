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

		const std::vector<std::unique_ptr<PipelineStage>> & Pipeline::getAllStages()
		{
			return stages;
		}

		void Pipeline::execute()
		{
			for (auto & st : stages)
			{
				st.get()->runStage();
			}
		}
	}
}