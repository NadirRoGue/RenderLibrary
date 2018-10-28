#ifndef __RENDERLIB_GPU_PROGRAM_PROGRAMMANAGER_H__
#define __RENDERLIB_GPU_PROGRAM_PROGRAMMANAGER_H__

#include <memory>
#include <unordered_map>
#include <vector>

#include <typeindex>
#include <typeinfo>

#include "GPU/program/Program.h"

#include "EngineException.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Program
		{
			class AbstractProgramFactory
			{
			public:
				virtual std::unique_ptr<Program> createProgram(UberParamMask mask) = 0;
			};

			template<class ProgramType>
			class ProgramFactory : public AbstractProgramFactory
			{
			public:
				std::unique_ptr<Program> createProgram(UberParamMask mask)
				{
					std::unique_ptr<Program> newProgram = std::make_unique<ProgramType>();
					Program * castedProg = static_cast<Program*>(newProgram.get());
					castedProg->init(mask);
					return newProgram;
				}
			};

			class ProgramManager
			{
			private:
				static std::unordered_map<std::type_index, std::unique_ptr<AbstractProgramFactory>> programFactories;
				std::unordered_map < std::type_index, std::unordered_map<UberParamMask, std::unique_ptr<Program>>> programList;
			public:
				ProgramManager();
				~ProgramManager();

				void clear();

				template<class T> 
				static void registerProgram()
				{
					if (!std::is_base_of<Program, T>::value)
					{
						throw EngineException("ProgramManager: Attempted to register a non-derived Program program class");
						return;
					}

					std::type_index id = typeid(T);
					std::unique_ptr<AbstractProgramFactory> newFactory = std::make_unique<ProgramFactory<T>>();
					programFactories[id] = std::move(newFactory);
				}

				template<class T>
				T * getProgram(const UberParamMask configMask = 0)
				{
					std::type_index id = typeid(T);

					if (!std::is_base_of<Program, T>::value)
					{
						throw new EngineException(
							"ProgramManager: Attempted to create a program from the non program-derived class " 
							+ std::string(id.name()));
						return NULL;
					}

					auto factoryId = programList.find(id);
					if (factoryId == programList.end())
					{
						registerProgram<T>();
					}
					
					return static_cast<T*>(getProgram(configMask, id));
				}

				Program * getProgram(const UberParamMask configMask, std::type_index programTypeId);
			};
		}
	}
}

#endif