#ifndef __RENDERLIB_GRAPHICS_CONTEXTMANAGER_H__
#define __RENDERLIB_GRAPHICS_CONTEXTMANAGER_H__

#include <memory>

#include <mutex>
#include <condition_variable>

#include <vector>

#include "EngineInstance.h"

namespace RenderLib
{
	namespace Graphics
	{
		enum ExecutionMode
		{
			EXECUTION_SEQUENTIAL,
			EXECUTION_PARALLEL
		};

		class ContextManager
		{
		private:
			static std::unique_ptr<ContextManager> INSTANCE;
		private:
			std::mutex mutex;
			std::condition_variable monitor;

			std::vector<std::unique_ptr<EngineInstance>> instances;

			EngineInstance * activeInstance;
			EngineInstance * windowFocusedInstance;
		public:
			static ContextManager & getInstance();
		public:
			ContextManager();
			~ContextManager();

			EngineInstance * createNewEngineInstance(const std::string & engineName = "");

			void start(const ExecutionMode & execMode);

			void setActiveFocusedEngineInstace(EngineInstance * instance);
			void aquireContext(EngineInstance * instance);
			void releaseContext();

			EngineInstance * getFocusedEngineInstance();
			EngineInstance * getActiveContext();
		private:
			void startSecuentialExecution();
			void startParallelExecution();
		};
	}
}

#endif