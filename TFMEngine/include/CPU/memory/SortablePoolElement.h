#ifndef __RENDERLIB_CPU_MEMORY_SORTABLEPOOLELEMENT_H__
#define __RENDERLIB_CPU_MEMORY_SORTABLEPOOLELEMENT_H__

namespace RenderLib
{
  namespace CPU
  {
    namespace Memory
    {
      class SortablePoolElement
      {
      public:
        virtual size_t
        getIndex()
            = 0;
      };
    } // namespace Memory
  } // namespace CPU
} // namespace RenderLib

#endif
