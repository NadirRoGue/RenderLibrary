#include "GPU/program/ShaderProgram.h"

#include "EngineException.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Program
		{
			ShaderProgram::ShaderProgram()
				: Program()
				, vShader(-1)
				, tcShader(-1)
				, teShader(-1)
				, geomShader(-1)
				, fShader(-1)
			{
			}

			ShaderProgram::~ShaderProgram()
			{
			}

			void ShaderProgram::initialize(std::vector<std::string> & definesBuffer)
			{
				if (vShaderF.empty() || fShaderF.empty())
				{
					throw EngineException("ShaderProgram: A minimun of a vertex shader and a fragment shader are required to create a ShaderProgram");
				}

				if (tcShaderF.empty() != teShaderF.empty())
				{
					throw EngineException("ShaderProgram: Tesselation stage requires both shaders files(control and evaluation)");
				}

				vShader = loadShaderFromFile(GL_VERTEX_SHADER, vShaderF, definesBuffer);
				fShader = loadShaderFromFile(GL_FRAGMENT_SHADER, fShaderF, definesBuffer);

				if (!tcShaderF.empty() && !teShaderF.empty())
				{
					tcShader = loadShaderFromFile(GL_TESS_CONTROL_SHADER, tcShaderF, definesBuffer);
					teShader = loadShaderFromFile(GL_TESS_EVALUATION_SHADER, teShaderF, definesBuffer);
				}

				if (!geomShaderF.empty())
				{
					geomShader = loadShaderFromFile(GL_GEOMETRY_SHADER, geomShaderF, definesBuffer);
				}

				attachShader(vShader);
				attachShader(tcShader);
				attachShader(teShader);
				attachShader(geomShader);
				attachShader(fShader);

				link();
			}

			void ShaderProgram::destroyShaders()
			{
				detachShader(vShader);
				detachShader(tcShader);
				detachShader(teShader);
				detachShader(geomShader);
				detachShader(fShader);
			}

			void ShaderProgram::onFrameBegin(EngineInstance * instance)
			{
				
			}

			void ShaderProgram::onRenderObject(const SceneObject & object, const Material::Material & material, const Camera & camera)
			{
				
			}

			void ShaderProgram::configureShaderAttribute(const std::string & shaderAttribute, Mesh::GPUAttribute & meshAttribute)
			{
				if (meshAttribute.numElements > 0)
				{
					auto it = shaderAttributes.find(shaderAttribute);
					if (it != shaderAttributes.end())
					{
						ShaderInput input = it->second;
						glVertexAttribPointer(input.id,
							(GLsizei)meshAttribute.elementCount,
							meshAttribute.dataType,
							meshAttribute.normalizeOnUpload,
							(GLsizei)(meshAttribute.stride),
							(GLvoid*)(meshAttribute.offset));

						glEnableVertexAttribArray(input.id);
					}
				}
			}
		}
	}
}