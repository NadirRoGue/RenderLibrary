#include "pipeline/Threadpool.h"

#include "logger/Log.h"

namespace RenderLib
{
	namespace Pipeline
	{
		void Runnable::run()
		{
			for (auto e : elementsToProcess)
			{
				stage->processElementWrapper(e);
			}
		}

		ThreadPool::ThreadPool()
		{
			init();
			Logger::Log::getInstance().logInfo("ThreadPool: Using " + std::to_string(poolSize) + " thread(s)");
		}

		void ThreadPool::init(int threadPoolSize)
		{
			active = true;
			if (threadPoolSize < 1)
			{
				poolSize = std::thread::hardware_concurrency();
				poolSize = poolSize < 1 ? 1 : poolSize;
			}
			else
			{
				poolSize = threadPoolSize;
			}

			for (unsigned i = 0; i < poolSize; i++)
			{
				pool.emplace_back(std::thread (&ThreadPool::pollTask, this));
			}
		}

		ThreadPool::~ThreadPool()
		{
			shutDown();
		}

		void ThreadPool::shutDown()
		{
			active = false;
			std::unique_lock<std::mutex> lock(globalLock);
			monitor.notify_all();
			lock.unlock();
			for (auto & thread : pool)
			{
				thread.join();
			}
		}

		void ThreadPool::addTask(std::unique_ptr<Runnable> & task)
		{
			std::unique_lock<std::mutex> lock(globalLock);
			tasks.push(std::move(task));
			lock.unlock();
			monitor.notify_one();
		}

		void ThreadPool::pollTask()
		{
			bool checkReleaseCallingThread = false;
			while (active)
			{
				std::unique_lock<std::mutex> lock(globalLock);
				while (tasks.empty() && active)
				{
					if (blockingStage && checkReleaseCallingThread)
					{
						callingThreadMonitor.notify_all();
					}

					monitor.wait(lock);
				}

				if (!tasks.empty())
				{
					std::unique_ptr<Runnable> task = std::move(tasks.front());
					tasks.pop();
					lock.unlock();
					checkReleaseCallingThread = true;

					task->run();
				}
				else
				{
					lock.unlock();
				}
			}
		}

		void ThreadPool::processStage(AbstractElementBasedStage & stage, bool blocking)
		{
			this->blockingStage = blocking;

			size_t elementsSize = stage.getRegisteredElements().size();
			size_t perThreadAmount = elementsSize / poolSize;
			perThreadAmount = std::max<size_t>(perThreadAmount, 1);
			size_t assigned = 0;
			auto itBegin = stage.getRegisteredElements().begin();
			while (assigned < elementsSize)
			{
				std::unique_ptr<Runnable> newTask = std::make_unique<Runnable>();

				size_t blockOffset = std::min<size_t>(assigned + perThreadAmount, elementsSize);
				newTask.get()->elementsToProcess.assign(itBegin + assigned, itBegin + blockOffset);
				newTask.get()->stage = &stage;

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
	}
}