#ifndef __RENDERLIB_CPU_MATERIAL_H__
#define __RENDERLIB_CPU_MATERIAL_H__

#include "Defines.h"

#include "GPU/texture/GPUTexture.h"
#include "CPU/texture/TextureStackBlendOperation.h"

namespace RenderLib
{
	namespace Material
	{
		// Its a copy from the IO::LoadedParameter class ... but this way we reduce dependency
		template<class T>
		class MaterialParameter
		{
		private:
			T value;
			bool exists;
		public:
			MaterialParameter()
				: exists(false)
			{
			}

			void setData(const T value, const bool & exists)
			{
				this->value = value;
				this->exists = exists;
			}

			void set(const T & newVal)
			{
				value = newVal;
				exists = true;
			}

			T & get()
			{
				return value;
			}

			T & operator()()
			{
				return value;
			}

			const T & get() const
			{
				return value;
			}

			const T & operator()() const
			{
				return value;
			}
		};

		class MaterialTexture
		{
		private:
			GPU::Texture::GPUTexture * texture;
			bool exists;
		public:
			GPU::Texture::GPUTextureConfig config;
			std::string fileName;
		public:
			MaterialTexture();
			bool textureExists();
			GPU::Texture::GPUTexture * getTexture();
			GPU::Texture::GPUTexture * getTexture() const;
			void setTexture(GPU::Texture::GPUTexture * texture);
		};

		class Material
		{
		public:
			unsigned int index;
			MaterialParameter<std::string> name;
			MaterialParameter<VECTOR3> diffuseColor;
			MaterialParameter<VECTOR3> specularColor;
			MaterialParameter<VECTOR3> ambientColor;
			MaterialParameter<VECTOR3> emissiveColor;
			MaterialParameter<VECTOR3> transparentColor; //wtf
			MaterialParameter<FLOAT> opacity;
			MaterialParameter<FLOAT> shininess;
			MaterialParameter<FLOAT> specularScale; // A.K.A shininessStrength
			MaterialParameter<FLOAT> indexOfRefraction;

			MaterialTexture diffuseTexture;
			MaterialTexture specularTexture;
			MaterialTexture shininessTexture;
			MaterialTexture ambientTexture;
			MaterialTexture emissiveTexture;
			MaterialTexture normalMapTexture;
			MaterialTexture opacityTexture;
			MaterialTexture heightMapTexture;
			MaterialTexture displacementTexture;
			MaterialTexture otherTexture;

			bool wireFrameRender;
			bool twoSidedRender;
		public:
			Material();
			~Material();
		};
	}
}

#endif