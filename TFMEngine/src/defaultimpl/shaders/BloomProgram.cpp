#include "defaultimpl/shaders/BloomProgram.h"

namespace RenderLib
{
  namespace DefaultImpl
  {
    BloomProgram::BloomProgram() : GPU::Program::PostProcessProgram()
    {
      fShaderFile = "shaders/default/postprocess/bloom.frag";
    }

    void
    BloomProgram::initializeShader(EngineInstance * instance)
    {
      fbos[0].fbo   = instance->getFBOManager().createFBO("BloomFBO1");
      fbos[0].input = fbos[0].fbo->addColorAttachment(
          0, "emissiveBuffer_0", GL_RGBA16F, GL_RGBA, GL_FLOAT);
      fbos[0].input->getConfig().setMinMaxFilter(GL_LINEAR, GL_LINEAR);
      fbos[0].fbo->setSize(500, 500);

      fbos[1].fbo   = instance->getFBOManager().createFBO("BloomFBO2");
      fbos[1].input = fbos[1].fbo->addColorAttachment(
          0, "emissiveBuffer_1", GL_RGBA16F, GL_RGBA, GL_FLOAT);
      fbos[1].input->getConfig().setMinMaxFilter(GL_LINEAR, GL_LINEAR);
      fbos[1].fbo->setSize(500, 500);
    }

    void
    BloomProgram::onRender(GPU::Texture::GPUTexture * previousOutput,
                           EngineInstance * instance)
    {
      // Grab currently bound fbo
      GLint currentFBO;
      glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFBO);
      // Grab current clear color
      GLfloat bkColor[4];
      glGetFloatv(GL_COLOR_CLEAR_VALUE, bkColor);

      // Clear emissive temporary buffers with black to avoid artifacts
      glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
      fbos[0].fbo->bind();
      glClear(GL_COLOR_BUFFER_BIT);
      fbos[1].fbo->bind();
      glClear(GL_COLOR_BUFFER_BIT);

      Render::FBO * gBuffers = instance->getFBOManager().getFBO("GBUFFER");

      // Send texel size
      float fWidth      = static_cast<float>(gBuffers->getWidth());
      float fHeight     = static_cast<float>(gBuffers->getHeight());
      float texelSize[] = { 1.0f / fWidth, 1.0f / fHeight };
      setUniform2FV("texelSize", 1, texelSize);

      // Send g-buffers initial texture
      GPU::Texture::GPUTexture * emissive
          = gBuffers->getAttachment("emissiveAndSpecScale");
      unsigned int texUnit = 0;
      setUniformTexture("emissive", emissive, texUnit);

      unsigned int i = 0;
      while (i < 8)
      {
        unsigned int tick = i % 2;
        fbos[tick].fbo->bind();

        setUniformI("blend", 0);
        setUniformI("horizontal", tick);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)0);

        // Prepare texture for next step
        texUnit = 0;
        setUniformTexture("emissive", fbos[tick].input, texUnit);
        i++;
      }

      setUniformI("horizontal", i % 2);
      setUniformI("blend", 1);
      setUniformTexture("inputColor", previousOutput, texUnit);

      glClearColor(bkColor[0], bkColor[1], bkColor[2], 0.0f);
      glBindFramebuffer(GL_FRAMEBUFFER, currentFBO);
    }
  } // namespace DefaultImpl
} // namespace RenderLib