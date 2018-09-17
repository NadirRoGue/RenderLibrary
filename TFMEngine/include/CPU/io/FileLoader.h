#ifndef __RENDERLIB_CPU_FILELOADER_H__
#define __RENDERLIB_CPU_FILELOADER_H__

#include <vector>
#include <string>

#include "CPU/io/FileLoadResult.h"

namespace RenderLib
{
	namespace CPU
	{
		namespace IO
		{
			class FileLoader
			{
			protected:
				std::vector<std::string> supportedExtensions;
			public:
				void registerFileExtension(const std::string & fileExtension);
				const std::vector<std::string> & getSupportedExtensions();
				virtual std::vector<AbstractLoadResultPtr> loadFile(const std::string & filePath, unsigned int options) = 0;
			};
		}
	}
}

#endif