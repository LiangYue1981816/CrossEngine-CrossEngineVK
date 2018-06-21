#ifndef _COMMON_INC_
#define _COMMON_INC_

#define PI 3.1415926535897932384626433832795


float pow2(float x)
{
	return x * x;
}

float pow3(float x)
{
	return x * x * x;
}

float pow4(float x)
{
	return x * x * x * x;
}

float pow5(float x)
{
	return x * x * x * x * x;
}

vec3 pow2(vec3 x)
{
	return x * x;
}

vec3 pow3(vec3 x)
{
	return x * x * x;
}

vec3 pow4(vec3 x)
{
	return x * x * x * x;
}

vec3 pow5(vec3 x)
{
	return x * x * x * x * x;
}

vec2 SphericalSampleing(vec3 v)
{
	vec2 invAtan = vec2(1.0 / (2.0 * PI), 1.0 / (1.0 * PI));
	vec2 uv = vec2(atan(v.x, v.z), -asin(v.y));

	uv *= invAtan;
	uv += 0.5;

	return uv;
}

float Attenuation(float dis, float linear, float square, float constant)
{
	return 1.0 / (linear * dis + square * dis * dis + constant);
}

#endif
