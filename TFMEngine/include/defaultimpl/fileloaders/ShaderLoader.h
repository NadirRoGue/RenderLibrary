#ifndef __RENDERLIB_DEFAULTIMPL_SHADERLOADER_H__
#define __RENDERLIB_DEFAULTIMPL_SHADERLOADER_H__

#include <vector>

#include "CPU/io/FileLoader.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		class ShaderLoader : public CPU::IO::FileLoader
		{
		public:
			ShaderLoader();
			std::vector<CPU::IO::AbstractLoadResultPtr> loadFile(const std::string & filePath, unsigned int options);
		};
	}
}

#endif