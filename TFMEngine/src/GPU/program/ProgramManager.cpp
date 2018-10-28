#include "GPU/program/ProgramManager.h"

#include "logger/Log.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Program
		{
			std::unordered_map<std::type_index, std::unique_ptr<AbstractProgramFactory>> ProgramManager::programFactories;

			ProgramManager::ProgramManager()
			{

			}

			ProgramManager::~ProgramManager()
			{

			}

			Program * ProgramManager::getProgram(const UberParamMask configMask, const std::type_index programTypeId)
			{
				Program * result = nullptr;

				// If exists, return it
				auto programIt = programList[programTypeId].find(configMask);
				if (programIt != programList[programTypeId].end())
				{
					result = programIt->second.get();
				}
				// Otherwise,  create it
				else
				{
					
					auto factoryIt = programFactories.find(programTypeId);

					if (factoryIt == programFactories.end())
					{
						Logger::Log::getInstance().logWarning("ProgramManager: Attempted to create an unregistered program: " + std::string(programTypeId.name()));
						return NULL;
					}

					std::unique_ptr<Program> newProgram = factoryIt->second.get()->createProgram(configMask);
					result = newProgram.get();
					programList[programTypeId][configMask] = std::move(newProgram);
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