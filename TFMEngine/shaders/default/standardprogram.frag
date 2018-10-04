#version 430 core

layout (location=0) out vec4 outColor;

layout (location=0) in vec3 pos;
layout (location=1) in vec3 normal;
layout (location=2) in vec2 uv;

uniform vec3 LightDiffuse = vec3(1.0);
uniform vec3 LightAmbient = vec3(0.1);
uniform vec3 LightSpecular = vec3(1.0);

uniform vec3 LightDir = vec3(1.0, 1.0, 0.0);

uniform sampler2D diffuseTexture;

void main()
{
	vec3 Ka = texture(diffuseTexture, uv).rgb;//vec3(1.0, 0.0, 0.0);
	vec3 Ks = vec3(1.0);

	vec3 c = vec3(0.0);

	c += LightAmbient * Ka;

	vec3 D_L = normalize(LightDir);
	
	// LUZ DIFUSA
	c += (LightDiffuse * Ka * max(dot(normal,D_L), 0));

	vec3 DL_R = normalize(reflect(-D_L, normal));
	vec3 DL_V = normalize(-pos);
	float DL_Factor = max(dot(DL_R,DL_V), 0.01);
	c += LightSpecular * Ks * DL_Factor;
	
	outColor = vec4(c, 1.0);
}