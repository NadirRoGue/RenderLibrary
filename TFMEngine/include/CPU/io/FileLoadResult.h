#ifndef __RENDERLIB_CPU_FILELOADRESULT_H__
#define __RENDERLIB_CPU_FILELOADRESULT_H__

#include <memory>

namespace RenderLib
{
	namespace CPU
	{
		namespace IO
		{
			template<class T>
			class LoadedParameter
			{
			private:
				T value;
				bool exists;
			public:
				LoadedParameter()
					: exists(false)
				{
				}

				void set(const T & newVal)
				{
					value = newVal;
					exists = true;
				}

				const T & get()
				{
					return value;
				}

				const T & operator()()
				{
					return value;
				}
			};

			class AbstractLoadResult
			{
			public:
				virtual size_t getResultSizeBytes() = 0;
			};

			typedef std::unique_ptr<AbstractLoadResult> AbstractLoadResultPtr;
		}
	}
}

#endif