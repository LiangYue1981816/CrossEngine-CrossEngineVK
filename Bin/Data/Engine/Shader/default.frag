precision mediump float;
#include "engine.inc"

layout (location = 0) in vec2 inTexcoord0;
layout (location = 0) out vec4 outFragColor;

DESCRIPTORSET_DRAW(8) uniform sampler2D diffuse;

void main() 
{
  outFragColor = texture(diffuse, inTexcoord0);
}