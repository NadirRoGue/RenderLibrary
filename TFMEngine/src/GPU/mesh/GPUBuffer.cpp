#include "GPU/mesh/GPUBuffer.h"

#include <gl/glew.h>

namespace RenderLib
{
  namespace GPU
  {
    namespace Mesh
    {
      GPUBuffer::GPUBuffer()
        : vao(-1), indexBuffer(-1), dataBuffer(-1), faceSize(0), dataSize(0)
      {
      }

      GPUBuffer::~GPUBuffer()
      {
      }

      void
      GPUBuffer::generate()
      {
        glGenVertexArrays(1, &vao);

        bind();

        GLuint result[2];
        glGenBuffers(2, &result[0]);
        indexBuffer = result[0];
        dataBuffer  = result[1];
        bindIndexBuffer();
        bindDataBuffer();

        unBind();
        unBindIndexBuffer();
        unBindDataBuffer();
      }

      void
      GPUBuffer::bind()
      {
        glBindVertexArray(vao);
      }

      void
      GPUBuffer::unBind()
      {
        glBindVertexArray(0);
      }

      void
      GPUBuffer::bindIndexBuffer()
      {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
      }

      void
      GPUBuffer::unBindIndexBuffer()
      {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      }

      void
      GPUBuffer::bindDataBuffer()
      {
        glBindBuffer(GL_ARRAY_BUFFER, dataBuffer);
      }

      void
      GPUBuffer::unBindDataBuffer()
      {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
      }

      void
      GPUBuffer::updateData(char * faceData, const size_t & faceSize,
                            char * vertexData, const size_t & vertexSize)
      {
        this->faceSize = faceSize;
        this->dataSize = dataSize;

        bindDataBuffer();
        glBufferData(GL_ARRAY_BUFFER, vertexSize, NULL, usage);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize, vertexData);

        bindIndexBuffer();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, faceSize, NULL, usage);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, faceSize, faceData);
      }

      const size_t &
      GPUBuffer::getFacesSizeBytes()
      {
        return faceSize;
      }

      const size_t &
      GPUBuffer::getDataSizeBytes()
      {
        return dataSize;
      }
    } // namespace Mesh
  } // namespace GPU
} // namespace RenderLib