#version 310 es
precision mediump float;
#include "engine.inc"
#include "light.inc"
#include "fog.inc"

USE_VERTEX_ATTRIBUTE_POSITION;
USE_VERTEX_ATTRIBUTE_NORMAL;
USE_VERTEX_ATTRIBUTE_BINORMAL;
USE_VERTEX_ATTRIBUTE_TEXCOORD0;

USE_INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL0;
USE_INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL1;
USE_INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL2;
USE_INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL3;

layout (location = 0) out vec2 outTexcoord;
layout (location = 1) out vec3 outHalfDirection;
layout (location = 2) out vec3 outViewDirection;
layout (location = 3) out mat3 outTBN;

void main()
{
	mat4 worldMatrix = mat4(inInstanceTransformMatrixCol0, inInstanceTransformMatrixCol1, inInstanceTransformMatrixCol2, inInstanceTransformMatrixCol3);

	vec3 worldPosition = (worldMatrix * vec4(inPosition.xyz, 1.0)).xyz;
	vec3 worldCameraPosition = (cameraViewInverseMatrix * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	vec3 worldViewDirection = worldCameraPosition - worldPosition;
	worldViewDirection = normalize(worldViewDirection);

	vec3 worldHalfDirection = mainDirectLightDirection + worldViewDirection;
	worldHalfDirection = normalize(worldHalfDirection);

	vec3 worldNormal = (worldMatrix * vec4(inNormal, 0.0f)).xyz;
	worldNormal = normalize(worldNormal);

	vec3 worldBinormal = (worldMatrix * vec4(inBinormal, 0.0f)).xyz;
	worldBinormal = normalize(worldBinormal);

	vec3 t = cross(worldBinormal, worldNormal);
	vec3 b = cross(worldNormal, t);
	mat3 tbn = mat3(t, b, worldNormal);

	outTexcoord = inTexcoord0;
	outHalfDirection = worldHalfDirection;
	outViewDirection = worldViewDirection;
	outTBN = tbn;

	gl_Position = cameraProjectionMatrix * cameraViewMatrix * worldMatrix * vec4(inPosition.xyz, 1.0);
}
