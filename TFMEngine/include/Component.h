#ifndef __CPU_COMPONENT__
#define __CPU_COMPONENT__

#include <typeinfo>
#include <typeindex>

namespace RenderLib
{
	class SceneObject;

	class Component
	{
	protected:
		SceneObject * object;
	public:
		bool enabled;
	public:
		Component();
		~Component();
		virtual void initialize();
		virtual void update();
		virtual void fixedUpdate();
		virtual void destroy();

		std::type_index getComponentType();
	};
}

#endif