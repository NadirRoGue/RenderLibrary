#ifndef __RENDERLIB_CPU_TEXTURE_TEXTUREMANAGER_H__
#define __RENDERLIB_CPU_TEXTURE_TEXTUREMANAGER_H__

#include <unordered_map>
#include <memory>
#include <mutex>

#include "CPU/texture/Texture.h"
#include "CPU/texture/TextureBlockConfiguration.h"
#include "CPU/texture/TextureLoadResult.h"
#include "CPU/texture/TextureBlendOperation.h"

#include "CPU/mesh/MeshLoadResult.h"

#include "logger/Log.h"

namespace RenderLib
{
	namespace CPU
	{
		namespace Texture
		{
			class AbstractTextureBlockConfigSetup
			{
			public:
				virtual void configureBlockConfig(TextureLoadResult * src, TextureBlockConfiguration & config) = 0;
			};

			class Texture1DBlockConfigSetup : public AbstractTextureBlockConfigSetup
			{
			public:
				void configureBlockConfig(TextureLoadResult * src, TextureBlockConfiguration & config);
			};

			class Texture2DBlockConfigSetup : public AbstractTextureBlockConfigSetup
			{
			public:
				void configureBlockConfig(TextureLoadResult * src, TextureBlockConfiguration & config);
			};

			class Texture3DBlockConfigSetup : public AbstractTextureBlockConfigSetup
			{
			public:
				void configureBlockConfig(TextureLoadResult * src, TextureBlockConfiguration & config);
			};

			class TextureManager
			{
			private:
				static TextureManager INSTANCE;
			public:
				static TextureManager & getInstance();
			private:
				std::unordered_map<std::string, std::unique_ptr<Texture>> textures;
				std::unordered_map<TextureType, std::unique_ptr<AbstractTextureBlockConfigSetup>> blockSetups;
				std::unordered_map<TextureStackBlendOperation, std::unique_ptr<TextureBlendOperation>> blendOperations;

				std::mutex mtx;
			private:
				TextureManager();
			public:
				~TextureManager();

				template<TextureType type, class T>
				void registerTextureTypeConfigurator()
				{
					if (std::is_base_of<AbstractTextureBlockConfigSetup, T>::value)
					{
						std::unique_ptr<AbstractTextureBlockConfigSetup> newConfigurator = std::make_unique<T>();
						blockSetups[type] = std::move(newConfigurator);
					}
					else
					{
						Logger::Log::getInstance().logWarning("TextureManager: Tried to register block config setup class non derived from AbstractTextureBlockConfigSetup");
					}
				}

				template<TextureStackBlendOperation bo, class T>
				void registerTextureBlendOperation()
				{
					if (std::is_base_of<TextureBlendOperation, T>::value)
					{
						std::unique_ptr<TextureBlendOperation> newOperation = std::make_unique<T>();
						blendOperations[bo] = std::move(newOperation);
					}
					else
					{
						Logger::Log::getInstance().logWarning("TextureManager: Tried to register blend operation class non derived from TextureBlendOperation");
					}
				}

				Texture * loadTexture(const std::string & file, TextureType type);

				Texture * loadAndProcessTextureStack(const std::string & name, VECTOR3 baseColor, std::vector<IO::LoadedParameter<Mesh::MaterialTextureInfo>> & stack);

				Texture * getTexture(const std::string & fileName);
			private:
				void loadTempTexture(const std::string & file, TextureType type, std::vector<unsigned char> & buf, unsigned int & width, unsigned int & height);
			};
		}
	}
}

#endif