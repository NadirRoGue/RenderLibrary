#ifndef __RENDERLIB_GPU_PROGRAM_PROGRAMMANAGER_H__
#define __RENDERLIB_GPU_PROGRAM_PROGRAMMANAGER_H__

#include <memory>
#include <map>

#include <typeindex>
#include <typeinfo>

#include "GPU/program/Program.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Program
		{
			class ProgramManager
			{
			private:
				std::unique_ptr<ProgramManager> INSTANCE;
			public:
				static ProgramManager & getInstance();
			private:
				std::map<std::type_index, std::unique_ptr<ProgramFactory>> factories;
			};
		}
	}
}

#endif