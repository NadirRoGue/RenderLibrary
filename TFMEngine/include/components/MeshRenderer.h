#ifndef __RENDERLIB_COMPONENTS_MESHRENDERER_H__
#define __RENDERLIB_COMPONENTS_MESHRENDERER_H__

#include "Component.h"

namespace RenderLib
{
	namespace Components
	{
		enum CPUToGPUSyncPolicy
		{
			SYNC_ONCE_AT_BEGINNING,
			SYNC_CONTINOUSLY,
			DO_NOT_SYNC
		};

		enum GPUToCPUSyncPolicy
		{
			SYNC_ONCE_AT_END,
			SYNC_CONTINOUSLY,
			DO_NOT_SYNC
		};

		class MeshRenderer : public Component
		{
		public:
			CPUToGPUSyncPolicy cpuToGpuSync;
			GPUToCPUSyncPolicy gpuToCpuSync;
		public:
			MeshRenderer();
			void start();
		};
	}
}

#endif