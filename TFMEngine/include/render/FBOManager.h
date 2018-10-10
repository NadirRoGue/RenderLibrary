#ifndef __RENDERLIB_RENDER_FBOMANAGER_H__
#define __RENDERLIB_RENDER_FBOMANAGER_H__

#include <string>
#include <unordered_map>

#include "GPU/texture/GPUTexture.h"

#include "render/FBO.h"

namespace RenderLib
{
	class EngineInstance;

	namespace Render
	{
		class FBOManager
		{
		private:
			std::unordered_map<std::string, std::unique_ptr<FBO>> fbos;
			EngineInstance * instance;
		public:
			FBOManager();
			~FBOManager();

			void setEngineInstance(EngineInstance * instance);

			FBO * createFBO(const std::string & name);

			FBO * getFBO(const std::string & name);

			void destroyFBO(const std::string & name);

			void onResize(const unsigned int & width, const unsigned int & height);

			void clean();
		};
	}
}

#endif