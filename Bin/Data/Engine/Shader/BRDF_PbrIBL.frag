#version 310 es
precision mediump float;
#include "engine.inc"
#include "light.inc"

DESCRIPTOR_SET_PASS(8)  uniform sampler2D texAO;
DESCRIPTOR_SET_PASS(9)  uniform sampler2D texAlbedo;
DESCRIPTOR_SET_PASS(10) uniform sampler2D texNormal;
DESCRIPTOR_SET_PASS(11) uniform sampler2D texRoughMetallic;
DESCRIPTOR_SET_PASS(12) uniform samplerCube texEnv;

layout (location = 0) in vec2 inTexcoord;
layout (location = 1) in vec3 inHalfDirection;
layout (location = 2) in vec3 inViewDirection;
layout (location = 3) in mat3 inTBN;

layout (location = 0) out vec4 outFragColor;


void main()
{
	vec3 albedoColor = Gamma2Linear(texture(texAlbedo, inTexcoord).rgb);
	vec3 rough_metallic = texture(texRoughMetallic, inTexcoord).rgb;
	vec3 ao = texture(texAO, inTexcoord).rgb;

	float metallic = rough_metallic.b;
	float roughness = rough_metallic.g;

	vec3 pixelNormal = texture(texNormal, inTexcoord).rgb * 2.0 - 1.0;
	pixelNormal = normalize(inTBN * pixelNormal);

	vec3 ambientColor = AmbientLightingSH9(albedoColor, metallic, pixelNormal);
	vec3 specularColor = EnvSpecular(inViewDirection, pixelNormal, albedoColor, metallic, roughness, 8.0, texEnv);
	vec3 lightColor = PBRLighting(mainDirectLightColor, mainDirectLightDirection, inHalfDirection, inViewDirection, pixelNormal, albedoColor, metallic, roughness);
	vec3 final = (ambientColor + specularColor + lightColor) * ao;

	outFragColor.rgb = Linear2Gamma(final);
	outFragColor.a = 1.0;
}
