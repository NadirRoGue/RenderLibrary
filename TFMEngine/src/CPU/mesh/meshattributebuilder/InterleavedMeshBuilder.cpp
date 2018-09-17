#include "CPU/mesh/meshattributebuilders/InterleavedMeshBuilder.h"

#include "CPU/mesh/MeshBlockConfiguration.h"

namespace RenderLib
{
	namespace CPU
	{
		namespace Mesh
		{
			void InterleavedMeshBuilder::configureAttributes(Memory::MemoryBlock * memBlock, Mesh * mesh, Memory::BlockConfiguration * data)
			{
				size_t offset = 0;
				MeshBlockConfiguration * meshConfig = dynamic_cast<MeshBlockConfiguration*>(data);
				if (!meshConfig)
				{
					throw std::runtime_error("InterleavedMeshBuilder: Wrong BlockConfiguration object passed to configure mesh attributes");
				}

				mesh->index = memBlock->index;

				// Faces are not interleaved, but stored as a contiguous block at the beginning of the mesh allocated memory
				mesh->faces.setAttributeSource(memBlock, 0, 0, meshConfig->numFaces);
				offset += meshConfig->numFaces * sizeof(IVECTOR3);

				size_t lineSize = sizeof(VECTOR3);
				lineSize += meshConfig->hasNormals ? sizeof(VECTOR3) : 0;
				lineSize += meshConfig->hasTangents ? sizeof(VECTOR3) : 0;
				lineSize += meshConfig->hasBiTangents ? sizeof(VECTOR3) : 0;
				lineSize += meshConfig->numUVChannels * sizeof(VECTOR2);
				lineSize += meshConfig->numColorChannels * sizeof(VECTOR4);

				mesh->vertices.setAttributeSource(memBlock, offset, lineSize - sizeof(VECTOR3), meshConfig->numVertices);
				offset += sizeof(VECTOR3);

				if (meshConfig->hasNormals)
				{
					mesh->normals.setAttributeSource(memBlock, offset, lineSize - sizeof(VECTOR3), (meshConfig->hasNormals ? 1 : 0) * meshConfig->numVertices);
					offset += sizeof(VECTOR3);
				}

				if (meshConfig->hasTangents)
				{
					mesh->tangents.setAttributeSource(memBlock, offset, lineSize - sizeof(VECTOR3), (meshConfig->hasTangents ? 1 : 0) * meshConfig->numVertices);
					offset += sizeof(VECTOR3);
				}

				if (meshConfig->hasBiTangents)
				{
					mesh->bitangents.setAttributeSource(memBlock, offset, lineSize - sizeof(VECTOR3), (meshConfig->hasBiTangents ? 1 : 0) * meshConfig->numVertices);
					offset += sizeof(VECTOR3);
				}

				size_t i = 0;
				mesh->uvs.resize(meshConfig->numUVChannels);
				while (i < meshConfig->numUVChannels)
				{
					mesh->uvs[i].setAttributeSource(memBlock, offset, lineSize - sizeof(VECTOR2), meshConfig->numVertices);
					offset += sizeof(VECTOR2);
					i++;
				}

				i = 0;
				mesh->colors.resize(meshConfig->numColorChannels);
				while (i < meshConfig->numColorChannels)
				{
					mesh->colors[i].setAttributeSource(memBlock, offset, lineSize - sizeof(VECTOR4), meshConfig->numVertices);
					offset += sizeof(VECTOR4);
					i++;
				}
			}
		}
	}
}