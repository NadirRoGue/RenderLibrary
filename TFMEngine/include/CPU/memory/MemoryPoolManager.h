#ifndef __CPU_MEMORY_MEMORYPOOLMANAGER__
#define __CPU_MEMORY_MEMORYPOOLMANAGER__

#include <map>
#include <string>

#include "CPU/memory/MemoryPool.h"

namespace RenderLib
{
	namespace CPU
	{
		namespace Memory
		{
			class MemoryPoolManager
			{
			private:
				static MemoryPoolManager INSTANCE;
			private:
				std::map<std::string, std::unique_ptr<MemoryPool>> memoryPool;
			private:
				MemoryPoolManager();
			public:
				static MemoryPoolManager & getInstance();

				~MemoryPoolManager();

				MemoryPool * createMemoryPool(std::string name, size_t sizeInBytes);
				MemoryPool * getMemoryPool(std::string name);

				void destroyMemoryPool(std::string name);
				void destroyAllPools();
			};
		}
	}
}

#endif