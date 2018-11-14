#include "CPU/memory/MemoryPool.h"

#include <cstdlib>

#include "CPU/memory/MemoryUtil.h"

#include "EngineException.h"

namespace RenderLib
{
  namespace CPU
  {
    namespace Memory
    {
      MemoryPool::MemoryPool(size_t size)
        : sizeBytes(size), usedBytes(0), allocatedMemory(0)
      {
        allocate(sizeBytes);
      }

      MemoryPool::MemoryPool(const MemoryPool & other)
        : sizeBytes(other.sizeBytes), usedBytes(0), allocatedMemory(0)
      {
        allocate(sizeBytes);
        memcpy(allocatedMemory, other.allocatedMemory, sizeBytes);
      }

      MemoryBlock *
      MemoryPool::requestMemoryBlock(size_t sizeBytes, bool checkForGaps)
      {
        return append(NULL, sizeBytes, checkForGaps);
      }

      MemoryBlock *
      MemoryPool::append(void * data, size_t sizeBytes, bool checkForGaps)
      {
        bool added           = false;
        MemoryBlock * result = NULL;

        if (checkForGaps)
        {
          size_t start = 0;
          for (std::unique_ptr<MemoryBlock> & mb : memoryBlockList)
          {
            MemoryBlock * mbPtr = mb.get();
            if (mbPtr->sizeUsedBlock < 1)
            {
              if (start < mbPtr->offset && mbPtr->length >= sizeBytes)
              {
                mbPtr->sizeUsedBlock = sizeBytes;
                if (data)
                {
                  copyToPool(data, sizeBytes, mbPtr);
                }
                result = mbPtr;
                added  = true;
                break;
              }
            }
            else
            {
              start += mbPtr->length;
            }
          }
        }

        if (!added)
        {
          size_t remainingMem = this->sizeBytes - usedBytes;
          // Attempt to resize pool
          if (remainingMem < sizeBytes)
          {
            resize(this->sizeBytes * 2);
          }

          remainingMem = this->sizeBytes - usedBytes;

          if (remainingMem >= sizeBytes)
          {
            std::unique_ptr<MemoryBlock> newBlock
                = std::make_unique<MemoryBlock>(usedBytes, sizeBytes, this);
            newBlock.get()->index = memoryBlockList.size();
            this->usedBytes += sizeBytes;
            result                       = newBlock.get();
            blocksByIndex[result->index] = result;
            memoryBlockList.push_back(std::move(newBlock));
            if (data)
            {
              copyToPool(data, sizeBytes, result);
            }
          }
        }

        return result;
      }

      void
      MemoryPool::retrieve(void * dst, MemoryBlock * block, size_t offset,
                           size_t length) const
      {
        copyFromPool(dst, block, offset, length);
      }

      void
      MemoryPool::compact()
      {
        size_t start           = 0;
        size_t index           = 0;
        MemoryBlock * previous = NULL;

        std::vector<std::unique_ptr<MemoryBlock>> tempBlocks;

        for (auto & mb : memoryBlockList)
        {
          MemoryBlock * mbPtr = mb.get();

          if (mbPtr == NULL)
            continue;

          if (mbPtr->sizeUsedBlock > 0)
          {
            if (mbPtr->offset > start)
            {
              size_t diff = mbPtr->offset - start;

              // Update previous block
              if (previous != NULL)
              {
                previous->length = previous->sizeUsedBlock;
              }

              // Update current
              // Copy current block's content to temporary buffer
              void * buf = (void *)malloc(mbPtr->sizeUsedBlock);
              copyBytes(buf, 0, allocatedMemory, mbPtr->offset,
                        mbPtr->sizeUsedBlock);
              // Copy temp memory buffer into main memory buffer correct position
              copyBytes(allocatedMemory, start, buf, 0, mbPtr->sizeUsedBlock);
              // update block
              mbPtr->offset = start;
            }

            // Update memory pool temp used offset
            start += mbPtr->sizeUsedBlock;

            // Update index. Will remain the same for each block if no block was dropped
            mbPtr->index = index;
            index++;

            tempBlocks.push_back(std::move(mb));
          }
          else
          {
            destroyBlock(std::move(mb));
          }
        }
      }

      void
      MemoryPool::allocate(size_t size)
      {
        if (allocatedMemory == 0)
        {
          allocatedMemory = std::malloc(size);
        }
        else
        {
          resize(size);
        }
      }

      void
      MemoryPool::release()
      {
        std::free(allocatedMemory);
        allocatedMemory = NULL;
      }

      void
      MemoryPool::resize(size_t size)
      {
        sizeBytes = size;
        realloc(allocatedMemory, sizeBytes);
      }

      void *
      MemoryPool::getData()
      {
        return allocatedMemory;
      }

      char *
      MemoryPool::getDataAsBytes()
      {
        return reinterpret_cast<char *>(allocatedMemory);
      }

      void
      MemoryPool::setBlockData(MemoryBlock * block, size_t offset,
                               size_t length, char * data)
      {
        bool check = block->length - offset > length;
        if (!check)
        {
          throw EngineException("MemoryManager: attempted to write on a "
                                "MemoryBlock, but length surpassed block size");
        }

        char * castedData = reinterpret_cast<char *>(this->allocatedMemory);

        memcpy(castedData + block->offset + offset, data, length);
      }

      std::unordered_map<size_t, MemoryBlock *> &
      MemoryPool::getBlocksByIndex()
      {
        return blocksByIndex;
      }

      void
      MemoryPool::destroyBlock(std::unique_ptr<MemoryBlock> block)
      {
        block.get()->sizeUsedBlock = 0;
      }

      void
      MemoryPool::copyToPool(void * src, size_t srcSizeBytes,
                             MemoryBlock * block)
      {
        if (srcSizeBytes <= block->length)
        {
          copyBytes(static_cast<char *>(allocatedMemory), block->offset, src, 0,
                    srcSizeBytes);
        }
      }

      void
      MemoryPool::copyFromPool(void * dst, MemoryBlock * block, size_t offset,
                               size_t lenght) const
      {
        size_t startCopy = block->offset + offset;
        size_t endCopy   = startCopy + lenght;

        if (startCopy < block->offset + block->length
            && endCopy <= block->offset + block->length)
        {
          copyBytes(dst, 0, allocatedMemory, block->offset + offset, lenght);
        }
      }
    } // namespace Memory
  } // namespace CPU
} // namespace RenderLib