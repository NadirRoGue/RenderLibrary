#ifndef __RENDERLIB_DEFAULTIMPL_PNGLOADER_H__
#define __RENDERLIB_DEFAULTIMPL_PNGLOADER_H__

#include <vector>

#include "CPU/io/FileLoader.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		class PNGLoader : public CPU::IO::FileLoader
		{
		public:
			PNGLoader();
			CPU::IO::AbstractLoadResultPtr loadFile(const std::string & filePath, unsigned int options);
		};
	}
}

#endif