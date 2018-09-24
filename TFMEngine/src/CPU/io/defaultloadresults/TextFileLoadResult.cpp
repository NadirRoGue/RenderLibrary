#include "CPU/io/defaultloadresults/TextFileLoadResult.h"

namespace RenderLib
{
	namespace CPU
	{
		namespace IO
		{
			size_t TextFileLoadResult::getResultSizeBytes()
			{
				return text.size();
			}
		}
	}
}