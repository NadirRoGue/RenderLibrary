#version 430 core

// Output
layout (location=0) out vec4 outColor;

layout (location=1) in vec2 inUV;

// Input
uniform sampler2D emissive;
uniform sampler2D inputColor;
uniform bool blend;
uniform vec2 texelSize;
uniform bool horizontal;
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
	// Apply bluring horizontally or vertically depending on what is setted (more eficent than matrix kernel)
	vec3 result = texture(emissive, inUV).rgb * weight[0]; // current fragment's contribution
    if(horizontal)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(emissive, inUV + vec2(texelSize.x * i, 0.0)).rgb * weight[i];
            result += texture(emissive, inUV - vec2(texelSize.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(emissive, inUV + vec2(0.0, texelSize.y * i)).rgb * weight[i];
            result += texture(emissive, inUV - vec2(0.0, texelSize.y * i)).rgb * weight[i];
        }
    }

	result = blend? texture(inputColor, inUV).rgb + result : result;
	
	outColor = vec4(result, 1.0);
}