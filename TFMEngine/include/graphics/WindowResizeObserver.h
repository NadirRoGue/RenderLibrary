#ifndef __RENDERLIB_GRAPHICS_WINDOWRESIZEOBSERVER_H__
#define __RENDERLIB_GRAPHICS_WINDOWRESIZEOBSERVER_H__

namespace RenderLib
{
	namespace Graphics
	{
		class WindowResizeObserver
		{
		public:
			virtual void onWindowResize(const int & width, const int & height) = 0;
		};
	}
}

#endif