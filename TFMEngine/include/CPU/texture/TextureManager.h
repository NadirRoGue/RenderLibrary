#ifndef __RENDERLIB_CPU_TEXTURE_TEXTUREMANAGER_H__
#define __RENDERLIB_CPU_TEXTURE_TEXTUREMANAGER_H__

#include <unordered_map>
#include <memory>

#include "CPU/texture/Texture.h"
#include "CPU/texture/TextureBlockConfiguration.h"
#include "CPU/texture/TextureLoadResult.h"

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
			private:
				TextureManager();
			public:
				~TextureManager();

				template<TextureType type, class T>
				void registerTextureTypeConfigurator()
				{
					std::unique_ptr<AbstractTextureBlockConfigSetup> newConfigurator = std::make_unique<T>();
					blockSetups[type] = std::move(newConfigurator);
				}

				Texture * loadTexture(const std::string & file, TextureType type);

				Texture * getTexture(const std::string & fileName);
			};
		}
	}
}

#endif