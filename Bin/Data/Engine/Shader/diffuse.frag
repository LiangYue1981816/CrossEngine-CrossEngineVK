precision mediump float;
#include "define.inc"

layout (location = 0) in vec2 inTexcoord0;

layout (set = DESCRIPTORSET_PASS, binding = 0) uniform sampler2D diffuse;

layout (location = 0) out vec4 outFragColor;

void main() 
{
  outFragColor = texture(diffuse, inTexcoord0);
}