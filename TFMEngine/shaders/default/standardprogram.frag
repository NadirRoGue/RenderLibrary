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
#include shaders/api/defaultmaterial.glsl
#include shaders/api/meshinputs.glsl
#include shaders/api/shadingutils.glsl
#include Shaders/api/transform.glsl

void main()
{
// ==============================================================
// Gather input parameters

	pos = inPos.xyz;

#if defined ENABLE_TANGENT && defined ENABLE_NORMAL
	mat3 TBN = computeTangentSpace(inNormal.xyz, inTangent.xyz);
#endif

#ifdef ENABLE_DISPLACEMENTMAP_TEXTURE
	vec2 uv = parallaxUVMapping(TBN, viewPos.xyz, inWorldPos.xyz, displacementTexture, inUV);
#else
	vec2 uv = inUV;
#endif

	if(uv.x < -1.0 || uv.x > 1.0 || uv.y < -1.0 || uv.y > 1.0)
		discard;

#if defined ENABLE_AMBIENT_COLOR
	Ka = ambientColor;
#elif defined ENABLE_AMBIENT_TEXTURE
	Ka = texture(ambientTexture, uv).rgb;
#else
	Ka = vec3(0.0);
#endif


#if defined ENABLE_DIFFUSE_TEXTURE
	Kd = texture(diffuseTexture, uv).rgb;
#elif defined ENABLE_DIFFUSE_COLOR
	Kd = diffuseColor;
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

#if defined ENABLE_SPECULAR_SCALE_VALUE
	specularScaleVal = specularScale;
#else
	specularScaleVal = 1.0;
#endif

#if defined ENABLE_NORMALMAP_TEXTURE && defined ENABLE_NORMAL && defined ENABLE_TANGENT
	normalVector = normalize(computeBumpMapping(texture(normalTexture, uv), TBN));
#elif defined ENABLE_NORMAL
	normalVector = normalize(inNormal.xyz);
#else
	normalVector = vec3(0.0);
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
	outPos						= vec4(pos, gl_FragDepth);
	outColorOpacity				= vec4(Kd, opacityVal);
	outNormal					= vec4(normalVector, 1.0);
	outSpecularColorShininess	= vec4(Ks, shininessVal);
	outEmissiveSpecScale		= vec4(Ke, specularScaleVal);	
	outAmbient					= vec4(Ka, 1.0);
#endif
}