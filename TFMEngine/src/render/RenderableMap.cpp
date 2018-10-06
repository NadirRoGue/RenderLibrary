#include "render/RenderableMap.h"

#include "logger/Log.h"

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
				if (program != NULL || (castedProg = dynamic_cast<GPU::Program::ShaderProgram*>(program)) != NULL)
				{
					newStub.program = castedProg;
					newStub.renderables.push_back(renderable);
					innerMap[mask] = newStub;
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

		void RenderableMap::renderMap(const Camera & fromCamera)
		{
			for (auto it = renderablesMap.begin(); it != renderablesMap.end(); it++)
			{
				for (auto innerIt = it->second.begin(); innerIt != it->second.end(); innerIt++)
				{
					RenderableStub & stub = innerIt->second;
					
					stub.program->bind();

					for (auto renderable : stub.renderables)
					{
						if (!renderable->enabled)
						{
							continue;
						}

						GPU::Mesh::GPUMesh * mesh = renderable->gpuMesh;

						stub.program->onRenderObject(*(renderable->object), *(renderable->material), fromCamera);

						GLenum drawMode = renderable->material->wireFrameRender ? GL_LINES : GL_TRIANGLES;

						glDrawElementsBaseVertex
						(
							drawMode,
							(GLsizei)(mesh->faces.numElements * mesh->vertices.elementCount),
							GL_UNSIGNED_INT,
							(void*)mesh->faceIndexOffset,
							(GLint)mesh->verticesBaseOffset
						);
					}
				}
			}
		}
	}
}