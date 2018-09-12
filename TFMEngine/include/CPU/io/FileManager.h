#ifndef __RENDERLIB_CPU_FILEMANAGER_H__
#define __RENDERLIB_CPU_FILEMANAGER_H__

#include <map>

#include "CPU/io/FileLoader.h"

namespace RenderLib
{
	namespace CPU
	{
		namespace IO
		{
			class FileManager
			{
			private:
				static std::map<std::string, FileLoader *> fileLoaders;
			public:
				static void registerFileLoader(FileLoader * loaderInstance);
				static std::vector<AbstractLoadResult *> loadFile(const std::string & filePath, unsigned int options);
			};
		}
	}
}

#endif