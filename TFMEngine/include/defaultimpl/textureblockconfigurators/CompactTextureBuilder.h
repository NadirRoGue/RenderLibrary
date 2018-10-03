#ifndef __RENDERLIB_DEFAULTIMPL_COMPACTTEXTUREBUILDER_H__
#define __RENDERLIB_DEFAULTIMPL_COMPACTTEXTUREBUILDER_H__

#include "CPU/texture/Texture.h"

#include "CPU/memory/PoolObjectBuilder.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		class CompactTextureBuilder : public CPU::Memory::PoolObjectBuilder<CPU::Texture::Texture>
		{
		public:
			void configureAttributes(CPU::Memory::MemoryBlock * memBlock, CPU::Texture::Texture * resource, CPU::Memory::BlockConfiguration * data);
		};
	}
}

#endif