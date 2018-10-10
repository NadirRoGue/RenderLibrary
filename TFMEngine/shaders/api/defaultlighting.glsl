// Variant variables
vec3 pos;
vec3 Ka;
vec3 Kd;
vec3 Ks;
vec3 Ke;
float shininessVal;
float specularScaleVal;
vec3 normalVector;

struct DirectionalLightStruct
{
	vec4 dlcolor;
	float dlkd;
	float dlks;
	float dlka;
	float dlpadding;
	vec4 dldir;
};

struct PointLightStruct
{
	vec4 plcolor;
	float plkd;
	float plks;
	float plka;
	float plpadding;
	vec4 plpos;
	vec4 plattenuation;
};

struct SpotLightStruct
{
	vec4 slcolor;
	float slkd;
	float slks;
	float slka;
	float slpadding;
	vec4 slposandapperture;
	vec4 sldirandtheta;
	vec4 slattenuation;
};


layout (std140, binding = 1) uniform DLBlock 
{
	int numDLLights;
	DirectionalLightStruct dirLights[MAX_DIRECTIONAL_LIGHTS];
};

layout (std140, binding = 2) uniform PLBlock 
{
	int numPLLights;
	PointLightStruct pointLights[MAX_POINT_LIGHTS];
};

layout (std140, binding = 3) uniform SLBlock
{
	int numSLLights;
	SpotLightStruct spotLights[MAX_SPOT_LIGHTS];
};

vec3 directionalLightShading()
{
	vec3 c = vec3(0.0);
	for(int i = 0; i < MAX_DIRECTIONAL_LIGHTS; i++)
	{
		if(i == numDLLights)
			break;

		vec3 dlclr = dirLights[i].dlcolor.rgb;
		vec3 dldir = dirLights[i].dldir.xyz;
		// AMBIENT
		c += dlclr * dirLights[i].dlka * Ka;
	
		// DIFFUSE
		c += (dlclr * dirLights[i].dlkd * Kd * max(dot(normalVector, dldir), 0));
			
		// SPECULAR
		vec3 DL_R = normalize(reflect(-dldir, normalVector));
		vec3 DL_V = normalize(-pos);
		float DL_Factor = max(dot(DL_R, DL_V), 0.01);
		c += dlclr * dirLights[i].dlks * Ks * pow(DL_Factor, shininessVal) * specularScaleVal;
	}

	return c;
}

vec3 pointLightShading()
{
	vec3 c = vec3(0.0);
	for(int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		if(i == numPLLights)
			break;

		vec3 plclr = pointLights[i].plcolor.rgb;
		vec3 plpos = pointLights[i].plpos.xyz;
		vec3 platt = pointLights[i].plattenuation.xyz;

		float PL_dist = length(plpos - pos);

		float PL_fatt = min(1/(platt.x + platt.y * PL_dist + platt.z * PL_dist * PL_dist), 1);

		vec3 pldir = normalize(plpos - pos);

		// AMBIENT
		c += clamp(plclr * pointLights[i].plka * Ka, 0, 1) * PL_fatt;
	
		// DIFFUSE
		c += clamp(plclr * pointLights[i].plkd * Kd * max(dot(normalVector, pldir), 0), 0, 1) * PL_fatt;
			
		// SPECULAR
		vec3 PL_R = normalize(reflect(-pldir, normalVector));
		vec3 PL_V = normalize(-pos);
		float PL_Factor = max(dot(PL_R, PL_V), 0.01);
		c += clamp(plclr * pointLights[i].plks * Ks * pow(PL_Factor, shininessVal) * specularScaleVal, 0, 1) * PL_fatt;
	}

	return c;
}

vec3 spotLightShading()
{
	vec3 c = vec3(0.0);
	for(int i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		if(i == numSLLights)
			break;

		vec3 slclr = spotLights[i].slcolor.rgb;

		vec4 slposRAW = spotLights[i].slposandapperture;
		vec3 slpos = slposRAW.xyz;
		float apperture = slposRAW.w;

		vec4 sldirRAW = spotLights[i].sldirandtheta;
		vec3 slconedir = sldirRAW.xyz;
		float theta = sldirRAW.w;

		vec3 slatt = spotLights[i].slattenuation.xyz;

		float SL_dist = length(slpos - pos);
		float SL_fatt = min(1/(slatt.x + slatt.y * SL_dist + slatt.z * SL_dist * SL_dist), 1);

		vec3 sldir = normalize(slpos - pos);

		float cosapperture = cos(radians(apperture));
		float deviation = dot(slconedir , -sldir);
		float spotfactor = max(pow(max(deviation - cosapperture, 0) / (1 - cosapperture), theta), 0);

		// AMBIENT
		c += slclr * spotLights[i].slka * Ka * SL_fatt;
	
		// DIFFUSE
		c += clamp(spotfactor * (slclr * spotLights[i].slkd * Kd * max(dot(normalVector, sldir), 0)) * SL_fatt, 0, 1);
			
		// SPECULAR
		vec3 SL_R = normalize(reflect(-sldir, normalVector));
		vec3 SL_V = normalize(-pos);
		float SL_Factor = max(dot(SL_R, SL_V), 0.01);
		c += clamp(spotfactor * slclr * spotLights[i].slks * Ks * pow(SL_Factor, shininessVal) * specularScaleVal * SL_fatt, 0, 1);
	}

	return c;
}