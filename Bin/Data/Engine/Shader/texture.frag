precision mediump float;
#include "engine.inc"

layout (location = 0) in vec2 inTexcoord0;
layout (location = 0) out vec4 outFragColor;

DESCRIPTORSET_FRAME(1) uniform sampler2D diffuse;
DESCRIPTORSET_PASS(0) uniform Color
{
	vec4 color;
} color;

void main() 
{
  outFragColor = texture(diffuse, inTexcoord0) * color.color;
}