#ifndef __RENDERLIB_GPU_PROGRAM_PROGRAMMANAGER_H__
#define __RENDERLIB_GPU_PROGRAM_PROGRAMMANAGER_H__

#include <memory>
#include <unordered_map>
#include <vector>

#include <typeindex>
#include <typeinfo>

#include "GPU/program/Program.h"

#include "EngineException.h"

#include <iostream>

namespace RenderLib
{
	namespace GPU
	{
		namespace Program
		{
			typedef std::unordered_map<UberParamMask, std::unique_ptr<Program>> ProgramMap;

			class ProgramManager
			{
			private:
				std::unordered_map<std::type_index, ProgramMap> programList;
			public:
				ProgramManager();
				~ProgramManager();

				void clear();

				template<class T>
				T * getProgram(const UberParamMask & configMask = 0)
				{
					std::type_index id = typeid(T);

					if (!std::is_base_of<Program, T>::value)
					{
						throw new EngineException("ProgramManager: Attempted to create a program from the non program-derived class " + std::string(id.name()));
						return NULL;
					}
					
					T * result = NULL;

					ProgramMap & progMap = programList[id];
					auto it = progMap.find(configMask);
					if (it == progMap.end())
					{
						std::unique_ptr<Program> newProgram = std::make_unique<T>();
						newProgram.get()->init(configMask);
						result = static_cast<T*>(newProgram.get());
						progMap[configMask] = std::move(newProgram);

						std::cout << "Added " << id.name() << std::endl;
					}
					else
					{
						result = static_cast<T*>(it->second.get());
					}

					return result;
				}

				Program * findProgram(const std::type_index & programType, const UberParamMask & configMask = 0);
			};
		}
	}
}

#endif