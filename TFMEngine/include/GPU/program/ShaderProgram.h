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
			typedef struct ShaderProgramParams : public ProgramParams
			{
				std::string vertShaderFile;
				std::string tesCntrlShaderFile;
				std::string tesEvalShaderFile;
				std::string geomShaderFile;
				std::string fragmentShaderFile;

				std::function<void(const SceneObject & obj, const Camera & cam)> renderCallback;

				ShaderProgramParams()
					: ProgramParams()
				{
				}

			}ShaderProgramParams;

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
				ShaderProgram(const ShaderProgramParams & params);
				~ShaderProgram();

				virtual void onRender(const SceneObject & object, const Camera & camera);
			};
		}
	}
}

#endif