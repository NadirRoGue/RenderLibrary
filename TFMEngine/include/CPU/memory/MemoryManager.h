#ifndef __CPU_MEMORY_MEMORYPOOLMANAGER__
#define __CPU_MEMORY_MEMORYPOOLMANAGER__

#include <unordered_map>
#include <string>
#include <typeinfo>
#include <typeindex>

#include "Defines.h"

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
				std::unordered_map<std::type_index, std::unique_ptr<MemoryPool>> memoryPool;
				std::unordered_map<std::type_index, std::unique_ptr<AbstractPoolObjectBuilder>> attributeBuilders;
			private:
				MemoryManager();
			public:
				static MemoryManager & getInstance();

				~MemoryManager();

				MemoryPool * createMemoryPool(const std::type_index & classType, const size_t & sizeInBytes);
				
				template<class TargetClass>
				MemoryPool * getMemoryPool()
				{
					auto it = memoryPool.find(typeid(TargetClass));
					if (it == memoryPool.end())
					{
						return createMemoryPool(typeid(TargetClass), NEW_MEMORY_POOL_DEFAULT_SIZE);
					}

					return it->second.get();
				}

				template<class TargetClass, class Configurator>
				void setAttributeBuilderForClass()
				{
					std::type_index classType = typeid(TargetClass);

					if (std::is_base_of<AbstractPoolObjectBuilder, Configurator>::value)
					{
						std::unique_ptr<AbstractPoolObjectBuilder> newBuilder = std::make_unique<Configurator>();
						attributeBuilders[classType] = std::move(newBuilder);
					}
					else
					{
						throw EngineException("MemoryManager: Attempted to set a non AbstractPoolObjectBuilder derived class for type " + std::string(classType.name()));
					}
				}

				template<class DestObjectClass>
				void configureObject(DestObjectClass * destObject, BlockConfiguration * sourceData)
				{
					if (!destObject || !sourceData)
					{
						throw EngineException("MemoryManager: NULL arguments passed when attempting to configure an object's pool attributes");
					}

					std::type_index classType = typeid(DestObjectClass);

					auto attribBuilderIt = attributeBuilders.find(classType);
					if (attribBuilderIt != attributeBuilders.end())
					{
						MemoryPool * objectPool = getMemoryPool<DestObjectClass>();
						PoolObjectBuilder<DestObjectClass> * castedBuilder = dynamic_cast<PoolObjectBuilder<DestObjectClass>*>(attribBuilderIt->second);

						if (castedBuilder == NULL)
						{
							throw EngineException("MemoryManager: The associated object builder with class type " + std::string(classType.name()) + " has a different object type [MemoryManager::writeObject]");
						}

						MemoryBlock * requiredBlock = objectPool->requestMemoryBlock(sourceData->getSizeBytes());
						if (!requiredBlock)
						{
							throw EngineException("MemoryManager: No space available to store the given object");
						}

						castedBuilder->configureAttributes(requiredBlock, destObject, sourceData);
					}
					else
					{
						throw EngineException("MemoryManager: No PoolObjectBuilder found for class " + std::string(classType.name()) + " [MemoryManager::writeObject]");
					}
				}

				void destroyMemoryPool(const std::type_index & classType);
				void destroyAllPools();
			};
		}
	}
}

#endif