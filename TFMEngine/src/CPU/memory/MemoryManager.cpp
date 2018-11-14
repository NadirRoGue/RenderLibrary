#include "CPU/memory/MemoryManager.h"

#include <new>

namespace RenderLib
{
  namespace CPU
  {
    namespace Memory
    {
      MemoryManager MemoryManager::INSTANCE;

      MemoryManager &
      MemoryManager::getInstance()
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

      MemoryPool *
      MemoryManager::createMemoryPool(const std::type_index & classType,
                                      const size_t & sizeInBytes)
      {
        std::unique_lock<std::mutex> lock(mtx);
        MemoryPool * poolResult = NULL;

        auto it = memoryPool.find(classType);
        if (it == memoryPool.end())
        {
          std::unique_ptr<MemoryPool> pool;
          try
          {
            pool       = std::make_unique<MemoryPool>(sizeInBytes);
            poolResult = pool.get();
          }
          catch (std::bad_alloc & balloc)
          {
            // FIXME: Error code / message returned. Could not create pool because there is no memory left
            lock.unlock();
            throw EngineException(
                "MemoryManager: Error when creating new memory pool: "
                + std::string(balloc.what()));
            return NULL;
          }

          if (poolResult != NULL)
          {
            memoryPool[classType] = std::move(pool);
          }
        }
        else
        {
          poolResult = it->second.get();
        }

        lock.unlock();

        return poolResult;
      }

      void
      MemoryManager::destroyMemoryPool(const std::type_index & classType)
      {
        std::unique_lock<std::mutex> lock(mtx);

        auto it = memoryPool.find(classType);
        if (it != memoryPool.end())
        {
          it->second.get()->release();
          memoryPool.erase(it);
        }

        lock.unlock();
      }

      void
      MemoryManager::destroyAllPools()
      {
        std::unique_lock<std::mutex> lock(mtx);

        auto it = memoryPool.begin();
        while (it != memoryPool.end())
        {
          it->second.get()->release();
          it++;
        }

        lock.unlock();
      }
    } // namespace Memory
  } // namespace CPU
} // namespace RenderLib