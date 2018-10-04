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
				config.pixelSize = src->numPixelComponents * sizeof(unsigned char);
			}

			void Texture2DBlockConfigSetup::configureBlockConfig(TextureLoadResult * src, TextureBlockConfiguration & config)
			{
				config.width = src->width;
				config.height = src->height;
				config.depth = 1;
				config.pixelSize = src->numPixelComponents * sizeof(unsigned char);
			}

			void Texture3DBlockConfigSetup::configureBlockConfig(TextureLoadResult * src, TextureBlockConfiguration & config)
			{
				config.width = src->width;
				config.height = src->width;
				config.depth = src->height / src->width;
				config.pixelSize = src->numPixelComponents * sizeof(unsigned char);
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
				std::unique_lock<std::mutex> lock(mtx);

				Texture * texture = getTexture(name);
				if (texture != NULL)
				{
					lock.unlock();
					return texture;
				}

				IO::AbstractLoadResultPtr loadResult = IO::FileManager::loadFile(name, 0);
				TextureLoadResult * texLoadResult = static_cast<TextureLoadResult*>(loadResult.get());

				std::unique_ptr<Texture> newTexture = std::make_unique<Texture>();

				TextureBlockConfiguration config;
				
				auto it = blockSetups.find(type);
				if (it == blockSetups.end())
				{
					lock.unlock();
					throw EngineException("TextureManager: Could not find a block configurator for texture " + name);
					return NULL;
				}

				it->second.get()->configureBlockConfig(texLoadResult, config);

				Memory::MemoryManager::getInstance().configureObject<Texture>(newTexture.get(), &config);

				newTexture.get()->pixels.setAttributes(texLoadResult->loadedPixels);

				texture = newTexture.get();
				
				textures[name] = std::move(newTexture);

				lock.unlock();

				return texture;
			}

			Texture * TextureManager::loadAndProcessTextureStack(const std::string & name, VECTOR3 baseColor, std::vector<IO::LoadedParameter<Mesh::MaterialTextureInfo>> & stack)
			{
				IO::LoadedParameter<Mesh::MaterialTextureInfo> baseParam;
				size_t i = 0;
				bool found = false;
				while(i < stack.size() && !found)
				{
					if (!stack[i].paramExists())
					{
						i++;
						continue;
					}

					baseParam = stack[i];
					found = false;
				}

				std::vector<unsigned char> base;
				Mesh::MaterialTextureInfo info = baseParam.get();
				unsigned int width, height;
				loadTempTexture(info.filePath, TextureType::TEXTURE2D, base, width, height);

				std::vector<unsigned char> baseColorBuf;
				baseColorBuf.resize(width * height * 4);
				baseColor.normalize();
				VECTOR3 bc(baseColor.x() * (FLOAT)255.0, baseColor.y() * (FLOAT)255.0, baseColor.z() * (FLOAT)255.0);
				for (size_t j = 0; j < width * height * 4; j++)
				{
					baseColorBuf[j] = static_cast<unsigned char>(baseColor.x());
					baseColorBuf[j + 1] = static_cast<unsigned char>(baseColor.y());
					baseColorBuf[j + 2] = static_cast<unsigned char>(baseColor.z());
					baseColorBuf[j + 3] = 255;
				}

				auto it = blendOperations.find(baseParam.get().blendOperation);
				if (it != blendOperations.end())
				{
					it->second.get()->doBlend(baseColorBuf, base, baseParam.get().blendStrength);
				}
				else
				{
					Logger::Log::getInstance().logWarning("TextureManager: Textue on st ack of " + name + " uses unknown blend operation. Skipping");
				}

				i++; // Start on next one
				for (; i < stack.size(); i++)
				{
					auto param = stack[i];
					if (param.paramExists())
					{
						auto blendOpIt = blendOperations.find(param.get().blendOperation);
						if (blendOpIt != blendOperations.end())
						{
							unsigned int w, h;
							std::vector<unsigned char> nextOnStack;
							loadTempTexture(param.get().filePath, TextureType::TEXTURE2D, nextOnStack, w, h);

							if (nextOnStack.size() == baseColorBuf.size())
							{
								blendOpIt->second.get()->doBlend(baseColorBuf, nextOnStack, param.get().blendStrength);
							}
							else
							{
								Logger::Log::getInstance().logWarning("TextureManager: Texture on stack of " + name + " has different size than base texture. Skipping");
							}
						}
						else
						{
							Logger::Log::getInstance().logWarning("TextureManger: Texture on stack of " + name + " uses unknown blend operation. Skipping");
						}
					}
				}

				TextureBlockConfiguration blockConfig;
				blockConfig.depth = 1;
				blockConfig.height = height;
				blockConfig.width = width;
				blockConfig.pixelSize = 4 * sizeof(unsigned char);

				std::unique_ptr<Texture> newTexture = std::make_unique<Texture>();

				Memory::MemoryManager::getInstance().configureObject<Texture>(newTexture.get(), &blockConfig);

				newTexture.get()->pixels.setAttributes(baseColorBuf);

				Texture * result = newTexture.get();

				textures[name] = std::move(newTexture);

				return result;
			}

			void TextureManager::loadTempTexture(const std::string & file, TextureType type, std::vector<unsigned char> & buf, unsigned int & width, unsigned int & height)
			{
				IO::AbstractLoadResultPtr loadResult = IO::FileManager::loadFile(file, 0);
				TextureLoadResult * texLoadResult = static_cast<TextureLoadResult*>(loadResult.get());

				buf = texLoadResult->loadedPixels;
				width = texLoadResult->width;
				height = texLoadResult->height;
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