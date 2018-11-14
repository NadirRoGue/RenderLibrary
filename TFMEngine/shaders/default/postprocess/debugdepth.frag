#version 430 core

layout (location=0) out vec4 outColor;

layout (location=1) in vec2 inUV;

uniform sampler2D inputTex;

void main()
{
	outColor = vec4(texture(inputTex, inUV).xxx, 1.0);
}