precision mediump float;
#include "engine.inc"

USE_VERTEX_ATTRIBUTE_POSITION;
USE_VERTEX_ATTRIBUTE_TEXCOORD0;

USE_INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL0;
USE_INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL1;
USE_INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL2;
USE_INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL3;

USE_ENGINE_CAMERA;

layout (location = 0) out vec2 outTexcoord0;

void main() 
{
	mat4 transformMatrix;
	transformMatrix[0] = inInstanceTransformMatrixCol0;
	transformMatrix[1] = inInstanceTransformMatrixCol1;
	transformMatrix[2] = inInstanceTransformMatrixCol2;
	transformMatrix[3] = inInstanceTransformMatrixCol3;
	
	outTexcoord0 = inTexcoord0;
	gl_Position = engineCamera.projectionMatrix * engineCamera.viewMatrix * transformMatrix * vec4(inPosition.xyz, 1.0);
}
