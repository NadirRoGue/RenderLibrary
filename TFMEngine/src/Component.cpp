#include "Component.h"

namespace RenderLib
{
	Component::Component()
	{
		enabled = true;
	}

	Component::~Component()
	{

	}

	void Component::initialize()
	{

	}

	void Component::update()
	{

	}

	void Component::fixedUpdate()
	{

	}

	void Component::destroy()
	{
		enabled = false;
	}

	std::type_index Component::getComponentType()
	{
		return typeid(*this);
	}
}