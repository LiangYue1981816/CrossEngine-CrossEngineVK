#define VERTEX_ATTRIBUTE_POSITION  layout (location = 0) in vec3 inPosition
#define VERTEX_ATTRIBUTE_NORMAL    layout (location = 1) in vec3 inNormal
#define VERTEX_ATTRIBUTE_BINORMAL  layout (location = 2) in vec3 inBinormal
#define VERTEX_ATTRIBUTE_COLOR     layout (location = 3) in vec3 inColor
#define VERTEX_ATTRIBUTE_TEXCOORD0 layout (location = 4) in vec2 inTexcoord0
#define VERTEX_ATTRIBUTE_TEXCOORD1 layout (location = 5) in vec2 inTexcoord1
#define VERTEX_ATTRIBUTE_INDICES   layout (location = 6) in vec4 inIndices
#define VERTEX_ATTRIBUTE_WEIGHTS   layout (location = 7) in vec4 inWeights

#define INSTANCE_ATTRIBUTE_MODEL_TO_WORLD_MATRIX_COL0 layout (location = 8)  in vec4 inInstanceModelToWorldMatrixCol0
#define INSTANCE_ATTRIBUTE_MODEL_TO_WORLD_MATRIX_COL1 layout (location = 9)  in vec4 inInstanceModelToWorldMatrixCol1
#define INSTANCE_ATTRIBUTE_MODEL_TO_WORLD_MATRIX_COL2 layout (location = 10) in vec4 inInstanceModelToWorldMatrixCol2
#define INSTANCE_ATTRIBUTE_MODEL_TO_WORLD_MATRIX_COL3 layout (location = 11) in vec4 inInstanceModelToWorldMatrixCol3

#ifdef GLES
	#define DESCRIPTORSET_FRAME(b) layout (binding = b)
	#define DESCRIPTORSET_PASS(b)  layout (binding = b)
	#define DESCRIPTORSET_DRAW(b)  layout (binding = b)
#else
	#define DESCRIPTORSET_FRAME(b) layout (set = 0, binding = b)
	#define DESCRIPTORSET_PASS(b)  layout (set = 1, binding = b)
	#define DESCRIPTORSET_DRAW(b)  layout (set = 2, binding = b)
#endif