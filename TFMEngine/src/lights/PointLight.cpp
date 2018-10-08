#include "lights/PointLight.h"

namespace RenderLib
{
	namespace Lighting
	{
		PointLight::PointLight()
		{
			setAttenuationFactors(1.5f, 0.f, 0.f);
		}

		PointLight::~PointLight()
		{

		}

		void PointLight::setPosition(const VECTOR3 & p)
		{
			this->position = VECTOR4(p.x(), p.y(), p.z(), 1.0);
		}

		void PointLight::setAttenuationFactors(const float & linear, const float & cuadratic, const float & cubic)
		{
			PointLightData & data = getData();
			data.attenuation[0] = linear;
			data.attenuation[1] = cuadratic;
			data.attenuation[2] = cubic;
			data.attenuation[3] = 0.f;
		}

		void PointLight::update(const Camera & cam)
		{
			VECTOR4 pos = cam.viewMatrix * transform.worldModelMatrix * position;
			
			PointLightData & data = getData();
			data.position[0] = pos.x();
			data.position[1] = pos.y();
			data.position[2] = pos.z();
			data.position[3] = pos.w();
		}
	}
}