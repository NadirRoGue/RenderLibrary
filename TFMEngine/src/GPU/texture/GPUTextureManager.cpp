#include "GPU/texture/GPUTextureManager.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Texture
		{
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