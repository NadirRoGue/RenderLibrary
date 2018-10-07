#include "GPU/program/Program.h"

#include <GL/glew.h>

#include "CPU/io/FileManager.h"
#include "GPU/program/ShaderLoadResult.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Program
		{
			inline void castDoubleToFloatArray(
				const size_t & count, 
				const double * array, 
				std::vector<float> & buffer)
			{
				buffer.resize(count);
				size_t i = 0;
				while (i < count)
				{
					buffer[i] = static_cast<float>(array[i]);
					i++;
				}
			}

			Program::Program()
			{
			}

			Program::~Program()
			{
				
			}

			const UberParamMask & Program::getConfigMask()
			{
				return configMask;
			}

			void Program::destroyShaders()
			{
			}

			void Program::destroy()
			{
				destroyShaders();
				glDeleteProgram(programId);
				programId = -1;
			}

			void Program::init(const UberParamMask & mask)
			{
				configMask = mask;

				std::vector<std::string> defineParams;
				getUberShaderDefines(defineParams);

				programId = glCreateProgram();

				initialize(defineParams);

				gatherInputs();
			}

			void Program::attachShader(unsigned int shaderId)
			{
				if (shaderId != -1)
				{
					glAttachShader(programId, shaderId);
				}
			}

			void Program::detachShader(unsigned int shaderId, bool deleteShader)
			{
				if (shaderId != -1)
				{
					glDetachShader(programId, shaderId);
					if (deleteShader)
					{
						glDeleteShader(shaderId);
					}
				}
			}

			void Program::link()
			{
				glLinkProgram(programId);

				int linked;
				glGetProgramiv(programId, GL_LINK_STATUS, &linked);
				if (!linked)
				{
					GLint logLen;
					glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLen);
					std::vector<char> logString(logLen);
					glGetProgramInfoLog(programId, logLen, NULL, &logString[0]);
					throw EngineException(&logString[0]);
				}
			}

			unsigned int Program::loadShaderFromFile(
				GLenum shaderType,
				const std::string & filePath,
				std::vector<std::string> & configStrings)
			{
				CPU::IO::AbstractLoadResultPtr fileloadResult =
					CPU::IO::FileManager::loadFile(filePath, 0);
				GPU::Program::ShaderLoadResult * loadResult = 
					dynamic_cast<GPU::Program::ShaderLoadResult*>(fileloadResult.get());

				size_t fileLen = loadResult->getResultSizeBytes() - 1;

				//std::string result;

				//if (!configStrings.empty())
				//{
					//std::string header = result.substr(0, VERSION_HEADER_LENGHT);
					//std::string body = result.substr(VERSION_HEADER_LENGHT, result.size() - VERSION_HEADER_LENGHT);
					std::string defines;

					for (auto & cnfgStr : configStrings)
					{
						defines += "#define " + cnfgStr + "\n";
					}
					std::string result = loadResult->header + "\n" + defines + "\n" + loadResult->body;
				//}

				char * finalSourceCStr = new char[result.size()];
				memcpy(finalSourceCStr, result.c_str(), result.size() * sizeof(char));
				fileLen = result.size();

				GLuint shader;
				shader = glCreateShader(shaderType);
				glShaderSource(shader, 1, (const GLchar **)&finalSourceCStr, (const GLint *)&fileLen);
				glCompileShader(shader);

				delete[] finalSourceCStr;

				GLint compiled;
				glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
				if (!compiled)
				{
					GLint logLen;
					glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
					std::vector<char> logString(logLen);
					glGetShaderInfoLog(shader, logLen, NULL, &logString[0]);
					glDeleteShader(shader);
					throw EngineException(&logString[0]);
				}

				return shader;
			}

			void Program::gatherInputs()
			{
				GLint i = 0;
				char nameBuffer[0xff];
				GLsizei nameLen = 0;
				GLint size = 0;
				GLenum type;

				GLint activeUniforms = 0;
				glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &activeUniforms);
				while (i < activeUniforms)
				{
					glGetActiveUniform(programId, i, 0xff, &nameLen, &size, &type, nameBuffer);

					std::string nameStr(nameBuffer);
					ShaderInput input;
					input.id = i;
					input.size = size;
					input.type = type;

					shaderUniforms[nameStr] = input;
					i++;
				}

				GLint activeAttributes = 0;
				glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTES, &activeAttributes);

				i = 0;
				while (i < activeAttributes)
				{
					glGetActiveAttrib(programId, i, 0xff, &nameLen, &size, &type, nameBuffer);

					std::string nameStr(nameBuffer);
					ShaderInput input;
					// A WHOLE F*CKING AFTERNOON LOST BECAUSE THE REAL BINDING IS NOT THE QUERIED ON glGetActiveAttrib
					input.id = glGetAttribLocation(programId, nameBuffer);
					input.size = size;
					input.type = type;

					shaderAttributes[nameStr] = input;
					i++;
				}

				GLint activeBlocks = 0;
				glGetProgramiv(programId, GL_ACTIVE_UNIFORM_BLOCKS, &activeBlocks);
				i = 0;
				while (i < activeBlocks)
				{
					ShaderInput newBlock;

					glGetActiveUniformBlockiv(programId, i, GL_UNIFORM_NAME_LENGTH, &nameLen);
					glGetActiveUniformBlockName(programId, i, 0xff, NULL, nameBuffer);

					newBlock.id = glGetUniformBlockIndex(programId, nameBuffer);
					
					std::string nameStr(nameBuffer);
					shaderBlocks[nameStr] = newBlock;

					i++;
				}
			}

			void Program::getUberShaderDefines(std::vector<std::string> & definesBuffer)
			{

			}

			void Program::bind()
			{
				glUseProgram(programId);
			}

			void Program::unBind()
			{
				glUseProgram(0);
			}

			bool Program::hasMask(const UberParamMask & prop)
			{
				return (configMask & prop) == prop;
			}

			void Program::setUniformI(const std::string & name, const int & val)
			{
				auto it = shaderUniforms.find(name);
				if (it == shaderUniforms.end())
				{
					return;
				}
				unsigned int id = it->second.id;

				glUniform1i(id, val);
			}

			void Program::setUniformI2(const std::string & name, const int & v1, const int & v2)
			{
				auto it = shaderUniforms.find(name);
				if (it == shaderUniforms.end())
				{
					return;
				}
				unsigned int id = it->second.id;

				glUniform2i(id, v1, v2);
			}

			void Program::setUniformI3(const std::string & name, const int & v1, const int & v2, const int & v3)
			{
				auto it = shaderUniforms.find(name);
				if (it == shaderUniforms.end())
				{
					return;
				}
				unsigned int id = it->second.id;

				glUniform3i(id, v1, v2, v3);
			}

			void Program::setUniformI4(const std::string & name, const int & v1, const int & v2, const int & v3, const int & v4)
			{
				auto it = shaderUniforms.find(name);
				if (it == shaderUniforms.end())
				{
					return;
				}
				unsigned int id = it->second.id;

				glUniform4i(id, v1, v2, v3, v4);
			}

			void Program::setUniformI2V(const std::string & name, const unsigned int & count, const int * val)
			{
				auto it = shaderUniforms.find(name);
				if (it == shaderUniforms.end())
				{
					return;
				}
				unsigned int id = it->second.id;

				glUniform2iv(id, count, val);
			}

			void Program::setUniformI3V(const std::string & name, const unsigned int & count, const int * val)
			{
				auto it = shaderUniforms.find(name);
				if (it == shaderUniforms.end())
				{
					return;
				}
				unsigned int id = it->second.id;

				glUniform3iv(id, count, val);
			}

			void Program::setUniformI4V(const std::string & name, const unsigned int & count, const int * val)
			{
				auto it = shaderUniforms.find(name);
				if (it == shaderUniforms.end())
				{
					return;
				}
				unsigned int id = it->second.id;

				glUniform4iv(id, count, val);
			}

			void Program::setUniformF(const std::string & name, const FLOAT & val)
			{
				auto it = shaderUniforms.find(name);
				if (it == shaderUniforms.end())
				{
					return;
				}
				unsigned int id = it->second.id;

#ifdef USE_DOUBLE_PRECISSION_ON_SHADERS__
				glUniform1d(id, val);
#else
#ifdef USE_DOUBLE_PRECISSION__
				glUniform1f(id, static_cast<float>(val));
#else
				glUniform1f(id, val);
#endif
#endif
			}

			void Program::setUniform2F(const std::string & name, const FLOAT & v1, const FLOAT & v2)
			{
				auto it = shaderUniforms.find(name);
				if (it == shaderUniforms.end())
				{
					return;
				}
				unsigned int id = it->second.id;

#ifdef USE_DOUBLE_PRECISSION_ON_SHADERS__
				glUniform2d(id, v1, v2);
#else
#ifdef USE_DOUBLE_PRECISSION__
				glUniform2f(id, static_cast<float>(v1), static_cast<float>(v2));
#else
				glUniform2f(id, v1, v2);
#endif
#endif
			}

			void Program::setUniform3F(const std::string & name, const FLOAT & v1, const FLOAT & v2, const FLOAT & v3)
			{
				auto it = shaderUniforms.find(name);
				if (it == shaderUniforms.end())
				{
					return;
				}
				unsigned int id = it->second.id;

#ifdef USE_DOUBLE_PRECISSION_ON_SHADERS__
				glUniform3d(id, v1, v2, v3);
#else
#ifdef USE_DOUBLE_PRECISSION__
				glUniform3f(id, static_cast<float>(v1), static_cast<float>(v2), static_cast<float>(v3));
#else
				glUniform3f(id, v1, v2, v3);
#endif
#endif
			}

			void Program::setUniform4F(const std::string & name, const FLOAT & v1, const FLOAT & v2, const FLOAT & v3, const FLOAT & v4)
			{
				auto it = shaderUniforms.find(name);
				if (it == shaderUniforms.end())
				{
					return;
				}
				unsigned int id = it->second.id;

#ifdef USE_DOUBLE_PRECISSION_ON_SHADERS__
				glUniform4d(id, v1, v2, v3, v4);
#else
#ifdef USE_DOUBLE_PRECISSION__
				glUniform4f(id, static_cast<float>(v1), static_cast<float>(v2), static_cast<float>(v3), static_cast<float>(v4));
#else
				glUniform4f(id, v1, v2, v3, v4);
#endif
#endif
			}

			void Program::setUniform2FV(const std::string & name, const unsigned int & count, const FLOAT * val)
			{
				auto it = shaderUniforms.find(name);
				if (it == shaderUniforms.end())
				{
					return;
				}
				unsigned int id = it->second.id;
				
#if defined USE_DOUBLE_PRECISSION__ && defined USE_DOUBLE_PRECISSION_ON_SHADERS__
				glUniform2dv(id, count, val);
#else
#ifdef USE_DOUBLE_PRECISSION__
				std::vector<float> buffer;
				castDoubleToFloatArray(count * 2, val, buffer);
				glUniform2fv(id, count, &buffer[0]);
#else
				glUniform2fv(id, count, val);
#endif
#endif
			}

			void Program::setUniform3FV(const std::string & name, const unsigned int & count, const FLOAT * val)
			{
				auto it = shaderUniforms.find(name);
				if (it == shaderUniforms.end())
				{
					return;
				}
				unsigned int id = it->second.id;

#if defined USE_DOUBLE_PRECISSION__ && defined USE_DOUBLE_PRECISSION_ON_SHADERS__
				glUniform3dv(id, count, val);
#else
#ifdef USE_DOUBLE_PRECISSION__
				std::vector<float> buffer;
				castDoubleToFloatArray(count * 3, val, buffer);
				glUniform3fv(id, count, &buffer[0]);
#else
				glUniform3fv(id, count, val);
#endif
#endif
			}

			void Program::setUniform4FV(const std::string & name, const unsigned int & count, const FLOAT * val)
			{
				auto it = shaderUniforms.find(name);
				if (it == shaderUniforms.end())
				{
					return;
				}
				unsigned int id = it->second.id;

#if defined USE_DOUBLE_PRECISSION__ && defined USE_DOUBLE_PRECISSION_ON_SHADERS__
				glUniform4dv(id, count, val);
#else
#ifdef USE_DOUBLE_PRECISSION__
				std::vector<float> buffer;
				castDoubleToFloatArray(count * 4, val, buffer);
				glUniform4fv(id, count, &buffer[0]);
#else
				glUniform4fv(id, count, val);
#endif
#endif
			}

			void Program::setUniformUI(const std::string & name, const unsigned int & val)
			{
				auto it = shaderUniforms.find(name);
				if (it == shaderUniforms.end())
				{
					return;
				}
				unsigned int id = it->second.id;

				glUniform1ui(id, val);
			}

			void Program::setUniformUI2(const std::string & name, const unsigned int & v1, const unsigned int & v2)
			{
				auto it = shaderUniforms.find(name);
				if (it == shaderUniforms.end())
				{
					return;
				}
				unsigned int id = it->second.id;

				glUniform2ui(id, v1, v2);
			}

			void Program::setUniformUI3(const std::string & name, const unsigned int & v1, const unsigned int & v2, const unsigned int & v3)
			{
				auto it = shaderUniforms.find(name);
				if (it == shaderUniforms.end())
				{
					return;
				}
				unsigned int id = it->second.id;

				glUniform3ui(id, v1, v2, v3);
			}

			void Program::setUniformUI4(const std::string & name, const unsigned int & v1, const unsigned int & v2, const unsigned int & v3, const unsigned int & v4)
			{
				auto it = shaderUniforms.find(name);
				if (it == shaderUniforms.end())
				{
					return;
				}
				unsigned int id = it->second.id;

				glUniform4ui(id, v1, v2, v3, v4);
			}

			void Program::setUniformUI2V(const std::string & name, const unsigned int & count, const unsigned int * val)
			{
				auto it = shaderUniforms.find(name);
				if (it == shaderUniforms.end())
				{
					return;
				}
				unsigned int id = it->second.id;

				glUniform2uiv(id, count, val);
			}

			void Program::setUniformUI3V(const std::string & name, const unsigned int & count, const unsigned int * val)
			{
				auto it = shaderUniforms.find(name);
				if (it == shaderUniforms.end())
				{
					return;
				}
				unsigned int id = it->second.id;

				glUniform3uiv(id, count, val);
			}

			void Program::setUniformUI4V(const std::string & name, const unsigned int & count, const unsigned int * val)
			{
				auto it = shaderUniforms.find(name);
				if (it == shaderUniforms.end())
				{
					return;
				}
				unsigned int id = it->second.id;

				glUniform4uiv(id, count, val);
			}

			void Program::setUniformMatrix2(const std::string & name, const unsigned int & count, const bool & transpose, const FLOAT * val)
			{
				auto it = shaderUniforms.find(name);
				if (it == shaderUniforms.end())
				{
					return;
				}
				unsigned int id = it->second.id;

#if defined USE_DOUBLE_PRECISSION__ && defined USE_DOUBLE_PRECISSION_ON_SHADERS__
				glUniformMatrix2dv(id, count, transpose, val);
#else
#ifdef USE_DOUBLE_PRECISSION__
				std::vector<float> buffer;
				castDoubleToFloatArray(count * 2 * 2, val, buffer);
				glUniformMatrix2fv(id, count, transpose, &buffer[0]);
#else
				glUniformMatrix2fv(id, count, transpose, val);
#endif
#endif
			}

			void Program::setUniformMatrix3(const std::string & name, const unsigned int & count, const bool & transpose, const FLOAT * val)
			{
				auto it = shaderUniforms.find(name);
				if (it == shaderUniforms.end())
				{
					return;
				}
				unsigned int id = it->second.id;

#if defined USE_DOUBLE_PRECISSION__ && defined USE_DOUBLE_PRECISSION_ON_SHADERS__
				glUniformMatrix3dv(id, count, transpose, val);
#else
#ifdef USE_DOUBLE_PRECISSION__
				std::vector<float> buffer;
				castDoubleToFloatArray(count * 3 * 3, val, buffer);
				glUniformMatrix3fv(id, count, transpose, &buffer[0]);
#else
				glUniformMatrix3fv(id, count, transpose, val);
#endif
#endif
			}

			void Program::setUniformMatrix4(const std::string & name, const unsigned int & count, const bool & transpose, const FLOAT * val)
			{
				auto it = shaderUniforms.find(name);
				if (it == shaderUniforms.end())
				{
					return;
				}
				unsigned int id = it->second.id;

#if defined USE_DOUBLE_PRECISSION__ && defined USE_DOUBLE_PRECISSION_ON_SHADERS__
				glUniformMatrix4dv(id, count, transpose, val);
#else
#ifdef USE_DOUBLE_PRECISSION__
				std::vector<float> buffer;
				castDoubleToFloatArray(count * 4 * 4, val, buffer);
				glUniformMatrix4fv(id, count, transpose, &buffer[0]);
#else
				glUniformMatrix4fv(id, count, transpose, val);
#endif
#endif
			}

			void Program::setUniformTexture(const std::string & name, Texture::GPUTexture * texture, unsigned int & textureUnit)
			{
				auto it = shaderUniforms.find(name);
				if (it == shaderUniforms.end())
				{
					return;
				}

				if (texture == NULL)
				{
					throw EngineException("Program " 
						+ std::string(typeid(*this).name()) 
						+ "[" + std::to_string(configMask) 
						+ "] expected texture " 
						+ name 
						+ ", but passed texture is NULL");

					return;
				}

				unsigned int id = it->second.id;

				glActiveTexture(GL_TEXTURE0 + textureUnit);
				texture->bind();
				glUniform1i(id, textureUnit);

				textureUnit++;
			}

			void Program::setUniformBlock(const std::string & name, const unsigned int & ubo)
			{
				auto it = shaderBlocks.find(name);
				if (it == shaderBlocks.end())
				{
					return;
				}

				glBindBufferBase(GL_UNIFORM_BUFFER, it->second.id, ubo);
			}
		}
	}
}