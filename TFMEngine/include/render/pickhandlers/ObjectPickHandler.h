#ifndef __RENDERLIB_RENDER_OBJECTPICKHANDLER_H__
#define __RENDERLIB_RENDER_OBJECTPICKHANDLER_H__

#include "render/AbstractPickHandler.h"

#include "defaultimpl/shaders/PickingProgram.h"

namespace RenderLib
{
	namespace Render
	{
		class ObjectPickHandler : public AbstractPickHandler
		{
		private:
			std::unordered_map<unsigned int, DefaultImpl::MeshRenderer *> pickTargets;
			DefaultImpl::PickingProgram * pickProgram;
		public:
			void initialize(EngineInstance * instance);
			PickType doPicking(PickingRenderStage * pickStage);
		private:
			void drawList(GPU::Mesh::GPUBuffer * buffer,
				const std::vector<DefaultImpl::MeshRenderer*> & renderables,
				const Camera & cam);
		};
	}
}

#endif