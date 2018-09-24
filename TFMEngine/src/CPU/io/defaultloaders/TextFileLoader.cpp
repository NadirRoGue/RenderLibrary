#include "CPU/io/defaultloaders/TextFileLoader.h"

#include <fstream>

#include "EngineException.h"

#include "CPU/io/defaultloadresults/TextFileLoadResult.h"

namespace RenderLib
{
	namespace CPU
	{
		namespace IO
		{
			TextFileLoader::TextFileLoader()
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

			std::vector<AbstractLoadResultPtr> TextFileLoader::loadFile(const std::string & filePath, unsigned int options)
			{
				std::ifstream file;
				file.open(filePath, std::ios::in);
				if (!file)
				{
					std::string message = "TextFileLoader: Could not load file " + filePath + ": file not found";
					throw EngineException(message.c_str());
				}

				std::vector<AbstractLoadResultPtr> result;
				std::unique_ptr<TextFileLoadResult> loadResultPtr = std::make_unique<TextFileLoadResult>();
				TextFileLoadResult * loadResult = loadResultPtr.get();

				file.seekg(0, std::ios::end);
				size_t fileLen = file.tellg();
				file.seekg(std::ios::beg);

				loadResult->text.resize(fileLen + 1);

				int i = 0;
				while (file.good())
				{
					loadResult->text[i] = char(file.get());
					if (!file.eof()) i++;
					else fileLen = i;
				}

				loadResult->text[fileLen] = '\0';
				file.close();

				result.push_back(std::move(loadResultPtr));

				return result;
			}
		}
	}
}