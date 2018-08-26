#include "Component.h"

namespace RenderLib
{
	std::type_index Component::getComponentType()
	{
		return typeid(*this);
	}
}