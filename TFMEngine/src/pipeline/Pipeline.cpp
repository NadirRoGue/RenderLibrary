#include "pipeline/Pipeline.h"

#include <iostream>

namespace RenderLib
{
	Pipeline::Pipeline()
	{
	}

	Pipeline::~Pipeline()
	{
	}

	void Pipeline::execute()
	{
		std::cout << "Pipeline.cpp: About to exectue " << stages.size() << " stage(s)" << std::endl;
		for (auto & st : stages)
		{
			st.get()->runStage();
		}
	}
}