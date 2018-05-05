precision mediump float;
#include "engine.inc"

USE_VERTEX_ATTRIBUTE_POSITION;
USE_VERTEX_ATTRIBUTE_TEXCOORD0;

USE_ENGINE_TRANSFORM;

layout (location = 0) out vec2 outTexcoord0;

void main() 
{
	outTexcoord0 = inTexcoord0;
	gl_Position = engineTransform.transformMatrix * vec4(inPosition.xyz, 1.0);
}
