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
		{
		}

		ContextManager::~ContextManager()
		{
		}

		void ContextManager::aquireContext()
		{
			std::unique_lock<std::mutex> lock(mutex);
			while (contextTaken)
			{
				monitor.wait(lock);
			}

			contextTaken = true;

			lock.unlock();
		}

		void ContextManager::releaseContext()
		{
			std::unique_lock<std::mutex> lock(mutex);
			contextTaken = false;
			lock.unlock();
			monitor.notify_one();
		}
	}
}