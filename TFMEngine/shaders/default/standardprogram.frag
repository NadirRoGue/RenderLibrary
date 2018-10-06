#version 430 core

#if defined FORWARD_SHADING
layout (location=0) out vec4 outColor;
#else
layout (location=0) out vec4 outColorOpacity;
layout (location=1) out vec4 outNormalSpecularScale;
layout (location=2) out vec4 outSpecularColorShininess;
layout (location=3) out vec4 outEmissive;
layout (location=4) out vec4 outAmbient;
#endif

layout (location=0) in vec3 pos;

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

uniform vec3 LightDiffuse = vec3(1.0);
uniform vec3 LightAmbient = vec3(0.1);
uniform vec3 LightSpecular = vec3(1.0);

uniform vec3 LightDir = vec3(1.0, 1.0, 0.0);

void main()
{
// ==============================================================
// Gather input parameters

#if defined ENABLE_AMBIENT_COLOR
	vec3 Ka = ambientColor;
#elif defined ENABLE_AMBIENT_TEXTURE
	vec3 Ka = texture(ambientTexture, uv).rgb;
#else
	vec3 Ka = vec3(0.0);
#endif

#if defined ENABLE_DIFFUSE_COLOR
	vec3 Kd = diffuseColor;
#elif defined ENABLE_DIFFUSE_TEXTURE
	vec3 Kd = texture(diffuseTexture, uv).rgb;
#else
	vec3 Kd = vec3(0.5); // Default gray indicates no color setted at all. Only default param
#endif

#if defined ENABLE_SPECULAR_COLOR
	vec3 Ks = specularColor;
#elif defined ENABLE_SPECULAR_TEXTURE
	vec3 Ks = texture(specularTexture, uv).rgb;
#else
	vec3 Ks = vec3(0.0);
#endif

#if defined ENABLE_SHININESS_VALUE
	float shininessVal = shininess;
#elif defined ENABLE_SHININESS_TEXTURE
	float shininessVal = texture(shininessTexture, uv).r;
#else
	float shininessVal = 1.0;
#endif

#if defined ENABLE_SPECULAR_SCALE
	float specularScaleVal = specularScale;
#else
	float specularScaleVal = 1.0;
#endif

// ENABLE_NORMAL_MAP is only defined if normal map, tangents, and normal attrib are presents
#if defined ENABLE_NORMAL_MAP
	vec3 normalizedTangent = normalize(tangent);
	vec3 normalizedNormal = normalize(normal);
	normalizedTangent = normalize(normalizedTangent - (dot(normalizedTangent, normalizedNormal) * normalizedNormal));
	vec3 bitangent = normalize(cross(normalizedNormal, normalizedTangent));
	mat3 tangentBasis = mat3(normalizedTangent, bitangent, normalizedNormal);
	vec3 bumpMapNormal = (((texture(normalTexture, uv).rgb) * 2) - 1);
	vec3 normalVector = normalize(tangentBasis * bumpMapNormal);
#elif defined ENABLE_NORMAL_ATTRIB
	vec3 normalVector = normal;
#else
	vec3 normalVector = vec3(0.0); // Unlit model
#endif

#if defined ENABLE_EMISSIVE_COLOR
	vec3 Ke = emissiveColor;
#elif defined ENABLE_EMISSIVE_TEXTURE
	vec3 Ke = texture(emissiveTexture, uv).rgb;
#else
	vec3 Ke = vec3(0.0);
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

	// AMBIENT
	c += LightAmbient * Ka;
	
	// DIFFUSE
	vec3 D_L = normalize(LightDir);
	c += (LightDiffuse * Kd * max(dot(normalVector,D_L), 0));

	// SPECULAR
	vec3 DL_R = normalize(reflect(-D_L, normalVector));
	vec3 DL_V = normalize(-pos);
	float DL_Factor = max(dot(DL_R,DL_V), 0.01);
	c += LightSpecular * Ks * pow(DL_Factor, shininessVal) * specularScaleVal;
	
	// APPLY OPACITY
	outColor = vec4(c, opacityVal);

#else
	outColorOpacity							= vec4(Kd, opacityVal);
	outNormalSpecularScale			= vec4(normalVector, specularScaleVal);
	outSpecularColorShininess		= vec4(Ks, shininessVal);
	outEmissive									= vec4(Ke, 1.0);	
	outAmbient									= vec4(Ka, 1.0);
#endif
}