#ifndef __RENDERLIB_RENDER_FBOMANAGER_H__
#define __RENDERLIB_RENDER_FBOMANAGER_H__

#include <string>
#include <unordered_map>

#include "GPU/texture/GPUTexture.h"

#include "render/FBO.h"

namespace RenderLib
{
	namespace Render
	{
		class FBOManager
		{
		private:
			static unsigned int MAX_RENDER_TARGETS;
		public:
			void queryMaxRenderTargets();
		private:
			std::unordered_map<std::string, std::unique_ptr<FBO>> fbos;
		public:
			FBOManager();
			~FBOManager();

			FBO * createFBO(const std::string & name);

			FBO * getFBO(const std::string & name);

			void destroyFBO(const std::string & name);

			void clean();
		};
	}
}

#endif