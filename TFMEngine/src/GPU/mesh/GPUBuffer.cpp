#include "GPU/mesh/GPUBuffer.h"

#include <gl/glew.h>

#include <iostream>

namespace RenderLib
{
	namespace GPU
	{
		namespace Mesh
		{
			GPUBuffer::GPUBuffer()
				: vao(-1)
			  , indexBuffer(-1)
				, dataBuffer(-1)
			{
			}

			GPUBuffer::~GPUBuffer()
			{
			}

			void GPUBuffer::bind()
			{
				glBindVertexArray(vao);
			}

			void GPUBuffer::unBind()
			{
				glBindVertexArray(0);
			}

			void GPUBuffer::bindIndexBuffer()
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
			}

			void GPUBuffer::unBindIndexBuffer()
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}

			void GPUBuffer::bindDataBuffer()
			{
				glBindBuffer(GL_ARRAY_BUFFER, dataBuffer);
			}

			void GPUBuffer::unBindDataBuffer()
			{
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}

			void GPUBuffer::updateData(char * faceData, const size_t & faceSize, char * vertexData, const size_t & vertexSize)
			{
				
				size_t numFaces = (faceSize / sizeof(unsigned int));
				std::cout << "Faces size " << faceSize << ", vertex size " << vertexSize << std::endl;
				/*
				unsigned int * castedFaces = reinterpret_cast<unsigned int*>(faceData);
				for (int i = 0; i < numFaces; i=i+3)
				{
					std::cout << castedFaces[i] << ", " << castedFaces[i + 1] << ", " << castedFaces[i + 2] << std::endl;
				}
				size_t numVertices = (vertexSize / sizeof(float));
				float * castedVertices = reinterpret_cast<float*>(vertexData);
				for (int i = 0; i < numVertices; i = i + 3)
				{
					std::cout << castedVertices[i] << ", " << castedVertices[i + 1] << ", " << castedVertices[i + 2] << std::endl;
				}
				*/

				if (vao == -1)
				{
					glGenVertexArrays(1, &vao);
				}

				bind();

				if (dataBuffer == -1)
				{
					glGenBuffers(1, &dataBuffer);
				}

				bindDataBuffer();
				glBufferData(GL_ARRAY_BUFFER, vertexSize, vertexData, usage);

				if (indexBuffer == -1)
				{
					glGenBuffers(1, &indexBuffer);
				}

				bindIndexBuffer();
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, faceSize, faceData, usage);
				//glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, faceSize, faceData);
				//unBindIndexBuffer();
				
				//glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize, vertexData);
				//unBindDataBuffer();
				
				//unBind();
			}
		}
	}
}