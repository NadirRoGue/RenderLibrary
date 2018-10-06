#ifndef __RENDERLIB_GPU_PROGRAM_UBERFACTORYMANAGER_H__
#define __RENDERLIB_GPU_PROGRAM_UBERFACTORYMANAGER_H__

#include <unordered_map>

#include "GPU/program/UberMaskFactory.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Program
		{
			class UberFactoryManager
			{
			private:
				static std::unordered_map<std::type_index, std::unique_ptr<AbstractMaskFactory>> factories;
			public:
				template<class ProgramType, class AbstractMaskFactoryType>
				static void registerUberFactory()
				{
					if (!std::is_base_of<Program, ProgramType>::value)
					{
						throw EngineException("UberFactoryManager: Attempted to register a UberFactory for a non Program derived class!");
					}

					if (!std::is_base_of<AbstractMaskFactory, AbstractMaskFactoryType>::value)
					{
						throw EngineException("UberFactoryManager:: Attempted to register a UberFactory non derived from AbstractMaskFactory!");
					}

					std::type_index programId = typeid(ProgramType);
					std::unique_ptr<AbstractMaskFactory> newFactory = std::make_unique<AbstractMaskFactoryType>();
					factories[programId] = std::move(newFactory);
				}

				static AbstractMaskFactory * getMaskFactory(const std::type_index & programType);
			};
		}
	}
}

#endif
