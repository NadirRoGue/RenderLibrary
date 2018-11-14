#ifndef __RENDERLIB_CPU_MESHBLOCKCONFIGURATION_H__
#define __RENDERLIB_CPU_MESHBLOCKCONFIGURATION_H__

#include "CPU/memory/PoolObjectBuilder.h"

namespace RenderLib
{
  namespace CPU
  {
    namespace Mesh
    {
      class MeshBlockConfiguration : public Memory::BlockConfiguration
      {
      public:
        size_t numFaces;
        size_t numVertices;
        bool hasNormals;
        bool hasTangents;
        bool hasBiTangents;
        size_t numUVChannels;
        size_t numColorChannels;

        size_t
        getSizeBytes();
      };
    } // namespace Mesh
  } // namespace CPU
} // namespace RenderLib

#endif
