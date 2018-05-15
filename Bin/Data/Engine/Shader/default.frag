precision mediump float;
#include "engine.inc"

layout (location = 0) in vec2 inTexcoord;
layout (location = 0) out vec4 outFragColor;

DESCRIPTOR_SET_PASS(8) uniform sampler2D texAlbedo;

void main()
{
	outFragColor = texture(texAlbedo, inTexcoord);
}