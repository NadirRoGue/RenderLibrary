#include "defaultimpl/meshblockconfigurators/CompactMeshBuilder.h"

#include "CPU/mesh/MeshBlockConfiguration.h"

#include <stdexcept>
#include <iostream>

namespace RenderLib
{
	namespace DefaultImpl
	{
		void CompactMeshBuilder::configureAttributes(
			CPU::Memory::MemoryBlock * memBlock, 
			CPU::Mesh::Mesh * dst, 
			CPU::Memory::BlockConfiguration * data)
		{
			size_t offset = 0;
			CPU::Mesh::MeshBlockConfiguration * meshConfig = 
				dynamic_cast<CPU::Mesh::MeshBlockConfiguration*>(data);
			if (!meshConfig)
			{
				throw std::runtime_error("CompactMeshBuilder: Wrong BlockConfiguration object passed to configure mesh attributes");
			}

			dst->memoryBlock = memBlock;
			dst->index = memBlock->index;

#ifdef USE_PACKED_ATTRIB_WHEN_POSSIBLE
			size_t sizeV3 = sizeof(int);
			size_t sizeV4 = sizeof(unsigned int);
			size_t sizeV2 = sizeof(unsigned short);
#else
			size_t sizeV3 = sizeof(VECTOR3);
			size_t sizeV4 = sizeof(VECTOR4);
			size_t sizeV2 = sizeof(VECTOR2);
#endif

			dst->faces.setAttributeSource(
				memBlock, offset, sizeof(IVECTOR3), meshConfig->numFaces);
			offset += meshConfig->numFaces * sizeof(IVECTOR3);

			const size_t verticesOffset = 
				meshConfig->numVertices * sizeof(VECTOR3);

			dst->vertices.setAttributeSource(
				memBlock, offset, sizeof(VECTOR3), meshConfig->numVertices);
			offset += verticesOffset;

			if (meshConfig->hasNormals)
			{
				dst->normals.setAttributeSource(
					memBlock, offset, sizeV3, meshConfig->numVertices);
				offset += (meshConfig->numVertices * sizeV3);
			}

			if (meshConfig->hasTangents)
			{
				dst->tangents.setAttributeSource(
					memBlock, offset, sizeV3, meshConfig->numVertices);
				offset += (meshConfig->numVertices * sizeV3);
			}

			if (meshConfig->hasBiTangents)
			{
				dst->bitangents.setAttributeSource(
					memBlock, offset, sizeV3, meshConfig->numVertices);
				offset += (meshConfig->numVertices * sizeV3);
			}

			if (meshConfig->numUVChannels > 0)
			{
				dst->uvs.resize(meshConfig->numUVChannels);
				for (auto & uvMap : dst->uvs)
				{
					uvMap.setAttributeSource(
						memBlock, offset, sizeV2, meshConfig->numVertices);
					offset += meshConfig->numVertices * sizeV2;
				}
			}

			if (meshConfig->numColorChannels > 0)
			{
				dst->colors.resize(meshConfig->numColorChannels);
				for (auto & colorMap : dst->colors)
				{
					colorMap.setAttributeSource(
						memBlock, offset, sizeV4, meshConfig->numVertices);
					offset += meshConfig->numVertices * sizeV4;
				}
			}
		}
	}
}