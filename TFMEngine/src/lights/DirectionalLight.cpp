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

		void DirectionalLight::setDirection(const VECTOR3 & direction)
		{
			DirectionalLightData & data = getData();
			data.lightDirection[0] = direction.x();
			data.lightDirection[1] = direction.y();
			data.lightDirection[2] = direction.z();
			data.lightDirection[3] = 0.0f;
		}

		void DirectionalLight::update(const Camera & camera)
		{
			DirectionalLightData & data = getData();
			VECTOR4 lightDir(
				data.lightDirection[0], 
				data.lightDirection[1], 
				data.lightDirection[2], 
				data.lightDirection[3]);

			VECTOR4 transformed = camera.viewMatrix * lightDir;

			data.lightDirection[0] = transformed.x();
			data.lightDirection[1] = transformed.y();
			data.lightDirection[2] = transformed.z();
			data.lightDirection[3] = transformed.w();
		}
	}
}