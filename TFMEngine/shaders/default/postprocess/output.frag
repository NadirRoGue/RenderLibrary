#version 430 core

layout (location=0) out vec4 outColor;

layout (location=1) in vec2 inUV;

uniform sampler2D inputColor;

void main()
{
    outColor = texture(inputColor, inUV);
}