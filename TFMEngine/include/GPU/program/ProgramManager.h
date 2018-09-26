#ifndef __RENDERLIB_GPU_PROGRAM_PROGRAMMANAGER_H__
#define __RENDERLIB_GPU_PROGRAM_PROGRAMMANAGER_H__

#include <memory>
#include <unordered_map>
#include <vector>

#include <typeindex>
#include <typeinfo>

#include <GL/glew.h>

#include "GPU/program/Program.h"

#include "EngineException.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Program
		{
			class ProgramManager
			{
			private:
				static std::unique_ptr<ProgramManager> INSTANCE;
			public:
				static ProgramManager & getInstance();
			private:
				std::unordered_map<std::type_index, std::unique_ptr<ProgramFactory>> factories;
			public:
				ProgramManager();
				~ProgramManager();

				template<class T>
				void registerProgram()
				{
					std::type_index id = typeid(T);

					if (!std::is_base_of<Program, T>::value)
					{
						std::string message = "ProgramManager: Attempted to create a program from the non program-derived class " + std::string(id.name());
						throw new EngineException(message.c_str());
					}

					auto it = factories.find(id);
					if (it == factories.end())
					{
						std::unique_ptr<ProgramFactory> newFactory = std::make_unique<ProgramFactory>();
						factories[id] = std::move(newFactory);
					}
				}

				template<class T>
				T * createOrGetProgram()
				{
					return NULL;
				}
			};
		}
	}
}

#endif