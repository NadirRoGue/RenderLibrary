#include "GPU/light/LightBuffer.h"

#include <iostream>

#include <cstdlib>

#include <gl/glew.h>

namespace RenderLib
{
  namespace GPU
  {
    namespace Light
    {
      LightBuffer::LightBuffer() : bufferId(-1)
      {
      }

      LightBuffer::~LightBuffer()
      {
      }

      unsigned int
      LightBuffer::getBufferId()
      {
        return bufferId;
      }

      void
      LightBuffer::generate()
      {
        glGenBuffers(1, &bufferId);
      }

      void
      LightBuffer::bind()
      {
        glBindBuffer(GL_UNIFORM_BUFFER, bufferId);
      }

      void
      LightBuffer::unBind()
      {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
      }

      void
      LightBuffer::setData(char * data, size_t sizeBytes)
      {
        /*
				float * test = reinterpret_cast<float*>(data + 8);
				size_t num = (sizeBytes - 8) / 4;
				for (int i = 0; i < num; i++)
				{
					//std::cout << test[i] << std::endl;
				}
				*/
        bind();
        glBufferData(GL_UNIFORM_BUFFER, sizeBytes, NULL, GL_DYNAMIC_DRAW);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeBytes, data);
        unBind();
      }

      void
      LightBuffer::destroy()
      {
        glDeleteBuffers(1, &bufferId);
      }
    } // namespace Light
  } // namespace GPU
} // namespace RenderLib