#include "defaultimpl/meshblockconfigurators/CompactMeshBuilder.h"

#include "CPU/mesh/MeshBlockConfiguration.h"

#include <stdexcept>
#include <iostream>

namespace RenderLib
{
	namespace DefaultImpl
	{
		void CompactMeshBuilder::configureAttributes(CPU::Memory::MemoryBlock * memBlock, CPU::Mesh::Mesh * dst, CPU::Memory::BlockConfiguration * data)
		{
			size_t offset = 0;
			CPU::Mesh::MeshBlockConfiguration * meshConfig = dynamic_cast<CPU::Mesh::MeshBlockConfiguration*>(data);
			if (!meshConfig)
			{
				throw std::runtime_error("CompactMeshBuilder: Wrong BlockConfiguration object passed to configure mesh attributes");
			}

			dst->memoryBlock = memBlock;
			dst->index = memBlock->index;

			dst->faces.setAttributeSource(memBlock, offset, 0, meshConfig->numFaces);
			offset += meshConfig->numFaces * sizeof(IVECTOR3);

			const size_t verticesOffset = meshConfig->numVertices * sizeof(VECTOR3);

			dst->vertices.setAttributeSource(memBlock, offset, 0, meshConfig->numVertices);
			offset += verticesOffset;

			if (meshConfig->hasNormals)
			{
				dst->normals.setAttributeSource(memBlock, offset, 0, meshConfig->numVertices);
				offset += verticesOffset;
				std::cout << "normals configured" << std::endl;
			}

			if (meshConfig->hasTangents)
			{
				dst->tangents.setAttributeSource(memBlock, offset, 0, meshConfig->numVertices);
				offset += verticesOffset;
			}

			if (meshConfig->hasBiTangents)
			{
				dst->bitangents.setAttributeSource(memBlock, offset, 0, meshConfig->numVertices);
				offset += verticesOffset;
			}

			if (meshConfig->numUVChannels > 0)
			{
				dst->uvs.resize(meshConfig->numUVChannels);
				for (auto & uvMap : dst->uvs)
				{
					uvMap.setAttributeSource(memBlock, offset, 0, meshConfig->numVertices);
					offset += meshConfig->numVertices * sizeof(VECTOR2);
				}
			}

			if (meshConfig->numColorChannels > 0)
			{
				dst->colors.resize(meshConfig->numColorChannels);
				for (auto & colorMap : dst->colors)
				{
					colorMap.setAttributeSource(memBlock, offset, 0, meshConfig->numVertices);
					offset += meshConfig->numVertices * sizeof(VECTOR4);
				}
			}
		}
	}
}