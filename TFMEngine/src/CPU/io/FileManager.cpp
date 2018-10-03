#include "CPU/io/FileManager.h"

#include "EngineException.h"

#include <algorithm>

namespace RenderLib
{
	namespace CPU
	{
		namespace IO
		{
			std::unordered_map <std::string, std::shared_ptr<FileLoader>> FileManager::fileLoaders;

			AbstractLoadResultPtr FileManager::loadFile(const std::string & path, unsigned int options)
			{
				AbstractLoadResultPtr result;

				if (path.empty())
				{
					throw EngineException("FileManager::loadFile passed an empty path");
				}

				size_t dotPosition = path.find_last_of(".");

				std::string extension = path.substr(dotPosition + 1, path.length() - dotPosition);
				std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

				auto it = fileLoaders.find(extension);
				
				if (it == fileLoaders.end())
				{
					throw EngineException("FileManager::loadFile unsupported file extendion: " + extension);
				}

				FileLoader * loader = it->second.get();

				if (loader)
				{
					result = loader->loadFile(path, options);
				}
				else
				{
					throw EngineException("FileManager: NULL FileLoader registered for file extension " + extension);
				}

				return result;
			}
		}
	}
}