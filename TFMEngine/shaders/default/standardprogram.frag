#version 430 core

#if defined FORWARD_SHADING
layout (location=0) out vec4 outColor;
#else
layout (location=0) out vec4 outPos;
layout (location=1) out vec4 outColorOpacity;
layout (location=2) out vec4 outNormal;
layout (location=3) out vec4 outSpecularColorShininess;
layout (location=4) out vec4 outEmissiveSpecScale;
layout (location=5) out vec4 outAmbient;
#endif

// Include basic lighting API
#include shaders/api/defaultlighting.glsl

layout (location=0) in vec3 inPos;

#ifdef ENABLE_NORMAL_ATTRIB
layout (location=1) in vec3 normal;
#endif

#ifdef ENABLE_UV
layout (location=2) in vec2 uv;
#endif

#ifdef ENABLE_TANGENT
layout (location=3) in vec3 tangent;
#endif

#ifdef ENABLE_COLOR_ATTRIB
layout (location=4) in vec4 inColor;
#endif

// ================================================================
// DIFUSE COLOR
#if defined ENABLE_DIFFUSE_COLOR
uniform vec3 diffuseColor;
#elif defined ENABLE_DIFFUSE_TEXTURE
uniform sampler2D diffuseTexture;
#endif

// SPECULAR COLOR
#if defined ENABLE_SPECULAR_COLOR
uniform vec3 specularColor;
#elif defined ENABLE_SPECULAR_TEXTURE
uniform sampler2D specularTexture;
#endif

// AMBIENT COLOR
#if defined ENABLE_AMBIENT_COLOR
uniform vec3 ambientColor;
#elif defined ENABLE_AMBIENT_TEXTURE
uniform sampler2D ambientTexture;
#endif

// SHININESS
#if defined ENABLE_SHININESS_VALUE
uniform float shininess;
#elif defined ENABLE_SHININESS_TEXTURE
uniform sampler2D shininessTexture;
#endif

// EMISSIVE COLOR
#if defined ENABLE_EMISSIVE_COLOR
uniform vec3 emissiveColor;
#elif defined ENABLE_EMISSIVE_TEXTURE
uniform sampler2D emissiveTexture;
#endif

// OPACITY
#if defined ENABLE_OPACITY_VALUE
uniform float opacity;
#elif defined ENABLE_OPACITY_TEXTURE
uniform sampler2D opacityTexture;
#endif

#ifdef ENABLE_NORMAL_MAP
uniform sampler2D normalTexture;
#endif

#ifdef ENABLE_SPECULAR_SCALE
uniform float specularScale;
#endif

void main()
{
// ==============================================================
// Gather input parameters

	pos = inPos;

#if defined ENABLE_AMBIENT_COLOR
	Ka = ambientColor;
#elif defined ENABLE_AMBIENT_TEXTURE
	Ka = texture(ambientTexture, uv).rgb;
#else
	Ka = vec3(0.0);
#endif

#if defined ENABLE_DIFFUSE_COLOR
	Kd = diffuseColor;
#elif defined ENABLE_DIFFUSE_TEXTURE
	Kd = texture(diffuseTexture, uv).rgb;
#else
	Kd = vec3(0.5); // Default gray indicates no color setted at all. Only default param
#endif

#if defined ENABLE_SPECULAR_COLOR
	Ks = specularColor;
#elif defined ENABLE_SPECULAR_TEXTURE
	Ks = texture(specularTexture, uv).rgb;
#else
	Ks = vec3(0.0);
#endif

#if defined ENABLE_SHININESS_VALUE
	shininessVal = shininess;
#elif defined ENABLE_SHININESS_TEXTURE
	shininessVal = texture(shininessTexture, uv).r;
#else
	shininessVal = 1.0;
#endif

#if defined ENABLE_SPECULAR_SCALE
	specularScaleVal = specularScale;
#else
	specularScaleVal = 1.0;
#endif

// ENABLE_NORMAL_MAP is only defined if normal map, tangents, and normal attrib are presents
#if defined ENABLE_NORMAL_MAP
	vec3 normalizedTangent = normalize(tangent);
	vec3 normalizedNormal = normalize(normal);
	normalizedTangent = normalize(normalizedTangent - (dot(normalizedTangent, normalizedNormal) * normalizedNormal));
	vec3 bitangent = normalize(cross(normalizedNormal, normalizedTangent));
	mat3 tangentBasis = mat3(normalizedTangent, bitangent, normalizedNormal);
	vec3 bumpMapNormal = (((texture(normalTexture, uv).rgb) * 2) - 1);
	normalVector = normalize(tangentBasis * bumpMapNormal);
#elif defined ENABLE_NORMAL_ATTRIB
	normalVector = normalize(normal);
#else
	normalVector = vec3(0.0); // Unlit model
#endif

#if defined ENABLE_EMISSIVE_COLOR
	Ke = emissiveColor;
#elif defined ENABLE_EMISSIVE_TEXTURE
	Ke = texture(emissiveTexture, uv).rgb;
#else
	Ke = vec3(0.0);
#endif

#if defined ENABLE_OPACITY_VALUE
	float opacityVal = opacity;
#elif defined ENABLE_OPACITY_TEXTURE
	float opacityVal = texture(opacityTexture, uv).r;
#else
	float opacityVal = 1.0;
#endif
// =============================================================

#if defined FORWARD_SHADING
	vec3 c = vec3(0.0);

	// EMISSIVE
	c += Ke;

	c += directionalLightShading();
	c += pointLightShading();
	c += spotLightShading();

	// APPLY OPACITY
	outColor = vec4(c, opacityVal);

#else
	outPos											= vec4(pos, 1.0);
	outColorOpacity							= vec4(Kd, opacityVal);
	outNormal										= vec4(normalVector, 1.0);
	outSpecularColorShininess		= vec4(Ks, shininessVal);
	outEmissiveSpecScale				= vec4(Ke, specularScaleVal);	
	outAmbient									= vec4(Ka, 1.0);
#endif
}