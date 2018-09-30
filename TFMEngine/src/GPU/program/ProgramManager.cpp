#include "GPU/program/ProgramManager.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Program
		{
			ProgramManager::ProgramManager()
			{

			}

			ProgramManager::~ProgramManager()
			{

			}

			void ProgramManager::clear()
			{
				auto it = programList.begin();
				while (it != programList.end())
				{
					auto innerIt = it->second.begin();
					while (innerIt != it->second.end())
					{
						innerIt->second.get()->destroy();
						innerIt++;
					}
					it++;
				}
			}
		}
	}
}