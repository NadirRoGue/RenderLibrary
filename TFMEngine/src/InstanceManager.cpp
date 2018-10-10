#include "InstanceManager.h"

#include <vector>

#include "logger/Log.h"

#include "EngineException.h"

#include "graphics/ContextManager.h"

#include "GPU/program/ProgramManager.h"

#include "CPU/memory/MemoryManager.h"

namespace RenderLib
{
	void SequentialInstanceExecution(std::vector<EngineInstance *> instancePool)
	{
		// Initialize graphics
		for (auto instance : instancePool)
		{
			instance->getWindow()->initialize();
			instance->loadActiveScene();
		}

		GPU::Texture::GPUTextureManager::queryAnisotropicFilterSupport();
		Render::FBO::queryMaxRenderTargets();

		bool activeInstances = true;
		std::vector<unsigned int> instancesToClean;
		instancesToClean.reserve(instancePool.size());

		while (activeInstances)
		{
			activeInstances = false;
			// Launch instances
			for (auto instance : instancePool)
			{
				if (instance->getWindow()->isActive() && instance->isEnabled())
				{
					instance->acquireContext();
					instance->executeIteration();
					instance->releaseContext();
					activeInstances = true;
				}
				else
				{
					instancesToClean.push_back(instance->getInstanceID());
				}
			}

			// Clean up finished instances
			if (instancesToClean.size() > 0)
			{
				for (auto ID : instancesToClean)
				{
					auto it = instancePool.begin();
					bool found = false;
					while (it != instancePool.end() && !found)
					{
						EngineInstance * instance = *it;
						if (instance && instance->getInstanceID() == ID)
						{
							found = true;
							instance->cleanUp();
							instancePool.erase(it);
						}
						it++;
					}
				}
			}
		}
	}

	void ThreadedInstanceExecution(EngineInstance * instance)
	{
		Graphics::WindowHandler * graphicsHandler = instance->getWindow();

		// Aquire GPU Context ownership
		Graphics::ContextManager::getInstance().aquireContext();
		// Initialize graphics
		graphicsHandler->initialize();
		GPU::Texture::GPUTextureManager::queryAnisotropicFilterSupport();
		Render::FBO::queryMaxRenderTargets();
		// Release context ownership
		Graphics::ContextManager::getInstance().releaseContext();

		instance->loadActiveScene();
		
		// Launch instance
		while (graphicsHandler->isActive() && instance->isEnabled())
		{
			instance->executeIteration();
		}

		instance->cleanUp();
	}

	InstanceManager InstanceManager::INSTANCE;

	InstanceManager & InstanceManager::getInstance()
	{
		return INSTANCE;
	}

	InstanceManager::InstanceManager()
		: instanceIDSeed(0)
	{
	}

	InstanceManager::~InstanceManager()
	{
		for (auto it = instances.begin(); it != instances.end(); it++)
		{
			destroyInstance(it->second);
		}
	}

	EngineInstance * InstanceManager::createInstance(const std::string & instanceName, Graphics::WindowHandler * windowHandler)
	{
		if (!windowHandler)
		{
			const std::string message = "InstanceManager: Failed to create instance " + instanceName + " - No WindowHandler provided";
			throw EngineException(message.c_str());

			return NULL;
		}

		std::unique_lock<std::mutex> lock(aquireIDMtx);
		//lock.lock();

		unsigned int id = instanceIDSeed;
		instanceIDSeed++;

		EngineInstancePtr newInstance = std::make_unique<EngineInstance>(id, instanceName, windowHandler);
		EngineInstance * result = newInstance.get();
		instances[id] = std::move(newInstance);

		lock.unlock();

		Logger::Log::getInstance().logInfo("InstanceManager: Created new EngineInstance with ID = " + std::to_string(id) + " and name = " + instanceName);

		return result;
	}

	EngineInstance * InstanceManager::getInstanceByID(unsigned int ID)
	{
		auto it = instances.find(ID);
		if (it != instances.end())
		{
			return it->second.get();
		}

		return NULL;
	}

	void InstanceManager::launchInstances(const ExecutionMode & mode)
	{
		std::vector<std::thread> instanceThreads;

		switch (mode)
		{
		case ExecutionMode::EXECUTION_PARALLEL:
			launchParallel(instanceThreads);
			break;
		case ExecutionMode::EXECUTION_SEQUENTIAL:
			launchSequential(instanceThreads);
			break;
		}

		for (auto & thread : instanceThreads)
		{
			thread.join();
		}

		// Release memory
		CPU::Memory::MemoryManager::getInstance().destroyAllPools();
	}

	void InstanceManager::launchParallel(std::vector<std::thread> & createdThreads)
	{
		for (auto it = instances.begin(); it != instances.end(); it++)
		{
			createdThreads.emplace_back(std::thread(ThreadedInstanceExecution, it->second.get()));
		}
	}

	void InstanceManager::launchSequential(std::vector<std::thread> & createdThreads)
	{
		std::vector<EngineInstance *> instancesPtr;
		instancesPtr.reserve(instances.size());

		for (auto it = instances.begin(); it != instances.end(); it++)
		{
			instancesPtr.push_back(it->second.get());
		}

		createdThreads.push_back(std::thread(SequentialInstanceExecution, instancesPtr));
	}

	void InstanceManager::destroyInstance(unsigned int ID)
	{
		auto it = instances.find(ID);
		if (it != instances.end())
		{
			destroyInstance(it->second);
			instances.erase(it);
		}
	}

	void InstanceManager::destroyInstance(EngineInstancePtr & instance)
	{
		instance.reset();
	}
}