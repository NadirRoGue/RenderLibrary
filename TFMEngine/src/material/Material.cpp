#include "material\Material.h"

#include "defaultimpl/shaders/StandardProgram.h"

namespace RenderLib
{
  namespace Material
  {
    // ==================================================================================

    MaterialTexture::MaterialTexture()
      : texture(NULL), exists(false), fileName("")
    {
    }

    bool
    MaterialTexture::textureExists()
    {
      return exists;
    }

    void
    MaterialTexture::setTexture(GPU::Texture::GPUTexture * texture)
    {
      if (texture != NULL)
      {
        this->texture = texture;
        exists        = true;
      }
    }

    GPU::Texture::GPUTexture *
    MaterialTexture::getTexture()
    {
      return texture;
    }

    GPU::Texture::GPUTexture *
    MaterialTexture::getTexture() const
    {
      return texture;
    }
    // ==================================================================================

    Material::Material()
      : twoSidedRender(false), wireFrameRender(false), uberMask(0)
    {
    }

    Material::~Material()
    {
    }

    void
    Material::setUberMask(unsigned long long mask)
    {
      uberMask = mask;
    }

    unsigned long long
    Material::getUberMask()
    {
      return uberMask;
    }
  } // namespace Material
} // namespace RenderLib