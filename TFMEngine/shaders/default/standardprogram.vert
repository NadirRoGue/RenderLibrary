#version 430 core

layout (location=0) in vec4 inPos;	
layout (location=1) in vec4 inNormal;

layout (location=0) out vec3 outPos;
layout (location=1) out vec3 outNormal;

uniform mat4 modelViewProj;
uniform mat4 modelView;
uniform mat4 normalMat;

void main()
{
	outPos = (modelView * inPos).xyz;
	outNormal = (normalMat * vec4(inNormal.xyz, 0.0)).xyz;
	gl_Position =  modelViewProj * inPos;
}
