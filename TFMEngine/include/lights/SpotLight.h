#ifndef __RENDERLIB_LIGHTING_SPOTLIGHT_H__
#define __RENDERLIB_LIGHTING_SPOTLIGHT_H__

#include "lights/Light.h"

namespace RenderLib
{
	namespace Lighting
	{
		typedef struct SpotLightData : public LightData
		{
			float positionAndApperture[4];
			float directionAndTheta[4];
			float attenuation[4];
		} SpotLightData;

		class SpotLight : public Light<SpotLightData>
		{
		private:
			VECTOR4 position;
			VECTOR4 direction;
		public:
			SpotLight();
			~SpotLight();

			void setPosition(const VECTOR3 & pos);
			void setApperture(const float & apperture);
			void setDirection(const VECTOR3 & direction);
			void setTheta(const float & theta);
			void setAttenuation(const float & linear, const float & cuadratic, const float & cubic);

			void update(const Camera & cam);
		};
	}
}

#endif