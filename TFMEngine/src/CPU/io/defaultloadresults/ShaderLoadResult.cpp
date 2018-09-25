#include "CPU/io/defaultloadresults/ShaderLoadResult.h"

namespace RenderLib
{
	namespace CPU
	{
		namespace IO
		{
			size_t ShaderLoadResult::getResultSizeBytes()
			{
				//return text.size();
				return (header.length() + body.length());
			}
		}
	}
}