#version 430 core

#include shaders/api/meshinputs.glsl
#include shaders/api/meshoutputs.glsl
#include shaders/api/transform.glsl

void main()
{
	//vec4 centerPos = vec4(0,0,0,1);
	//vec4 centerPosWorld = model * centerPos;

	//float len = length(viewPos - centerPosWorld.xyz) * 0.2;
	//mat4 scaleMat = mat4(len);
	//scaleMat[3][3] = 1.0;

	outPos = (modelView * inPos).xyz;
	outNormal = (normalMat * vec4(inNormal.xyz, 0.0)).xyz;
	
	vec4 projPos =  modelViewProj * inPos;
	gl_Position = projPos;
}
