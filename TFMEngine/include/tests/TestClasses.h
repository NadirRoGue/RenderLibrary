#ifndef __RENDERLIB_TEST_TESTCLASSES_H__
#define __RENDERLIB_TEST_TESTCLASSES_H__

#include <string>

#include "pipeline/PipelineStage.h"
#include "Component.h"

#include "components/MeshFilter.h"

namespace RenderLib
{
	namespace Test
	{
		class TestComponent : public Component
		{
		public:
			int anIntVar;
			std::string aStringVar;
			TestComponent();
			~TestComponent();
			void initialize();
			void update();
			void fixedUpdate();
			void destroy();
		};

		class TestComponent2 : public Component
		{
		public:
			int anIntVar;
			std::string aStringVar;
			TestComponent2();
			~TestComponent2();
			void initialize();
			void update();
			void fixedUpdate();
			void destroy();
		};

		class TestGlobalStage : public Pipeline::PipelineStage
		{
		public:
			void runStage();
		};

		class TestElementStage : public Pipeline::ElementBasedStage<Components::MeshFilter>
		{
		public:
			void processElement(Components::MeshFilter * component);
		};
	}
}

#endif