#ifndef __RENDER_LIB_ENGINEINSTANCE_H__
#define __RENDER_LIB_ENGINEINSTANCE_H__

#include <memory>
#include <string>

#include "SceneManager.h"
#include "pipeline/PipelineManager.h"

#include "graphics/WindowHandler.h"

#include "GPU/mesh/GPUMeshManager.h"
#include "GPU/program/ProgramManager.h"

namespace RenderLib
{
	class EngineInstance
	{
	private:
		std::string instanceName;

		unsigned int instanceID;

		std::unique_ptr<Pipeline::PipelineManager> pipelineManager;
		//std::unique_ptr<SceneManager> sceneManager;
		//std::unique_ptr<GPU::Mesh::GPUMeshManager> gpuMeshManager;
		//std::unique_ptr<GPU::
		SceneManager sceneManager;
		GPU::Mesh::GPUMeshManager gpuMeshManager;
		GPU::Program::ProgramManager gpuProgramManager;

		Graphics::WindowHandler * window;

		bool enableFlag;
	public:
		EngineInstance(const unsigned int & ID, const std::string & instanceName, Graphics::WindowHandler * windowHandler);
		~EngineInstance();

		const std::string & getInstanceName();
		const unsigned int & getInstanceID();

		void loadActiveScene();
		void loadScene(const std::string & name);
		
		Graphics::WindowHandler * getWindow();
		Pipeline::PipelineManager & getPipelineManager();
		SceneManager & getSceneManager();
		GPU::Mesh::GPUMeshManager & getGPUMeshManager();
		GPU::Program::ProgramManager & getProgramManager();

		void acquireContext();
		void releaseContext();

		void disable();
		bool isEnabled();

		void executeIteration();
		void cleanUp();
	};
}

#endif