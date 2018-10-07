#ifndef __RENDERLIB_LIGHTING_DIRECTIONALLIGHT_H__
#define __RENDERLIB_LIGHTING_DIRECTIONALLIGHT_H__

#include "lights/Light.h"

namespace RenderLib
{
	namespace Lighting
	{
		typedef struct DirectionalLightData : public LightData
		{
			float lightDirection[4];
		} DirectionalLightData;

		class DirectionalLight : public Light<DirectionalLightData>
		{
		public:
			DirectionalLight();
			~DirectionalLight();

			void setDirection(const VECTOR3 & direction);

			void update(const Camera & camera);
		};
	}
}

#endif