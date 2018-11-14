#include "defaultimpl/pipelinestages/LightSyncStage.h"

#include "EngineInstance.h"

#include <iostream>

namespace RenderLib
{
  namespace DefaultImpl
  {
    void
    LightSyncStage::preRunStage()
    {
      engineInstance->acquireContext();
      engineInstance->getGPULightManager().initializeBuffers();
      engineInstance->releaseContext();
    }

    void
    LightSyncStage::runStage()
    {
      syncDirLights();
      syncPLLights();
      syncSLLights();
    }

    void
    LightSyncStage::syncDirLights()
    {
      Scene * scene = engineInstance->getSceneManager().getActiveScene();
      std::vector<Lighting::DirectionalLight *> dirLights
          = scene->getDirectionalLights();

      if (dirLights.size() > 0)
      {
        size_t dirLightSize = dirLights[0]->getDataSize();
        size_t totalSize    = dirLights.size() * dirLightSize;

        std::vector<char> buffer;
        buffer.resize(totalSize + sizeof(int) * 4);

        int maxLights
            = std::min<int>((int)dirLights.size(), MAX_DIRECTIONAL_LIGHTS);
        memcpy(&buffer[0], &maxLights, sizeof(int));

        size_t ptr = sizeof(int) * 4;
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

    void
    LightSyncStage::syncPLLights()
    {
      Scene * scene = engineInstance->getSceneManager().getActiveScene();
      std::vector<Lighting::PointLight *> plLights = scene->getPointLights();

      if (plLights.size() > 0)
      {
        size_t plLightSize = plLights[0]->getDataSize();
        size_t totalSize   = plLights.size() * plLightSize;

        std::vector<char> buffer;
        buffer.resize(totalSize + sizeof(int) * 4);

        int maxLights = std::min<int>((int)plLights.size(), MAX_POINT_LIGHTS);
        memcpy(&buffer[0], &maxLights, sizeof(int));

        size_t ptr = sizeof(int) * 4;
        for (auto pl : plLights)
        {
          pl->update(*(scene->getActiveCamera()));
          memcpy(&buffer[0] + ptr, &(pl->getData()), plLightSize);
          ptr += plLightSize;
        }

        engineInstance->acquireContext();
        engineInstance->getGPULightManager().setPointLightData(buffer);
        engineInstance->releaseContext();
      }
    }

    void
    LightSyncStage::syncSLLights()
    {
      Scene * scene = engineInstance->getSceneManager().getActiveScene();
      std::vector<Lighting::SpotLight *> slLights = scene->getSpotLights();

      if (slLights.size() > 0)
      {
        size_t slLightSize = slLights[0]->getDataSize();
        size_t totalSize   = slLights.size() * slLightSize;

        std::vector<char> buffer;
        buffer.resize(totalSize + sizeof(int) * 4);

        int maxLights = std::min<int>((int)slLights.size(), MAX_SPOT_LIGHTS);
        memcpy(&buffer[0], &maxLights, sizeof(int));

        size_t ptr = sizeof(int) * 4;
        for (auto sl : slLights)
        {
          sl->update(*(scene->getActiveCamera()));
          memcpy(&buffer[0] + ptr, &(sl->getData()), slLightSize);
          ptr += slLightSize;
        }

        engineInstance->acquireContext();
        engineInstance->getGPULightManager().setSpotLightData(buffer);
        engineInstance->releaseContext();
      }
    }
  } // namespace DefaultImpl
} // namespace RenderLib