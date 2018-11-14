#ifndef __RENDERLIB_DEFAULTIMPL_MESHFILTER_H__
#define __RENDERLIB_DEFAULTIMPL_MESHFILTER_H__

#include "Component.h"

#include "CPU/memory/SortablePoolElement.h"

#include "CPU/mesh/Mesh.h"

namespace RenderLib
{
  namespace DefaultImpl
  {
    class MeshFilter : public Component, public CPU::Memory::SortablePoolElement
    {
    public:
      CPU::Mesh::Mesh * mesh;

    public:
      MeshFilter();
      void
      start();
      size_t
      getIndex();
    };
  } // namespace DefaultImpl
} // namespace RenderLib

#endif
