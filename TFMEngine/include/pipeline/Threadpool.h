/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once

#include <condition_variable>
#include <list>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "pipeline/PipelineStage.h"

namespace RenderLib
{
  namespace Pipeline
  {
    class Runnable
    {
    public:
      std::vector<Component *> elementsToProcess;
      AbstractElementBasedStage * stage;

      void
      run();
    };

    void
    dummyCallback(AbstractElementBasedStage * stage, Component * comp);

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
      unsigned int activeThreads;

    public:
      ThreadPool();
      ~ThreadPool();

      unsigned int
      getPoolSize()
      {
        return poolSize;
      }
      bool
      isActive()
      {
        return active;
      }
      void
      init(int threadPoolSize = -1);
      void
      addTask(std::unique_ptr<Runnable> & task);
      void
      shutDown();
      void
      pollTask();

      void
      processStage(AbstractElementBasedStage & stage);
    };
  } // namespace Pipeline
} // namespace RenderLib