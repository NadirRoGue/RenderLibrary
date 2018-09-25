#ifndef __RENDERLIB_CPU_IO_SHADERLOADER_H__
#define __RENDERLIB_CPU_IO_SHADERLOADER_H__

#include <vector>

#include "CPU/io/FileLoader.h"

namespace RenderLib
{
	namespace CPU
	{
		namespace IO
		{
			class ShaderLoader : public FileLoader
			{
			public:
				ShaderLoader();
				std::vector<AbstractLoadResultPtr> loadFile(const std::string & filePath, unsigned int options);
			};
		}
	}
}

#endif