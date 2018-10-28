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
			return allRenderables.size();
		}

		const std::vector<DefaultImpl::MeshRenderer *> & RenderableMap::getAllRenderables()
		{
			return allRenderables;
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
				GPU::Program::Program * program = programManager.getProgram(mask, progType);

				GPU::Program::ShaderProgram * castedProg = NULL;
				if (program != NULL && (castedProg = static_cast<GPU::Program::ShaderProgram*>(program)) != NULL)
				{
					newStub.program = castedProg;
					newStub.renderables.push_back(renderable);
					innerMap[mask] = newStub;
					allRenderables.push_back(renderable);
				}
				else
				{
					throw EngineException("RenderableMap: Attempted to register a non ShaderProgram program instance for a mesh");
				}
			}
			else
			{
				it->second.renderables.push_back(renderable);
				allRenderables.push_back(renderable);
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
						stub.program->configureMeshParameters(*renderable->gpuMesh);
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
					
					stub.program->bind(); 
					stub.program->onFrameBegin(instance);
					
					for (auto renderable : stub.renderables)
					{
						if (!renderable->enabled)
						{
							continue;
						}

						GPU::Mesh::GPUMesh * mesh = renderable->gpuMesh;

						stub.program->configureMeshParameters(*(renderable->gpuMesh));
						stub.program->sendMaterialParameters(*(renderable->material));
						stub.program->sendTransformParameters(*(renderable->object), fromCamera);
						stub.program->onRenderObject(*renderable, fromCamera);
						GLenum drawMode = renderable->material->wireFrameRender ? GL_LINES : GL_TRIANGLES;

						glDrawElements
						(
							drawMode,
							(GLsizei)(mesh->faces.numElements * mesh->vertices.elementCount),
							GL_UNSIGNED_INT,
							(void*)mesh->faceIndexOffset
						);
					}
				}
			}
		}
	}
}