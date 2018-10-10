#version 430 core

layout (location=0) in vec4 inPos;	
layout (location=1) in vec2 inUV;

layout (location=0) out vec3 outPos;
layout (location=1) out vec2 outUV;

void main()
{
	outUV = inUV;//vec2(0.5) + inPos.xy * 0.5 ;
	outPos = inPos.xyz;
	gl_Position = inPos;
}