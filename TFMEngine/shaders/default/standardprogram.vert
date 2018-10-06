#version 430 core

// ==========================================================
// INPUT

layout (location=0) in vec4 inPos;

#ifdef ENABLE_NORMAL_ATTRIB
layout (location=1) in vec4 inNormal;
#endif

#ifdef ENABLE_UV
layout (location=2) in vec2 inUV;
#endif

#ifdef ENABLE_TANGENT
layout (location=3) in vec4 inTangent;
#endif

#ifdef ENABLE_COLOR_ATTRIB
layout (location=4) in vec4 inColor;
#endif

// ==========================================================
// OUTPUT

layout (location=0) out vec3 outPos;

#ifdef ENABLE_NORMAL_ATTRIB
layout (location=1) out vec3 outNormal;
#endif

#ifdef ENABLE_UV
layout (location=2) out vec2 outUV;
#endif

#ifdef ENABLE_TANGENT
layout (location=3) out vec3 outTangent;
#endif

#ifdef ENABLE_COLOR_ATTRIB
layout (location=4) out vec4 outColor;
#endif

// =========================================================
// UNIFORMS

uniform mat4 modelViewProj;
uniform mat4 modelView;

#ifdef ENABLE_NORMAL_ATTRIB
uniform mat4 normalMat;
#endif

// ==========================================================

void main()
{
	outPos = (modelView * inPos).xyz;

#ifdef ENABLE_NORMAL_ATTRIB
	outNormal = (normalMat * vec4(inNormal.xyz, 0.0)).xyz;
#endif

#ifdef ENABLE_UV
	outUV = inUV;
#endif

#ifdef ENABLE_TANGENT
	outTanget = (modelView * inTangent).xyz;
#endif

#ifdef ENABLE_COLOR_ATTRIB
	outColor = inColor;
#endif
	
	gl_Position =  modelViewProj * inPos;
}
