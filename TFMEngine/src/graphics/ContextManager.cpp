#include "graphics/ContextManager.h"

#include "pipeline/PipelineManager.h"

namespace RenderLib
{
	namespace Graphics
	{
		std::unique_ptr<ContextManager> ContextManager::INSTANCE = std::make_unique<ContextManager>();

		ContextManager & ContextManager::getInstance()
		{
			return *(INSTANCE.get());
		}

		ContextManager::ContextManager()
			: activeInstance(NULL)
			, windowFocusedInstance(NULL)
		{
		}

		ContextManager::~ContextManager()
		{
		}

		EngineInstance * ContextManager::createNewEngineInstance(const std::string & instanceName)
		{
			std::unique_ptr<EngineInstance> newInstance = std::make_unique<EngineInstance>(instanceName);
			EngineInstance * result = newInstance.get();

			instances.push_back(std::move(newInstance));

			return result;
		}

		void ContextManager::start(const ExecutionMode & mode)
		{
			switch (mode)
			{
			case ExecutionMode::EXECUTION_SEQUENTIAL:
				startSecuentialExecution();
				break;
			case ExecutionMode::EXECUTION_PARALLEL:
				startParallelExecution();
				break;
			}
		}

		void ContextManager::setActiveFocusedEngineInstace(EngineInstance * instance)
		{
			windowFocusedInstance = instance;
		}

		EngineInstance * ContextManager::getFocusedEngineInstance()
		{
			return windowFocusedInstance;
		}

		void ContextManager::aquireContext(EngineInstance * instance)
		{
			std::unique_lock<std::mutex> lock(mutex);
			while (activeInstance != NULL)
			{
				monitor.wait(lock);
			}

			activeInstance = instance;
		}

		void ContextManager::releaseContext()
		{
			std::unique_lock<std::mutex> lock(mutex);
			activeInstance = NULL;
		}

		EngineInstance * ContextManager::getActiveContext()
		{
			return activeInstance;
		}

		void ContextManager::startSecuentialExecution()
		{
			while (instances.size() > 0)
			{
				for (auto & instance : instances)
				{
					//instance.get()->getPipelineManager()->executePipeline();
				}
			}
		}

		void ContextManager::startParallelExecution()
		{

		}
	}
}