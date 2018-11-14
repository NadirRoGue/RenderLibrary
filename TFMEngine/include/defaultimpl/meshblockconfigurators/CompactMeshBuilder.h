#ifndef __RENDERLIB_DEFAULTIMPL_COMPACTMESHBUILDER_H__
#define __RENDERLIB_DEFAULTIMPL_COMPACTMESHBUILDER_H__

#include "CPU/mesh/Mesh.h"

#include "CPU/memory/PoolObjectBuilder.h"

namespace RenderLib
{
  namespace DefaultImpl
  {
    class CompactMeshBuilder
      : public CPU::Memory::PoolObjectBuilder<CPU::Mesh::Mesh>
    {
    public:
      void
      configureAttributes(CPU::Memory::MemoryBlock * memBlock,
                          CPU::Mesh::Mesh * resource,
                          CPU::Memory::BlockConfiguration * data);
    };
  } // namespace DefaultImpl
} // namespace RenderLib

#endif
