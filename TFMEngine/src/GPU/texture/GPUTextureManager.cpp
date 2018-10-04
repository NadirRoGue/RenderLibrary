#include "GPU/texture/GPUTextureManager.h"

#include <algorithm>

#include "logger/Log.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Texture
		{
			float GPUTextureManager::MAX_ANISOTROPIC_FILTERING = 0.0f;

			void GPUTextureManager::queryAnisotropicFilterSupport()
			{
				MAX_ANISOTROPIC_FILTERING = 0.0f;
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &MAX_ANISOTROPIC_FILTERING);
				MAX_ANISOTROPIC_FILTERING = std::max<float>(MAX_ANISOTROPIC_FILTERING, 0.f);
				Logger::Log::getInstance().logInfo("Textures: Max anysotropic filtering level: " + std::to_string(MAX_ANISOTROPIC_FILTERING));
			}

			GPUTextureManager::GPUTextureManager()
			{

			}

			GPUTextureManager::~GPUTextureManager()
			{

			}

			void GPUTextureManager::clean()
			{
				for (auto it = gpuTextures.begin(); it != gpuTextures.end(); it++)
				{
					it->second.get()->destroy();
				}
			}
		}
	}
}