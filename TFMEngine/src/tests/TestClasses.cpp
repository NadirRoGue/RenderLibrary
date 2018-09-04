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
			std::cout << "Executing global stage" << std::endl;
		}

		// =================================================================================
		// Element based pipeline stage test class

		void TestElementStage::processElement(TestComponent * component)
		{
			std::cout << "Processing element: anIntVar=" << component->anIntVar << ", aStringVar=" << component->aStringVar << std::endl;
		}
	}
}