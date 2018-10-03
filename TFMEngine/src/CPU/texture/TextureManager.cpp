#include "CPU/texture/TextureManager.h"

#include "CPU/io/FileManager.h"

#include "CPU/memory/MemoryManager.h"

#include "CPU/texture/Texture.h"

namespace RenderLib
{
	namespace CPU
	{
		namespace Texture
		{
			void Texture1DBlockConfigSetup::configureBlockConfig(TextureLoadResult * src, TextureBlockConfiguration & config)
			{
				config.width = src->width;
				config.height = src->height;
				config.depth = 1;
			}

			void Texture2DBlockConfigSetup::configureBlockConfig(TextureLoadResult * src, TextureBlockConfiguration & config)
			{
				config.width = src->width;
				config.height = src->height;
				config.depth = 1;
			}

			void Texture3DBlockConfigSetup::configureBlockConfig(TextureLoadResult * src, TextureBlockConfiguration & config)
			{
				config.width = src->width;
				config.height = src->width;
				config.depth = src->height / src->width;
			}

			// ==========================================================================

			TextureManager TextureManager::INSTANCE;

			TextureManager & TextureManager::getInstance()
			{
				return INSTANCE;
			}

			TextureManager::TextureManager()
			{
				registerTextureTypeConfigurator<TextureType::TEXTURE1D, Texture1DBlockConfigSetup>();
				registerTextureTypeConfigurator<TextureType::TEXTURE2D, Texture2DBlockConfigSetup>();
				registerTextureTypeConfigurator<TextureType::TEXTURE3D, Texture3DBlockConfigSetup>();
			}

			TextureManager::~TextureManager()
			{

			}

			Texture * TextureManager::loadTexture(const std::string & name, TextureType type)
			{
				IO::AbstractLoadResultPtr loadResult = IO::FileManager::loadFile(name, 0);
				TextureLoadResult * texLoadResult = static_cast<TextureLoadResult*>(loadResult.get());

				std::unique_ptr<Texture> newTexture = std::make_unique<Texture>();

				TextureBlockConfiguration config;
				
				auto it = blockSetups.find(type);
				if (it == blockSetups.end())
				{
					throw EngineException("TextureManager: Could not find a block configurator for texture " + name);
					return NULL;
				}

				it->second.get()->configureBlockConfig(texLoadResult, config);

				Memory::MemoryManager::getInstance().configureObject<Texture>(newTexture.get(), &config);

				newTexture.get()->pixels.setAttributes(texLoadResult->loadedPixels);

				Texture * returnResult = newTexture.get();

				textures[name] = std::move(newTexture);

				return returnResult;
			}

			Texture * TextureManager::getTexture(const std::string & fileName)
			{
				auto it = textures.find(fileName);
				if (it != textures.end())
				{
					return it->second.get();
				}

				return NULL;
			}
		}
	}
}