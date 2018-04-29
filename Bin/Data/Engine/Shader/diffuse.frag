precision mediump float;
#include "engine.inc"

layout (location = 0) in vec2 inTexcoord0;

DESCRIPTORSET_PASS(0) uniform sampler2D diffuse;

layout (location = 0) out vec4 outFragColor;

void main() 
{
  outFragColor = texture(diffuse, inTexcoord0);
}