precision mediump float;
#include "engine.inc"

VERTEX_ATTRIBUTE_POSITION;
VERTEX_ATTRIBUTE_TEXCOORD0;

layout (location = 0) out vec2 outTexcoord0;

void main() 
{
	outTexcoord0 = inTexcoord0;
	gl_Position = camera.viewProjectionMatrix * camera.worldViewMatrix * transform.modelWorldMatrix * vec4(inPosition.xyz, 1.0);
}
