#ifndef __RENDERLIB_DEFAULTIMPL_IMAGELOADER_H__
#define __RENDERLIB_DEFAULTIMPL_IMAGELOADER_H__

#include <vector>

#include "CPU/io/FileLoader.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		class ImageLoader : public CPU::IO::FileLoader
		{
		public:
			ImageLoader();
			CPU::IO::AbstractLoadResultPtr loadFile(const std::string & filePath, unsigned int options);
		};
	}
}

#endif