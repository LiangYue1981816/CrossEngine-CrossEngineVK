precision mediump float;
#include "engine.inc"

VERTEX_ATTRIBUTE_POSITION;
VERTEX_ATTRIBUTE_TEXCOORD0;

DESCRIPTORSET_FRAME(0) uniform Transform
{
	mat4 modelViewProjectionMatrix;
} transform;

layout (location = 0) out vec2 outTexcoord0;

void main() 
{
	outTexcoord0 = inTexcoord0;
	gl_Position = transform.modelViewProjectionMatrix * vec4(inPosition.xyz, 1.0);
}
