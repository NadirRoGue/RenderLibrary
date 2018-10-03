/*
* @author Nadir Rom�n Guerrero
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
			AbstractElementBasedStage * stage;

			void run();
		};

		class ThreadPool
		{
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
			ThreadPool();
			~ThreadPool();

			unsigned int getPoolSize() { return poolSize; }
			bool isActive() { return active; }
			void init(int threadPoolSize = -1);
			void addTask(std::unique_ptr<Runnable> & task);
			void shutDown();
			void pollTask();

			void processStage(AbstractElementBasedStage & stage, bool blockingStage);
		};
	}
}