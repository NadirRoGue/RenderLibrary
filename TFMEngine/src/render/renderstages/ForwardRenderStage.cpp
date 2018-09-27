#include "render/renderstages/ForwardRenderStage.h"

#include <GL/glew.h>

#include <iostream>

#include "EngineInstance.h"

namespace RenderLib
{
	namespace Render
	{
		unsigned int loadShader(const char * src, GLenum type)
		{
			GLuint shader;
			shader = glCreateShader(type);
			glShaderSource(shader, 1, (const GLchar **)&src, NULL);
			glCompileShader(shader);

			GLint compiled;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
			if (!compiled)
			{
				GLint logLen;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
				char *logString = new char[logLen];
				glGetShaderInfoLog(shader, logLen, NULL, logString);
				std::cout << "Error: " << logString << std::endl;
				delete[] logString;
				glDeleteShader(shader);
				exit(-1);
			}

			return shader;
		}


		void ForwardRenderStage::tryRegisterElement(Components::MeshRenderer * renderable)
		{
			renderables.push_back(renderable);
		}

		void ForwardRenderStage::initialize()
		{
			const char *vertexShader =
				"#version 430 core\n"\
				"layout (location=0) in vec3 aPos;\n"\
				"uniform mat4 modelViewProj;\n"\
				"void main() {\n"\
				" gl_Position = modelViewProj * vec4(aPos, 1.0);\n"\
				"}\n\0";

			const char *fragShader =
				"#version 430 core\n"\
				"layout (location=0) out vec4 fragColor;\n"\
				"void main() {\n"\
				" fragColor=vec4(1.0,0.0,0.0,1.0);\n"\
				"}\n\0";

			unsigned int vShader = loadShader(vertexShader, GL_VERTEX_SHADER);
			unsigned int fShader = loadShader(fragShader, GL_FRAGMENT_SHADER);

			programId = glCreateProgram();

			glAttachShader(programId, vShader);
			glAttachShader(programId, fShader);

			glLinkProgram(programId);

			int linked;
			glGetProgramiv(programId, GL_LINK_STATUS, &linked);
			if (!linked)
			{
				GLint logLen;
				glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLen);
				char *logString = new char[logLen];
				glGetProgramInfoLog(programId, logLen, NULL, logString);
				std::cout << "Error: " << logString << std::endl;
				delete[] logString;
				exit(-1);
			}

			mvp = glGetUniformLocation(programId, "modelViewProj");
			apos = glGetAttribLocation(programId, "aPos");

			GPU::Mesh::GPUBuffer * buffer = engineInstance->getGPUMeshManager().getStaticMeshBuffer();
			buffer->bind();
			buffer->bindDataBuffer();
			glVertexAttribPointer(apos, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(apos);
		}

		void ForwardRenderStage::runStage()
		{
			GPU::Mesh::GPUBuffer * buffer = engineInstance->getGPUMeshManager().getStaticMeshBuffer();
			
			Camera * cam = engineInstance->getSceneManager().getActiveScene()->getAllCameras()[0].get();
			cam->updateViewMatrix();

			glDisable(GL_CULL_FACE);
			
			glUseProgram(programId);
			buffer->bind();
			for (auto r : renderables)
			{
				GPU::Mesh::GPUMesh * mesh = r->gpuMesh;

				r->object->transform.update();
				MATRIX4 modelViewProj = cam->projectionMatrix * cam->transform.modelMatrix * r->object->transform.modelMatrix;

				glUniformMatrix4fv(mvp, 1, GL_FALSE, modelViewProj.data());

				glDrawElementsBaseVertex(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, (void*)0, 0);
			}
		}
	}
}