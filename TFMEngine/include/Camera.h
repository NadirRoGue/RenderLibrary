#ifndef __RENDERLIB_CAMERA_H__
#define __RENDERLIB_CAMERA_H__

#include <memory>

#include "Defines.h"
#include "SceneObject.h"

#include "graphics/WindowResizeObserver.h"

namespace RenderLib
{
	typedef std::unique_ptr<Camera> CameraPtr;

	class Camera : public SceneObject, public Graphics::WindowResizeObserver
	{
	public:
		MATRIX4 projectionMatrix;
		MATRIX4 viewMatrix;

		FLOAT farPlane;
		FLOAT nearPlane;
		FLOAT fov;

	public:
		Camera(FLOAT near, FLOAT far, FLOAT FOV);
		~Camera();


		void lookAt(const VECTOR3 & eye, const VECTOR3 & target, const VECTOR3 & up);

		void translateView(const VECTOR3 & translation);
		void rotateView(const VECTOR3 & angles);

		void updateViewMatrix();

		void onWindowResize(const int & width, const int & height);

	private:
		void initializeProjectionMatrix();
		void initializeViewMatrix();
	};
}

#endif