#include "GPU/program/ProgramManager.h"

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