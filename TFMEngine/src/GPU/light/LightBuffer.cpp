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
			LightBuffer::LightBuffer()
				: bufferId(-1)
			{
			}

			LightBuffer::~LightBuffer()
			{
			}

			unsigned int LightBuffer::getBufferId()
			{
				return bufferId;
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
				float * test = reinterpret_cast<float*>(data + 8);
				size_t num = (sizeBytes - 8) / 4;
				for (int i = 0; i < num; i++)
				{
					//std::cout << test[i] << std::endl;
				}
				std::cout << "setData" << std::endl;
				std::cout << glGetError() << std::endl;
				bind();
				std::cout << glGetError() << std::endl;
				glBufferData(GL_UNIFORM_BUFFER, sizeBytes - 8, NULL, GL_DYNAMIC_DRAW);
				std::cout << glGetError() << std::endl;
				glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeBytes - 8, data+8);
				std::cout << glGetError() << std::endl;
				unBind();
				std::cout << glGetError() << " END" << std::endl;
			}

			void LightBuffer::destroy()
			{

			}
		}
	}
}