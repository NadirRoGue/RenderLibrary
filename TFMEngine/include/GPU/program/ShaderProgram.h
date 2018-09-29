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
			protected:
				std::string vShaderF;
				std::string tcShaderF;
				std::string teShaderF;
				std::string geomShaderF;
				std::string fShaderF;
			private:
				unsigned int vShader;
				unsigned int tcShader;
				unsigned int teShader;
				unsigned int geomShader;
				unsigned int fShader;
			
				std::function<void(const SceneObject & obj, const Camera & cam)> renderCallback;
			public:
				ShaderProgram();
				~ShaderProgram();

				virtual void initialize(std::vector<std::string> & definesBuffer);
				
				void destroyShaders();

				virtual void onFrameBegin();
				virtual void onRenderObject(const SceneObject & object, const Camera & camera);
			};
		}
	}
}

#endif