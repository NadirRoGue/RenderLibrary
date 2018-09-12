
#include <iostream>

#include "Defines.h"

#include "EngineInstance.h"
#include "Transform.h"

#include "pipeline/PipelineManager.h"

#include "CPU/memory/MemoryManager.h"

#include "CPU/mesh/MeshManager.h"
#include "CPU/mesh/meshattributebuilders/InterleavedMeshBuilder.h"

#include "CPU/io/FileManager.h"
#include "CPU/io/defaultloaders/AssimpFileLoader.h"

#include "Scene.h"
#include "SceneManager.h"

#include <functional>

using namespace RenderLib;

int main(int argc, void ** arg)
{
	// Engine Instance
	RenderLib::EngineInstance instance ("TestInstance");

	// Pipeline set up
	//instance.getPipelineManager().addPipelineStage<RenderLib::Test::TestGlobalStage>();
	//instance.getPipelineManager().addPipelineStage<RenderLib::Test::TestElementStage>();

	// Scene set up
	RenderLib::Scene * scene = instance.getSceneManager().createScene("TestScene");
	instance.getSceneManager().setActiveScene("TestScene");

	if (scene == NULL)
	{
		std::cout << "RenderLib: Could not create a new scene named TestScene" << std::endl;
		return -1;
	}

	// Object set up
	RenderLib::SceneObjectPtr object = SceneObject::createObject("TestObject");
	scene->addObject(object);
	

	// Run pipeline
	instance.getPipelineManager().executePipeline();
	
	CPU::Memory::MemoryManager::getInstance().setAttributeBuilderForClass<CPU::Mesh::Mesh>(new CPU::Mesh::InterleavedMeshBuilder());
	CPU::IO::FileManager::registerFileLoader(new CPU::IO::AssimpFileLoader());

	std::cout << "Configured!" << std::endl;

	std::vector<CPU::Mesh::Mesh*> loaded = CPU::Mesh::MeshManager::getInstance().loadMeshFromFile("./assets/cube.obj", 0);

	std::cout << "Mesh loaded!" << std::endl;

	if (loaded.size() > 0)
	{
		CPU::Mesh::Mesh * singleMesh = loaded[0];
		std::cout << "Num faces: " << singleMesh->faces.size() << std::endl;
		std::cout << "Num vertices: " << singleMesh->vertices.size() << std::endl;

		std::cout << "Printing normals" << std::endl;
		for (int i = 0; i < singleMesh->normals.size(); i++)
		{
			VECTOR3 normal = singleMesh->normals[i];
			std::cout << normal.x() << ", " << normal.y() << ", " << normal.z() << std::endl;
		}
	}
	else
	{
		std::cout << "0 meshes loaded from ./assets/cube.obj" << std::endl;
	}

	return 0;
}