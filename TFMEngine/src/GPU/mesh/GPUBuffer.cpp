#include "GPU/mesh/GPUBuffer.h"

#include <gl/glew.h>

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
				bind();

				bindIndexBuffer();
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, faceSize, NULL, usage);
				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, faceSize, faceData);
				unBindIndexBuffer();

				bindDataBuffer();
				glBufferData(GL_ARRAY_BUFFER, vertexSize, NULL, usage);
				glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize, vertexData);
				unBindDataBuffer();

				unBind();
			}
		}
	}
}