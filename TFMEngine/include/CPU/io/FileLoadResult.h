#ifndef __RENDERLIB_CPU_FILELOADRESULT_H__
#define __RENDERLIB_CPU_FILELOADRESULT_H__

namespace RenderLib
{
	namespace CPU
	{
		namespace IO
		{
			class AbstractLoadResult
			{
			public:
				virtual size_t getResultSizeBytes() = 0;
			};
		}
	}
}

#endif