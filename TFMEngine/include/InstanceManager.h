#ifndef __RENDERLIB_INSTANCEMANAGER_H__
#define __RENDERLIB_INSTANCEMANAGER_H__

#include <unordered_map>

#include <memory>

#include <mutex>
#include <thread>

#include "EngineInstance.h"

namespace RenderLib
{
  typedef std::unique_ptr<EngineInstance> EngineInstancePtr;

  enum ExecutionMode
  {
    EXECUTION_SEQUENTIAL,
    EXECUTION_PARALLEL
  };

  class InstanceManager
  {
  private:
    static InstanceManager INSTANCE;

  public:
    static InstanceManager &
    getInstance();

  private:
    std::unordered_map<unsigned int, EngineInstancePtr> instances;
    std::mutex aquireIDMtx;
    unsigned int instanceIDSeed;

  private:
    InstanceManager();

  public:
    ~InstanceManager();

    EngineInstance *
    createInstance(const std::string & instanceName,
                   Graphics::WindowHandler * windowHandler);

    void
    launchInstances(const ExecutionMode & mode);

    EngineInstance *
    getInstanceByID(unsigned int ID);

    void
    destroyInstance(unsigned int ID);
    void
    destroyInstance(EngineInstancePtr & instance);

  private:
    void
    launchSequential(std::vector<std::thread> & createdThreads);
    void
    launchParallel(std::vector<std::thread> & createdThreads);
  };
} // namespace RenderLib

#endif