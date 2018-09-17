#ifndef __RENDERLIB_COMPONENT__
#define __RENDERLIB_COMPONENT__

#include <typeinfo>
#include <typeindex>

namespace RenderLib
{
	class SceneObject;

	class Component
	{
	public:
		bool enabled;
		SceneObject * object;
	public:
		Component();
		~Component();
		virtual void initialize();
		virtual void start();
		virtual void update();
		virtual void fixedUpdate();
		virtual void destroy();

		virtual std::type_index getComponentType();
	};
}

#endif