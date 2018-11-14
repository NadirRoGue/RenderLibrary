#version 430 core

layout (location=0) out vec4 outColor;

uniform vec3 pickColor;

uniform int zeroDepth;

void main()
{
	outColor = vec4(pickColor, 0.0);

	gl_FragDepth = zeroDepth == 1? 0.0 : gl_FragDepth;
}