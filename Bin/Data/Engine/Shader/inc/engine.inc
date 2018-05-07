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
	#define DESCRIPTOR_SET_DRAW(b)  layout (binding = b)
	#define DESCRIPTOR_SET_PASS(b)  layout (binding = b)
	#define DESCRIPTOR_SET_FRAME(b) layout (binding = b)
#else
	#define DESCRIPTOR_SET_DRAW(b)  layout (set = 0, binding = b)
	#define DESCRIPTOR_SET_PASS(b)  layout (set = 1, binding = b)
	#define DESCRIPTOR_SET_FRAME(b) layout (set = 2, binding = b)
#endif

#define DESCRIPTOR_BIND_CAMERA              0
#define DESCRIPTOR_BIND_TRANSFORM           1
#define DESCRIPTOR_BIND_AMBIENT_LIGHT       2
#define DESCRIPTOR_BIND_POINT_LIGHT         3
#define DESCRIPTOR_BIND_DIRECTION_LIGHT     4


#define USE_ENGINE_CAMERA                                                      \
	DESCRIPTOR_SET_FRAME(DESCRIPTOR_BIND_CAMERA) uniform Camera                  \
	{                                                                            \
		vec3 position;                                                             \
		mat4 projectionMatrix;                                                     \
		mat4 viewMatrix;                                                           \
		mat4 viewInverseMatrix;                                                    \
	} engineCamera

#define USE_ENGINE_TRANSFORM                                                   \
	DESCRIPTOR_SET_DRAW(DESCRIPTOR_BIND_TRANSFORM) uniform Transform             \
	{                                                                            \
		mat4 transformMatrix;                                                      \
	} engineTransform

#define USE_ENGINE_AMBIENT_LIGHT                                               \
	DESCRIPTOR_SET_DRAW(DESCRIPTOR_BIND_AMBIENT_LIGHT) uniform AmbientLight      \
	{                                                                            \
		vec3 shRed0;                                                               \
		vec3 shRed1;                                                               \
		vec3 shRed2;                                                               \
		                                                                           \
		vec3 shGreen0;                                                             \
		vec3 shGreen1;                                                             \
		vec3 shGreen2;                                                             \
		                                                                           \
		vec3 shBlue0;                                                              \
		vec3 shBlue1;                                                              \
		vec3 shBlue2;                                                              \
	} engineAmbientLight

#define USE_ENGINE_POINT_LIGHT                                                 \
	DESCRIPTOR_SET_DRAW(DESCRIPTOR_BIND_POINT_LIGHT) uniform PointLight          \
	{                                                                            \
		vec3 position;                                                             \
		vec3 color;                                                                \
	} enginePointLight

#define USE_ENGINE_DIRECTION_LIGHT                                             \
	DESCRIPTOR_SET_DRAW(DESCRIPTOR_BIND_DIRECTION_LIGHT) uniform DirectionLight  \
	{                                                                            \
		vec3 position;                                                             \
		vec3 color;                                                                \
	} engineDirectionLight
