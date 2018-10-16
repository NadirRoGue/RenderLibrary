#include "GPU/program/PostProcessProgram.h"

#include "EngineException.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Program
		{
			PostProcessProgram::PostProcessProgram()
				: Program()
				, vShader(-1)
				, fShader(-1)
			{
				vShaderFile = "shaders/default/postprocess.vert";
			}

			PostProcessProgram::~PostProcessProgram()
			{

			}

			void PostProcessProgram::initialize(std::vector<std::string> & definesBuffer)
			{
				if (fShaderFile.empty())
				{
					throw EngineException("PostProcessProgram: A fragment shader file must be specified");
				}

				vShader = loadShaderFromFile(GL_VERTEX_SHADER, vShaderFile, definesBuffer);
				fShader = loadShaderFromFile(GL_FRAGMENT_SHADER, fShaderFile, definesBuffer);

				attachShader(vShader);
				attachShader(fShader);

				link();
			}

			void PostProcessProgram::configureRenderQuad(GPU::Mesh::GPUMesh * quad)
			{
				configureShaderAttribute("inPos", quad->vertices);
				configureShaderAttribute("inUV", quad->uvs[0]);
			}

			void PostProcessProgram::destroyShaders()
			{
				detachShader(vShader);
				detachShader(fShader);
			}

			void PostProcessProgram::initializeShader(EngineInstance * instance)
			{

			}

			void PostProcessProgram::onRender(GPU::Texture::GPUTexture * previousOutput, EngineInstance * instance)
			{

			}
		}
	}
}