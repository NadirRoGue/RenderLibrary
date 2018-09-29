#include "GPU/program/ShaderLoadResult.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Program
		{
			size_t ShaderLoadResult::getResultSizeBytes()
			{
				//return text.size();
				return (header.length() + body.length());
			}
		}
	}
}