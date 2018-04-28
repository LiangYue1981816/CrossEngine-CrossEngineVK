precision mediump float;

layout (location = 0) in vec3 inPosition;
layout (location = 4) in vec2 inTexcoord0;

layout (set = 0, binding = 0) uniform Transform
{
	mat4 modelViewProjectionMatrix;
} transform;

layout (location = 0) out vec2 outTexcoord0;

void main() 
{
	outTexcoord0 = inTexcoord0;
	gl_Position = transform.modelViewProjectionMatrix * vec4(inPosition.xyz, 1.0);
}
