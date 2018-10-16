#version 430 core

layout (location=0) out vec4 outColor;

layout (location=1) in vec2 inUV;

uniform sampler2D inputColor;

uniform float exposure = 4.0;
uniform float gamma = 0.1;
uniform vec3 tint = vec3(1.0);

void main()
{
	// Get base color and point it towards wanted tone (tint)
    vec3 hdrColor = texture(inputColor, inUV).rgb * tint;
    // Exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    // Gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));
  
    outColor = vec4(mapped, 1.0);
}