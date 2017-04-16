precision mediump float;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;

layout (binding = 0) uniform UBO
{
	mat4 modelViewProjectionMatrix;
} ubo;

layout (location = 0) out vec3 outColor;

void main() 
{
	outColor = inColor;
	gl_Position = ubo.modelViewProjectionMatrix * vec4(inPosition.xyz, 1.0);
}
