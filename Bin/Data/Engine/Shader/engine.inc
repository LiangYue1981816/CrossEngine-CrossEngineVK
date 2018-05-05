#define VERTEX_ATTRIBUTE_POSITION  layout (location = 0) in vec3 inPosition
#define VERTEX_ATTRIBUTE_NORMAL    layout (location = 1) in vec3 inNormal
#define VERTEX_ATTRIBUTE_BINORMAL  layout (location = 2) in vec3 inBinormal
#define VERTEX_ATTRIBUTE_COLOR     layout (location = 3) in vec3 inColor
#define VERTEX_ATTRIBUTE_TEXCOORD0 layout (location = 4) in vec2 inTexcoord0
#define VERTEX_ATTRIBUTE_TEXCOORD1 layout (location = 5) in vec2 inTexcoord1
#define VERTEX_ATTRIBUTE_INDICES   layout (location = 6) in vec4 inIndices
#define VERTEX_ATTRIBUTE_WEIGHTS   layout (location = 7) in vec4 inWeights

#define INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL0 layout (location = 8)  in vec4 inInstanceTransformMatrixCol0
#define INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL1 layout (location = 9)  in vec4 inInstanceTransformMatrixCol1
#define INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL2 layout (location = 10) in vec4 inInstanceTransformMatrixCol2
#define INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL3 layout (location = 11) in vec4 inInstanceTransformMatrixCol3

#ifdef GLES
	#define DESCRIPTORSET_FRAME(b) layout (binding = b)
	#define DESCRIPTORSET_PASS(b)  layout (binding = b)
	#define DESCRIPTORSET_DRAW(b)  layout (binding = b)
#else
	#define DESCRIPTORSET_FRAME(b) layout (set = 0, binding = b)
	#define DESCRIPTORSET_PASS(b)  layout (set = 1, binding = b)
	#define DESCRIPTORSET_DRAW(b)  layout (set = 2, binding = b)
#endif

DESCRIPTORSET_FRAME(0) uniform Camera
{
	vec3 position;
	mat4 viewProjectionMatrix;
	mat4 worldViewMatrix;
	mat4 worldViewInverseMatrix;
} camera;

DESCRIPTORSET_DRAW(1) uniform Transform
{
	mat4 modelWorldMatrix;
} transform;

DESCRIPTORSET_DRAW(2) uniform AmbientLight
{
	vec3 shRed0;
	vec3 shRed1;
	vec3 shRed2;
	
	vec3 shGreen0;
	vec3 shGreen1;
	vec3 shGreen2;
	
	vec3 shBlue0;
	vec3 shBlue1;
	vec3 shBlue2;
} ambientLight;

DESCRIPTORSET_DRAW(3) uniform PointLight
{
	vec3 position;
	vec3 color;
} pointLight;

DESCRIPTORSET_DRAW(4) uniform DirectionLight
{
	vec3 position;
	vec3 color;
} directionLight;
