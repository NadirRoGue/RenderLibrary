#include "defaultimpl/components/UserScript.h"

#include <iostream>

namespace RenderLib
{
	namespace DefaultImpl
	{
		std::type_index UserScript::getComponentType()
		{
			return typeid(UserScript);
		}
	}
}