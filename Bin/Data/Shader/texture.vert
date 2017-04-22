precision mediump float;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexcoord0;

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
