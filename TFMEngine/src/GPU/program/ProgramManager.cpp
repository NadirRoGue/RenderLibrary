#include "GPU/program/ProgramManager.h"

#include <GL/glew.h>

#include "CPU/io/FileManager.h"
#include "CPU/io/defaultloadresults/ShaderLoadResult.h"

#define VERSION_HEADER_LENGHT 17

namespace RenderLib
{
	namespace GPU
	{
		namespace Program
		{
			std::unique_ptr<ProgramManager> ProgramManager::INSTANCE = std::make_unique<ProgramManager>();

			ProgramManager & ProgramManager::getInstance()
			{
				return *(INSTANCE.get());
			}

			ProgramManager::ProgramManager()
			{

			}

			ProgramManager::~ProgramManager()
			{

			}

			void ProgramManager::initializeShader(Program * program)
			{

			}

			unsigned int ProgramManager::loadShaderFromFile(GLenum shaderType, const std::string & filePath, std::vector<std::string> & configStrings)
			{
				std::vector<CPU::IO::AbstractLoadResultPtr> fileloadResultVector = CPU::IO::FileManager::loadFile(filePath, 0);
				CPU::IO::ShaderLoadResult * loadResult = dynamic_cast<CPU::IO::ShaderLoadResult*>(fileloadResultVector[0].get());

				size_t fileLen = loadResult->getResultSizeBytes() - 1;

				std::string result(&loadResult->text[0]);

				if (!configStrings.empty())
				{
					//std::string header = result.substr(0, VERSION_HEADER_LENGHT);
					//std::string body = result.substr(VERSION_HEADER_LENGHT, result.size() - VERSION_HEADER_LENGHT);
					std::string defines;

					for (auto & cnfgStr : configStrings)
					{
						defines += "#define " + cnfgStr + "\n";
					}
					result = loadResult->header + "\n" + defines + "\n" + loadResult->body;
				}

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
					char *logString = new char[logLen];
					glGetShaderInfoLog(shader, logLen, NULL, logString);
					std::string message = "ProgramManager: Error while loading shader " + filePath + ": " + std::string(logString);
					delete[] logString;
					glDeleteShader(shader);
					throw EngineException(message.c_str());
				}

				return shader;
			}
		}
	}
}