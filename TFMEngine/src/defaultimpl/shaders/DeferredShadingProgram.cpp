#include "defaultimpl/shaders/DeferredShadingProgram.h"

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
			Render::FBO * gBuffers = instance->getFBOManager().getFBO("GBUFFER");

			GPU::Texture::GPUTexture * positionAndSpecScale =
				gBuffers->getAttachment("positionAndSpecScale");
			GPU::Texture::GPUTexture * diffuseAndOpacity =
				gBuffers->getAttachment("diffuseAndOpacity");
			GPU::Texture::GPUTexture * normals =
				gBuffers->getAttachment("normals");
			GPU::Texture::GPUTexture * specularAndShininess =
				gBuffers->getAttachment("specularAndShininess");
			GPU::Texture::GPUTexture * emissive =
				gBuffers->getAttachment("emissive");
			GPU::Texture::GPUTexture * ambient =
				gBuffers->getAttachment("ambient");

			unsigned int texUnit = 0;
			setUniformTexture("positionAndSpecScale", positionAndSpecScale, texUnit);
			setUniformTexture("diffuseAndOpacity", diffuseAndOpacity, texUnit);
			setUniformTexture("normals", normals, texUnit);
			setUniformTexture("specularAndShininess", specularAndShininess, texUnit);
			setUniformTexture("emissive", emissive, texUnit);
			setUniformTexture("ambient", ambient, texUnit);
		}
	}
}