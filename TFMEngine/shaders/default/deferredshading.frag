#version 430 core

layout (location=1) in vec2 inUV;

layout (location=0) out vec4 outColor;

#include shaders/api/defaultlighting.glsl

uniform sampler2D position;
uniform sampler2D diffuseAndOpacity;
uniform sampler2D normals;
uniform sampler2D specularAndShininess;
uniform sampler2D emissiveAndSpecScale;
uniform sampler2D ambient;
uniform sampler2D depth;


void main()
{
	vec4 emiAndSpecScale = texture(emissiveAndSpecScale, inUV);
	vec4 diffAndOpac = texture(diffuseAndOpacity, inUV);
	vec4 specAndShini = texture(specularAndShininess, inUV);
	
	pos = texture(position, inUV).xyz;
	Ka = texture(ambient, inUV).rgb;
	Kd = diffAndOpac.rgb;
	Ks = specAndShini.rgb;
	Ke = emiAndSpecScale.rgb;
	shininessVal = specAndShini.w;
	specularScaleVal = emiAndSpecScale.a;
	normalVector = normalize(texture(normals, inUV).xyz);

	vec3 color = vec3(0.0);

	color += Ke;

	color += directionalLightShading();
	color += pointLightShading();
	color += spotLightShading();

	outColor = vec4(color, diffAndOpac.a);
	gl_FragDepth = texture(depth, inUV).r;
}