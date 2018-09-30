#include "defaultimpl/meshblockconfigurators/InterleavedMeshBuilder.h"

#include "CPU/mesh/MeshBlockConfiguration.h"

#include <iostream>

namespace RenderLib
{
	namespace DefaultImpl
	{
		void InterleavedMeshBuilder::configureAttributes(CPU::Memory::MemoryBlock * memBlock, CPU::Mesh::Mesh * mesh, CPU::Memory::BlockConfiguration * data)
		{
			size_t offset = 0;
			CPU::Mesh::MeshBlockConfiguration * meshConfig = dynamic_cast<CPU::Mesh::MeshBlockConfiguration*>(data);
			if (!meshConfig)
			{
				throw std::runtime_error("InterleavedMeshBuilder: Wrong BlockConfiguration object passed to configure mesh attributes");
			}

			mesh->index = memBlock->index;
			mesh->memoryBlock = memBlock;

			// Faces are not interleaved, but stored as a contiguous block at the beginning of the mesh allocated memory
			mesh->faces.setAttributeSource(memBlock, 0, sizeof(IVECTOR3), meshConfig->numFaces);
			offset += meshConfig->numFaces * sizeof(IVECTOR3);

			size_t lineSize = sizeof(VECTOR3);
			lineSize += meshConfig->hasNormals ? sizeof(VECTOR3) : 0;
			lineSize += meshConfig->hasTangents ? sizeof(VECTOR3) : 0;
			lineSize += meshConfig->hasBiTangents ? sizeof(VECTOR3) : 0;
			lineSize += meshConfig->numUVChannels * sizeof(VECTOR2);
			lineSize += meshConfig->numColorChannels * sizeof(VECTOR4);

			mesh->vertices.setAttributeSource(memBlock, offset, lineSize, meshConfig->numVertices);
			offset += sizeof(VECTOR3);

			if (meshConfig->hasNormals)
			{
				mesh->normals.setAttributeSource(memBlock, offset, lineSize, (meshConfig->hasNormals ? 1 : 0) * meshConfig->numVertices);
				offset += sizeof(VECTOR3);
			}

			if (meshConfig->hasTangents)
			{
				std::cout << "Tangents" << std::endl;
				mesh->tangents.setAttributeSource(memBlock, offset, lineSize, (meshConfig->hasTangents ? 1 : 0) * meshConfig->numVertices);
				offset += sizeof(VECTOR3);
			}

			if (meshConfig->hasBiTangents)
			{
				std::cout << "Bitangents" << std::endl;
				mesh->bitangents.setAttributeSource(memBlock, offset, lineSize, (meshConfig->hasBiTangents ? 1 : 0) * meshConfig->numVertices);
				offset += sizeof(VECTOR3);
			}

			size_t i = 0;
			mesh->uvs.resize(meshConfig->numUVChannels);
			while (i < meshConfig->numUVChannels)
			{
				std::cout << "UVs" << std::endl;
				mesh->uvs[i].setAttributeSource(memBlock, offset, lineSize, meshConfig->numVertices);
				offset += sizeof(VECTOR2);
				i++;
			}

			i = 0;
			mesh->colors.resize(meshConfig->numColorChannels);
			while (i < meshConfig->numColorChannels)
			{
				std::cout << "Colors" << std::endl;
				mesh->colors[i].setAttributeSource(memBlock, offset, lineSize, meshConfig->numVertices);
				offset += sizeof(VECTOR4);
				i++;
			}
		}
	}
}