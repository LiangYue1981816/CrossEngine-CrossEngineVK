precision mediump float;

layout (location = 0) in vec2 inTexcoord0;
layout (location = 0) out vec4 outFragColor;

layout (set = 0, binding = 1) uniform sampler2D diffuse;
layout (set = 1, binding = 0) uniform Color
{
	vec4 color;
} color;

void main() 
{
  outFragColor = texture(diffuse, inTexcoord0) * color.color;
}