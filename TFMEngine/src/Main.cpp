#include "Initialization.h"
#include "InstanceManager.h"

#include "graphics/ContextManager.h"
#include "defaultimpl/windowhandlers/GLFWWindowHandler.h"

#include "CPU/mesh/MeshManager.h"

#include "defaultimpl/components/MeshFilter.h"
#include "defaultimpl/components/MeshRenderer.h"
#include "defaultimpl/components/ObjectSpinner.h"

#include "defaultimpl/pipelinestages/ComponentRegisterStage.h"
#include "defaultimpl/pipelinestages/CPUToGPUMeshSyncStage.h"
#include "defaultimpl/pipelinestages/GPUToCPUMeshSyncStage.h"
#include "defaultimpl/pipelinestages/CPUToGPUTextureSyncStage.h"
#include "defaultimpl/pipelinestages/RenderStage.h"
#include "defaultimpl/pipelinestages/IterationEndStage.h"
#include "defaultimpl/pipelinestages/TransformUpdateStage.h"
#include "defaultimpl/pipelinestages/ShaderCompilationStage.h"
#include "defaultimpl/pipelinestages/LightSyncStage.h"

#include "logger/Log.h"

#include <iostream>

using namespace RenderLib;
using namespace RenderLib::CPU;
using namespace RenderLib::Logger;

Mesh::Mesh * loadMesh(const std::string & file, unsigned int options)
{
	std::vector<Mesh::Mesh *> meshes = Mesh::MeshManager::getInstance().loadMeshFromFile(file, options);
	if (meshes.size() == 0)
	{
		Log::getInstance().logError("No meshes were found in the given file");
		return NULL;
	}
	return meshes[0];
}

int main(int argc, void ** arg)
{

	/*** INSTANCE SET UP ***/
	// Default initialization (registers components for asset loading, memory building, synchronization, etc.)
	DefaultEngineInitialization();

	// Asset load
	Mesh::Mesh * cube = loadMesh("./assets/mat_cube.obj", Mesh::Mesh::OPTION_COMPUTE_NORMALS_IF_ABSENT);
	Mesh::Mesh * sphere = loadMesh("./assets/sphere_mat_opacity.obj", Mesh::Mesh::OPTION_COMPUTE_NORMALS_IF_ABSENT);
	Mesh::Mesh * cube2 = loadMesh("./assets/mat_cube_2.obj", Mesh::Mesh::OPTION_COMPUTE_NORMALS_IF_ABSENT);

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
	instance->getPipelineManager().addPipelineStage<DefaultImpl::CPUToGPUTextureSyncStage>();
	instance->getPipelineManager().addPipelineStage<DefaultImpl::ShaderCompilationStage>();
	instance->getPipelineManager().addPipelineStage<Pipeline::ElementBasedStage<DefaultImpl::ObjectSpinner>>();
	instance->getPipelineManager().addPipelineStage<DefaultImpl::TransformUpdateStage>();
	instance->getPipelineManager().addPipelineStage<DefaultImpl::LightSyncStage>();
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

	// Light set up
	Lighting::DirectionalLight * dl = scene->addDirectionalLight("Sun");
	dl->setLightColor(VECTOR3(1.0, 1.0, 1.0));
	dl->setDiffuseIntensity(1.0f);
	dl->setSpecularIntensity(1.0f);
	dl->setAmbientIntensity(0.15f);
	//dl->setPosition(VECTOR3(-2.0, 2.0, 5.0));
	dl->setDirection(VECTOR3(1.0, 1.0, 1.0));

	Lighting::PointLight * pl = scene->addPointLight("Bulb");
	pl->setLightColor(VECTOR3(1.0, 1.0, 0.4));
	pl->setPosition(VECTOR3(-3.0, 2.0, 5.0));
	pl->setDiffuseIntensity(1.0f);
	pl->setAmbientIntensity(0.1f);
	pl->setSpecularIntensity(0.9f);
	pl->setAttenuationFactors(0.7f, 0.2f, 0.1f);

	// Object set up
	// "Cube"
	RenderLib::SceneObject * obj = scene->addObject<SceneObject>("Cube");
	obj->addComponent<DefaultImpl::MeshFilter>()->mesh = cube;
	obj->addComponent<DefaultImpl::MeshRenderer>()->preferredRender = DefaultImpl::FORWARD_RENDER;
	obj->addComponent<DefaultImpl::ObjectSpinner>();

	// "Moon"
	RenderLib::SceneObject * moon = scene->addObject<SceneObject>("Moon");
	moon->transform.translate(VECTOR3(5, 0, 0));
	moon->transform.scale(VECTOR3(0.5, 0.5, 0.5));
	moon->addComponent<DefaultImpl::MeshFilter>()->mesh = sphere;
	moon->addComponent<DefaultImpl::MeshRenderer>()->preferredRender = DefaultImpl::FORWARD_RENDER;
	moon->addComponent<DefaultImpl::ObjectSpinner>();
	moon->setParent(obj);

	// Cube crown
	RenderLib::SceneObject * crown = scene->addObject<SceneObject>("Crown");
	crown->transform.translate(VECTOR3(0, 5, 0));
	crown->transform.scale(VECTOR3(0.5, 0.5, 0.5));
	crown->addComponent<DefaultImpl::MeshFilter>()->mesh = cube2;
	crown->addComponent<DefaultImpl::MeshRenderer>()->preferredRender = DefaultImpl::FORWARD_RENDER;
	crown->addComponent<DefaultImpl::ObjectSpinner>();
	crown->setParent(obj);

	/*** EXECUTION (BLOCKING UNTIL ALL INSTANCES ARE DONE) ***/
	// Run pipeline
	InstanceManager::getInstance().launchInstances(ExecutionMode::EXECUTION_PARALLEL);
	
	return 0;
}