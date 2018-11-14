#ifndef __RENDERLIB_GPU_TEXTURE_GPUTEXTUREMANAGER_H__
#define __RENDERLIB_GPU_TEXTURE_GPUTEXTUREMANAGER_H__

#include <memory>
#include <unordered_map>

#include "EngineException.h"

#include "logger/Log.h"

#include "GPU/texture/GPUTexture.h"

namespace RenderLib
{
  namespace GPU
  {
    namespace Texture
    {
      class GPUTextureManager
      {
      public:
        static float MAX_ANISOTROPIC_FILTERING;

      public:
        static void
        queryAnisotropicFilterSupport();

      private:
        std::unordered_map<size_t, std::unique_ptr<GPUTexture>> gpuTextures;
        std::unordered_map<std::string, std::unique_ptr<GPUTexture>>
            customTextures;

      public:
        GPUTextureManager();
        ~GPUTextureManager();

        template <class T>
        T *
        createTexture(size_t index)
        {
          // If present, return the same texture
          auto it = gpuTextures.find(index);
          if (it != gpuTextures.end())
          {
            return static_cast<T *>(it->second.get());
          }

          // Create a new one otherwise
          if (std::is_base_of<GPUTexture, T>::value)
          {
            std::unique_ptr<GPUTexture> newTexture = std::make_unique<T>();
            newTexture.get()->index                = index;
            newTexture.get()->generate();
            T * result         = static_cast<T *>(newTexture.get());
            gpuTextures[index] = std::move(newTexture);

            return result;
          }

          throw EngineException("GPUMeshManager: Attempted to create a non "
                                "GPUTexture based texture");
          return NULL;
        }

        template <class T>
        T *
        getTexture(size_t index)
        {
          if (std::is_base_of<GPUTexture, T>::value)
          {
            auto it = gpuTextures.find(index);
            if (it != gpuTextures.end())
            {
              return dynamic_cast<T *>(it->second.get());
            }
            else
            {
              return NULL;
            }
          }

          return NULL;
        }

        template <class T>
        T *
        createGPUOnlyTexture(const std::string & name)
        {
          if (std::is_base_of<GPUTexture, T>::value)
          {
            auto it = customTextures.find(name);
            if (it != customTextures.end())
            {
              Logger::Log::getInstance().logWarning(
                  "GPUTextureManager: Overwrote custom texture " + name);
            }

            std::unique_ptr<GPUTexture> newTexture = std::make_unique<T>();
            newTexture.get()->generate();
            T * result           = static_cast<T *>(newTexture.get());
            customTextures[name] = std::move(newTexture);

            return result;
          }

          throw EngineException("GPUMeshManager: Attempted to create a non "
                                "GPUTexture based texture");
          return NULL;
        }

        template <class T>
        T *
        getGPUOnlyTexture(const std::string & name)
        {
          if (std::is_base_of<GPUTexture, T>::value)
          {
            auto it = customTextures.find(name);
            if (it != customTextures.end())
            {
              return dynamic_cast<T *>(it->second.get());
            }
            else
            {
              return NULL;
            }
          }

          return NULL;
        }

        void
        clean();
      };
    } // namespace Texture
  } // namespace GPU
} // namespace RenderLib

#endif