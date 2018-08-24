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

#define DESCRIPTOR_BIND_ENGINE 0
#define DESCRIPTOR_BIND_CAMERA 1


#define USE_ENGINE_UNIFORM                                                         \
	DESCRIPTOR_SET_FRAME(DESCRIPTOR_BIND_ENGINE) uniform Engine                    \
	{                                                                              \
		vec4 time;                                                                 \
		vec4 sinTime;                                                              \
		vec4 cosTime;                                                              \
		vec4 deltaTime;                                                            \
		                                                                           \
		mat4 shadowProjectionMatrix;                                               \
		mat4 shadowProjectionViewMatrix;                                           \
		mat4 shadowViewMatrix;                                                     \
		vec4 shadowParams;                                                         \
		                                                                           \
		vec4 lightFactor;                                                          \
		                                                                           \
		vec4 ambientLightRed0;                                                     \
		vec4 ambientLightRed1;                                                     \
		vec4 ambientLightRed2;                                                     \
		vec4 ambientLightGreen0;                                                   \
		vec4 ambientLightGreen1;                                                   \
		vec4 ambientLightGreen2;                                                   \
		vec4 ambientLightBlue0;                                                    \
		vec4 ambientLightBlue1;                                                    \
		vec4 ambientLightBlue2;                                                    \
		mat4 ambientLightRotationMatrix;                                           \
		                                                                           \
		vec4 pointLightColor;                                                      \
		vec4 pointLightPosition;                                                   \
		vec4 pointLightAttenuation;                                                \
		                                                                           \
		vec4 directLightColor;                                                     \
		vec4 directLightDirection;                                                 \
		                                                                           \
		vec4 fogColor;                                                             \
		vec4 fogHeightDensity;                                                     \
		vec4 fogDistanceDensity;                                                   \
	} engine
	
#define USE_CAMERA_UNIFORM                                                         \
	DESCRIPTOR_SET_CAMERA(DESCRIPTOR_BIND_CAMERA) uniform Camera                   \
	{                                                                              \
		vec4 screen;                                                               \
		vec4 zbuffer;                                                              \
		vec4 projection;                                                           \
		                                                                           \
		mat4 projectionMatrix;                                                     \
		mat4 projectionViewMatrix;                                                 \
		mat4 viewMatrix;                                                           \
		mat4 viewInverseMatrix;                                                    \
		mat4 viewInverseTransposeMatrix;                                           \
	} camera

USE_ENGINE_UNIFORM;
#define shadowProjectionMatrix engine.shadowProjectionMatrix
#define shadowProjectionViewMatrix engine.shadowProjectionViewMatrix
#define shadowViewMatrix engine.shadowViewMatrix
#define shadowClipPlane engine.shadowParams
#define shadowDistance engine.shadowParams.z
#define shadowResolution engine.shadowParams.w

#define ambientLightFactor engine.lightFactor.x
#define pointLightFactor engine.lightFactor.y
#define directLightFactor engine.lightFactor.z
#define envLightFactor engine.lightFactor.w

#define mainPointLightColor engine.pointLightColor.rgb
#define mainPointLightPosition engine.pointLightPosition.xyz
#define mainDirectLightColor engine.directLightColor.rgb
#define mainDirectLightDirection engine.directLightDirection.xyz

#define fogHeightStart engine.fogHeightDensity.x
#define fogHeightEnd engine.fogHeightDensity.y
#define fogHeightDensity engine.fogHeightDensity.z
#define fogDistanceStart engine.fogDistanceDensity.x
#define fogDistanceEnd engine.fogDistanceDensity.y
#define fogDistanceDensity engine.fogDistanceDensity.z

USE_CAMERA_UNIFORM;
#define cameraScreen camera.screen
#define cameraZBuffer camera.zbuffer
#define cameraProjection camera.projection
#define cameraProjectionMatrix camera.projectionMatrix
#define cameraProjectionViewMatrix camera.projectionViewMatrix
#define cameraViewMatrix camera.viewMatrix
#define cameraViewInverseMatrix camera.viewInverseMatrix
#define cameraViewInverseTransposeMatrix camera.viewInverseTransposeMatrix

#endif
