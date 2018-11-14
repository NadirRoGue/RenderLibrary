#ifndef __CPU_MEMORY_MEMORYPOOLMANAGER__
#define __CPU_MEMORY_MEMORYPOOLMANAGER__

#include <mutex>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

#include "Defines.h"
#include "EngineInstance.h"

#include "CPU/memory/MemoryPool.h"
#include "CPU/memory/PoolObjectBuilder.h"

namespace RenderLib
{
  namespace CPU
  {
    namespace Memory
    {
      class MemoryManager
      {
      private:
        static MemoryManager INSTANCE;

      private:

		/// <summary>
		/// The list of memory pools, classified by the type of
		/// objects the will store
		/// </summary>
		std::unordered_map<std::type_index, std::unique_ptr<MemoryPool>>
			memoryPool;


		/// <summary>
		/// The attribute builders list, associated with the type of object they
		/// must build. The object builders will be invoked to write the objects
		/// data into the byte pool.
		/// </summary>
		std::unordered_map<std::type_index,
                           std::unique_ptr<AbstractPoolObjectBuilder>>
            attributeBuilders;


		/// <summary>
		/// A mutex object to synchronize the access to the memory pools
		/// </summary>
		std::mutex mtx;

      private:

		/// <summary>
		/// Prevents a default instance of the <see cref="MemoryManager"/> class from being created.
		/// </summary>
		MemoryManager();

      public:

		/// <summary>
		/// Gets the singleton instance of this class (common
		/// across the whole engine)
		/// </summary>
		/// <returns>The singleton instance of MemoryManager</returns>
		static MemoryManager &
        getInstance();

        ~MemoryManager();

		/// <summary>
		/// Creates the memory pool for the given type of object and the given size in bytes.
		/// If a pool for the given type already exists, is returned instead of creating a new one
		/// </summary>
		/// <param name="classType">Type of the class.</param>
		/// <param name="sizeInBytes">The size in bytes.</param>
		/// <returns>A pointer to the newly created or already existing
		/// MemoryPool</returns>
		MemoryPool *
        createMemoryPool(const std::type_index & classType,
                         const size_t & sizeInBytes);


		/// <summary>
		/// Gets the memory pool associated with the given type of object.
		/// If the pool doesnt exist, it creates it with the size in bytes
		/// defined by the compilation constant NEW_MEMORY_POOL_DEFAULT_SIZE
		/// (Seee Defines.h)
		/// </summary>
		/// <returns>A newly created or existing MemoryPool</returns>
		template <class TargetClass>
        MemoryPool *
        getMemoryPool()
        {
          auto it = memoryPool.find(typeid(TargetClass));
          if (it == memoryPool.end())
          {
            return createMemoryPool(typeid(TargetClass),
                                    NEW_MEMORY_POOL_DEFAULT_SIZE);
          }

          return it->second.get();
        }


		/// <summary>
		/// Sets the attribute builder for the given type of object.
		/// Attribute buildes will be invoked when the object is about
		/// to be written into the corresponding pool
		/// </summary>
		template <class TargetClass, class Configurator>
        void
        setAttributeBuilderForClass()
        {
          std::type_index classType = typeid(TargetClass);

          if (std::is_base_of<AbstractPoolObjectBuilder, Configurator>::value)
          {
            std::unique_ptr<AbstractPoolObjectBuilder> newBuilder
                = std::make_unique<Configurator>();
            attributeBuilders[classType] = std::move(newBuilder);
          }
          else
          {
            throw EngineException("MemoryManager: Attempted to set a non "
                                  "AbstractPoolObjectBuilder "
                                  "derived class for type "
                                  + std::string(classType.name()));
          }
        }


		/// <summary>
		/// Configures the parameters of an object in order to give it access
		/// to its memory portion.
		/// </summary>
		/// <param name="destObject">The object we want to configure</param>
		/// <param name="sourceData">The the configuration of the parameters.</param>
		template <class DestObjectClass>
        void
        configureObject(DestObjectClass * destObject,
                        BlockConfiguration * sourceData)
        {
          if (!destObject || !sourceData)
          {
            throw EngineException("MemoryManager: NULL arguments passed when "
                                  "attempting to configure "
                                  "an object's pool attributes");
          }

          std::type_index classType = typeid(DestObjectClass);

          auto attribBuilderIt = attributeBuilders.find(classType);
          if (attribBuilderIt != attributeBuilders.end())
          {
            MemoryPool * objectPool = getMemoryPool<DestObjectClass>();
            PoolObjectBuilder<DestObjectClass> * castedBuilder
                = dynamic_cast<PoolObjectBuilder<DestObjectClass> *>(
                    attribBuilderIt->second.get());

            if (castedBuilder == NULL)
            {
              throw EngineException("MemoryManager: The associated object "
                                    "builder with class type "
                                    + std::string(classType.name())
                                    + " has a different object type "
                                      "[MemoryManager::writeObject]");
            }

            MemoryBlock * requiredBlock
                = objectPool->requestMemoryBlock(sourceData->getSizeBytes());
            if (!requiredBlock)
            {
              throw EngineException("MemoryManager: No space available to "
                                    "store the given object");
            }

            castedBuilder->configureAttributes(requiredBlock, destObject,
                                               sourceData);
          }
          else
          {
            throw EngineException(
                "MemoryManager: No PoolObjectBuilder found for class "
                + std::string(classType.name())
                + " [MemoryManager::writeObject]");
          }
        }


		/// <summary>
		/// Destroys a memory pool of the given object type.
		/// </summary>
		/// <param name="classType">Type of the class.</param>
		void
        destroyMemoryPool(const std::type_index & classType);


		/// <summary>
		/// Destroys all memory pools.
		/// </summary>
		void
        destroyAllPools();
      };
    } // namespace Memory
  } // namespace CPU
} // namespace RenderLib

#endif
