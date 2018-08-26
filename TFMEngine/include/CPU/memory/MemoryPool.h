#ifndef __CPU_MEMORY_MEMORYPOOL__
#define __CPU_MEMORY_MEMORYPOOL__

#include <vector>
#include <memory>

namespace RenderLib
{
	namespace CPU
	{
		namespace Memory
		{

			class MemoryPool;

			typedef struct MemoryBlock
			{
				size_t index;
				size_t offset;
				size_t length;
				size_t sizeUsedBlock;
				const MemoryPool * pool;

				MemoryBlock(size_t offset, size_t lenght, const MemoryPool * pool)
					: offset(offset)
					, length(lenght)
					, pool(pool)
				{
				}

			} MemoryBlock;

			class MemoryPool
			{
			private:

				void * allocatedMemory;
				size_t sizeBytes;
				size_t usedBytes;
				std::vector<std::unique_ptr<MemoryBlock>> memoryBlockList;

			public:

				MemoryPool(size_t size);
				MemoryPool(const MemoryPool & other);

				MemoryBlock * append(void * data, size_t sizeBytes, bool checkForGaps = false);
				void retrieve(void * dst, MemoryBlock * block, size_t offset, size_t length) const;
				void compact();

				void resize(size_t newSizeBytes);
				void allocate(size_t newSizeBytes);
				void release();

				const void * getData() const;
				const char * getDataAsBytes() const;

			private:
				void destroyBlock(std::unique_ptr<MemoryBlock> block); // Pass-by-value unique_ptr http://www.codingstandard.com/rule/8-2-4-do-not-pass-stdunique_ptr-by-const-reference/
				void copyToPool(void * src, size_t srcSizeBytes, MemoryBlock * block);
				void copyFromPool(void * dst, MemoryBlock * block, size_t offset, size_t lenght) const;
			};
		}
	}
}

#endif