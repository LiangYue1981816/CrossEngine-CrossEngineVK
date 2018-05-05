precision mediump float;
#include "engine.inc"

VERTEX_ATTRIBUTE_POSITION;
VERTEX_ATTRIBUTE_TEXCOORD0;

INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL0;
INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL1;
INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL2;
INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL3;

layout (location = 0) out vec2 outTexcoord0;

void main() 
{
	mat4 modelWorldMatrix;
	modelWorldMatrix[0] = inInstanceTransformMatrixCol0;
	modelWorldMatrix[1] = inInstanceTransformMatrixCol1;
	modelWorldMatrix[2] = inInstanceTransformMatrixCol2;
	modelWorldMatrix[3] = inInstanceTransformMatrixCol3;
	
	outTexcoord0 = inTexcoord0;
	gl_Position = camera.viewProjectionMatrix * camera.worldViewMatrix * modelWorldMatrix * vec4(inPosition.xyz, 1.0);
}
