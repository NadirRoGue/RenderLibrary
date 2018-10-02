/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once

#include <thread>
#include <condition_variable>
#include <mutex>
#include <list>
#include <queue>
#include <vector>

#include "pipeline/PipelineStage.h"

namespace RenderLib
{
	namespace Pipeline
	{
		class Runnable
		{
		public:
			std::vector<Component*> elementsToProcess;
			Pipeline::AbstractElementBasedStage * stage;

			void run();
		};

		/*
		class ElementRunnable
		{
		public:
			std::vector<Component*> elementsToProcess;
			Pipeline::AbstractElementBasedStage * stage;

			void run()
			{
				for (auto e : elementsToProcess)
				{
					stage->processElementWrapper(e);
				}
			}
		};*/

		class ThreadPool
		{
		private:
			static ThreadPool * INSTANCE;
		private:
			std::list<std::thread> pool;
			std::queue<std::unique_ptr<Runnable>> tasks;

			std::mutex globalLock;
			std::condition_variable monitor;

			std::mutex callingThreadLock;
			std::condition_variable callingThreadMonitor;

			bool active;
			unsigned int poolSize;
			bool blockingStage;
		public:
			static ThreadPool & getInstance();
		private:
			ThreadPool();
		public:
			~ThreadPool();

			unsigned int getPoolSize() { return poolSize; }
			bool isActive() { return active; }
			void init();
			void addTask(std::unique_ptr<Runnable> & task);
			void shutDown();
			void pollTask();

			void processStage(AbstractElementBasedStage & stage, bool blockingStage);
		};
	}
}