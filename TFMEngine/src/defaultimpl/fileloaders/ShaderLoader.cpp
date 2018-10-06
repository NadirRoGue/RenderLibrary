#include "defaultimpl/fileloaders/ShaderLoader.h"

#include <fstream>

#include "EngineException.h"

#include "GPU/program/ShaderLoadResult.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		ShaderLoader::ShaderLoader()
		{
			// GLSL does not enforce file extension
			// However: https://www.khronos.org/opengles/sdk/tools/Reference-Compiler/
			registerFileExtension("vert");
			registerFileExtension("tesc");
			registerFileExtension("tese");
			registerFileExtension("geom");
			registerFileExtension("frag");
			registerFileExtension("comp");
		}

		CPU::IO::AbstractLoadResultPtr ShaderLoader::loadFile(
			const std::string & filePath, 
			unsigned int options)
		{
			std::ifstream file;
			file.open(filePath, std::ios::in);
			if (!file)
			{
				std::string message = 
					"ShaderLoader: Could not load file " + filePath + ": file not found";

				throw EngineException(message.c_str());
			}

			std::unique_ptr<GPU::Program::ShaderLoadResult> loadResultPtr = 
				std::make_unique<GPU::Program::ShaderLoadResult>();
			GPU::Program::ShaderLoadResult * loadResult = loadResultPtr.get();

			file.seekg(0, std::ios::end);
			size_t fileLen = file.tellg();
			file.seekg(std::ios::beg);

			char buf[1024];

			if (file.good())
			{
				file.getline(buf, 1024);
				size_t rawStrLen = strlen(buf);
				fileLen += rawStrLen;
				loadResult->header = std::string(buf);
			}

			if (!file.eof())
			{
				while (file.good())
				{
					file.getline(buf, 1024);
					size_t rawStrLen = strlen(buf);
					fileLen += rawStrLen;
					loadResult->body += std::string(buf) + "\n";
				}
			}

			loadResult->body += std::string("\0");

			file.close();

			return loadResultPtr;
		}
	}
}