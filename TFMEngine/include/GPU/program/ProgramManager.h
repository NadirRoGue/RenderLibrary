#ifndef __RENDERLIB_GPU_PROGRAM_PROGRAMMANAGER_H__
#define __RENDERLIB_GPU_PROGRAM_PROGRAMMANAGER_H__

#include <memory>
#include <unordered_map>
#include <vector>

#include <typeindex>
#include <typeinfo>

#include "GPU/program/Program.h"
#include "GPU/program/UberMaskFactory.h"

#include "GPU/program/ShaderProgram.h"


#include "EngineException.h"

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
				static std::unordered_map<std::type_index, std::unique_ptr<UberMaskFactory>> uberFactories;
			public:
				template<class ProgramType, class UberFactoryType>
				static void registerUberMaskFactory()
				{
					if (!std::is_base_of<Program, ProgramType>::value)
					{
						throw EngineException("ProgramManager: Attempted to register a non Program derived class for an uber mask factory");
					}

					if (!std::is_base_of<UberMaskFactory, UberFactoryType>::value)
					{
						throw EngineException("ProgramManager: Attempted to register a non UberMaskFactory derived class for an uber mask factory");
					}

					if (std::is_base_of<ShaderProgram, ProgramType>::value && !std::is_base_of<ShaderUberMaskFactory, UberFactoryType>::value)
					{
						throw EngineException("ProgramManager: Attempted to register a non ShaderUberMaskFactory factory for a ShaderProgram class. Not allowed");
					}

					std::type_info programType = typeid(ProgramType);
					
					std::unique_ptr<UberMaskFactory> newFactory = std::make_unique<UberFactoryType>();

					uberFactories[programType] = std::move(newFactory);
				}

			private:
				std::unordered_map<std::type_index, ProgramMap> programList;
			public:
				ProgramManager();
				~ProgramManager();

				void clear();

				template<class T>
				T * getProgram(UberFactoryParam * param = NULL)
				{
					if (!std::is_base_of<Program, T>::value)
					{
						throw new EngineException("ProgramManager: Attempted to create a program from the non program-derived class " + std::string(id.name()));
						return NULL;
					}

					std::type_index id = typeid(T);
					
					Program * raw = getProgram(id, param);

					return static_cast<T*>(raw);
				}

				Program * getProgram(std::type_index programType, UberFactoryParam * param = NULL);

				Program * getProgram(std::type_index programType, UberParamMask mask);
			};
		}
	}
}

#endif