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
		public:
			static FBO GBUFFERS;
		private:
			std::unordered_map<std::string, std::unique_ptr<FBO>> fbos;
		public:
			FBOManager();
			~FBOManager();

			FBO * createFBO(const std::string & name);

			FBO * getFBO(const std::string & name);

			void destroyFBO(const std::string & name);

			void onResize(const unsigned int & width, const unsigned int & height);

			void clean();
		};
	}
}

#endif