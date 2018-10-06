#include "GPU/program/UberFactoryManager.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Program
		{
			std::unordered_map<std::type_index, std::unique_ptr<AbstractMaskFactory>> UberFactoryManager::factories;

			AbstractMaskFactory * UberFactoryManager::getMaskFactory(const std::type_index & programType)
			{
				auto it = factories.find(programType);
				if (it != factories.end())
				{
					return it->second.get();
				}

				return NULL;
			}
		}
	}
}