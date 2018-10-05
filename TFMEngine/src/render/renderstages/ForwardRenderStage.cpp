#include "render/renderstages/ForwardRenderStage.h"

#include <GL/glew.h>

#include <iostream>

#include "GPU/program/ProgramManager.h"
#include "defaultimpl/shaders/StandardProgram.h"

#include "CPU/texture/TextureManager.h"
#include "GPU/texture/GPUTexture2D.h"

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

			CPU::Texture::Texture * cpuTexture = CPU::Texture::TextureManager::getInstance().loadTexture("./assets/texture.png", CPU::Texture::TextureType::TEXTURE2D);
			texture = manager.createTexture<GPU::Texture::GPUTexture2D>(cpuTexture->index);
			std::vector<unsigned char> data;
			cpuTexture->pixels.dumpAttributes(data);
			texture->upload(&data[0], cpuTexture->width, cpuTexture->height);
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
			
			unsigned int unit = 0;
			for (auto r : renderables)
			{
				GPU::Mesh::GPUMesh * mesh = r->gpuMesh;

				standard->onRenderObject(*(r->object), *(r->material), *cam);
				standard->setUniformTexture("diffuseTexture", texture, unit);
				unit = 0;

				glDrawElementsBaseVertex(GL_TRIANGLES, (GLsizei)(mesh->faces.numElements * mesh->vertices.elementCount), GL_UNSIGNED_INT, (void*)0, 0);
				
			}
		}
	}
}