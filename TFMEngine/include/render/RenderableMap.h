#ifndef __RENDERLIB_RENDER_RENDERABLEMAP_H__
#define __RENDERLIB_RENDER_RENDERABLEMAP_H__

#include <unordered_map>
#include <vector>

#include "GPU/program/ShaderProgram.h"
#include "GPU/program/ProgramManager.h"

#include "Camera.h"

#include "defaultimpl/components/MeshRenderer.h"

namespace RenderLib
{
	namespace Render
	{
		typedef struct RenderableStub
		{
			GPU::Program::ShaderProgram * program;
			std::vector<DefaultImpl::MeshRenderer*> renderables;

		} RenderableStub;

		class RenderableMap
		{
		private:
			std::string mapRenderOwnerName;
			std::unordered_map<std::type_index, std::unordered_map<GPU::Program::UberParamMask, RenderableStub>> renderablesMap;
		public:
			RenderableMap();
			~RenderableMap();

			size_t getSize();

			void setName(const std::string & name);
			const std::string & getName();

			void addRenderable(GPU::Program::ProgramManager & programManager, DefaultImpl::MeshRenderer * renderable);

			void initializeMap();
			void renderMap(const Camera & fromCamera);
		};
	}
}

#endif