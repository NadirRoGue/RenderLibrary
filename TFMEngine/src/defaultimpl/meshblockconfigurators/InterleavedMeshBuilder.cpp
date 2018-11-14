#include "defaultimpl/meshblockconfigurators/InterleavedMeshBuilder.h"

#include "CPU/mesh/MeshBlockConfiguration.h"

namespace RenderLib
{
  namespace DefaultImpl
  {
    void
    InterleavedMeshBuilder::configureAttributes(
        CPU::Memory::MemoryBlock * memBlock, CPU::Mesh::Mesh * mesh,
        CPU::Memory::BlockConfiguration * data)
    {
      size_t offset = 0;
      CPU::Mesh::MeshBlockConfiguration * meshConfig
          = dynamic_cast<CPU::Mesh::MeshBlockConfiguration *>(data);
      if (!meshConfig)
      {
        throw std::runtime_error(
            "InterleavedMeshBuilder: Wrong BlockConfiguration object passed to "
            "configure mesh attributes");
      }

      mesh->index       = memBlock->index;
      mesh->memoryBlock = memBlock;

#ifdef USE_PACKED_ATTRIB_WHEN_POSSIBLE
      size_t sizeV3 = sizeof(int);
      size_t sizeV4 = sizeof(unsigned int);
      size_t sizeV2 = sizeof(unsigned short);
#else
      size_t sizeV3 = sizeof(VECTOR3);
      size_t sizeV4 = sizeof(VECTOR4);
      size_t sizeV2 = sizeof(VECTOR2);
#endif

      // Faces are not interleaved, but stored as a contiguous block at the beginning of the mesh allocated memory
      mesh->faces.setAttributeSource(memBlock, 0, sizeof(IVECTOR3),
                                     meshConfig->numFaces);
      offset += meshConfig->numFaces * sizeof(IVECTOR3);

      size_t lineSize = sizeof(VECTOR3);
      lineSize += meshConfig->hasNormals ? sizeV3 : 0;
      lineSize += meshConfig->hasTangents ? sizeV3 : 0;
      lineSize += meshConfig->hasBiTangents ? sizeV3 : 0;
      lineSize += meshConfig->numUVChannels * sizeV2;
      lineSize += meshConfig->numColorChannels * sizeV4;

      mesh->vertices.setAttributeSource(memBlock, offset, lineSize,
                                        meshConfig->numVertices);
      offset += sizeof(VECTOR3);

      if (meshConfig->hasNormals)
      {
        mesh->normals.setAttributeSource(memBlock, offset, lineSize,
                                         (meshConfig->hasNormals ? 1 : 0)
                                             * meshConfig->numVertices);
        offset += sizeV3;
      }

      if (meshConfig->hasTangents)
      {
        mesh->tangents.setAttributeSource(memBlock, offset, lineSize,
                                          (meshConfig->hasTangents ? 1 : 0)
                                              * meshConfig->numVertices);
        offset += sizeV3;
      }

      if (meshConfig->hasBiTangents)
      {
        mesh->bitangents.setAttributeSource(memBlock, offset, lineSize,
                                            (meshConfig->hasBiTangents ? 1 : 0)
                                                * meshConfig->numVertices);
        offset += sizeV3;
      }

      size_t i = 0;
      mesh->uvs.resize(meshConfig->numUVChannels);
      while (i < meshConfig->numUVChannels)
      {
        mesh->uvs[i].setAttributeSource(memBlock, offset, lineSize,
                                        meshConfig->numVertices);
        offset += sizeV2;
        i++;
      }

      i = 0;
      mesh->colors.resize(meshConfig->numColorChannels);
      while (i < meshConfig->numColorChannels)
      {
        mesh->colors[i].setAttributeSource(memBlock, offset, lineSize,
                                           meshConfig->numVertices);
        offset += sizeV4;
        i++;
      }
    }
  } // namespace DefaultImpl
} // namespace RenderLib