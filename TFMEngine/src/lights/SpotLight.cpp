#include "lights/SpotLight.h"

namespace RenderLib
{
	namespace Lighting
	{
		SpotLight::SpotLight()
		{
			setAttenuation(0.0f, 0.0f, 0.0f);
		}

		SpotLight::~SpotLight()
		{

		}

		void SpotLight::setPosition(const VECTOR3 & pos)
		{
			this->position = VECTOR4(pos.x(), pos.y(), pos.z(), 1.0);
		}

		void SpotLight::setApperture(const float & apperture)
		{
			SpotLightData & data = getData();
			data.positionAndApperture[3] = apperture;
		}

		void SpotLight::setDirection(const VECTOR3 & d)
		{
			this->direction = VECTOR4(d.x(), d.y(), d.z(), 0.0);
			this->direction.normalize();
		}

		void SpotLight::setTheta(const float & theta)
		{
			SpotLightData & data = getData();
			data.directionAndTheta[3] = theta;
		}

		void SpotLight::setAttenuation(const float & linear, const float & cuadratic, const float & cubic)
		{
			SpotLightData & data = getData();
			data.attenuation[0] = linear;
			data.attenuation[1] = cuadratic;
			data.attenuation[2] = cubic;
		}

		void SpotLight::update(const Camera & cam)
		{
			//transform.updateGraph();
			VECTOR4 pos = cam.viewMatrix * transform.worldModelMatrix * position;
			VECTOR4 dir = cam.viewMatrix * transform.worldModelMatrix * direction;
			dir.w() = 0.0;
			dir.normalize();

			SpotLightData & data = getData();

			data.positionAndApperture[0] = pos.x();
			data.positionAndApperture[1] = pos.y();
			data.positionAndApperture[2] = pos.z();

			data.directionAndTheta[0] = dir.x();
			data.directionAndTheta[1] = dir.y();
			data.directionAndTheta[2] = dir.z();
		}
	}
}