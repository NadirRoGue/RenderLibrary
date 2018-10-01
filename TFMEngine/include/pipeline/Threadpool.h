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
			virtual void run() = 0;
		};

		template<class T>
		class ElementRunnable : public Runnable
		{
		public:
			std::vector<T*> elementsToProcess;
			Pipeline::ElementBasedStage<T> * stage;

			void run()
			{
				for (auto e : elementsToProcess)
				{
					stage->processElement(e);
				}
			}
		};

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
			void addTask(std::unique_ptr<Runnable> task);
			void shutDown();
			void pollTask();

			template<class T>
			void processStage(ElementBasedStage<T> & stage, bool blockingStage)
			{
				this->blockingStage = blockingStage;
				size_t elementsSize = stage.elements.size();
				size_t perThreadAmount = elementsSize / poolSize;
				perThreadAmount = std::max<size_t>(perThreadAmount, 1);
				size_t assigned = 0;
				auto itBegin = stage.elements.begin();
				while (assigned < elementsSize)
				{
					std::unique_ptr<ElementRunnable<T>> newTask = std::make_unique<ElementRunnable<T>>();

					size_t blockOffset = std::min<size_t>(assigned + perThreadAmount, elementsSize);
					newTask.get()->elementsToProcess.assign(itBegin + assigned, itBegin + blockOffset);

					addTask(newTask);

					assigned += (blockOffset - assigned);
				}

				if (blockingStage)
				{
					// Wait for the pool to complete
					std::unique_lock<std::mutex> lock(globalLock);
					while (!tasks.empty())
					{
						callingThreadMonitor.wait(lock);
					}
					lock.unlock();
				}
			}
		};
	}
}