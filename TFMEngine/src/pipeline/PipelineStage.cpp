#include "pipeline/PipelineStage.h"

#include "pipeline/Threadpool.h"

namespace RenderLib
{
	namespace Pipeline
	{
		PipelineStage::PipelineStage()
		{

		}

		PipelineStage::~PipelineStage()
		{

		}

		void PipelineStage::preRunStage()
		{

		}

		void PipelineStage::postRunStage()
		{

		}

		AbstractElementBasedStage::AbstractElementBasedStage()
		{

		}

		AbstractElementBasedStage::~AbstractElementBasedStage()
		{

		}

		void AbstractElementBasedStage::registerElement(Component * comp)
		{
			if (comp != NULL)
			{
				elements.push_back(comp);
			}
		}

		const std::vector<Component*> & AbstractElementBasedStage::getRegisteredElements()
		{
			return elements;
		}

		void AbstractElementBasedStage::runStage()
		{
			if (elements.size() > 0)
			{
				ThreadPool::getInstance().processStage(*this, true);
			}
		}
	}
}