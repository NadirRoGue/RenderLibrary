#include "GPU/program/ProgramManager.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Program
		{
			std::unordered_map<std::type_index, std::unique_ptr<UberMaskFactory>> ProgramManager::uberFactories;

			ProgramManager::ProgramManager()
			{

			}

			ProgramManager::~ProgramManager()
			{

			}

			Program * ProgramManager::getProgram(std::type_index programType, UberFactoryParam * param)
			{
				UberParamMask configMask = 0;
				auto factoryIt = uberFactories.find(programType);
				if (factoryIt != uberFactories.end())
				{
					configMask = factoryIt->second.get()->computeUberMask(param);
				}

				Program * result = NULL;

				ProgramMap & progMap = programList[programType];
				auto it = progMap.find(configMask);
				if (it == progMap.end())
				{
					std::unique_ptr<Program> newProgram = std::make_unique<T>();
					newProgram.get()->init(configMask);
					result = newProgram.get();
					progMap[configMask] = std::move(newProgram);
				}
				else
				{
					result = it->second.get();
				}

				return result;
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