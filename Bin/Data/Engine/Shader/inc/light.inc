#include "common.inc"


// ToneMapping
vec3 ToneMapping(vec3 x)
{
	float a = 2.51;
	float b = 0.03;
	float c = 2.43;
	float d = 0.59;
	float e = 0.14;
	return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

// Gamma
vec3 Gamma2Linear(vec3 gamma)
{
	return pow(gamma, vec3(2.2));
}

vec3 Linear2Gamma(vec3 linear)
{
	return pow(linear, vec3(1.0 / 2.2));
}


// BRDF
vec3 Ambient_SH4(vec3 albedoColor, float metallic, vec3 normal, vec4 shRed, vec4 shGreen, vec4 shBlue)
{
	vec4 basis = vec4(0.5, normal.z, normal.x, normal.y);
	vec3 diffuse = vec3(dot(basis, shRed), dot(basis, shGreen), dot(basis, shBlue));

	return (1.0 - metallic) * diffuse * albedoColor / PI;
}

vec3 Ambient_SH9(vec3 albedoColor, float metallic, vec3 normal, vec3 shRed0, vec3 shRed1, vec3 shRed2, vec3 shGreen0, vec3 shGreen1, vec3 shGreen2, vec3 shBlue0, vec3 shBlue1, vec3 shBlue2)
{
	float x = normal.x;
	float y = normal.y;
	float z = normal.z;

	vec3 basis0 = vec3(0.5, z, x);
	vec3 basis1 = vec3(y, z * z * 2.0 - x * x - y * y, z * x);
	vec3 basis2 = vec3(y * z, x * x - y * y, x * y);

	float r = dot(basis0, shRed0)   + dot(basis1, shRed1)   + dot(basis2, shRed2);
	float g = dot(basis0, shGreen0) + dot(basis1, shGreen1) + dot(basis2, shGreen2);
	float b = dot(basis0, shBlue0)  + dot(basis1, shBlue1)  + dot(basis2, shBlue2);
	vec3 diffuse = vec3(r, g, b);

	return (1.0 - metallic) * diffuse * albedoColor / PI;
}

vec3 Diffuse_Lambert(vec3 albedoColor)
{
	return albedoColor / PI;
}

float D_Blinn(float ndh, float ldh, float a, float spec)
{
	return (a + 2.0) / (8.0 * PI) * pow(ndh, a) * (spec + (1.0 - spec) * (1.0 - ldh));
}

float D_GGX(float ndh, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float d = (ndh * a2 - ndh) * ndh + 1.0;
	return a2 / (PI * d * d);
}

float G_Neumann(float ndv, float ndl)
{
	return 1.0 / (4.0 * max(ndv, ndl));
}

float G_Schlick(float ndv, float ndl, float roughness)
{
	float k = (roughness * roughness) * 0.5;

	float ggx1 = ndv * (1.0 - k) + k;
	float ggx2 = ndl * (1.0 - k) + k;

	return 0.25 / (ggx1 * ggx2);
}

float G_SchlickSmithGGX(float ndv, float ndl, float roughness)
{
	float r = roughness + 1.0;
	float k = (r * r) / 8.0;

	float ggx1 = ndv * (1.0 - k) + k;
	float ggx2 = ndl * (1.0 - k) + k;

	return 0.25 / (ggx1 * ggx2);
}

vec3 F_Schlick(float ndv, float metallic, vec3 albedo)
{
	vec3 F0 = mix(vec3(0.04), albedo, metallic);
	return F0 + (1.0 - F0) * pow5(1.0 - ndv);
}

vec3 Env_Specular(vec3 SpecularColor, float ndv, float roughness)
{
	vec4 c0 = vec4(-1.0, -0.0275, -0.572, 0.022);
	vec4 c1 = vec4( 1.0,  0.0425,  1.04, -0.04 );
	vec4 r = roughness * c0 + c1;
	float a004 = min(r.x * r.x, exp2(-9.28 * ndv)) * r.x + r.y;
	vec2 AB = vec2(-1.04, 1.04) * a004 + r.zw;
	return SpecularColor * AB.x + AB.y;
}


vec3 BRDF_Blinn(vec3 lightColor, vec3 L, vec3 H, vec3 N, vec3 albedoColor, vec3 specularColor, float roughness)
{
	float ndl = max(dot(N, L), 0.0);
	float ndh = max(dot(N, H), 0.0);
	float ldh = max(dot(L, H), 0.0);

	float D = D_Blinn(ndh, ldh, (1.0 - roughness) * 128.0, 0.0);

	vec3 diffuse = Diffuse_Lambert(albedoColor);
	vec3 specular = D * specularColor;

	return (diffuse + specular) * ndl * lightColor;
}

vec3 BRDF_BlinnGGX(vec3 lightColor, vec3 L, vec3 H, vec3 V, vec3 N, vec3 albedoColor, vec3 specularColor, float roughness)
{
	float ndl = max(dot(N, L), 0.0);
	float ndh = max(dot(N, H), 0.0);
	float ndv = max(dot(N, V), 0.0);

	float D = D_GGX(ndh, roughness);
	float G = G_Neumann(ndv, ndl);

	vec3 diffuse = Diffuse_Lambert(albedoColor);
	vec3 specular = D * G * specularColor;

	return (diffuse + specular) * ndl * lightColor;
}

vec3 BRDF_Pbr(vec3 lightColor, vec3 L, vec3 H, vec3 V, vec3 N, vec3 albedoColor, float metallic, float roughness)
{
	float ndl = max(dot(N, L), 0.0);
	float ndh = max(dot(N, H), 0.0);
	float ndv = max(dot(N, V), 0.0);

	float D = D_GGX(ndh, roughness);
	float G = G_SchlickSmithGGX(ndv, ndl, roughness);
	vec3  F = F_Schlick(ndv, metallic, albedoColor);

	vec3 factor = vec3(1.0 - metallic);
	vec3 diffuse = Diffuse_Lambert(albedoColor) * factor;
	vec3 specular = D * F * G;

	return (diffuse + specular) * ndl * lightColor;
}

vec3 BRDF_EnvSpecular(vec3 V, vec3 N, vec3 albedoColor, float metallic, float roughness, samplerCube env)
{
	float ndv = max(dot(N, V), 0.0);

	vec3 F = F_Schlick(ndv, metallic, albedoColor);
	vec3 R = reflect(-V, N);
	vec3 specularColor = Gamma2Linear(textureLod(env, R, roughness * 8.0).rgb);

	return Env_Specular(F, ndv, roughness) * specularColor;
}

vec3 BRDF_EnvSpecular(vec3 V, vec3 N, vec3 albedoColor, float metallic, float roughness, sampler2D env)
{
	float ndv = max(dot(N, V), 0.0);

	vec3 F = F_Schlick(ndv, metallic, albedoColor);
	vec3 R = reflect(-V, N);
	vec3 specularColor = Gamma2Linear(textureLod(env, SphericalSampleing(R), roughness * 8.0).rgb);

	return Env_Specular(F, ndv, roughness) * specularColor;
}
