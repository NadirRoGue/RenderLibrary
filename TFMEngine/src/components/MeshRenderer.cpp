#include "components/MeshRenderer.h"

#include "SceneObject.h"

#include "components/MeshFilter.h"

#include "logger/Log.h"

namespace RenderLib
{
	namespace Components
	{
		MeshRenderer::MeshRenderer()
			: cpuToGpuSync(CPUToGPUSyncPolicy::SYNC_ONCE_AT_BEGINNING)
			, gpuToCpuSync(GPUToCPUSyncPolicy::DO_NOT_SYNC)
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
	}
}