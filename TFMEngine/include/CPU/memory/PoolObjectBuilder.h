#ifndef __RENDERLIB_CPU_MEMORY_MEMORYWRITTER_H__
#define __RENDERLIB_CPU_MEMORY_MEMORYWRITTER_H__

#include <typeindex>
#include <typeinfo>

#include "CPU/memory/MemoryPool.h"

namespace RenderLib
{
  namespace CPU
  {
    namespace Memory
    {
      class BlockConfiguration
      {
      public:
        virtual size_t
        getSizeBytes()
            = 0;
      };

      class AbstractPoolObjectBuilder
      {
      public:
        virtual std::type_index
        getBuilderObjectType()
            = 0;
      };

      template <class T>
      class PoolObjectBuilder : public AbstractPoolObjectBuilder
      {
      public:
        PoolObjectBuilder()
        {
        }

        ~PoolObjectBuilder()
        {
        }

        std::type_index
        getBuilderObjectType()
        {
          return typeid(T);
        }

        virtual void
        configureAttributes(MemoryBlock * memBlock, T * resource,
                            BlockConfiguration * config = NULL)
            = 0;
      };
    } // namespace Memory
  } // namespace CPU
} // namespace RenderLib

#endif
