#ifndef __RENDERLIB_RENDER_FORWARDRENDERSTAGE_H__
#define __RENDERLIB_RENDER_FORWARDRENDERSTAGE_H__

#include "render/AbstractRenderingStage.h"

#include "GPU/mesh/GPUBuffer.h"

#include <vector>

namespace RenderLib
{
	namespace Render
	{
		class ForwardRenderStage : public MeshRenderingStage
		{
	public:
			bool shouldRegisterRenderable(DefaultImpl::MeshRenderer * renderable);
			void runStage();
		};
	}
}

#endif