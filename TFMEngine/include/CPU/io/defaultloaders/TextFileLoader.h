#ifndef __RENDERLIB_CPU_IO_TEXTFILELOADER_H__
#define __RENDERLIB_CPU_IO_TEXTFILELOADER_H__

#include <vector>

#include "CPU/io/FileLoader.h"

namespace RenderLib
{
	namespace CPU
	{
		namespace IO
		{
			class TextFileLoader : public FileLoader
			{
			public:
				TextFileLoader();
				std::vector<AbstractLoadResultPtr> loadFile(const std::string & filePath, unsigned int options);
			};
		}
	}
}

#endif