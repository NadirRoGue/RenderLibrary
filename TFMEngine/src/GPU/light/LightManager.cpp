#include "GPU/light/LightManager.h"

#include <gl/glew.h>

#include <iostream>

namespace RenderLib
{
  namespace GPU
  {
    namespace Light
    {
      LightManager::LightManager()
      {
      }

      LightManager::~LightManager()
      {
      }

      void
      LightManager::initializeBuffers()
      {
        dirLightBuffer.generate();
        pointLightBuffer.generate();
        spotLightBuffer.generate();
      }

      LightBuffer &
      LightManager::getDirectionalLightBuffer()
      {
        return dirLightBuffer;
      }

      LightBuffer &
      LightManager::getPointLightBuffer()
      {
        return pointLightBuffer;
      }

      LightBuffer &
      LightManager::getSpotLightBuffer()
      {
        return spotLightBuffer;
      }

      void
      LightManager::setDirectionalLightData(std::vector<char> & bytesData)
      {
        dirLightBuffer.setData(&bytesData[0], bytesData.size());
      }

      void
      LightManager::setPointLightData(std::vector<char> & bytesData)
      {
        pointLightBuffer.setData(&bytesData[0], bytesData.size());
      }

      void
      LightManager::setSpotLightData(std::vector<char> & bytesData)
      {
        spotLightBuffer.setData(&bytesData[0], bytesData.size());
      }

      void
      LightManager::clean()
      {
        dirLightBuffer.destroy();
        spotLightBuffer.destroy();
        pointLightBuffer.destroy();
      }
    } // namespace Light
  } // namespace GPU
} // namespace RenderLib