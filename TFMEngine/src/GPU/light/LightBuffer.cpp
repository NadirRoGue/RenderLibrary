#include "GPU/light/LightBuffer.h"

#include <cstdlib>

#include <gl/glew.h>

namespace RenderLib
{
	namespace GPU
	{
		namespace Light
		{
			LightBuffer::LightBuffer()
				: bufferId(-1)
			{
			}

			LightBuffer::~LightBuffer()
			{
			}

			void LightBuffer::generate()
			{
				glGenBuffers(1, &bufferId);
			}

			void LightBuffer::bind()
			{
				glBindBuffer(GL_UNIFORM_BUFFER, bufferId);
			}

			void LightBuffer::unBind()
			{
				glBindBuffer(GL_UNIFORM_BUFFER, 0);
			}

			void LightBuffer::setData(char * data, size_t sizeBytes)
			{
				bind();
				glBufferData(GL_UNIFORM_BUFFER, sizeBytes, NULL, GL_DYNAMIC_DRAW);
				glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeBytes, data);
				unBind();
			}

			void LightBuffer::destroy()
			{

			}
		}
	}
}