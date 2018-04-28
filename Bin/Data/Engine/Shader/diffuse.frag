precision mediump float;

layout (set = 1, binding = 0) uniform sampler2D diffuse;

layout (location = 0) in vec2 inTexcoord0;
layout (location = 0) out vec4 outFragColor;

void main() 
{
  outFragColor = texture(diffuse, inTexcoord0);
}