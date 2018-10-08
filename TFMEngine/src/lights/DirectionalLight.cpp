#include "lights/DirectionalLight.h"

namespace RenderLib
{
	namespace Lighting
	{
		DirectionalLight::DirectionalLight()
			: Light<DirectionalLightData>()
		{

		}

		DirectionalLight::~DirectionalLight()
		{

		}

		void DirectionalLight::setDirection(const VECTOR3 & d)
		{
			this->direction = VECTOR4(d.x(), d.y(), d.z(), 0.0);
			this->direction.normalize();
		}

		void DirectionalLight::update(const Camera & camera)
		{
			VECTOR4 transformed = camera.viewMatrix 
				* transform.worldModelMatrix 
				* direction;
			transformed.normalize();

			DirectionalLightData & data = getData();
			data.lightDirection[0] = transformed.x();
			data.lightDirection[1] = transformed.y();
			data.lightDirection[2] = transformed.z();
			data.lightDirection[3] = transformed.w();
		}
	}
}