#ifndef __RENDERLIB_GPU_MESH_GPUBUFFER_H__
#define __RENDERLIB_GPU_MESH_GPUBUFFER_H__

#include <memory>

#include <GL/glew.h>

namespace RenderLib
{
	namespace GPU
	{
		namespace Mesh
		{
			class GPUBuffer
			{
			public:
				unsigned int vao;
				unsigned int indexBuffer;
				unsigned int dataBuffer;

				GLenum usage;

				GPUBuffer();
				~GPUBuffer();

				void generate();

				void bind();
				void unBind();

				void bindIndexBuffer();
				void unBindIndexBuffer();

				void bindDataBuffer();
				void unBindDataBuffer();

				void updateData(char * faceData, const size_t & faceSize, char * vertexData, const size_t & vertexSize);
			};

			typedef std::unique_ptr<GPUBuffer> GPUBufferPtr;
		}
	}
}

#endif