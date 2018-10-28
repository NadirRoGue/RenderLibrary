#version 430 core

#include shaders/api/meshinputs.glsl
#include shaders/api/meshoutputs.glsl
#include shaders/api/transform.glsl

void main()
{
	outPos = (modelView * inPos).xyz;
	outWorldPos = (model * inPos).xyz;

#ifdef ENABLE_NORMAL
	outNormal = (normalMat * vec4(inNormal.xyz, 0.0)).xyz;
#endif

#ifdef ENABLE_UV
	outUV = inUV;
#endif

#ifdef ENABLE_TANGENT
	outTangent = (modelView * inTangent).xyz;
#endif

#ifdef ENABLE_COLOR
	outColor = inColor;
#endif
	
	gl_Position =  modelViewProj * inPos;
}
