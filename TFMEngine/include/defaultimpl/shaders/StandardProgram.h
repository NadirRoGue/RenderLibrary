#ifndef __RENDERLIB_DEFAULTIMPL_STANDARDPROGRAM_H__
#define __RENDERLIB_DEFAULTIMPL_STANDARDPROGRAM_H__

#include "GPU/program/ShaderProgram.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		class StandardProgram : public GPU::Program::ShaderProgram
		{
		public:
			StandardProgram();
			~StandardProgram();

			void getUberShaderDefines(std::vector<std::string> & definesBuffer);

			void configureShaderAttributes(GPU::Mesh::GPUMesh * targetMesh);
			void onRenderObject(const SceneObject & object, const Camera & camera);
		};
	}
}

#endif