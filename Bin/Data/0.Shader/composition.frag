precision mediump float;
precision mediump subpassInput;

layout (input_attachment_index = 0, binding = 0) uniform subpassInput samplerTexture;
layout (location = 0) out vec4 outFragColor;

void main() 
{
	outFragColor = subpassLoad(samplerTexture);
}