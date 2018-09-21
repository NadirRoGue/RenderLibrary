#ifndef __RENDERLIB_MATERIAL_H__
#define __RENDERLIB_MATERIAL_H__

#include "Defines.h"

namespace RenderLib
{
	enum ShadingModel
	{
		FLAT,
		GOURAUD,
		PHONG,
		BLINN,
		TOON,
		OREN_NAYAR,
		MINNAERT,
		COOK_TORRANCE,
		FRESNEL,
		NO_SHADING_CONSTANT_COLOR
	};

	enum BlendFunc
	{
		SRC_ALPHA_ONE_MINUS_SRC_ALPHA,
		ADDITIVE
	};

	class Material
	{
	public:
		std::string name;

		VECTOR3 diffuseColor;
		VECTOR3 specularColor;
		VECTOR3 ambientColor;
		VECTOR3 emissiveColor;
		//VECTOR3 transparentColor;
		bool twoSided;
		int shadingModel;
		ShadingModel shadingModel;
		BlendFunc func;
		FLOAT opacity;
		FLOAT shininess;
		FLOAT shininessStrength;
		FLOAT indexOfRefraction;
	};
}

#endif