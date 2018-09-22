#include "Initialization.h"
#include "InstanceManager.h"

#include "graphics/ContextManager.h"
#include "graphics/defaultwindowhandlers/GLFWWindowHandler.h"

#include "CPU/mesh/MeshManager.h"

#include "tests/TestClasses.h"

#include "components/MeshFilter.h"
#include "components/MeshRenderer.h"

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
	config.openGLMinorVersion = 5;
	config.windowHeight = 512;
	config.windowWidth = 512;
	config.windowTitle = "Test Instance";
	config.windowPosX = 50;
	config.windowPosY = 50;
	Graphics::GLFWWindowHandler * window = Graphics::ContextManager::getInstance().createWindow<Graphics::GLFWWindowHandler>(config);

	// Engine Instance creation
	EngineInstance * instance = InstanceManager::getInstance().createInstance("TestInstance", window);

	// Pipeline set up
	instance->getPipelineManager().addPipelineStage<Test::TestGlobalStage>();
	instance->getPipelineManager().addPipelineStage<Test::TestElementStage>();

	// Scene set up
	RenderLib::Scene * scene = instance->getSceneManager().createScene("TestScene");
	instance->getSceneManager().setActiveScene("TestScene");

	if (scene == NULL)
	{
		std::cout << "RenderLib: Could not create a new scene named TestScene" << std::endl;
		return -1;
	}

	// Camera set up
	RenderLib::CameraPtr camera = Camera::createCamera(0.5, 1000.0, 45.0);
	scene->addCamera(camera);

	// Object set up
	RenderLib::SceneObjectPtr object = SceneObject::createObject("TestObject");
	RenderLib::SceneObject * obj = object.get();
	scene->addObject(object);

	// Set position
	obj->transform.translate(VECTOR3(0, 0, 5));
	
	// Object component set up
	Components::MeshFilter * meshFilter = obj->addComponent<Components::MeshFilter>();
	meshFilter->mesh = mesh;

	Components::MeshRenderer * meshRenderer = obj->addComponent<Components::MeshRenderer>();

	char * rawData = mesh->memoryBlock->pool->getDataAsBytes();
	IVECTOR3 firstFace = mesh->faces[0];
	std::cout << "As IVECTOR3: " << firstFace.x() << ", " << firstFace.y() << ", " << firstFace.z() << std::endl;
	int * x = reinterpret_cast<int*>(rawData);
	int * y = reinterpret_cast<int*>(rawData + sizeof(int));
	int * z = reinterpret_cast<int*>(rawData + sizeof(int) * 2);
	std::cout << "As raw: " << *x << ", " << *y << ", " << *z << std::endl;

	/*** EXECUTION (BLOCKING UNTIL ALL INSTANCES ARE DONE) ***/
	// Run pipeline
	InstanceManager::getInstance().launchInstances(ExecutionMode::EXECUTION_PARALLEL);
	
	return 0;
}