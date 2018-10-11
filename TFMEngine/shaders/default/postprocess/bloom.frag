#version 430 core

#define MASK_MAX_SIZE 64u

// Output
layout (location=0) out vec4 outColor;
//layout (location=1) out vec4 outEmission;

layout (location=1) in vec2 inUV;

// Input
uniform sampler2D emissive;
//uniform sampler2D color;

uniform vec2 texelSize;

uniform bool horizontal;
uniform bool blend;

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

	// ================================================================================

	//vec4 inColor = texture(color, inUV);
	
	// If its the last pass, we will blend the result with the color texture
	//vec3 finalColor = blend? inColor.rgb + result : inColor.rgb;
	outColor = vec4(result, 1.0);
	//outColor = vec4(finalColor, 1.0);
	//outEmission = vec4(result * 1.2, 1.0);
}