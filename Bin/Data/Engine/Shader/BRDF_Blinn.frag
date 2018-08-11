#version 310 es
precision mediump float;
#include "engine.inc"
#include "light.inc"

DESCRIPTOR_SET_PASS(8)  uniform sampler2D texAlbedo;
DESCRIPTOR_SET_PASS(9)  uniform sampler2D texNormal;
DESCRIPTOR_SET_PASS(10) uniform sampler2D texSpecular;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexcoord;
layout (location = 2) in vec3 inHalfDirection;
layout (location = 3) in vec3 inViewDirection;
layout (location = 4) in mat3 inTBN;

layout (location = 0) out vec4 outFragColor;


void main()
{
	vec3 albedoColor = Gamma2Linear(texture(texAlbedo, inTexcoord).rgb);
	vec3 specularColor = texture(texSpecular, inTexcoord).rgb;

	float metallic = 0.7;
	float roughness = 0.3;

	vec3 pixelNormal = texture(texNormal, inTexcoord).rgb * 2.0 - 1.0;
	pixelNormal = normalize(inTBN * pixelNormal);

	vec3 pointLightColor = mainPointLightColor;
	vec3 pointLightDirection = mainPointLightPosition - inPosition;
	pointLightColor = pointLightColor * PointLightAttenuation(length(pointLightDirection));
	pointLightDirection = normalize(pointLightDirection);

	vec3 ambientLightingColor = AmbientLightingSH9(albedoColor, 0.0, pixelNormal) * ambientLightFactor;
	vec3 pointLightingColor = SimpleLighting(pointLightColor, pointLightDirection, pixelNormal, albedoColor) * pointLightFactor;
	vec3 directLightingColor = BlinnLighting(mainDirectLightColor, mainDirectLightDirection, inHalfDirection, inViewDirection, pixelNormal, albedoColor, specularColor, metallic, roughness) * directLightFactor;
	vec3 finalLighting = ambientLightingColor + pointLightingColor + directLightingColor;

	finalLighting = ToneMapping(finalLighting);
	finalLighting = Linear2Gamma(finalLighting);

	outFragColor.rgb = finalLighting;
	outFragColor.a = 1.0;
}
