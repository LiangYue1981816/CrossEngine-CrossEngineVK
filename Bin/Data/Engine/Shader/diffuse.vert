precision mediump float;
#include "engine.inc"

VERTEX_ATTRIBUTE_POSITION;
VERTEX_ATTRIBUTE_TEXCOORD0;

INSTANCE_ATTRIBUTE_MODEL_TO_WORLD_MATRIX_COL0;
INSTANCE_ATTRIBUTE_MODEL_TO_WORLD_MATRIX_COL1;
INSTANCE_ATTRIBUTE_MODEL_TO_WORLD_MATRIX_COL2;
INSTANCE_ATTRIBUTE_MODEL_TO_WORLD_MATRIX_COL3;

DESCRIPTORSET_FRAME(0) uniform Camera
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
