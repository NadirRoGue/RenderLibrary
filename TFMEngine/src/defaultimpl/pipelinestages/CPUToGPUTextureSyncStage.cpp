#include "defaultimpl/pipelinestages/CPUToGPUTextureSyncStage.h"

#include "material/Material.h"

#include "CPU/texture/TextureManager.h"
#include "GPU/texture/GPUTextureManager.h"
#include "EngineInstance.h"

#include "GPU/texture/GPUTexture2D.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		void CPUToGPUTextureSyncStage::preRunStage()
		{
			syncTextures();
		}

		void CPUToGPUTextureSyncStage::runStage()
		{
			syncTextures();
		}

		void CPUToGPUTextureSyncStage::syncTextures()
		{
			if (elements.size() > 0)
			{
				engineInstance->acquireContext();

				for (auto component : elements)
				{
					MeshRenderer * renderable = static_cast<MeshRenderer*>(component);

					Material::Material * mat = renderable->material;

					syncTexture(mat->diffuseTexture);
					syncTexture(mat->ambientTexture);
					syncTexture(mat->specularTexture);
					syncTexture(mat->shininessTexture);
					syncTexture(mat->heightMapTexture);
					syncTexture(mat->displacementTexture);
					syncTexture(mat->normalMapTexture);
					syncTexture(mat->opacityTexture);
					syncTexture(mat->emissiveTexture);
					syncTexture(mat->otherTexture);
				}
				
				engineInstance->releaseContext();

				elements.clear();
			}
		}

		void CPUToGPUTextureSyncStage::syncTexture(Material::MaterialTexture & textureParameter)
		{
			CPU::Texture::Texture * cpuTexture = 
				CPU::Texture::TextureManager::getInstance().getTexture(textureParameter.fileName);

			if (cpuTexture != NULL)
			{
				std::vector<unsigned char> buf;
				cpuTexture->pixels.dumpAttributes(buf);

				if (buf.size() > 0)
				{
					GPU::Texture::GPUTexture * gpuTexture =
						engineInstance->getGPUTextureManager().createTexture<GPU::Texture::GPUTexture2D>(cpuTexture->index);

					gpuTexture->upload(&buf[0], cpuTexture->width, cpuTexture->height, cpuTexture->depth);

					textureParameter.setTexture(gpuTexture);
				}
				
			}
		}
	}
}