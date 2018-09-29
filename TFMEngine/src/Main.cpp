#include "Initialization.h"
#include "InstanceManager.h"

#include "graphics/ContextManager.h"
#include "defaultimpl/windowhandlers/GLFWWindowHandler.h"

#include "CPU/mesh/MeshManager.h"

#include "defaultimpl/components/MeshFilter.h"
#include "defaultimpl/components/MeshRenderer.h"

#include "defaultimpl/pipelinestages/ComponentRegisterStage.h"
#include "defaultimpl/pipelinestages/CPUToGPUMeshSyncStage.h"
#include "defaultimpl/pipelinestages/GPUToCPUMeshSyncStage.h"
#include "defaultimpl/pipelinestages/RenderStage.h"
#include "defaultimpl/pipelinestages/IterationEndStage.h"

#include "logger/Log.h"

using namespace RenderLib;
using namespace RenderLib::CPU;
using namespace RenderLib::Logger;

int main(int argc, void ** arg)
{
	/*** INSTANCE SET UP ***/
	// Default initialization (registers components for asset loading, memory building, synchronization, etc.)
	DefaultEngineInitialization();

	// Asset load
	std::vector<Mesh::Mesh *> meshes = Mesh::MeshManager::getInstance().loadMeshFromFile("./assets/cube.obj", 0);
	if (meshes.size() == 0)
	{
		Log::getInstance().logError("No meshes were found in the given file");
		return -1;
	}
	Mesh::Mesh * mesh = meshes[0];

	// Window creation
	Graphics::WindowConfiguration config;
	config.openGLContextProfile = GLFW_OPENGL_CORE_PROFILE;
	config.openGLMajorVersion = 4;
	config.openGLMinorVersion = 2;
	config.windowHeight = 512;
	config.windowWidth = 512;
	config.windowTitle = "Test Instance";
	config.windowPosX = 50;
	config.windowPosY = 50;
	DefaultImpl::GLFWWindowHandler * window = Graphics::ContextManager::getInstance().createWindow<DefaultImpl::GLFWWindowHandler>(config);

	// Engine Instance creation
	EngineInstance * instance = InstanceManager::getInstance().createInstance("TestInstance", window);

	// Pipeline set up
	instance->getPipelineManager().addPipelineStage<DefaultImpl::ComponentRegisterStage>();
	instance->getPipelineManager().addPipelineStage<DefaultImpl::CPUToGPUMeshSyncStage>();
	instance->getPipelineManager().addPipelineStage<DefaultImpl::RenderStage>();
	instance->getPipelineManager().addPipelineStage<DefaultImpl::IterationEndStage>();
	
	// Scene set up
	RenderLib::Scene * scene = instance->getSceneManager().createScene("TestScene");
	instance->getSceneManager().setActiveScene("TestScene");

	if (scene == NULL)
	{
		std::cout << "RenderLib: Could not create a new scene named TestScene" << std::endl;
		return -1;
	}

	// Camera set up
	RenderLib::Camera * cam = scene->addCamera<RenderLib::Camera>("Main_Camera");
	cam->setProjectionParams((FLOAT)0.5, (FLOAT)75.0, (FLOAT)45.0);
	cam->translateView(VECTOR3(0.0, 0.0, -5.0));

	// Object set up
	RenderLib::SceneObject * obj = scene->addObject<SceneObject>();

	// Set position
	//obj->transform.translate(VECTOR3(0, 0, 5));
	
	// Object component set up
	DefaultImpl::MeshFilter * meshFilter = obj->addComponent<DefaultImpl::MeshFilter>();
	meshFilter->mesh = mesh;

	DefaultImpl::MeshRenderer * meshRenderer = obj->addComponent<DefaultImpl::MeshRenderer>();

	/*** EXECUTION (BLOCKING UNTIL ALL INSTANCES ARE DONE) ***/
	// Run pipeline
	InstanceManager::getInstance().launchInstances(ExecutionMode::EXECUTION_PARALLEL);
	
	return 0;
}