precision mediump float;
#include "define.inc"

layout (location = 0) in vec3 inPosition;
layout (location = 4) in vec2 inTexcoord0;

layout (location = 8)  in vec4 inInstanceModelToWorldMatrixCol0;
layout (location = 9)  in vec4 inInstanceModelToWorldMatrixCol1;
layout (location = 10) in vec4 inInstanceModelToWorldMatrixCol2;
layout (location = 11) in vec4 inInstanceModelToWorldMatrixCol3;

layout (set = DESCRIPTORSET_FRAME, binding = 0) uniform Camera
{
	mat4 viewProjectionMatrix;
} camera;

layout (location = 0) out vec2 outTexcoord0;

void main() 
{
	mat4 modelToWorldMatrix;
	modelToWorldMatrix[0] = inInstanceModelToWorldMatrixCol0;
	modelToWorldMatrix[1] = inInstanceModelToWorldMatrixCol1;
	modelToWorldMatrix[2] = inInstanceModelToWorldMatrixCol2;
	modelToWorldMatrix[3] = inInstanceModelToWorldMatrixCol3;
	
	outTexcoord0 = inTexcoord0;
	gl_Position = camera.viewProjectionMatrix * modelToWorldMatrix * vec4(inPosition.xyz, 1.0);
}
