#include "GPU/program/ShaderProgram.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Program
		{
			ShaderProgram::ShaderProgram(const UberParamMask & params)
				: Program(params)
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