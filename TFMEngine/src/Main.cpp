
#include <iostream>

#include "EngineInstance.h"

#include "SceneManager.h"
#include "pipeline/PipelineManager.h"

#include "tests/TestClasses.h"

int main(int argc, void ** arg)
{
	// Engine Instance
	RenderLib::EngineInstance instance;

	// Pipeline set up
	instance.getPipelineManager().addPipelineStage<RenderLib::Test::TestGlobalStage>();
	instance.getPipelineManager().addPipelineStage<RenderLib::Test::TestElementStage>();

	// Scene set up
	RenderLib::Scene * scene = instance.createScene("TestScene");

	if (scene == NULL)
	{
		std::cout << "RenderLib: Could not create a new scene named TestScene" << std::endl;
		return -1;
	}

	// Object set up
	RenderLib::SceneObject * object = instance.createObject();

	RenderLib::Test::TestComponent * component = object->addComponent<RenderLib::Test::TestComponent>();
	component->anIntVar = 78;
	component->aStringVar = "This is a test string";

	RenderLib::Test::TestComponent2 * comp2 = object->addComponent<RenderLib::Test::TestComponent2>();
	comp2->anIntVar = 56;
	comp2->aStringVar = "This should not appear on screen";

	// Run pipeline
	instance.getPipelineManager().executePipeline();
	
	std::cout << "End" << std::endl;

	RenderLib::Transform transform;
	
	std::cout << transform.transform.matrix() << std::endl;

	std::cout << transform.rotationV.matrix() << std::endl;

	transform.rotate(VECTOR3(0, 1, 0), FLOAT(20));
	std::cout << "@@@@@@" << std::endl;

	std::cout << transform.rotationV.matrix() << std::endl;

	transform.update();

	std::cout << transform.transform.matrix() << std::endl;

	VECTOR3 v(1, 2, 3);
	std::cout << v << std::endl;

	return 0;
}