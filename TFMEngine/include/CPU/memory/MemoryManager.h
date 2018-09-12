#ifndef __CPU_MEMORY_MEMORYPOOLMANAGER__
#define __CPU_MEMORY_MEMORYPOOLMANAGER__

#include <map>
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
				std::map<std::type_index, std::unique_ptr<MemoryPool>> memoryPool;
				std::map<std::type_index, AbstractPoolObjectBuilder*> attributeBuilders;
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

				template<class TargetClass>
				void setAttributeBuilderForClass(AbstractPoolObjectBuilder * builder)
				{
					if (builder)
					{
						std::type_index classType = typeid(TargetClass);

						auto it = attributeBuilders.find(classType);
						if (it != attributeBuilders.end())
						{
							delete it->second;
							it->second = builder;
						}
						else
						{
							attributeBuilders[classType] = builder;
						}
					}
				}

				template<class DestObjectClass>
				void configureObject(DestObjectClass * destObject, BlockConfiguration * sourceData)
				{
					if (!destObject || !sourceData)
					{
						throw std::runtime_error("MemoryManager: NULL arguments passed when attempting to configure an object's pool attributes");
					}

					std::type_index classType = typeid(DestObjectClass);

					auto attribBuilderIt = attributeBuilders.find(classType);
					if (attribBuilderIt != attributeBuilders.end())
					{
						MemoryPool * objectPool = getMemoryPool<DestObjectClass>();
						PoolObjectBuilder<DestObjectClass> * castedBuilder = dynamic_cast<PoolObjectBuilder<DestObjectClass>*>(attribBuilderIt->second);

						if (castedBuilder == NULL)
						{
							throw std::runtime_error("MemoryManager: The associated object builder with class type " + std::string(classType.name()) + " has a different object type [MemoryManager::writeObject]");
						}

						MemoryBlock * requiredBlock = objectPool->requestMemoryBlock(sourceData->getSizeBytes());
						if (!requiredBlock)
						{
							throw std::runtime_error("MemoryManager: No space available to store the given object");
						}

						castedBuilder->configureAttributes(requiredBlock, destObject, sourceData);
					}
					else
					{
						throw std::runtime_error("MemoryManager: No PoolObjectBuilder found for class " + std::string(classType.name()) + " [MemoryManager::writeObject]");
					}
				}

				void destroyMemoryPool(const std::type_index & classType);
				void destroyAllPools();
			};
		}
	}
}

#endif