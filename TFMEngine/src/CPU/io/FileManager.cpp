#include "CPU/io/FileManager.h"

#include <iostream>

namespace RenderLib
{
	namespace CPU
	{
		namespace IO
		{
			std::map <std::string, FileLoader*> FileManager::fileLoaders;

			void FileManager::registerFileLoader(FileLoader * loaderInstance)
			{
				if (!loaderInstance)
				{
					std::cerr << "FileManager: NULL FileLoader instance passed to registration" << std::endl;
					return;
				}

				for (auto & extension : loaderInstance->getSupportedExtensions())
				{
					auto it = fileLoaders.find(extension);
					if (it != fileLoaders.end() && it->second != NULL)
					{
						std::cerr << "FileManager: [Warning] Overwritted file loader "
							<< typeid(it->second).name()
							<< " for "
							<< typeid(loaderInstance).name()
							<< " for extension \""
							<< extension << "\""
							<< std::endl;

						delete it->second;
						it->second = loaderInstance;
					}
					else
					{
						fileLoaders[extension] = loaderInstance;
					}
				}
			}

			std::vector<AbstractLoadResultPtr> FileManager::loadFile(const std::string & path, unsigned int options)
			{
				std::vector<AbstractLoadResultPtr> result;

				if (path.empty())
				{
					throw std::runtime_error("FileManager::loadFile passed an empty path");
				}

				size_t dotPosition = path.find_last_of(".");

				std::string extension = path.substr(dotPosition + 1, path.length() - dotPosition);

				auto it = fileLoaders.find(extension);

				if (it == fileLoaders.end())
				{
					throw std::runtime_error("FileManager::loadFile unsupported file extendion: " + extension);
				}

				FileLoader * loader = it->second;

				if (loader)
				{
					result = loader->loadFile(path, options);
				}
				else
				{
					throw std::runtime_error("FileManager: NULL FileLoader registered for file extension " + extension);
				}

				return std::move(result);
			}
		}
	}
}