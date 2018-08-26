#include "pipeline/Pipeline.h"

namespace RenderLib
{
	Pipeline::Pipeline()
	{
	}

	Pipeline::~Pipeline()
	{
	}

	void Pipeline::executePipeline()
	{
		for(size_t i = 0; i < stages.size(); i++)
		{
			stages[i].get()->runStage();
		}
	}
}