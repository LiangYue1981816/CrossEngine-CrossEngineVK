precision mediump float;
#include "engine.inc"

USE_VERTEX_ATTRIBUTE_POSITION;
USE_VERTEX_ATTRIBUTE_TEXCOORD0;

USE_INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL0;
USE_INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL1;
USE_INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL2;
USE_INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL3;

USE_ENGINE_CAMERA;

layout (location = 0) out vec2 outTexcoord;

void main()
{
	mat4 worldMatrix = mat4(inInstanceTransformMatrixCol0, inInstanceTransformMatrixCol1, inInstanceTransformMatrixCol2, inInstanceTransformMatrixCol3);

	outTexcoord = inTexcoord0;
	gl_Position = engineCamera.projectionMatrix * engineCamera.viewMatrix * worldMatrix * vec4(inPosition.xyz, 1.0);
}
