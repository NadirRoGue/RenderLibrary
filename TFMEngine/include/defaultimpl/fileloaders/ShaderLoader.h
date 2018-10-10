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
			CPU::IO::AbstractLoadResultPtr loadFile(const std::string & filePath, unsigned int options);
		private:
			void handleInclude(const std::string & include, std::string & dst);
		};
	}
}

#endif