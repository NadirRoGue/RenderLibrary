#include "GPU/program/UberFactoryManager.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Program
		{
			std::unordered_map<std::type_index, std::unique_ptr<UberMaskFactory>> UberFactoryManager::factories;
			std::unique_ptr<UberMaskFactory> UberFactoryManager::defaultFactory;

			GPU::Program::UberParamMask UberFactoryManager::computeMask(DefaultImpl::MeshRenderer * renderable)
			{
				// If specific uber factory exists, use it
				if (renderable->material != nullptr)
				{
					std::type_index progType = renderable->material->getShaderType();
					auto it = factories.find(progType);
					if (it != factories.end())
					{
						return it->second.get()->computeMask(renderable);
					}
				}

				// Otherwise, try to fall back to default one
				if (defaultFactory.get() != nullptr)
				{
					return defaultFactory.get()->computeMask(renderable);
				}

				return 0;
			}
		}
	}
}