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

		void Pipeline::execute()
		{
			for (auto & st : stages)
			{
				st.get()->runStage();
			}
		}
	}
}