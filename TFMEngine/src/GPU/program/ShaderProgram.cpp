#include "GPU/program/ShaderProgram.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Program
		{
			ShaderProgram::ShaderProgram(const ShaderProgramParams & params)
				: Program(params)
				, vShaderF(params.vertShaderFile)
				, tcShaderF(params.tesCntrlShaderFile)
				, teShaderF(params.tesEvalShaderFile)
				, geomShaderF(params.geomShaderFile)
				, fShaderF(params.fragmentShaderFile)
				, renderCallback(params.renderCallback)
			{
			}

			ShaderProgram::~ShaderProgram()
			{
			}

			void ShaderProgram::onRender(const SceneObject & object, const Camera & camera)
			{
				if (renderCallback)
				{
					renderCallback(object, camera);
				}
			}
		}
	}
}