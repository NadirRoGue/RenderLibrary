#ifndef __RENDERLIB_CPU_IO_TEXTFILELOADRESULT_H__
#define __RENDERLIB_CPU_IO_TEXTFILELOADRESULT_H__

#include <vector>

#include "CPU/io/FileLoadResult.h"

namespace RenderLib
{
	namespace CPU
	{
		namespace IO
		{
			class TextFileLoadResult : public AbstractLoadResult
			{
			public:
				std::vector<char> text;
			public:
				size_t getResultSizeBytes();
			};
		}
	}
}

#endif