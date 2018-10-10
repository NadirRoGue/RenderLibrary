#include "defaultimpl/shaders/DeferredShadingProgram.h"

#include <iostream>

namespace RenderLib
{
	namespace DefaultImpl
	{
		DeferredShadingProgram::DeferredShadingProgram()
			: GPU::Program::PostProcessProgram()
		{
			fShaderFile = "shaders/default/deferredshading.frag";
		}

		void DeferredShadingProgram::onRender(EngineInstance * instance)
		{
			setUniformBlock("DLBlock", instance->getGPULightManager().getDirectionalLightBuffer().getBufferId());
			setUniformBlock("PLBlock", instance->getGPULightManager().getPointLightBuffer().getBufferId());
			setUniformBlock("SLBlock", instance->getGPULightManager().getSpotLightBuffer().getBufferId());

			Render::FBO * gBuffers = instance->getFBOManager().getFBO("GBUFFER");

			GPU::Texture::GPUTexture * positionAndSpecScale =
				gBuffers->getAttachment("position");
			GPU::Texture::GPUTexture * diffuseAndOpacity =
				gBuffers->getAttachment("diffuseAndOpacity");
			GPU::Texture::GPUTexture * normals =
				gBuffers->getAttachment("normals");
			GPU::Texture::GPUTexture * specularAndShininess =
				gBuffers->getAttachment("specularAndShininess");
			GPU::Texture::GPUTexture * emissive =
				gBuffers->getAttachment("emissiveAndSpecScale");
			GPU::Texture::GPUTexture * ambient =
				gBuffers->getAttachment("ambient");
			GPU::Texture::GPUTexture * depth =
				gBuffers->getAttachment("depth");

			unsigned int texUnit = 0;
			setUniformTexture("position", positionAndSpecScale, texUnit);
			setUniformTexture("diffuseAndOpacity", diffuseAndOpacity, texUnit);
			setUniformTexture("normals", normals, texUnit);
			setUniformTexture("specularAndShininess", specularAndShininess, texUnit);
			setUniformTexture("emissiveAndSpecScale", emissive, texUnit);
			setUniformTexture("ambient", ambient, texUnit);
			setUniformTexture("depth", depth, texUnit);
		}
	}
}