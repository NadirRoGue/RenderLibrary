#version 430 core

layout (location=0) out vec4 outColor;

#include shaders/api/defaultmaterial.glsl
#include shaders/api/meshinputs.glsl
#include shaders/api/shadingutils.glsl

void main()
{
	vec3 pos = inPos.xyz;
	vec3 Kd = diffuseColor;
	vec3 Ka = Kd;
	vec3 Ks = specularColor;
	//float shininessVal = 1.0;
	vec3 normalVector = normalize(inNormal.xyz);

	vec3 c = vec3(0.0);

	vec3 dlclr = vec3(10.0);
	vec3 dldir = normalize(vec3(1,1,1));
	// AMBIENT
	c += dlclr * 0.1 * Ka;
	
	// DIFFUSE
	c += (dlclr * Kd * max(dot(normalVector, dldir), 0));
			
	// SPECULAR
	//vec3 DL_R = normalize(reflect(-dldir, normalVector));
	//vec3 DL_V = normalize(-pos);
	//float DL_Factor = max(dot(DL_R, DL_V), 0.01);
	//c += dlclr * Ks * pow(DL_Factor, shininessVal);

// =============================================================

	outColor = vec4(c, 1.0);
	gl_FragDepth = 0.0;
}