#include "render/pickhandlers/ObjectPickHandler.h"

#include "EngineInstance.h"

namespace RenderLib
{
	namespace Render
	{
		void ObjectPickHandler::initialize(EngineInstance * instance)
		{
			pickProgram = instance->getProgramManager().getProgram<DefaultImpl::PickingProgram>(0);
		}

		PickType ObjectPickHandler::doPicking(PickingRenderStage * pickStage)
		{
			EngineInstance * engineInstance = pickStage->engineInstance;
			PickType result;

			Scene * scene = engineInstance->getSceneManager().getActiveScene();

			if (scene->getInputManager().mouseButtonDown(0))
			{
				pickTargets.clear();

				pickProgram->bind();

				const Camera & cam = *(scene->getActiveCamera());

				GPU::Mesh::GPUBuffer * staticBuf = engineInstance->getGPUMeshManager().getStaticMeshBuffer();
				drawList(staticBuf, pickStage->getStaticRenderables().getAllRenderables(), cam);

				GPU::Mesh::GPUBuffer * dynamicBuf = engineInstance->getGPUMeshManager().getDynamicMeshBuffer();
				drawList(dynamicBuf, pickStage->getDynamicRenderables().getAllRenderables(), cam);

				glFlush();
				glFinish();

				unsigned int x = scene->getInputManager().getLastMouseX();
				unsigned int y = engineInstance->getWindow()->getHeight() - scene->getInputManager().getLastMouseY();

				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				unsigned char readColor[4];
				glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, readColor);

				unsigned int pickedID = readColor[0] + readColor[1] * 256 +
					readColor[2] * 256 * 256;

				auto it = pickTargets.find(pickedID);
				if (it != pickTargets.end())
				{
					scene->getInputManager().consumeMouseInput();
					result.type = PickTypeEnum::PICK_OBJECT;
					result.data = it->second;
					std::cout << "Picked " << it->second->getCPUMesh()->sourceFileName << std::endl;
				}
			}

			return result;
		}

		void ObjectPickHandler::drawList(GPU::Mesh::GPUBuffer * buffer,
			const std::vector<DefaultImpl::MeshRenderer*> & renderables,
			const Camera & cam)
		{
			if (renderables.size() > 0)
			{
				buffer->bind();
				buffer->bindDataBuffer();

				for (auto r : renderables)
				{
					unsigned int tempID = (unsigned int)pickTargets.size() + 1;

					float red = (float)(tempID & 0xff);
					float green = (float)((tempID >> 8) & 0xff);
					float blue = (float)((tempID >> 16 & 0xff));

					pickTargets[tempID] = r;

					GPU::Mesh::GPUMesh * mesh = r->gpuMesh;

					pickProgram->configureShaderAttributes(mesh);
					pickProgram->setUniform3F("pickColor", red / 255.0f, green / 255.0f, blue / 255.0f);
					pickProgram->onRenderObject(*(r->object), *(r->material), cam);

					glDrawElements
					(
						GL_TRIANGLES,
						(GLsizei)(mesh->faces.numElements * mesh->vertices.elementCount),
						GL_UNSIGNED_INT,
						(void*)mesh->faceIndexOffset
					);
				}
			}
		}
	}
}