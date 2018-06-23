#ifndef _SHADOW_INC_
#define _SHADOW_INC_


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

#endif
