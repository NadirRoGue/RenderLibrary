#version 430 core

layout (location=0) in vec4 inPos;

uniform mat4 modelViewProj;

void main()
{
	gl_Position = modelViewProj * inPos;
}