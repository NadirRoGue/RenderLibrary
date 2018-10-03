#ifndef __RENDERLIB_CPU_TEXTURE_TEXTUREMANAGER_H__
#define __RENDERLIB_CPU_TEXTURE_TEXTUREMANAGER_H__

#include <unordered_map>
#include <memory>

#include "CPU/texture/Texture.h"

namespace RenderLib
{
	namespace CPU
	{
		namespace Texture
		{
			class TextureManager
			{
			private:
				static TextureManager INSTANCE;
			public:
				static TextureManager & getInstance();
			private:
				std::unordered_map<std::string, std::unique_ptr<Texture>> textures;
			private:
				TextureManager();
			public:
				~TextureManager();

				Texture * loadTexture(const std::string & file, TextureType type);

				Texture * getTexture(const std::string & fileName);

				void  clean();
			};
		}
	}
}

#endif