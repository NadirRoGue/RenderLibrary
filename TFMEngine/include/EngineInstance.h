#ifndef __RENDER_LIB_ENGINEINSTANCE_H__
#define __RENDER_LIB_ENGINEINSTANCE_H__

#include "pipeline/PipelineManager.h"

#include "SceneManager.h"

namespace RenderLib
{
	class EngineInstance
	{
	private:
		PipelineManager pipelineManager;
		SceneManager sceneManager;
	};
}

#endif