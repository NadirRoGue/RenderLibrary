#include "render/RenderableMap.h"

#include "logger/Log.h"

#include "EngineException.h"

#include "GPU/program/ShaderProgram.h"

namespace RenderLib
{
	namespace Render
	{
		RenderableMap::RenderableMap()
		{

		}

		RenderableMap::~RenderableMap()
		{

		}

		void RenderableMap::setName(const std::string & name)
		{
			mapRenderOwnerName = name;
		}

		const std::string & RenderableMap::getName()
		{
			return mapRenderOwnerName;
		}

		size_t RenderableMap::getSize()
		{
			size_t size = 0;
			for (auto it = renderablesMap.begin(); it != renderablesMap.end(); it++)
			{
				for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
				{
					size += it2->second.renderables.size();
				}
			}

			return size;
		}

		void RenderableMap::addRenderable(GPU::Program::ProgramManager & programManager, DefaultImpl::MeshRenderer * renderable)
		{
			std::type_index progType = renderable->material->getShaderType();
			GPU::Program::UberParamMask mask = renderable->material->getUberMask();

			auto & innerMap = renderablesMap[progType];

			auto it = innerMap.find(mask);
			if (it == innerMap.end())
			{
				RenderableStub newStub;
				GPU::Program::Program * program = programManager.findProgram(progType, mask);

				GPU::Program::ShaderProgram * castedProg = NULL;
				if (program != NULL && (castedProg = dynamic_cast<GPU::Program::ShaderProgram*>(program)) != NULL)
				{
					newStub.program = castedProg;
					newStub.renderables.push_back(renderable);
					innerMap[mask] = newStub;
				}
				else
				{
					throw EngineException("RenderableMap: Attempted to register a non ShaderProgram program instance for a mesh");
				}
			}
			else
			{
				it->second.renderables.push_back(renderable);
			}
		}

		void RenderableMap::initializeMap()
		{
			for (auto it = renderablesMap.begin(); it != renderablesMap.end(); it++)
			{
				for (auto innerIt = it->second.begin(); innerIt != it->second.end(); innerIt++)
				{
					RenderableStub & stub = innerIt->second;

					stub.program->bind();

					for (auto renderable : stub.renderables)
					{
						stub.program->configureShaderAttributes(renderable->gpuMesh);
					}
				}
			}
		}

		void RenderableMap::renderMap(const Camera & fromCamera, EngineInstance * instance)
		{
			for (auto it = renderablesMap.begin(); it != renderablesMap.end(); it++)
			{
				for (auto innerIt = it->second.begin(); innerIt != it->second.end(); innerIt++)
				{
					RenderableStub & stub = innerIt->second;
					
					std::cout << "Before program bind " << glGetError() << std::endl;
					stub.program->bind(); 
					std::cout << "After program bind " << glGetError() << std::endl;
					//instance->getGPULightManager().getDirectionalLightBuffer().bind();
					stub.program->setUniformBlock("DLBlock", instance->getGPULightManager().getDirectionalLightBuffer().getBufferId());
					
					for (auto renderable : stub.renderables)
					{
						if (!renderable->enabled)
						{
							continue;
						}

						std::cout << renderable->getCPUMesh()->sourceFileName << std::endl;

						GPU::Mesh::GPUMesh * mesh = renderable->gpuMesh;

						stub.program->configureShaderAttributes(mesh);
						std::cout << "After config shader attrib " << glGetError() << std::endl;
						stub.program->onRenderObject(*(renderable->object), *(renderable->material), fromCamera);
						std::cout << "After onRenderObject " << glGetError() << std::endl;
						GLenum drawMode = renderable->material->wireFrameRender ? GL_LINES : GL_TRIANGLES;

						glDrawElements
						(
							drawMode,
							(GLsizei)(mesh->faces.numElements * mesh->vertices.elementCount),
							GL_UNSIGNED_INT,
							(void*)mesh->faceIndexOffset
						);

						std::cout << "After render " << glGetError() << std::endl;
					}
				}
			}
		}
	}
}