#ifndef __RENDERLIB_DEFAULTIMPL_GPUTOCPUMESHSYNCSTAGE_H__
#define __RENDERLIB_DEFAULTIMPL_GPUTOCPUMESHSYNCSTAGE_H__

#include "pipeline/PipelineStage.h"

#include "defaultimpl/components/MeshRenderer.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		class GPUToCPUSyncStage : public Pipeline::ElementBasedStage<DefaultImpl::MeshRenderer>
		{

		};
	}
}

#endif