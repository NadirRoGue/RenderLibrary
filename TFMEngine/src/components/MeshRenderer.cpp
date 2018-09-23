#include "components/MeshRenderer.h"

#include "SceneObject.h"

#include "components/MeshFilter.h"

#include "logger/Log.h"

namespace RenderLib
{
	namespace Components
	{
		MeshRenderer::MeshRenderer()
			: cpuToGpuSync(CPUToGPUSyncPolicy::CPU_SYNC_ONCE_AT_BEGINNING)
			, gpuToCpuSync(GPUToCPUSyncPolicy::GPU_DO_NOT_SYNC)
		{

		}

		void MeshRenderer::start()
		{
			MeshFilter * meshFilter = object->getComponent<MeshFilter>();
			if (meshFilter == NULL)
			{
				enabled = false;

				Logger::Log::getInstance().logWarning("MeshRenderer: Object " + object->objectName + " has no MeshFilter component attached, MeshRenderer component is disabled");
			}
		}

		size_t MeshRenderer::getIndex()
		{
			CPU::Mesh::Mesh * cpuMesh = getCPUMesh();
			if (cpuMesh != NULL)
			{
				return cpuMesh->index;
			}

			return 0;
		}

		CPU::Mesh::Mesh * MeshRenderer::getCPUMesh()
		{
			MeshFilter * meshFilter = object->getComponent<MeshFilter>();
			if (meshFilter == NULL)
			{
				enabled = false;
				return NULL;
			}

			return meshFilter->mesh;
		}
	}
}