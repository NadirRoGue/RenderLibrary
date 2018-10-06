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

			Program * ProgramManager::findProgram(const std::type_index & programType, const UberParamMask & configMask)
			{
				auto outerIt = programList.find(programType);
				if (outerIt != programList.end())
				{
					auto innerIt = outerIt->second.find(configMask);

					if (innerIt != outerIt->second.end())
					{
						return innerIt->second.get();
					}
				}

				return NULL;
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