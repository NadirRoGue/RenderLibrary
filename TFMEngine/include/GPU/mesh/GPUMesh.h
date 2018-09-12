#ifndef __RENDERLIB_GPU_MESH_GPUMESH_H__
#define __RENDERLIB_GPU_MESH_GPUMESH_H__

namespace RenderLib
{
	namespace GPU
	{
		namespace Mesh
		{
			class GPUMesh
			{
			private:
				unsigned int vao;
				
				unsigned int faceIndexOffset;

				unsigned int numFaces;
				unsigned int numVertices;
			};
		}
	}
}

#endif