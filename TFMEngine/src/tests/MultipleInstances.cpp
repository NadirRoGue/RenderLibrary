#include "tests/MultipleInstances.h"

#include "tests/BoneFlower.h"

#include "Initialization.h"

#include "InstanceManager.h"

#include "graphics/ContextManager.h"
#include "defaultimpl/windowhandlers/GLFWWindowHandler.h"

#include "CPU/mesh/MeshManager.h"

#include "defaultimpl/components/MeshFilter.h"
#include "defaultimpl/components/MeshRenderer.h"
#include "defaultimpl/components/ObjectSpinner.h"
#include "defaultimpl/components/CameraController.h"

#include "logger/Log.h"

using namespace RenderLib;
using namespace RenderLib::CPU;
using namespace RenderLib::Logger;

namespace MultipleInstances
{
	EngineInstance * createInstance(const std::string & instanceName)
	{
		// Window creation
		Graphics::WindowConfiguration config;
		config.openGLContextProfile = GLFW_OPENGL_CORE_PROFILE;
		config.openGLMajorVersion = 4;
		config.openGLMinorVersion = 2;
		config.windowHeight = 500;
		config.windowWidth = 500;
		config.windowTitle = instanceName;
		config.windowPosX = 50;
		config.windowPosY = 50;
		DefaultImpl::GLFWWindowHandler * window = Graphics::ContextManager::getInstance().createWindow<DefaultImpl::GLFWWindowHandler>(config);

		// Engine Instance creation
		EngineInstance * instance = InstanceManager::getInstance().createInstance(instanceName, window);

		// Scene set up
		RenderLib::Scene * scene = instance->getSceneManager().createScene("TestScene");
		instance->getSceneManager().setActiveScene("TestScene");

		if (scene == NULL)
		{
			std::cout << "RenderLib: Could not create a new scene named TestScene" << std::endl;
			return NULL;
		}

		// Camera set up
		RenderLib::Camera * cam = scene->addCamera<RenderLib::Camera>("Main_Camera");
		cam->addComponent<DefaultImpl::CameraController>();
		cam->setProjectionParams((FLOAT)0.5, (FLOAT)75.0, (FLOAT)45.0);
		cam->translateView(VECTOR3(0.0, 0.0, -8.0));

		// Light set up
		Lighting::DirectionalLight * dl = scene->addDirectionalLight("Sun");
		dl->setLightColor(VECTOR3(1.0, 1.0, 1.0));
		dl->setDiffuseIntensity(1.0f);
		dl->setSpecularIntensity(1.0f);
		dl->setAmbientIntensity(0.15f);
		dl->setDirection(VECTOR3(1.0, 1.0, 1.0));

		std::vector<Mesh::Mesh *> meshes = Mesh::MeshManager::getInstance().getMesh("assets/pig/pig_triangulated.obj");
		if (meshes.size() == 0)
		{
			Log::getInstance().logInfo("No meshes were found in the given file");

			return NULL;
		}

		int i = 0;
		for (auto m : meshes)
		{
			SceneObject * obj = scene->addObject<SceneObject>("part_" + std::to_string(i));
			obj->transform.scale(VECTOR3(8.0, 8.0, 8.0));
			//obj->transform.rotate(VECTOR3(0.0, 1.0, 0.0), 90.0 * 3.1415 / 180.0);
			i++;
			obj->addComponent<DefaultImpl::MeshFilter>()->mesh = m;
			obj->addComponent<DefaultImpl::MeshRenderer>();
		}

		return instance;
	}

	int main(int argc, char ** argv)
	{
		DefaultEngineInitialization();

		unsigned int options =
			(Mesh::Mesh::OPTION_COMPUTE_SMOOTHNORMALS_IF_ABSENT
				| Mesh::Mesh::OPTION_JOIN_IDENTICAL_VERTICES);
		Mesh::MeshManager::getInstance().loadMeshFromFile("assets/pig/pig_triangulated.obj", options);

		EngineInstance * render = createInstance("Material Render");
		EngineInstance * pos = createInstance("Position");
		EngineInstance * normal = createInstance("Normal");
		EngineInstance * color = createInstance("Color");
		EngineInstance * depth = createInstance("Depth");

		InstanceManager::getInstance().launchInstances(ExecutionMode::EXECUTION_PARALLEL);

		return 0;
	}
}