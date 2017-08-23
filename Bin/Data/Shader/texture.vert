precision mediump float;

#include "vertex.inc"

layout (binding = 0) uniform UBO
{
	mat4 modelViewProjectionMatrix;
} ubo;

layout (location = 0) out vec2 outTexcoord0;

void main() 
{
	outTexcoord0 = inTexcoord0;
	gl_Position = ubo.modelViewProjectionMatrix * vec4(inPosition.xyz, 1.0);
}
