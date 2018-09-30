#include "render/renderstages/ForwardRenderStage.h"

#include <GL/glew.h>

#include <iostream>

#include "GPU/program/ProgramManager.h"
#include "defaultimpl/shaders/StandardProgram.h"

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


		void ForwardRenderStage::tryRegisterElement(DefaultImpl::MeshRenderer * renderable)
		{
			switch (renderable->cpuToGpuSync)
			{
			case DefaultImpl::CPUToGPUSyncPolicy::CPU_SYNC_ONCE_AT_BEGINNING:
				staticRenderables.push_back(renderable);
				break;
			case DefaultImpl::CPUToGPUSyncPolicy::CPU_SYNC_CONTINOUSLY:
				dynamicRenderables.push_back(renderable);
				break;
			}
		}

		void ForwardRenderStage::initialize()
		{
			DefaultImpl::StandardProgram * standard = engineInstance->getProgramManager().getProgram<DefaultImpl::StandardProgram>(0);
			GPU::Mesh::GPUBuffer * buffer = engineInstance->getGPUMeshManager().getStaticMeshBuffer();
			buffer->bind();
			buffer->bindDataBuffer();
			standard->bind();
			for (auto renderables : staticRenderables)
			{
				standard->configureShaderAttributes(renderables->gpuMesh);
			}
		}

		void ForwardRenderStage::runStage()
		{
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
			
			GPU::Mesh::GPUBuffer * staticBuf = engineInstance->getGPUMeshManager().getStaticMeshBuffer();
			doRender(staticRenderables, staticBuf);

			GPU::Mesh::GPUBuffer * dynamicBuf = engineInstance->getGPUMeshManager().getDynamicMeshBuffer();
			doRender(dynamicRenderables, dynamicBuf);
		}

		void ForwardRenderStage::doRender(std::vector<DefaultImpl::MeshRenderer *> & renderables, GPU::Mesh::GPUBuffer * meshBuffer)
		{
			Camera * cam = engineInstance->getSceneManager().getActiveScene()->getActiveCamera();
			
			//glUseProgram(programId);
			DefaultImpl::StandardProgram * standard = engineInstance->getProgramManager().getProgram<DefaultImpl::StandardProgram>(0);
			standard->bind();
			meshBuffer->bind();
			
			for (auto r : renderables)
			{
				GPU::Mesh::GPUMesh * mesh = r->gpuMesh;

				r->object->transform.rotate(VECTOR3(0, 1, 0), 0.01f);
				r->object->transform.update();
				standard->onRenderObject(*(r->object), *cam);
				glDrawElementsBaseVertex(GL_TRIANGLES, (GLsizei)(mesh->faces.numElements * mesh->vertices.elementCount), GL_UNSIGNED_INT, (void*)0, 0);
				
			}
		}
	}
}