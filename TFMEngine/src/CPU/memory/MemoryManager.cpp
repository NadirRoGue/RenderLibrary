#include "CPU/memory/MemoryManager.h"

#include <new>

namespace RenderLib
{
	namespace CPU
	{
		namespace Memory
		{
			MemoryManager MemoryManager::INSTANCE;

			MemoryManager & MemoryManager::getInstance()
			{
				return INSTANCE;
			}

			MemoryManager::MemoryManager()
			{
			}

			MemoryManager::~MemoryManager()
			{
				destroyAllPools();
			}

			MemoryPool * MemoryManager::createMemoryPool(const std::type_index & classType, const size_t & sizeInBytes)
			{
				if (memoryPool.find(classType) == memoryPool.end())
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

					MemoryPool * newPool = pool.get();

					memoryPool[classType] = std::move(pool);

					return newPool;
				}
				else
				{
					// Return existing pool or warn about duplicate pool name
				}

				return NULL;
			}

			void MemoryManager::destroyMemoryPool(const std::type_index & classType)
			{
				auto it = memoryPool.find(classType);
				if (it != memoryPool.end())
				{
					std::unique_ptr<MemoryPool> pool = std::move(it->second);
					memoryPool.erase(it);
				}
			}

			void MemoryManager::destroyAllPools()
			{
				auto it = memoryPool.begin();
				while (it != memoryPool.end())
				{
					it->second->release();
					it++;
				}
			}
		}
	}
}