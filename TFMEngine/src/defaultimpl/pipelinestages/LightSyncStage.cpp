#include "defaultimpl/pipelinestages/LightSyncStage.h"

#include "EngineInstance.h"

#include <iostream>

namespace RenderLib
{
	namespace DefaultImpl
	{
		void LightSyncStage::preRunStage()
		{
			engineInstance->acquireContext();
			engineInstance->getGPULightManager().initializeBuffers();
			engineInstance->releaseContext();

			syncDirLights();
			syncPLLights();
			syncSLLights();
		}

		void LightSyncStage::runStage()
		{
			syncDirLights();
			syncPLLights();
			syncSLLights();
		}

		void LightSyncStage::syncDirLights()
		{
			Scene * scene = engineInstance->getSceneManager().getActiveScene();
			std::vector<Lighting::DirectionalLight*> dirLights = scene->geteDirectionalLights();

			if (dirLights.size() > 0)
			{
				size_t dirLightSize = dirLights[0]->getDataSize();
				//std::cout << dirLightSize << std::endl;
				size_t totalSize = dirLights.size() * dirLightSize;

				std::vector<char> buffer;
				buffer.resize(totalSize);

				size_t ptr = 0;
				for (auto dl : dirLights)
				{
					dl->update(*(scene->getActiveCamera()));
					memcpy(&buffer[0] + ptr, &(dl->getData()), dirLightSize);
					ptr += dirLightSize;
				}

				engineInstance->acquireContext();
				engineInstance->getGPULightManager().setDirectionalLightData(buffer);
				engineInstance->releaseContext();
			}
		}

		void LightSyncStage::syncPLLights()
		{

		}

		void LightSyncStage::syncSLLights()
		{

		}
	}
}