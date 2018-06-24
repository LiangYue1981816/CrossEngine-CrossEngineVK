#version 310 es
precision mediump float;
#include "engine.inc"
#include "light.inc"
#include "fog.inc"

DESCRIPTOR_SET_PASS(8)  uniform sampler2D texAlbedo;
DESCRIPTOR_SET_PASS(9)  uniform sampler2D texNormal;
DESCRIPTOR_SET_PASS(10) uniform sampler2D texSpecular;

layout (location = 0) in vec2 inTexcoord;
layout (location = 1) in vec3 inHalfDirection;
layout (location = 2) in vec3 inViewDirection;
layout (location = 3) in mat3 inTBN;

layout (location = 0) out vec4 outFragColor;


void main()
{
	vec3 albedoColor = Gamma2Linear(texture(texAlbedo, inTexcoord).rgb);
	vec3 specularColor = texture(texSpecular, inTexcoord).rgb;

	float metallic = 0.7;
	float roughness = 0.3;

	vec3 pixelNormal = texture(texNormal, inTexcoord).rgb * 2.0 - 1.0;
	pixelNormal = normalize(inTBN * pixelNormal);

	vec3 ambientColor = AmbientLightingSH9(albedoColor, 0.0, pixelNormal);
	vec3 lightColor = BlinnLighting(mainDirectLightColor, mainDirectLightDirection, inHalfDirection, inViewDirection, pixelNormal, albedoColor, specularColor, metallic, roughness);
	vec3 final = ambientColor + lightColor;

	outFragColor.rgb = Linear2Gamma(final);
	outFragColor.a = 1.0;
}
