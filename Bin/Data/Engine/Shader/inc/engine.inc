#ifndef _ENGINE_INC_
#define _ENGINE_INC_

#define USE_VERTEX_ATTRIBUTE_POSITION  layout (location = 0) in vec3 inPosition
#define USE_VERTEX_ATTRIBUTE_NORMAL    layout (location = 1) in vec3 inNormal
#define USE_VERTEX_ATTRIBUTE_BINORMAL  layout (location = 2) in vec3 inBinormal
#define USE_VERTEX_ATTRIBUTE_COLOR     layout (location = 3) in vec3 inColor
#define USE_VERTEX_ATTRIBUTE_TEXCOORD0 layout (location = 4) in vec2 inTexcoord0
#define USE_VERTEX_ATTRIBUTE_TEXCOORD1 layout (location = 5) in vec2 inTexcoord1
#define USE_VERTEX_ATTRIBUTE_INDICES   layout (location = 6) in vec4 inIndices
#define USE_VERTEX_ATTRIBUTE_WEIGHTS   layout (location = 7) in vec4 inWeights

#define USE_INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL0 layout (location = 8)  in vec4 inInstanceTransformMatrixCol0
#define USE_INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL1 layout (location = 9)  in vec4 inInstanceTransformMatrixCol1
#define USE_INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL2 layout (location = 10) in vec4 inInstanceTransformMatrixCol2
#define USE_INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL3 layout (location = 11) in vec4 inInstanceTransformMatrixCol3


#ifdef GLES
	#define DESCRIPTOR_SET_DRAW(b)   layout (binding = b)
	#define DESCRIPTOR_SET_PASS(b)   layout (binding = b)
	#define DESCRIPTOR_SET_FRAME(b)  layout (binding = b)
	#define DESCRIPTOR_SET_CAMERA(b) layout (binding = b)
#else
	#define DESCRIPTOR_SET_DRAW(b)   layout (set = 0, binding = b)
	#define DESCRIPTOR_SET_PASS(b)   layout (set = 1, binding = b)
	#define DESCRIPTOR_SET_FRAME(b)  layout (set = 2, binding = b)
	#define DESCRIPTOR_SET_CAMERA(b) layout (set = 3, binding = b)
#endif

#define DESCRIPTOR_BIND_CAMERA              0
#define DESCRIPTOR_BIND_SHADOW              1
#define DESCRIPTOR_BIND_AMBIENT_LIGHT       2
#define DESCRIPTOR_BIND_DIRECT_LIGHT        3
#define DESCRIPTOR_BIND_POINT_LIGHT         4
#define DESCRIPTOR_BIND_FOG                 5


#define USE_ENGINE_CAMERA                                                          \
	DESCRIPTOR_SET_CAMERA(DESCRIPTOR_BIND_CAMERA) uniform Camera                   \
	{                                                                              \
		mat4 projectionMatrix;                                                     \
		mat4 viewMatrix;                                                           \
		mat4 viewInverseMatrix;                                                    \
		mat4 viewInverseTransposeMatrix;                                           \
	} engineCamera

#define USE_ENGINE_SHADOW                                                          \
	DESCRIPTOR_SET_FRAME(DESCRIPTOR_BIND_SHADOW) uniform Shadow                    \
	{                                                                              \
		mat4 projectionMatrix;                                                     \
		mat4 viewMatrix;                                                           \
		vec4 params;                                                               \
	} engineShadow

#define USE_ENGINE_AMBIENT_LIGHT                                                   \
	DESCRIPTOR_SET_FRAME(DESCRIPTOR_BIND_AMBIENT_LIGHT) uniform AmbientLight       \
	{                                                                              \
		vec4 shRed0;                                                               \
		vec4 shRed1;                                                               \
		vec4 shRed2;                                                               \
		                                                                           \
		vec4 shGreen0;                                                             \
		vec4 shGreen1;                                                             \
		vec4 shGreen2;                                                             \
		                                                                           \
		vec4 shBlue0;                                                              \
		vec4 shBlue1;                                                              \
		vec4 shBlue2;                                                              \
		                                                                           \
		mat4 rotationMatrix;                                                       \
	} engineAmbientLight

#define USE_ENGINE_POINT_LIGHT                                                     \
	DESCRIPTOR_SET_FRAME(DESCRIPTOR_BIND_POINT_LIGHT) uniform PointLight           \
	{                                                                              \
		vec3 color;                                                                \
		vec3 position;                                                             \
		vec3 attenuation;                                                          \
	} enginePointLight

#define USE_ENGINE_DIRECT_LIGHT                                                    \
	DESCRIPTOR_SET_FRAME(DESCRIPTOR_BIND_DIRECT_LIGHT) uniform DirectLight         \
	{                                                                              \
		vec3 color;                                                                \
		vec3 direction;                                                            \
	} engineDirectLight

#define USE_ENGINE_FOG                                                             \
	DESCRIPTOR_SET_FRAME(DESCRIPTOR_BIND_FOG) uniform Fog                          \
	{                                                                              \
		vec3 color;                                                                \
		vec3 heightDensity;                                                        \
		vec3 distanceDensity;                                                      \
	} engineFog


USE_ENGINE_CAMERA;
#define cameraProjectionMatrix engineCamera.projectionMatrix
#define cameraViewMatrix engineCamera.viewMatrix
#define cameraViewInverseMatrix engineCamera.viewInverseMatrix
#define cameraViewInverseTransposeMatrix engineCamera.viewInverseTransposeMatrix

#endif
