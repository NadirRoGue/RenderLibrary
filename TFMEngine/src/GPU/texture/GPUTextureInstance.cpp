#include "GPU/texture/GPUTextureInstance.h"

#include <algorithm>

#include "EngineException.h"
#include "logger/Log.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Texture
		{
			
			float GPUTextureInstance::anysotropicFilterSupport = 0.0f;

			void GPUTextureInstance::queryMaxAnysotropyFiltering()
			{
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anysotropicFilterSupport);
				anysotropicFilterSupport = std::max<float>(anysotropicFilterSupport, 0.f);
				Logger::Log::getInstance().logInfo("Textures: Max anysotropic filtering level: " + std::to_string(anysotropicFilterSupport));
			}
			
			GPUTextureInstance::GPUTextureInstance(GPUTexture * base)
				: texture(base)
			{
				if (texture == NULL)
				{
					throw EngineException("GPUTextureInstance: Null base texture passed as constructor argument");
				}

				setMinMaxFilter();
				setWrapType();
				setAnisotropyLevel();
			}

			GPUTextureInstance::~GPUTextureInstance()
			{

			}

			void GPUTextureInstance::setMinMaxFilter(int minification, int magnification)
			{
				minificationFilter = minification;
				magnificationFilter = magnification;
			}

			void GPUTextureInstance::setWrapType(int wrapT, int wrapS, int wrapR)
			{
				tComponentWrapType = wrapT;
				sComponentWrapType = wrapS;
				rComponentWrapType = wrapR;
			}

			void GPUTextureInstance::setAnisotropyLevel(float val)
			{
				anysotropyLevel = val;
			}

			void GPUTextureInstance::bind()
			{
				texture->bind();

				GLenum type = texture->getTexturType();
				glTexParameteri(type, GL_TEXTURE_MIN_FILTER, minificationFilter);
				glTexParameteri(type, GL_TEXTURE_MAG_FILTER, magnificationFilter);
				glTexParameteri(type, GL_TEXTURE_WRAP_T, tComponentWrapType);
				glTexParameteri(type, GL_TEXTURE_WRAP_S, sComponentWrapType);
				glTexParameteri(type, GL_TEXTURE_WRAP_R, rComponentWrapType);

			}
		}
	}
}