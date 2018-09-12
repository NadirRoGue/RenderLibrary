#ifndef __RENDERLIB_COMPONENTS_MESHFILTER_H__
#define __RENDERLIB_COMPONENTS_MESHFILTER_H__

#include "Component.h"

#include "CPU/memory/SortablePoolElement.h"

#include "CPU/mesh/Mesh.h"

namespace RenderLib
{
	namespace Components
	{
		class MeshFilter : public Component
		{
		public:
			CPU::Mesh::Mesh * mesh;
		public:
			void start();
			size_t getIndex();
		};
	}
}

#endif