#ifndef __RENDERLIB_GPU_PROGRAM_SHADERPROGRAM_H__
#define __RENDERLIB_GPU_PROGRAM_SHADERPROGRAM_H__

#include "GPU/program/Program.h"

#include "SceneObject.h"
#include "Camera.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Program
		{
			class ShaderProgram : public Program
			{
			public:
				std::string vShaderF;
				std::string tcShaderF;
				std::string teShaderF;
				std::string geomShaderF;
				std::string fShaderF;
			private:
				std::function<void(const SceneObject & obj, const Camera & cam)> renderCallback;
			public:
				ShaderProgram(const UberParamMask & params);
				~ShaderProgram();

				virtual void onRender(const SceneObject & object, const Camera & camera);
			};
		}
	}
}

#endif