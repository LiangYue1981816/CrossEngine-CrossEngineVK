precision mediump float;
#include "engine.inc"

layout (location = 0) in vec2 inTexcoord0;
layout (location = 0) out vec4 outFragColor;

DESCRIPTORSET_FRAME(16) uniform sampler2D diffuse;
DESCRIPTORSET_PASS(17) uniform Color
{
	vec4 color;
} color;

void main() 
{
  outFragColor = texture(diffuse, inTexcoord0) * color.color;
}