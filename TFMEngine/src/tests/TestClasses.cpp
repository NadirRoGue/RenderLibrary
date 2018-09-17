#include "tests/TestClasses.h"

#include <iostream>

namespace RenderLib
{
	namespace Test
	{
		TestComponent::TestComponent()
		{

		}

		TestComponent::~TestComponent()
		{

		}

		void TestComponent::initialize()
		{
			std::cout << "TestComponent initializing" << std::endl;
		}

		void TestComponent::update()
		{
			std::cout << "TestComponent render loop update" << std::endl;
		}

		void TestComponent::fixedUpdate()
		{
			std::cout << "TestComponent fixed loop update" << std::endl;
		}

		void TestComponent::destroy()
		{
			Component::destroy();
			std::cout << "TestComponent destroyed" << std::endl;
		}

		// =================================================================================

		TestComponent2::TestComponent2()
		{

		}

		TestComponent2::~TestComponent2()
		{

		}

		void TestComponent2::initialize()
		{
			std::cout << "TestComponent2: initializing" << std::endl;
		}

		void TestComponent2::update()
		{
			std::cout << "TestComponent2: update" << std::endl;
		}

		void TestComponent2::fixedUpdate()
		{
			std::cout << "TestComponent2: fixedUpdate" << std::endl;
		}

		void TestComponent2::destroy()
		{
			Component::destroy();
			std::cout << "TestComponent2: destroy" << std::endl;
		}

		// =================================================================================
		// Global pipeline stage test class

		void TestGlobalStage::runStage()
		{
			//std::cout << "Executing global stage" << std::endl;
		}

		// =================================================================================
		// Element based pipeline stage test class

		void TestElementStage::processElement(Components::MeshFilter * component)
		{
			CPU::Mesh::Mesh * mesh = component->mesh;
			//std::cout << "Num faces: " << mesh->faces.size() << std::endl;
			//std::cout << "Num vertices: " << mesh->vertices.size() << std::endl;
			/*std::cout << "Printing normals" << std::endl;
			for (int i = 0; i < mesh->getNumVertices(); i++)
			{
				VECTOR3 normal = mesh->normals[i];
				std::cout << normal.x() << ", " << normal.y() << ", " << normal.z() << std::endl;
			}*/
		}
	}
}