#include "defaultimpl/shaders/BloomProgram.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		BloomProgram::BloomProgram()
			: GPU::Program::PostProcessProgram()
		{
			fShaderFile = "shaders/default/postprocess/bloom.frag";
		}

		void BloomProgram::initializeShader(EngineInstance * instance)
		{
			emissive1 = instance->getFBOManager().createFBO("BloomFBO1");
			emissive1->addColorAttachment(0, "emissiveBuffer", GL_RGBA16F, GL_RGBA, GL_FLOAT);
			emissive1->setSize(500, 500);

			emissive2 = instance->getFBOManager().createFBO("BloomFBO2");
			emissive2->addColorAttachment(0, "emissiveBuffer", GL_RGBA16F, GL_RGBA, GL_FLOAT);
			emissive2->setSize(500, 500);

			fbos[0] = emissive1;
			fbos[1] = emissive2;
		}

		void BloomProgram::onRender(EngineInstance * instance)
		{
			// Grab currently bound fbo
			GLint currentFBO;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFBO);

			Render::FBO * gBuffers = instance->getFBOManager().getFBO("GBUFFER");

			// Send texel size
			float fWidth = static_cast<float>(gBuffers->getWidth());
			float fHeight = static_cast<float>(gBuffers->getHeight());
			float texelSize[] = { 1.0f / fWidth,1.0f / fHeight };
			setUniform2FV("texelSize", 1, texelSize);

			// Send g-buffers initial texture
			GPU::Texture::GPUTexture * emissive =
				gBuffers->getAttachment("emissiveAndSpecScale");
			unsigned int texUnit = 0;
			setUniformTexture("emissive", emissive, texUnit);

			setUniformI("blend", 0);
			unsigned int i = 0;
			while (i < 8)
			{
				unsigned int tick = i % 2;

				fbos[tick]->bind();

				setUniformI("horizontal", tick);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

				// Prepare texture for next step
				texUnit = 0;
				setUniformTexture("emissive", fbos[tick]->getAttachment("emissiveBuffer"), texUnit);
				i++;
			}

			setUniformI("horizontal", 1);
			setUniformI("blend", 1);

			glBindFramebuffer(GL_FRAMEBUFFER, currentFBO);

			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
		}
	}
}