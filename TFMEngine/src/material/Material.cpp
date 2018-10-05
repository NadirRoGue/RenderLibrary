#include "material\Material.h"

#include "defaultimpl/shaders/StandardProgram.h"

namespace RenderLib
{
	namespace Material
	{
		// ==================================================================================

		MaterialTexture::MaterialTexture()
			: texture(NULL)
			, exists(false)
			, fileName("")
		{
		}

		bool MaterialTexture::textureExists()
		{
			return exists;
		}

		void MaterialTexture::setTexture(GPU::Texture::GPUTexture * texture)
		{
			if (texture != NULL)
			{
				this->texture = texture;
				exists = true;
			}
		}

		GPU::Texture::GPUTexture * MaterialTexture::getTexture()
		{
			return texture;
		}

		GPU::Texture::GPUTexture * MaterialTexture::getTexture() const
		{
			return texture;
		}
		// ==================================================================================

		Material::Material()
			:	twoSidedRender(false)
			,	wireFrameRender(false)
			, gpuProgram(typeid(DefaultImpl::StandardProgram))
		{

		}

		Material::~Material()
		{

		}

		const std::type_index & Material::getShaderType()
		{
			return gpuProgram;
		}
	}
}