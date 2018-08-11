#ifndef _SHADOW_INC_
#define _SHADOW_INC_

#include "engine.inc"
#include "common.inc"


vec3 EncodeFloatRGB(float value)
{
	vec3 encode;
	value = sqrt(value) * 8388608.0;
	encode.x = floor(value * 0.00001526);
	value -= encode.x * 65536.0;
	encode.y = floor(value * 0.00390625);
	value -= encode.y * 256.0;
	encode.z = floor(value);
	encode = vec3(127.0, 255.0, 255.0) - encode;
	return encode * 0.003922;
}

float DecodeFloatRGB(vec3 encode) 
{
	vec3 factor = vec3(1.9921875, 0.0077820, 0.000030398);
	float value = 1.0 - dot(encode, factor);
	return value * value;
}

float SimpleShadowDepth(sampler2D shadowmap, vec2 texCoord)
{
	return DecodeFloatRGB(texture(shadowmap, texCoord.xy).rgb);
}

float LinearShadowDepth(sampler2D shadowmap, vec2 texCoord)
{
	vec2 unnormalized = texCoord * shadowResolution;
	vec2 fractional = fract(unnormalized);
	unnormalized = floor(unnormalized);

	float v0 = DecodeFloatRGB(texture(shadowmap, (unnormalized + vec2(0.0f, 0.0f)) / shadowResolution).rgb);
	float v1 = DecodeFloatRGB(texture(shadowmap, (unnormalized + vec2(1.0f, 0.0f)) / shadowResolution).rgb);
	float v2 = DecodeFloatRGB(texture(shadowmap, (unnormalized + vec2(1.0f, 1.0f)) / shadowResolution).rgb);
	float v3 = DecodeFloatRGB(texture(shadowmap, (unnormalized + vec2(0.0f, 1.0f)) / shadowResolution).rgb);

	float v = 0.0;
	v = (v0 + (v3 - v0) * fractional.y);
	v = v + ((v1 + (v2 - v1) * fractional.y) - v) * fractional.x;

	return v;
}

float ShadowValue(vec3 position, sampler2D shadowmap)
{
	vec3 shadowViewPosition = (shadowViewMatrix * vec4(position.xyz, 1.0)).xyz;
	float curDepth = length(shadowViewPosition) - 0.001;

	vec4 projectCoord = shadowProjectionMatrix * shadowViewMatrix * vec4(position.xyz, 1.0);
	projectCoord.xy /= projectCoord.w;
 	projectCoord.xy = projectCoord.xy * 0.5 + vec2(0.5, 0.5);

//	float shadowDepth = SimpleShadowDepth(shadowmap, projectCoord.xy) * shadowClipPlane.x;
	float shadowDepth = LinearShadowDepth(shadowmap, projectCoord.xy) * shadowClipPlane.x;

	vec3 cameraPosition = (cameraViewInverseMatrix * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	float shadowAtten = (shadowDistance - length(position - cameraPosition)) / shadowDistance;
	shadowAtten = pow2(shadowAtten);
	shadowAtten = clamp(shadowAtten, 0.0, 1.0);

	float shadowValue = exp(shadowDepth - curDepth);
	shadowValue = 1.0 - (1.0 - shadowValue) * shadowAtten;
	shadowValue = clamp(shadowValue, 0.35, 1.0);

	return shadowValue;
}

#endif
