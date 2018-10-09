#ifndef __RENDERLIB_RENDER_FBO_H__
#define __RENDERLIB_RENDER_FBO_H__

#include <unordered_map>
#include <string>

#include "GPU/texture/GPUTexture.h"

namespace RenderLib
{
	namespace Render
	{
		class FBO
		{
		private:
			std::unordered_map<std::string, GPU::Texture::GPUTexture *> attachments;
		};
	}
}

#endif