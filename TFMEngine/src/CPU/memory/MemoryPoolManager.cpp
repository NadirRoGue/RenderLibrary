#include "CPU/memory/MemoryPoolManager.h"

#include <new>

namespace RenderLib
{
	namespace CPU
	{
		namespace Memory
		{
			MemoryPoolManager MemoryPoolManager::INSTANCE;

			MemoryPoolManager & MemoryPoolManager::getInstance()
			{
				return INSTANCE;
			}

			MemoryPoolManager::MemoryPoolManager()
			{
			}

			MemoryPoolManager::~MemoryPoolManager()
			{
				destroyAllPools();
			}

			MemoryPool * MemoryPoolManager::createMemoryPool(std::string name, size_t sizeInBytes)
			{
				if (memoryPool.find(name) == memoryPool.end())
				{
					std::unique_ptr<MemoryPool> pool;
					try
					{
						pool = std::make_unique<MemoryPool>(sizeInBytes);
					}
					catch (std::bad_alloc & balloc)
					{
						// FIXME: Error code / message returned. Could not create pool because there is no memory left
						const char * exception = balloc.what();
						return NULL;
					}

					memoryPool[name] = std::move(pool);

					return pool.get();
				}
				else
				{
					// Return existing pool or warn about duplicate pool name
				}

				return NULL;
			}

			MemoryPool * MemoryPoolManager::getMemoryPool(std::string name)
			{
				std::map<std::string, std::unique_ptr<MemoryPool>>::iterator it = memoryPool.find(name);
				if (it != memoryPool.end())
				{
					return it->second.get();
				}

				// FIXME: Raise error code/message
				return NULL;
			}

			void MemoryPoolManager::destroyMemoryPool(std::string name)
			{
				std::map<std::string, std::unique_ptr<MemoryPool>>::iterator it = memoryPool.find(name);
				if (it != memoryPool.end())
				{
					std::unique_ptr<MemoryPool> pool = std::move(it->second);
					memoryPool.erase(it);
				}
			}

			void MemoryPoolManager::destroyAllPools()
			{
				std::map<std::string, std::unique_ptr<MemoryPool>>::iterator it = memoryPool.begin();
				while (it != memoryPool.end())
				{
					std::unique_ptr<MemoryPool> temp = std::move(it->second);
					it++;
				}

				memoryPool.clear();
			}
		}
	}
}