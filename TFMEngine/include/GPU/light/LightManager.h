#ifndef __RENDERLIB_GPU_LIGHT_LIGHTMANAGER_H__
#define __RENDERLIB_GPU_LIGHT_LIGHTMANAGER_H__

#include <vector>

#include "LightBuffer.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Light
		{
			class LightManager
			{
			private:
				LightBuffer dirLightBuffer;
				LightBuffer pointLightBuffer;
				LightBuffer spotLightBuffer;
			public:
				LightManager();
				~LightManager();

				void initializeBuffers();

				const LightBuffer & getDirectionalLightBuffer();
				const LightBuffer & getPointLightBuffer();
				const LightBuffer & getSpotLightBuffer();

				void setDirectionalLightData(std::vector<char> & bytesData);
				void setPointLightData(std::vector<char> & bytesData);
				void setSpotLightData(std::vector<char> & bytesData);

				void clean();
			};
		}
	}
}

#endif