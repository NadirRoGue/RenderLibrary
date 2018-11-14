#ifndef __RENDERLIB_GPU_TEXTURE_TEXTURE_H__
#define __RENDERLIB_GPU_TEXTURE_TEXTURE_H__

#include <GL/glew.h>
#include <cstdlib>

namespace RenderLib
{
  namespace GPU
  {
    namespace Texture
    {
      typedef struct GPUTextureConfig
      {
        int internalFormat; // How do we want to store the texture in the gpu memory?
        GLenum
            format; // Whats the format of the image we got form disk (what does each element we read mean)?
        GLenum
            pixelType; // Hows each element of the texture codified? (3 floats, 1 unsigned int, 1 unsigned char,...)
        bool generateMipMaps;

        int minificationFilter;
        int magnificationFilter;

        int tComponentWrapType;
        int sComponentWrapType;
        int rComponentWrapType;

        float anysotropyLevel;

        bool isMutable;

        GPUTextureConfig()
          : isMutable(true)
          , generateMipMaps(false)
          , internalFormat(GL_RGBA8)
          , format(GL_RGBA)
          , pixelType(GL_UNSIGNED_BYTE)
          , anysotropyLevel(0.0f)
        {
          setMinMaxFilter();
          setWrapType();
        }

        void
        setMinMaxFilter(int minification  = GL_NEAREST,
                        int magnification = GL_NEAREST)
        {
          minificationFilter  = minification;
          magnificationFilter = magnification;
        }

        void
        setWrapType(int wrapT = GL_REPEAT, int wrapS = GL_REPEAT,
                    int wrapR = GL_REPEAT)
        {
          tComponentWrapType = wrapT;
          sComponentWrapType = wrapS;
          rComponentWrapType = wrapR;
        }
      } GPUTextureConfig;

      class GPUTexture
      {
      protected:
        unsigned int id;

        GPUTextureConfig config;

        unsigned int width, height, depth;

      public:
        size_t index;

      public:
        GPUTexture();
        ~GPUTexture();

        virtual GLenum
        getTexturType()
            = 0;

        void
        generate();
        void
        bind();
        void
        unBind();
        unsigned int
        getId();

        void
        setSize(unsigned int width, unsigned int height = 1,
                unsigned int depth = 1);
        const unsigned int &
        getWidth();
        const unsigned int &
        getHeight();
        const unsigned int &
        getDepth();

        bool
        isMutable();

        GPUTextureConfig &
        getConfig();

        void
        upload(void * data, unsigned int width, unsigned int height = 1,
               unsigned int depth = 1);

        void
        destroy();

      protected:
        virtual void
        uploadMutable(void * data)
            = 0;
        virtual void
        uploadInmutable(void * data)
            = 0;
      };
    } // namespace Texture
  } // namespace GPU
} // namespace RenderLib

#endif