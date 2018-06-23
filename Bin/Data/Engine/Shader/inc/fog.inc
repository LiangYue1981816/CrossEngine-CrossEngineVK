#ifndef _FOG_INC_
#define _FOG_INC_

#include "engine.inc"
#include "common.inc"

USE_ENGINE_FOG;

#define fogHeightStart engineFog.heightDensity.x
#define fogHeightEnd engineFog.heightDensity.y
#define fogHeightDensity engineFog.heightDensity.z

#define fogDistanceStart engineFog.distanceDensity.x
#define fogDistanceEnd engineFog.distanceDensity.y
#define fogDistanceDensity engineFog.distanceDensity.z


float FogDensityLinear(vec3 cameraPosition, vec3 position)
{
	float distance = length(position - cameraPosition);

	float heightDensity = clamp((fogHeightEnd - position.y) / (fogHeightEnd - fogHeightStart), 0.0, 1.0) * fogHeightDensity;
	float distanceDensity = clamp((distance - fogDistanceStart) / (fogDistanceEnd - fogDistanceStart), 0.0, 1.0) * fogDistanceDensity;

	return clamp(heightDensity + distanceDensity, 0.0, 1.0);
}

float FogDensityExp(vec3 cameraPosition, vec3 position)
{
	float distance = length(position - cameraPosition);

	float heightDensity = (fogHeightEnd - position.y) * fogHeightDensity;
	float distanceDensity = (distance - fogDistanceStart) * fogDistanceDensity;

	heightDensity = exp(heightDensity);
	distanceDensity = exp(distanceDensity);

	heightDensity = clamp(heightDensity, 0.0, 1.0);
	distanceDensity = clamp(distanceDensity, 0.0, 1.0);

	return clamp(heightDensity + distanceDensity, 0.0, 1.0);
}

float FogDensityExp2(vec3 cameraPosition, vec3 position)
{
	float distance = length(position - cameraPosition);

	float heightDensity = (fogHeightEnd - position.y) * fogHeightDensity;
	float distanceDensity = (distance - fogDistanceStart) * fogDistanceDensity;

	heightDensity = exp(heightDensity * heightDensity);
	distanceDensity = exp(distanceDensity * distanceDensity);

	heightDensity = clamp(heightDensity, 0.0, 1.0);
	distanceDensity = clamp(distanceDensity, 0.0, 1.0);

	return clamp(heightDensity + distanceDensity, 0.0, 1.0);
}

vec3 FogColor(vec3 color, float density)
{
	return mix(color, engineFog.color, density);
}

#endif
