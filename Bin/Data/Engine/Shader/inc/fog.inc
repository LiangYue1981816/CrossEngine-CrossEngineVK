#ifndef _FOG_INC_
#define _FOG_INC_

#include "engine.inc"
#include "common.inc"

USE_ENGINE_FOG;


float FogDensityLinear(vec3 cameraPosition, vec3 position)
{
	float distance = length(position - cameraPosition);

	float heightDensity = (engineFog.heightDensity.x - position.y) / (engineFog.heightDensity.y - engineFog.heightDensity.x) * engineFog.heightDensity.z;
	float distanceDensity = (distance - engineFog.distanceDensity.x) / (engineFog.distanceDensity.y - engineFog.distanceDensity.x) * engineFog.distanceDensity.z;

	heightDensity = clamp(heightDensity, 0.0, 1.0);
	distanceDensity = clamp(distanceDensity, 0.0, 1.0);

	return clamp(heightDensity + distanceDensity, 0.0, 1.0);
}

float FogDensityExp(vec3 cameraPosition, vec3 position)
{
	float distance = length(position - cameraPosition);

	float heightDensity = (engineFog.heightDensity.x - position.y) * engineFog.heightDensity.z;
	float distanceDensity = (distance - engineFog.distanceDensity.x) * engineFog.distanceDensity.z;

	heightDensity = exp(heightDensity);
	distanceDensity = exp(distanceDensity);

	heightDensity = clamp(heightDensity, 0.0, 1.0);
	distanceDensity = clamp(distanceDensity, 0.0, 1.0);

	return clamp(heightDensity + distanceDensity, 0.0, 1.0);
}

float FogDensityExp2(vec3 cameraPosition, vec3 position)
{
	float distance = length(position - cameraPosition);

	float heightDensity = (engineFog.heightDensity.x - position.y) * engineFog.heightDensity.z;
	float distanceDensity = (distance - engineFog.distanceDensity.x) * engineFog.distanceDensity.z;

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
