#ifndef __RENDERLIB_CPU_MESHUTIL_H__
#define __RENDERLIB_CPU_MESHUTIL_H__

#include "CPU/mesh/Mesh.h"

namespace RenderLib
{
	namespace CPU
	{
		void ComputeNormals(Mesh * mesh);
		void ComputeTangents(Mesh * mesh);
		void ComputeBitangents(Mesh * mesh);
	}
}

#endif