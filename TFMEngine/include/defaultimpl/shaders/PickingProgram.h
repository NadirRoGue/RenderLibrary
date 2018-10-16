#ifndef __RENDERLIB_DEFAULTIMPL_PICKINGPROGRAM_H__
#define __RENDERLIB_DEFAULTIMPL_PICKINGPROGRAM_H__

#include "GPU/program/ShaderProgram.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		class PickingProgram : public GPU::Program::ShaderProgram
		{
		public:
			PickingProgram();
			~PickingProgram();

			void configureShaderAttributes(GPU::Mesh::GPUMesh * targetMesh);
			void onRenderObject(const SceneObject & object, const Material::Material & material, const Camera & camera);
		};
	}
}

#endif