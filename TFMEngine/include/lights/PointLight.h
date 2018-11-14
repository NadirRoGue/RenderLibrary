#ifndef __RENDERLIB_LIGHTING_POINTLIGHT_H__
#define __RENDERLIB_LIGHTING_POINTLIGHT_H__

#include "lights/Light.h"

namespace RenderLib
{
  namespace Lighting
  {
    typedef struct PointLightData : public LightData
    {
      float position[4];
      float attenuation[4];
    } PointLightData;

    class PointLight : public Light<PointLightData>
    {
    private:
      VECTOR4 position;

    public:
      PointLight();
      ~PointLight();

      void
      setPosition(const VECTOR3 & position);
      void
      setAttenuationFactors(const float & linear, const float & cuadratic,
                            const float & cubic);

      void
      update(const Camera & cam);
    };
  } // namespace Lighting
} // namespace RenderLib

#endif