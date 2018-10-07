#ifndef __RENDERLIB_LIGHTING_LIGHT_H__
#define __RENDERLIB_LIGHTING_LIGHT_H__

#include <memory>

#include "SceneObject.h"
#include "EngineException.h"

#include "Camera.h"

namespace RenderLib
{
	namespace Lighting
	{
		typedef struct LightData
		{
			float lightColor[4];
			float lightKd;
			float lightKs;
			float lightKa;

			LightData()
			{

			}

			virtual ~LightData()
			{

			}
		} LightData;

		template<class T>
		class Light : public SceneObject
		{
		private:
			std::unique_ptr<LightData> lightData;
		public:
			Light()
				: SceneObject()
			{
				if (std::is_base_of<LightData, T>::value)
				{
					lightData = std::make_unique<T>();
				}
				else
				{
					throw EngineException(
						"Light: Attempted to instantiate a light with a non LightData derived light data struct");
				}
			}

			virtual ~Light()
			{

			}

			T & getData()
			{
				return *(static_cast<T*>(lightData.get()));
			}

			void setLightColor(const VECTOR3 & color)
			{
				LightData * data = lightData.get();
				data->lightColor[0] = color.x();
				data->lightColor[1] = color.y();
				data->lightColor[2] = color.z();
				data->lightColor[3] = 1.0f;
			}

			void setDiffuseIntensity(const float & value)
			{
				lightData.get()->lightKd = value;
			}

			void setSpecularIntensity(const float & value)
			{
				lightData.get()->lightKs = value;
			}

			void setAmbientIntensity(const float & value)
			{
				lightData.get()->lightKa = value;
			}

			size_t getDataSize()
			{
				return sizeof(T);
			}

			virtual void update(const Camera & camera) = 0;
		};
	}
}

#endif