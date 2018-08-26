#ifndef __CPU_PIPELINE_H__
#define __CPU_PIPELINE_H__

#include <vector>
#include <memory>

#include "pipeline/PipelineStage.h"

namespace RenderLib
{
	class Pipeline
	{
	private:
		std::vector<std::unique_ptr<PipelineStage>> stages;
	public:
		Pipeline();
		~Pipeline();

		template<class T>
		T * registerStage()
		{
			std::unique_ptr<T> newStage = std::make_unique<T>();
			stages.push_back(std::move(newStage));

			return newStage.get();
		}

		template<class T>
		T * getStage()
		{
			for (auto st : stages)
			{
				T * castTest = dynamic_cast<T*>(st.get());
				if (castTest != NULL)
				{
					return castTest;
				}
			}

			return NULL;
		}

		template<class T>
		void removeStage()
		{
			std::vector<std::unique_ptr<PipelineStage>>::iterator it = stages.begin();
			bool found = false;
			while (it != stages.end() && !found)
			{
				T * castTest = dynamic_cast<T*>((*it).get());
				if (castTest != NULL)
				{
					stages.erase(it);
					found = true;
				}
				it++;
			}
		}

		void executePipeline();
	};
}

#endif