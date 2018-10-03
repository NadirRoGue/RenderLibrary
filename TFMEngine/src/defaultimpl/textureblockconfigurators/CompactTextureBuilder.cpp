#include "defaultimpl/textureblockconfigurators/CompactTextureBuilder.h"

#include "CPU/texture/TextureBlockConfiguration.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		void CompactTextureBuilder::configureAttributes(CPU::Memory::MemoryBlock * memBlock, CPU::Texture::Texture * resource, CPU::Memory::BlockConfiguration * data)
		{
			CPU::Texture::TextureBlockConfiguration * config = 
				static_cast<CPU::Texture::TextureBlockConfiguration*>(data);

			resource->pixels.setAttributeSource(memBlock, 0, sizeof(FLOAT), config->depth * config->width * config->height);
		}
	}
}