#include "render/renderstages/ForwardRenderStage.h"

namespace RenderLib
{
	namespace Render
	{
		void ForwardRenderStage::tryRegisterElement(Components::MeshRenderer * renderable)
		{
			renderables.push_back(renderable);
		}

		void ForwardRenderStage::runStage()
		{
			GPU::Mesh::GPUBuffer * buffer = gpuMeshManager->getStaticMeshBuffer();
			buffer->bind();
			for (auto r : renderables)
			{
				GPU::Mesh::GPUMesh * mesh = r->gpuMesh;
				
				glDrawElementsBaseVertex(GL_TRIANGLES, mesh->getNumFaces() * 3, GL_UNSIGNED_INT, (void*)0, 0);
			}
			buffer->unBind();
		}
	}
}