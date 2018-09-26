#include "GPU/program/ProgramManager.h"

#include <GL/glew.h>

#include "CPU/io/FileManager.h"
#include "CPU/io/defaultloadresults/ShaderLoadResult.h"

#define VERSION_HEADER_LENGHT 17

namespace RenderLib
{
	namespace GPU
	{
		namespace Program
		{
			std::unique_ptr<ProgramManager> ProgramManager::INSTANCE = std::make_unique<ProgramManager>();

			ProgramManager & ProgramManager::getInstance()
			{
				return *(INSTANCE.get());
			}

			ProgramManager::ProgramManager()
			{

			}

			ProgramManager::~ProgramManager()
			{

			}
		}
	}
}