#ifndef __RENDERLIB_CPU_INTERLEAVEDMESHBUILDER_H__
#define __RENDERLIB_CPU_INTERLEAVEDMESHBUILDER_H__

#include "CPU/mesh/Mesh.h"

#include "CPU/memory/PoolObjectBuilder.h"

namespace RenderLib
{
	namespace CPU
	{
		namespace Mesh
		{
			class InterleavedMeshBuilder : public Memory::PoolObjectBuilder<Mesh>
			{
			public:
				void configureAttributes(Memory::MemoryBlock * memBlock, Mesh * resource, Memory::BlockConfiguration * data);
			};
		}
	}
}

#endif