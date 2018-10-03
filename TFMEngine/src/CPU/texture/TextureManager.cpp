#include "CPU/texture/TextureManager.h"

namespace RenderLib
{
	namespace CPU
	{
		namespace Texture
		{
			TextureManager TextureManager::INSTANCE;

			TextureManager & TextureManager::getInstance()
			{
				return INSTANCE;
			}

			TextureManager::TextureManager()
			{

			}

			TextureManager::~TextureManager()
			{

			}

			Texture * TextureManager::loadTexture(const std::string & name, TextureType type)
			{

			}
		}
	}
}