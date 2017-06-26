#ifdef PLATFORM_WINDOWS

#ifndef __gl31_h_
#define __gl31_h_ 1

#ifdef __cplusplus
extern "C" {
#endif

/*
** Copyright (c) 2013-2015 The Khronos Group Inc.
**
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and/or associated documentation files (the
** "Materials"), to deal in the Materials without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Materials, and to
** permit persons to whom the Materials are furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be included
** in all copies or substantial portions of the Materials.
**
** THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
** MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
*/
/*
** This header is generated from the Khronos OpenGL / OpenGL ES XML
** API Registry. The current version of the Registry, generator scripts
** used to make the header, and the header can be found at
**   http://www.opengl.org/registry/
**
** Khronos $Revision$ on $Date$
*/

#include <GLES3/gl3platform.h>

#ifndef GL_APIENTRYP
#define GL_APIENTRYP GL_APIENTRY*
#endif

#ifndef GL_GLES_PROTOTYPES
#define GL_GLES_PROTOTYPES 1
#endif

#ifndef GLEW_FUN_EXPORT
#define GLEW_FUN_EXPORT extern __declspec(dllimport)
#endif

#ifndef GLEW_GET_FUN
#define GLEW_GET_FUN(x) x
#endif

/* Generated on date 20151015 */

/* Generated C header for:
 * API: gles2
 * Profile: common
 * Versions considered: 2\.[0-9]|3\.[01]
 * Versions emitted: .*
 * Default extensions included: None
 * Additional extensions included: _nomatch_^
 * Extensions removed: _nomatch_^
 */

#ifndef GL_ES_VERSION_2_0
#define GL_ES_VERSION_2_0 1
#include <KHR/khrplatform.h>
typedef khronos_int8_t GLbyte;
typedef khronos_float_t GLclampf;
typedef khronos_int32_t GLfixed;
typedef short GLshort;
typedef unsigned short GLushort;
typedef void GLvoid;
typedef struct __GLsync *GLsync;
typedef khronos_int64_t GLint64;
typedef khronos_uint64_t GLuint64;
typedef unsigned int GLenum;
typedef unsigned int GLuint;
typedef char GLchar;
typedef khronos_float_t GLfloat;
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;
typedef unsigned int GLbitfield;
typedef int GLint;
typedef unsigned char GLboolean;
typedef int GLsizei;
typedef khronos_uint8_t GLubyte;
#define GL_DEPTH_BUFFER_BIT               0x00000100
#define GL_STENCIL_BUFFER_BIT             0x00000400
#define GL_COLOR_BUFFER_BIT               0x00004000
#define GL_FALSE                          0
#define GL_TRUE                           1
#define GL_POINTS                         0x0000
#define GL_LINES                          0x0001
#define GL_LINE_LOOP                      0x0002
#define GL_LINE_STRIP                     0x0003
#define GL_TRIANGLES                      0x0004
#define GL_TRIANGLE_STRIP                 0x0005
#define GL_TRIANGLE_FAN                   0x0006
#define GL_ZERO                           0
#define GL_ONE                            1
#define GL_SRC_COLOR                      0x0300
#define GL_ONE_MINUS_SRC_COLOR            0x0301
#define GL_SRC_ALPHA                      0x0302
#define GL_ONE_MINUS_SRC_ALPHA            0x0303
#define GL_DST_ALPHA                      0x0304
#define GL_ONE_MINUS_DST_ALPHA            0x0305
#define GL_DST_COLOR                      0x0306
#define GL_ONE_MINUS_DST_COLOR            0x0307
#define GL_SRC_ALPHA_SATURATE             0x0308
#define GL_FUNC_ADD                       0x8006
#define GL_BLEND_EQUATION                 0x8009
#define GL_BLEND_EQUATION_RGB             0x8009
#define GL_BLEND_EQUATION_ALPHA           0x883D
#define GL_FUNC_SUBTRACT                  0x800A
#define GL_FUNC_REVERSE_SUBTRACT          0x800B
#define GL_BLEND_DST_RGB                  0x80C8
#define GL_BLEND_SRC_RGB                  0x80C9
#define GL_BLEND_DST_ALPHA                0x80CA
#define GL_BLEND_SRC_ALPHA                0x80CB
#define GL_CONSTANT_COLOR                 0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR       0x8002
#define GL_CONSTANT_ALPHA                 0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA       0x8004
#define GL_BLEND_COLOR                    0x8005
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_ARRAY_BUFFER_BINDING           0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING   0x8895
#define GL_STREAM_DRAW                    0x88E0
#define GL_STATIC_DRAW                    0x88E4
#define GL_DYNAMIC_DRAW                   0x88E8
#define GL_BUFFER_SIZE                    0x8764
#define GL_BUFFER_USAGE                   0x8765
#define GL_CURRENT_VERTEX_ATTRIB          0x8626
#define GL_FRONT                          0x0404
#define GL_BACK                           0x0405
#define GL_FRONT_AND_BACK                 0x0408
#define GL_TEXTURE_2D                     0x0DE1
#define GL_CULL_FACE                      0x0B44
#define GL_BLEND                          0x0BE2
#define GL_DITHER                         0x0BD0
#define GL_STENCIL_TEST                   0x0B90
#define GL_DEPTH_TEST                     0x0B71
#define GL_SCISSOR_TEST                   0x0C11
#define GL_POLYGON_OFFSET_FILL            0x8037
#define GL_SAMPLE_ALPHA_TO_COVERAGE       0x809E
#define GL_SAMPLE_COVERAGE                0x80A0
#define GL_NO_ERROR                       0
#define GL_INVALID_ENUM                   0x0500
#define GL_INVALID_VALUE                  0x0501
#define GL_INVALID_OPERATION              0x0502
#define GL_OUT_OF_MEMORY                  0x0505
#define GL_CW                             0x0900
#define GL_CCW                            0x0901
#define GL_LINE_WIDTH                     0x0B21
#define GL_ALIASED_POINT_SIZE_RANGE       0x846D
#define GL_ALIASED_LINE_WIDTH_RANGE       0x846E
#define GL_CULL_FACE_MODE                 0x0B45
#define GL_FRONT_FACE                     0x0B46
#define GL_DEPTH_RANGE                    0x0B70
#define GL_DEPTH_WRITEMASK                0x0B72
#define GL_DEPTH_CLEAR_VALUE              0x0B73
#define GL_DEPTH_FUNC                     0x0B74
#define GL_STENCIL_CLEAR_VALUE            0x0B91
#define GL_STENCIL_FUNC                   0x0B92
#define GL_STENCIL_FAIL                   0x0B94
#define GL_STENCIL_PASS_DEPTH_FAIL        0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS        0x0B96
#define GL_STENCIL_REF                    0x0B97
#define GL_STENCIL_VALUE_MASK             0x0B93
#define GL_STENCIL_WRITEMASK              0x0B98
#define GL_STENCIL_BACK_FUNC              0x8800
#define GL_STENCIL_BACK_FAIL              0x8801
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL   0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS   0x8803
#define GL_STENCIL_BACK_REF               0x8CA3
#define GL_STENCIL_BACK_VALUE_MASK        0x8CA4
#define GL_STENCIL_BACK_WRITEMASK         0x8CA5
#define GL_VIEWPORT                       0x0BA2
#define GL_SCISSOR_BOX                    0x0C10
#define GL_COLOR_CLEAR_VALUE              0x0C22
#define GL_COLOR_WRITEMASK                0x0C23
#define GL_UNPACK_ALIGNMENT               0x0CF5
#define GL_PACK_ALIGNMENT                 0x0D05
#define GL_MAX_TEXTURE_SIZE               0x0D33
#define GL_MAX_VIEWPORT_DIMS              0x0D3A
#define GL_SUBPIXEL_BITS                  0x0D50
#define GL_RED_BITS                       0x0D52
#define GL_GREEN_BITS                     0x0D53
#define GL_BLUE_BITS                      0x0D54
#define GL_ALPHA_BITS                     0x0D55
#define GL_DEPTH_BITS                     0x0D56
#define GL_STENCIL_BITS                   0x0D57
#define GL_POLYGON_OFFSET_UNITS           0x2A00
#define GL_POLYGON_OFFSET_FACTOR          0x8038
#define GL_TEXTURE_BINDING_2D             0x8069
#define GL_SAMPLE_BUFFERS                 0x80A8
#define GL_SAMPLES                        0x80A9
#define GL_SAMPLE_COVERAGE_VALUE          0x80AA
#define GL_SAMPLE_COVERAGE_INVERT         0x80AB
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS     0x86A3
#define GL_DONT_CARE                      0x1100
#define GL_FASTEST                        0x1101
#define GL_NICEST                         0x1102
#define GL_GENERATE_MIPMAP_HINT           0x8192
#define GL_BYTE                           0x1400
#define GL_UNSIGNED_BYTE                  0x1401
#define GL_SHORT                          0x1402
#define GL_UNSIGNED_SHORT                 0x1403
#define GL_INT                            0x1404
#define GL_UNSIGNED_INT                   0x1405
#define GL_FLOAT                          0x1406
#define GL_FIXED                          0x140C
#define GL_DEPTH_COMPONENT                0x1902
#define GL_ALPHA                          0x1906
#define GL_RGB                            0x1907
#define GL_RGBA                           0x1908
#define GL_LUMINANCE                      0x1909
#define GL_LUMINANCE_ALPHA                0x190A
#define GL_UNSIGNED_SHORT_4_4_4_4         0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1         0x8034
#define GL_UNSIGNED_SHORT_5_6_5           0x8363
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_MAX_VERTEX_ATTRIBS             0x8869
#define GL_MAX_VERTEX_UNIFORM_VECTORS     0x8DFB
#define GL_MAX_VARYING_VECTORS            0x8DFC
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS 0x8B4C
#define GL_MAX_TEXTURE_IMAGE_UNITS        0x8872
#define GL_MAX_FRAGMENT_UNIFORM_VECTORS   0x8DFD
#define GL_SHADER_TYPE                    0x8B4F
#define GL_DELETE_STATUS                  0x8B80
#define GL_LINK_STATUS                    0x8B82
#define GL_VALIDATE_STATUS                0x8B83
#define GL_ATTACHED_SHADERS               0x8B85
#define GL_ACTIVE_UNIFORMS                0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH      0x8B87
#define GL_ACTIVE_ATTRIBUTES              0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH    0x8B8A
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C
#define GL_CURRENT_PROGRAM                0x8B8D
#define GL_NEVER                          0x0200
#define GL_LESS                           0x0201
#define GL_EQUAL                          0x0202
#define GL_LEQUAL                         0x0203
#define GL_GREATER                        0x0204
#define GL_NOTEQUAL                       0x0205
#define GL_GEQUAL                         0x0206
#define GL_ALWAYS                         0x0207
#define GL_KEEP                           0x1E00
#define GL_REPLACE                        0x1E01
#define GL_INCR                           0x1E02
#define GL_DECR                           0x1E03
#define GL_INVERT                         0x150A
#define GL_INCR_WRAP                      0x8507
#define GL_DECR_WRAP                      0x8508
#define GL_VENDOR                         0x1F00
#define GL_RENDERER                       0x1F01
#define GL_VERSION                        0x1F02
#define GL_EXTENSIONS                     0x1F03
#define GL_NEAREST                        0x2600
#define GL_LINEAR                         0x2601
#define GL_NEAREST_MIPMAP_NEAREST         0x2700
#define GL_LINEAR_MIPMAP_NEAREST          0x2701
#define GL_NEAREST_MIPMAP_LINEAR          0x2702
#define GL_LINEAR_MIPMAP_LINEAR           0x2703
#define GL_TEXTURE_MAG_FILTER             0x2800
#define GL_TEXTURE_MIN_FILTER             0x2801
#define GL_TEXTURE_WRAP_S                 0x2802
#define GL_TEXTURE_WRAP_T                 0x2803
#define GL_TEXTURE                        0x1702
#define GL_TEXTURE_CUBE_MAP               0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP       0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X    0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X    0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y    0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y    0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z    0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z    0x851A
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE      0x851C
#define GL_TEXTURE0                       0x84C0
#define GL_TEXTURE1                       0x84C1
#define GL_TEXTURE2                       0x84C2
#define GL_TEXTURE3                       0x84C3
#define GL_TEXTURE4                       0x84C4
#define GL_TEXTURE5                       0x84C5
#define GL_TEXTURE6                       0x84C6
#define GL_TEXTURE7                       0x84C7
#define GL_TEXTURE8                       0x84C8
#define GL_TEXTURE9                       0x84C9
#define GL_TEXTURE10                      0x84CA
#define GL_TEXTURE11                      0x84CB
#define GL_TEXTURE12                      0x84CC
#define GL_TEXTURE13                      0x84CD
#define GL_TEXTURE14                      0x84CE
#define GL_TEXTURE15                      0x84CF
#define GL_TEXTURE16                      0x84D0
#define GL_TEXTURE17                      0x84D1
#define GL_TEXTURE18                      0x84D2
#define GL_TEXTURE19                      0x84D3
#define GL_TEXTURE20                      0x84D4
#define GL_TEXTURE21                      0x84D5
#define GL_TEXTURE22                      0x84D6
#define GL_TEXTURE23                      0x84D7
#define GL_TEXTURE24                      0x84D8
#define GL_TEXTURE25                      0x84D9
#define GL_TEXTURE26                      0x84DA
#define GL_TEXTURE27                      0x84DB
#define GL_TEXTURE28                      0x84DC
#define GL_TEXTURE29                      0x84DD
#define GL_TEXTURE30                      0x84DE
#define GL_TEXTURE31                      0x84DF
#define GL_ACTIVE_TEXTURE                 0x84E0
#define GL_REPEAT                         0x2901
#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_MIRRORED_REPEAT                0x8370
#define GL_FLOAT_VEC2                     0x8B50
#define GL_FLOAT_VEC3                     0x8B51
#define GL_FLOAT_VEC4                     0x8B52
#define GL_INT_VEC2                       0x8B53
#define GL_INT_VEC3                       0x8B54
#define GL_INT_VEC4                       0x8B55
#define GL_BOOL                           0x8B56
#define GL_BOOL_VEC2                      0x8B57
#define GL_BOOL_VEC3                      0x8B58
#define GL_BOOL_VEC4                      0x8B59
#define GL_FLOAT_MAT2                     0x8B5A
#define GL_FLOAT_MAT3                     0x8B5B
#define GL_FLOAT_MAT4                     0x8B5C
#define GL_SAMPLER_2D                     0x8B5E
#define GL_SAMPLER_CUBE                   0x8B60
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED    0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE       0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE     0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE       0x8625
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED 0x886A
#define GL_VERTEX_ATTRIB_ARRAY_POINTER    0x8645
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING 0x889F
#define GL_IMPLEMENTATION_COLOR_READ_TYPE 0x8B9A
#define GL_IMPLEMENTATION_COLOR_READ_FORMAT 0x8B9B
#define GL_COMPILE_STATUS                 0x8B81
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_SHADER_SOURCE_LENGTH           0x8B88
#define GL_SHADER_COMPILER                0x8DFA
#define GL_SHADER_BINARY_FORMATS          0x8DF8
#define GL_NUM_SHADER_BINARY_FORMATS      0x8DF9
#define GL_LOW_FLOAT                      0x8DF0
#define GL_MEDIUM_FLOAT                   0x8DF1
#define GL_HIGH_FLOAT                     0x8DF2
#define GL_LOW_INT                        0x8DF3
#define GL_MEDIUM_INT                     0x8DF4
#define GL_HIGH_INT                       0x8DF5
#define GL_FRAMEBUFFER                    0x8D40
#define GL_RENDERBUFFER                   0x8D41
#define GL_RGBA4                          0x8056
#define GL_RGB5_A1                        0x8057
#define GL_RGB565                         0x8D62
#define GL_DEPTH_COMPONENT16              0x81A5
#define GL_STENCIL_INDEX8                 0x8D48
#define GL_RENDERBUFFER_WIDTH             0x8D42
#define GL_RENDERBUFFER_HEIGHT            0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT   0x8D44
#define GL_RENDERBUFFER_RED_SIZE          0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE        0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE         0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE        0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE        0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE      0x8D55
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE 0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME 0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL 0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE 0x8CD3
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_STENCIL_ATTACHMENT             0x8D20
#define GL_NONE                           0
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS 0x8CD9
#define GL_FRAMEBUFFER_UNSUPPORTED        0x8CDD
#define GL_FRAMEBUFFER_BINDING            0x8CA6
#define GL_RENDERBUFFER_BINDING           0x8CA7
#define GL_MAX_RENDERBUFFER_SIZE          0x84E8
#define GL_INVALID_FRAMEBUFFER_OPERATION  0x0506
typedef void (GL_APIENTRYP PFNGLACTIVETEXTUREPROC) (GLenum texture);
typedef void (GL_APIENTRYP PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (GL_APIENTRYP PFNGLBINDATTRIBLOCATIONPROC) (GLuint program, GLuint index, const GLchar *name);
typedef void (GL_APIENTRYP PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
typedef void (GL_APIENTRYP PFNGLBINDFRAMEBUFFERPROC) (GLenum target, GLuint framebuffer);
typedef void (GL_APIENTRYP PFNGLBINDRENDERBUFFERPROC) (GLenum target, GLuint renderbuffer);
typedef void (GL_APIENTRYP PFNGLBINDTEXTUREPROC) (GLenum target, GLuint texture);
typedef void (GL_APIENTRYP PFNGLBLENDCOLORPROC) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (GL_APIENTRYP PFNGLBLENDEQUATIONPROC) (GLenum mode);
typedef void (GL_APIENTRYP PFNGLBLENDEQUATIONSEPARATEPROC) (GLenum modeRGB, GLenum modeAlpha);
typedef void (GL_APIENTRYP PFNGLBLENDFUNCPROC) (GLenum sfactor, GLenum dfactor);
typedef void (GL_APIENTRYP PFNGLBLENDFUNCSEPARATEPROC) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
typedef void (GL_APIENTRYP PFNGLBUFFERDATAPROC) (GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef void (GL_APIENTRYP PFNGLBUFFERSUBDATAPROC) (GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
typedef GLenum (GL_APIENTRYP PFNGLCHECKFRAMEBUFFERSTATUSPROC) (GLenum target);
typedef void (GL_APIENTRYP PFNGLCLEARPROC) (GLbitfield mask);
typedef void (GL_APIENTRYP PFNGLCLEARCOLORPROC) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (GL_APIENTRYP PFNGLCLEARDEPTHFPROC) (GLfloat d);
typedef void (GL_APIENTRYP PFNGLCLEARSTENCILPROC) (GLint s);
typedef void (GL_APIENTRYP PFNGLCOLORMASKPROC) (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
typedef void (GL_APIENTRYP PFNGLCOMPILESHADERPROC) (GLuint shader);
typedef void (GL_APIENTRYP PFNGLCOMPRESSEDTEXIMAGE2DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
typedef void (GL_APIENTRYP PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
typedef void (GL_APIENTRYP PFNGLCOPYTEXIMAGE2DPROC) (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void (GL_APIENTRYP PFNGLCOPYTEXSUBIMAGE2DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef GLuint (GL_APIENTRYP PFNGLCREATEPROGRAMPROC) (void);
typedef GLuint (GL_APIENTRYP PFNGLCREATESHADERPROC) (GLenum type);
typedef void (GL_APIENTRYP PFNGLCULLFACEPROC) (GLenum mode);
typedef void (GL_APIENTRYP PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint *buffers);
typedef void (GL_APIENTRYP PFNGLDELETEFRAMEBUFFERSPROC) (GLsizei n, const GLuint *framebuffers);
typedef void (GL_APIENTRYP PFNGLDELETEPROGRAMPROC) (GLuint program);
typedef void (GL_APIENTRYP PFNGLDELETERENDERBUFFERSPROC) (GLsizei n, const GLuint *renderbuffers);
typedef void (GL_APIENTRYP PFNGLDELETESHADERPROC) (GLuint shader);
typedef void (GL_APIENTRYP PFNGLDELETETEXTURESPROC) (GLsizei n, const GLuint *textures);
typedef void (GL_APIENTRYP PFNGLDEPTHFUNCPROC) (GLenum func);
typedef void (GL_APIENTRYP PFNGLDEPTHMASKPROC) (GLboolean flag);
typedef void (GL_APIENTRYP PFNGLDEPTHRANGEFPROC) (GLfloat n, GLfloat f);
typedef void (GL_APIENTRYP PFNGLDETACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (GL_APIENTRYP PFNGLDISABLEPROC) (GLenum cap);
typedef void (GL_APIENTRYP PFNGLDISABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (GL_APIENTRYP PFNGLDRAWARRAYSPROC) (GLenum mode, GLint first, GLsizei count);
typedef void (GL_APIENTRYP PFNGLDRAWELEMENTSPROC) (GLenum mode, GLsizei count, GLenum type, const void *indices);
typedef void (GL_APIENTRYP PFNGLENABLEPROC) (GLenum cap);
typedef void (GL_APIENTRYP PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (GL_APIENTRYP PFNGLFINISHPROC) (void);
typedef void (GL_APIENTRYP PFNGLFLUSHPROC) (void);
typedef void (GL_APIENTRYP PFNGLFRAMEBUFFERRENDERBUFFERPROC) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (GL_APIENTRYP PFNGLFRAMEBUFFERTEXTURE2DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GL_APIENTRYP PFNGLFRONTFACEPROC) (GLenum mode);
typedef void (GL_APIENTRYP PFNGLGENBUFFERSPROC) (GLsizei n, GLuint *buffers);
typedef void (GL_APIENTRYP PFNGLGENERATEMIPMAPPROC) (GLenum target);
typedef void (GL_APIENTRYP PFNGLGENFRAMEBUFFERSPROC) (GLsizei n, GLuint *framebuffers);
typedef void (GL_APIENTRYP PFNGLGENRENDERBUFFERSPROC) (GLsizei n, GLuint *renderbuffers);
typedef void (GL_APIENTRYP PFNGLGENTEXTURESPROC) (GLsizei n, GLuint *textures);
typedef void (GL_APIENTRYP PFNGLGETACTIVEATTRIBPROC) (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
typedef void (GL_APIENTRYP PFNGLGETACTIVEUNIFORMPROC) (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
typedef void (GL_APIENTRYP PFNGLGETATTACHEDSHADERSPROC) (GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders);
typedef GLint (GL_APIENTRYP PFNGLGETATTRIBLOCATIONPROC) (GLuint program, const GLchar *name);
typedef void (GL_APIENTRYP PFNGLGETBOOLEANVPROC) (GLenum pname, GLboolean *data);
typedef void (GL_APIENTRYP PFNGLGETBUFFERPARAMETERIVPROC) (GLenum target, GLenum pname, GLint *params);
typedef GLenum (GL_APIENTRYP PFNGLGETERRORPROC) (void);
typedef void (GL_APIENTRYP PFNGLGETFLOATVPROC) (GLenum pname, GLfloat *data);
typedef void (GL_APIENTRYP PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) (GLenum target, GLenum attachment, GLenum pname, GLint *params);
typedef void (GL_APIENTRYP PFNGLGETINTEGERVPROC) (GLenum pname, GLint *data);
typedef void (GL_APIENTRYP PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint *params);
typedef void (GL_APIENTRYP PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (GL_APIENTRYP PFNGLGETRENDERBUFFERPARAMETERIVPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (GL_APIENTRYP PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint *params);
typedef void (GL_APIENTRYP PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (GL_APIENTRYP PFNGLGETSHADERPRECISIONFORMATPROC) (GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision);
typedef void (GL_APIENTRYP PFNGLGETSHADERSOURCEPROC) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
typedef const GLubyte *(GL_APIENTRYP PFNGLGETSTRINGPROC) (GLenum name);
typedef void (GL_APIENTRYP PFNGLGETTEXPARAMETERFVPROC) (GLenum target, GLenum pname, GLfloat *params);
typedef void (GL_APIENTRYP PFNGLGETTEXPARAMETERIVPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (GL_APIENTRYP PFNGLGETUNIFORMFVPROC) (GLuint program, GLint location, GLfloat *params);
typedef void (GL_APIENTRYP PFNGLGETUNIFORMIVPROC) (GLuint program, GLint location, GLint *params);
typedef GLint (GL_APIENTRYP PFNGLGETUNIFORMLOCATIONPROC) (GLuint program, const GLchar *name);
typedef void (GL_APIENTRYP PFNGLGETVERTEXATTRIBFVPROC) (GLuint index, GLenum pname, GLfloat *params);
typedef void (GL_APIENTRYP PFNGLGETVERTEXATTRIBIVPROC) (GLuint index, GLenum pname, GLint *params);
typedef void (GL_APIENTRYP PFNGLGETVERTEXATTRIBPOINTERVPROC) (GLuint index, GLenum pname, void **pointer);
typedef void (GL_APIENTRYP PFNGLHINTPROC) (GLenum target, GLenum mode);
typedef GLboolean (GL_APIENTRYP PFNGLISBUFFERPROC) (GLuint buffer);
typedef GLboolean (GL_APIENTRYP PFNGLISENABLEDPROC) (GLenum cap);
typedef GLboolean (GL_APIENTRYP PFNGLISFRAMEBUFFERPROC) (GLuint framebuffer);
typedef GLboolean (GL_APIENTRYP PFNGLISPROGRAMPROC) (GLuint program);
typedef GLboolean (GL_APIENTRYP PFNGLISRENDERBUFFERPROC) (GLuint renderbuffer);
typedef GLboolean (GL_APIENTRYP PFNGLISSHADERPROC) (GLuint shader);
typedef GLboolean (GL_APIENTRYP PFNGLISTEXTUREPROC) (GLuint texture);
typedef void (GL_APIENTRYP PFNGLLINEWIDTHPROC) (GLfloat width);
typedef void (GL_APIENTRYP PFNGLLINKPROGRAMPROC) (GLuint program);
typedef void (GL_APIENTRYP PFNGLPIXELSTOREIPROC) (GLenum pname, GLint param);
typedef void (GL_APIENTRYP PFNGLPOLYGONOFFSETPROC) (GLfloat factor, GLfloat units);
typedef void (GL_APIENTRYP PFNGLREADPIXELSPROC) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);
typedef void (GL_APIENTRYP PFNGLRELEASESHADERCOMPILERPROC) (void);
typedef void (GL_APIENTRYP PFNGLRENDERBUFFERSTORAGEPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GL_APIENTRYP PFNGLSAMPLECOVERAGEPROC) (GLfloat value, GLboolean invert);
typedef void (GL_APIENTRYP PFNGLSCISSORPROC) (GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GL_APIENTRYP PFNGLSHADERBINARYPROC) (GLsizei count, const GLuint *shaders, GLenum binaryformat, const void *binary, GLsizei length);
typedef void (GL_APIENTRYP PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
typedef void (GL_APIENTRYP PFNGLSTENCILFUNCPROC) (GLenum func, GLint ref, GLuint mask);
typedef void (GL_APIENTRYP PFNGLSTENCILFUNCSEPARATEPROC) (GLenum face, GLenum func, GLint ref, GLuint mask);
typedef void (GL_APIENTRYP PFNGLSTENCILMASKPROC) (GLuint mask);
typedef void (GL_APIENTRYP PFNGLSTENCILMASKSEPARATEPROC) (GLenum face, GLuint mask);
typedef void (GL_APIENTRYP PFNGLSTENCILOPPROC) (GLenum fail, GLenum zfail, GLenum zpass);
typedef void (GL_APIENTRYP PFNGLSTENCILOPSEPARATEPROC) (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
typedef void (GL_APIENTRYP PFNGLTEXIMAGE2DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (GL_APIENTRYP PFNGLTEXPARAMETERFPROC) (GLenum target, GLenum pname, GLfloat param);
typedef void (GL_APIENTRYP PFNGLTEXPARAMETERFVPROC) (GLenum target, GLenum pname, const GLfloat *params);
typedef void (GL_APIENTRYP PFNGLTEXPARAMETERIPROC) (GLenum target, GLenum pname, GLint param);
typedef void (GL_APIENTRYP PFNGLTEXPARAMETERIVPROC) (GLenum target, GLenum pname, const GLint *params);
typedef void (GL_APIENTRYP PFNGLTEXSUBIMAGE2DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
typedef void (GL_APIENTRYP PFNGLUNIFORM1FPROC) (GLint location, GLfloat v0);
typedef void (GL_APIENTRYP PFNGLUNIFORM1FVPROC) (GLint location, GLsizei count, const GLfloat *value);
typedef void (GL_APIENTRYP PFNGLUNIFORM1IPROC) (GLint location, GLint v0);
typedef void (GL_APIENTRYP PFNGLUNIFORM1IVPROC) (GLint location, GLsizei count, const GLint *value);
typedef void (GL_APIENTRYP PFNGLUNIFORM2FPROC) (GLint location, GLfloat v0, GLfloat v1);
typedef void (GL_APIENTRYP PFNGLUNIFORM2FVPROC) (GLint location, GLsizei count, const GLfloat *value);
typedef void (GL_APIENTRYP PFNGLUNIFORM2IPROC) (GLint location, GLint v0, GLint v1);
typedef void (GL_APIENTRYP PFNGLUNIFORM2IVPROC) (GLint location, GLsizei count, const GLint *value);
typedef void (GL_APIENTRYP PFNGLUNIFORM3FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (GL_APIENTRYP PFNGLUNIFORM3FVPROC) (GLint location, GLsizei count, const GLfloat *value);
typedef void (GL_APIENTRYP PFNGLUNIFORM3IPROC) (GLint location, GLint v0, GLint v1, GLint v2);
typedef void (GL_APIENTRYP PFNGLUNIFORM3IVPROC) (GLint location, GLsizei count, const GLint *value);
typedef void (GL_APIENTRYP PFNGLUNIFORM4FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (GL_APIENTRYP PFNGLUNIFORM4FVPROC) (GLint location, GLsizei count, const GLfloat *value);
typedef void (GL_APIENTRYP PFNGLUNIFORM4IPROC) (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (GL_APIENTRYP PFNGLUNIFORM4IVPROC) (GLint location, GLsizei count, const GLint *value);
typedef void (GL_APIENTRYP PFNGLUNIFORMMATRIX2FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GL_APIENTRYP PFNGLUNIFORMMATRIX3FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GL_APIENTRYP PFNGLUNIFORMMATRIX4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GL_APIENTRYP PFNGLUSEPROGRAMPROC) (GLuint program);
typedef void (GL_APIENTRYP PFNGLVALIDATEPROGRAMPROC) (GLuint program);
typedef void (GL_APIENTRYP PFNGLVERTEXATTRIB1FPROC) (GLuint index, GLfloat x);
typedef void (GL_APIENTRYP PFNGLVERTEXATTRIB1FVPROC) (GLuint index, const GLfloat *v);
typedef void (GL_APIENTRYP PFNGLVERTEXATTRIB2FPROC) (GLuint index, GLfloat x, GLfloat y);
typedef void (GL_APIENTRYP PFNGLVERTEXATTRIB2FVPROC) (GLuint index, const GLfloat *v);
typedef void (GL_APIENTRYP PFNGLVERTEXATTRIB3FPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z);
typedef void (GL_APIENTRYP PFNGLVERTEXATTRIB3FVPROC) (GLuint index, const GLfloat *v);
typedef void (GL_APIENTRYP PFNGLVERTEXATTRIB4FPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GL_APIENTRYP PFNGLVERTEXATTRIB4FVPROC) (GLuint index, const GLfloat *v);
typedef void (GL_APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef void (GL_APIENTRYP PFNGLVIEWPORTPROC) (GLint x, GLint y, GLsizei width, GLsizei height);
#if GL_GLES_PROTOTYPES
GLEW_FUN_EXPORT PFNGLACTIVETEXTUREPROC __glewActiveTexture;
GLEW_FUN_EXPORT PFNGLATTACHSHADERPROC __glewAttachShader;
GLEW_FUN_EXPORT PFNGLBINDATTRIBLOCATIONPROC __glewBindAttribLocation;
GLEW_FUN_EXPORT PFNGLBINDBUFFERPROC __glewBindBuffer;
GLEW_FUN_EXPORT PFNGLBINDFRAMEBUFFERPROC __glewBindFramebuffer;
GLEW_FUN_EXPORT PFNGLBINDRENDERBUFFERPROC __glewBindRenderbuffer;
GLEW_FUN_EXPORT PFNGLBLENDCOLORPROC __glewBlendColor;
GLEW_FUN_EXPORT PFNGLBLENDEQUATIONPROC __glewBlendEquation;
GLEW_FUN_EXPORT PFNGLBLENDEQUATIONSEPARATEPROC __glewBlendEquationSeparate;
GLEW_FUN_EXPORT PFNGLBLENDFUNCSEPARATEPROC __glewBlendFuncSeparate;
GLEW_FUN_EXPORT PFNGLBUFFERDATAPROC __glewBufferData;
GLEW_FUN_EXPORT PFNGLBUFFERSUBDATAPROC __glewBufferSubData;
GLEW_FUN_EXPORT PFNGLCHECKFRAMEBUFFERSTATUSPROC __glewCheckFramebufferStatus;
GLEW_FUN_EXPORT PFNGLCLEARDEPTHFPROC __glewClearDepthf;
GLEW_FUN_EXPORT PFNGLCOMPILESHADERPROC __glewCompileShader;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXIMAGE2DPROC __glewCompressedTexImage2D;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC __glewCompressedTexSubImage2D;
GLEW_FUN_EXPORT PFNGLCREATEPROGRAMPROC __glewCreateProgram;
GLEW_FUN_EXPORT PFNGLCREATESHADERPROC __glewCreateShader;
GLEW_FUN_EXPORT PFNGLDELETEBUFFERSPROC __glewDeleteBuffers;
GLEW_FUN_EXPORT PFNGLDELETEFRAMEBUFFERSPROC __glewDeleteFramebuffers;
GLEW_FUN_EXPORT PFNGLDELETEPROGRAMPROC __glewDeleteProgram;
GLEW_FUN_EXPORT PFNGLDELETERENDERBUFFERSPROC __glewDeleteRenderbuffers;
GLEW_FUN_EXPORT PFNGLDELETESHADERPROC __glewDeleteShader;
GLEW_FUN_EXPORT PFNGLDEPTHRANGEFPROC __glewDepthRangef;
GLEW_FUN_EXPORT PFNGLDETACHSHADERPROC __glewDetachShader;
GLEW_FUN_EXPORT PFNGLDISABLEVERTEXATTRIBARRAYPROC __glewDisableVertexAttribArray;
GLEW_FUN_EXPORT PFNGLENABLEVERTEXATTRIBARRAYPROC __glewEnableVertexAttribArray;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERRENDERBUFFERPROC __glewFramebufferRenderbuffer;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERTEXTURE2DPROC __glewFramebufferTexture2D;
GLEW_FUN_EXPORT PFNGLGENBUFFERSPROC __glewGenBuffers;
GLEW_FUN_EXPORT PFNGLGENERATEMIPMAPPROC __glewGenerateMipmap;
GLEW_FUN_EXPORT PFNGLGENFRAMEBUFFERSPROC __glewGenFramebuffers;
GLEW_FUN_EXPORT PFNGLGENRENDERBUFFERSPROC __glewGenRenderbuffers;
GLEW_FUN_EXPORT PFNGLGETACTIVEATTRIBPROC __glewGetActiveAttrib;
GLEW_FUN_EXPORT PFNGLGETACTIVEUNIFORMPROC __glewGetActiveUniform;
GLEW_FUN_EXPORT PFNGLGETATTACHEDSHADERSPROC __glewGetAttachedShaders;
GLEW_FUN_EXPORT PFNGLGETATTRIBLOCATIONPROC __glewGetAttribLocation;
GLEW_FUN_EXPORT PFNGLGETBUFFERPARAMETERIVPROC __glewGetBufferParameteriv;
GLEW_FUN_EXPORT PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC __glewGetFramebufferAttachmentParameteriv;
GLEW_FUN_EXPORT PFNGLGETPROGRAMIVPROC __glewGetProgramiv;
GLEW_FUN_EXPORT PFNGLGETPROGRAMINFOLOGPROC __glewGetProgramInfoLog;
GLEW_FUN_EXPORT PFNGLGETRENDERBUFFERPARAMETERIVPROC __glewGetRenderbufferParameteriv;
GLEW_FUN_EXPORT PFNGLGETSHADERIVPROC __glewGetShaderiv;
GLEW_FUN_EXPORT PFNGLGETSHADERINFOLOGPROC __glewGetShaderInfoLog;
GLEW_FUN_EXPORT PFNGLGETSHADERPRECISIONFORMATPROC __glewGetShaderPrecisionFormat;
GLEW_FUN_EXPORT PFNGLGETSHADERSOURCEPROC __glewGetShaderSource;
GLEW_FUN_EXPORT PFNGLGETUNIFORMFVPROC __glewGetUniformfv;
GLEW_FUN_EXPORT PFNGLGETUNIFORMIVPROC __glewGetUniformiv;
GLEW_FUN_EXPORT PFNGLGETUNIFORMLOCATIONPROC __glewGetUniformLocation;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBFVPROC __glewGetVertexAttribfv;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBIVPROC __glewGetVertexAttribiv;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBPOINTERVPROC __glewGetVertexAttribPointerv;
GLEW_FUN_EXPORT PFNGLISBUFFERPROC __glewIsBuffer;
GLEW_FUN_EXPORT PFNGLISFRAMEBUFFERPROC __glewIsFramebuffer;
GLEW_FUN_EXPORT PFNGLISPROGRAMPROC __glewIsProgram;
GLEW_FUN_EXPORT PFNGLISRENDERBUFFERPROC __glewIsRenderbuffer;
GLEW_FUN_EXPORT PFNGLISSHADERPROC __glewIsShader;
GLEW_FUN_EXPORT PFNGLLINKPROGRAMPROC __glewLinkProgram;
GLEW_FUN_EXPORT PFNGLRELEASESHADERCOMPILERPROC __glewReleaseShaderCompiler;
GLEW_FUN_EXPORT PFNGLRENDERBUFFERSTORAGEPROC __glewRenderbufferStorage;
GLEW_FUN_EXPORT PFNGLSAMPLECOVERAGEPROC __glewSampleCoverage;
GLEW_FUN_EXPORT PFNGLSHADERBINARYPROC __glewShaderBinary;
GLEW_FUN_EXPORT PFNGLSHADERSOURCEPROC __glewShaderSource;
GLEW_FUN_EXPORT PFNGLSTENCILFUNCSEPARATEPROC __glewStencilFuncSeparate;
GLEW_FUN_EXPORT PFNGLSTENCILMASKSEPARATEPROC __glewStencilMaskSeparate;
GLEW_FUN_EXPORT PFNGLSTENCILOPSEPARATEPROC __glewStencilOpSeparate;
GLEW_FUN_EXPORT PFNGLUNIFORM1FPROC __glewUniform1f;
GLEW_FUN_EXPORT PFNGLUNIFORM1FVPROC __glewUniform1fv;
GLEW_FUN_EXPORT PFNGLUNIFORM1IPROC __glewUniform1i;
GLEW_FUN_EXPORT PFNGLUNIFORM1IVPROC __glewUniform1iv;
GLEW_FUN_EXPORT PFNGLUNIFORM2FPROC __glewUniform2f;
GLEW_FUN_EXPORT PFNGLUNIFORM2FVPROC __glewUniform2fv;
GLEW_FUN_EXPORT PFNGLUNIFORM2IPROC __glewUniform2i;
GLEW_FUN_EXPORT PFNGLUNIFORM2IVPROC __glewUniform2iv;
GLEW_FUN_EXPORT PFNGLUNIFORM3FPROC __glewUniform3f;
GLEW_FUN_EXPORT PFNGLUNIFORM3FVPROC __glewUniform3fv;
GLEW_FUN_EXPORT PFNGLUNIFORM3IPROC __glewUniform3i;
GLEW_FUN_EXPORT PFNGLUNIFORM3IVPROC __glewUniform3iv;
GLEW_FUN_EXPORT PFNGLUNIFORM4FPROC __glewUniform4f;
GLEW_FUN_EXPORT PFNGLUNIFORM4FVPROC __glewUniform4fv;
GLEW_FUN_EXPORT PFNGLUNIFORM4IPROC __glewUniform4i;
GLEW_FUN_EXPORT PFNGLUNIFORM4IVPROC __glewUniform4iv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX2FVPROC __glewUniformMatrix2fv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX3FVPROC __glewUniformMatrix3fv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX4FVPROC __glewUniformMatrix4fv;
GLEW_FUN_EXPORT PFNGLUSEPROGRAMPROC __glewUseProgram;
GLEW_FUN_EXPORT PFNGLVALIDATEPROGRAMPROC __glewValidateProgram;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1FPROC __glewVertexAttrib1f;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1FVPROC __glewVertexAttrib1fv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2FPROC __glewVertexAttrib2f;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2FVPROC __glewVertexAttrib2fv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3FPROC __glewVertexAttrib3f;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3FVPROC __glewVertexAttrib3fv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4FPROC __glewVertexAttrib4f;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4FVPROC __glewVertexAttrib4fv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBPOINTERPROC __glewVertexAttribPointer;
#define glActiveTexture GLEW_GET_FUN(__glewActiveTexture)
#define glAttachShader GLEW_GET_FUN(__glewAttachShader)
#define glBindAttribLocation GLEW_GET_FUN(__glewBindAttribLocation)
#define glBindBuffer GLEW_GET_FUN(__glewBindBuffer)
#define glBindFramebuffer GLEW_GET_FUN(__glewBindFramebuffer)
#define glBindRenderbuffer GLEW_GET_FUN(__glewBindRenderbuffer)
#define glBlendColor GLEW_GET_FUN(__glewBlendColor)
#define glBlendEquation GLEW_GET_FUN(__glewBlendEquation)
#define glBlendEquationSeparate GLEW_GET_FUN(__glewBlendEquationSeparate)
#define glBlendFuncSeparate GLEW_GET_FUN(__glewBlendFuncSeparate)
#define glBufferData GLEW_GET_FUN(__glewBufferData)
#define glBufferSubData GLEW_GET_FUN(__glewBufferSubData)
#define glCheckFramebufferStatus GLEW_GET_FUN(__glewCheckFramebufferStatus)
#define glClearDepthf GLEW_GET_FUN(__glewClearDepthf)
#define glCompileShader GLEW_GET_FUN(__glewCompileShader)
#define glCompressedTexImage2D GLEW_GET_FUN(__glewCompressedTexImage2D)
#define glCompressedTexSubImage2D GLEW_GET_FUN(__glewCompressedTexSubImage2D)
#define glCreateProgram GLEW_GET_FUN(__glewCreateProgram)
#define glCreateShader GLEW_GET_FUN(__glewCreateShader)
#define glDeleteBuffers GLEW_GET_FUN(__glewDeleteBuffers)
#define glDeleteFramebuffers GLEW_GET_FUN(__glewDeleteFramebuffers)
#define glDeleteProgram GLEW_GET_FUN(__glewDeleteProgram)
#define glDeleteRenderbuffers GLEW_GET_FUN(__glewDeleteRenderbuffers)
#define glDeleteShader GLEW_GET_FUN(__glewDeleteShader)
#define glDepthRangef GLEW_GET_FUN(__glewDepthRangef)
#define glDetachShader GLEW_GET_FUN(__glewDetachShader)
#define glDisableVertexAttribArray GLEW_GET_FUN(__glewDisableVertexAttribArray)
#define glEnableVertexAttribArray GLEW_GET_FUN(__glewEnableVertexAttribArray)
#define glFramebufferRenderbuffer GLEW_GET_FUN(__glewFramebufferRenderbuffer)
#define glFramebufferTexture2D GLEW_GET_FUN(__glewFramebufferTexture2D)
#define glGenBuffers GLEW_GET_FUN(__glewGenBuffers)
#define glGenerateMipmap GLEW_GET_FUN(__glewGenerateMipmap)
#define glGenFramebuffers GLEW_GET_FUN(__glewGenFramebuffers)
#define glGenRenderbuffers GLEW_GET_FUN(__glewGenRenderbuffers)
#define glGetActiveAttrib GLEW_GET_FUN(__glewGetActiveAttrib)
#define glGetActiveUniform GLEW_GET_FUN(__glewGetActiveUniform)
#define glGetAttachedShaders GLEW_GET_FUN(__glewGetAttachedShaders)
#define glGetAttribLocation GLEW_GET_FUN(__glewGetAttribLocation)
#define glGetBufferParameteriv GLEW_GET_FUN(__glewGetBufferParameteriv)
#define glGetFramebufferAttachmentParameteriv GLEW_GET_FUN(__glewGetFramebufferAttachmentParameteriv)
#define glGetProgramiv GLEW_GET_FUN(__glewGetProgramiv)
#define glGetProgramInfoLog GLEW_GET_FUN(__glewGetProgramInfoLog)
#define glGetRenderbufferParameteriv GLEW_GET_FUN(__glewGetRenderbufferParameteriv)
#define glGetShaderiv GLEW_GET_FUN(__glewGetShaderiv)
#define glGetShaderInfoLog GLEW_GET_FUN(__glewGetShaderInfoLog)
#define glGetShaderPrecisionFormat GLEW_GET_FUN(__glewGetShaderPrecisionFormat)
#define glGetShaderSource GLEW_GET_FUN(__glewGetShaderSource)
#define glGetUniformfv GLEW_GET_FUN(__glewGetUniformfv)
#define glGetUniformiv GLEW_GET_FUN(__glewGetUniformiv)
#define glGetUniformLocation GLEW_GET_FUN(__glewGetUniformLocation)
#define glGetVertexAttribfv GLEW_GET_FUN(__glewGetVertexAttribfv)
#define glGetVertexAttribiv GLEW_GET_FUN(__glewGetVertexAttribiv)
#define glGetVertexAttribPointerv GLEW_GET_FUN(__glewGetVertexAttribPointerv)
#define glIsBuffer GLEW_GET_FUN(__glewIsBuffer)
#define glIsFramebuffer GLEW_GET_FUN(__glewIsFramebuffer)
#define glIsProgram GLEW_GET_FUN(__glewIsProgram)
#define glIsRenderbuffer GLEW_GET_FUN(__glewIsRenderbuffer)
#define glIsShader GLEW_GET_FUN(__glewIsShader)
#define glLinkProgram GLEW_GET_FUN(__glewLinkProgram)
#define glReleaseShaderCompiler GLEW_GET_FUN(__glewReleaseShaderCompiler)
#define glRenderbufferStorage GLEW_GET_FUN(__glewRenderbufferStorage)
#define glSampleCoverage GLEW_GET_FUN(__glewSampleCoverage)
#define glShaderBinary GLEW_GET_FUN(__glewShaderBinary)
#define glShaderSource GLEW_GET_FUN(__glewShaderSource)
#define glStencilFuncSeparate GLEW_GET_FUN(__glewStencilFuncSeparate)
#define glStencilMaskSeparate GLEW_GET_FUN(__glewStencilMaskSeparate)
#define glStencilOpSeparate GLEW_GET_FUN(__glewStencilOpSeparate)
#define glUniform1f GLEW_GET_FUN(__glewUniform1f)
#define glUniform1fv GLEW_GET_FUN(__glewUniform1fv)
#define glUniform1i GLEW_GET_FUN(__glewUniform1i)
#define glUniform1iv GLEW_GET_FUN(__glewUniform1iv)
#define glUniform2f GLEW_GET_FUN(__glewUniform2f)
#define glUniform2fv GLEW_GET_FUN(__glewUniform2fv)
#define glUniform2i GLEW_GET_FUN(__glewUniform2i)
#define glUniform2iv GLEW_GET_FUN(__glewUniform2iv)
#define glUniform3f GLEW_GET_FUN(__glewUniform3f)
#define glUniform3fv GLEW_GET_FUN(__glewUniform3fv)
#define glUniform3i GLEW_GET_FUN(__glewUniform3i)
#define glUniform3iv GLEW_GET_FUN(__glewUniform3iv)
#define glUniform4f GLEW_GET_FUN(__glewUniform4f)
#define glUniform4fv GLEW_GET_FUN(__glewUniform4fv)
#define glUniform4i GLEW_GET_FUN(__glewUniform4i)
#define glUniform4iv GLEW_GET_FUN(__glewUniform4iv)
#define glUniformMatrix2fv GLEW_GET_FUN(__glewUniformMatrix2fv)
#define glUniformMatrix3fv GLEW_GET_FUN(__glewUniformMatrix3fv)
#define glUniformMatrix4fv GLEW_GET_FUN(__glewUniformMatrix4fv)
#define glUseProgram GLEW_GET_FUN(__glewUseProgram)
#define glValidateProgram GLEW_GET_FUN(__glewValidateProgram)
#define glVertexAttrib1f GLEW_GET_FUN(__glewVertexAttrib1f)
#define glVertexAttrib1fv GLEW_GET_FUN(__glewVertexAttrib1fv)
#define glVertexAttrib2f GLEW_GET_FUN(__glewVertexAttrib2f)
#define glVertexAttrib2fv GLEW_GET_FUN(__glewVertexAttrib2fv)
#define glVertexAttrib3f GLEW_GET_FUN(__glewVertexAttrib3f)
#define glVertexAttrib3fv GLEW_GET_FUN(__glewVertexAttrib3fv)
#define glVertexAttrib4f GLEW_GET_FUN(__glewVertexAttrib4f)
#define glVertexAttrib4fv GLEW_GET_FUN(__glewVertexAttrib4fv)
#define glVertexAttribPointer GLEW_GET_FUN(__glewVertexAttribPointer)
GL_APICALL void GL_APIENTRY glBindTexture(GLenum target, GLuint texture);
GL_APICALL void GL_APIENTRY glBlendFunc(GLenum sfactor, GLenum dfactor);
GL_APICALL void GL_APIENTRY glClear(GLbitfield mask);
GL_APICALL void GL_APIENTRY glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
GL_APICALL void GL_APIENTRY glClearStencil(GLint s);
GL_APICALL void GL_APIENTRY glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
GL_APICALL void GL_APIENTRY glCopyTexImage2D(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
GL_APICALL void GL_APIENTRY glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
GL_APICALL void GL_APIENTRY glCullFace(GLenum mode);
GL_APICALL void GL_APIENTRY glDeleteTextures(GLsizei n, const GLuint *textures);
GL_APICALL void GL_APIENTRY glDepthFunc(GLenum func);
GL_APICALL void GL_APIENTRY glDepthMask(GLboolean flag);
GL_APICALL void GL_APIENTRY glDisable(GLenum cap);
GL_APICALL void GL_APIENTRY glDrawArrays(GLenum mode, GLint first, GLsizei count);
GL_APICALL void GL_APIENTRY glDrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices);
GL_APICALL void GL_APIENTRY glEnable(GLenum cap);
GL_APICALL void GL_APIENTRY glFinish(void);
GL_APICALL void GL_APIENTRY glFlush(void);
GL_APICALL void GL_APIENTRY glFrontFace(GLenum mode);
GL_APICALL void GL_APIENTRY glGenTextures(GLsizei n, GLuint *textures);
GL_APICALL void GL_APIENTRY glGetBooleanv(GLenum pname, GLboolean *params);
GL_APICALL GLenum GL_APIENTRY glGetError(void);
GL_APICALL void GL_APIENTRY glGetFloatv(GLenum pname, GLfloat *params);
GL_APICALL void GL_APIENTRY glGetIntegerv(GLenum pname, GLint *params);
GL_APICALL const GLubyte * GL_APIENTRY glGetString(GLenum name);
GL_APICALL void GL_APIENTRY glGetTexParameterfv(GLenum target, GLenum pname, GLfloat *params);
GL_APICALL void GL_APIENTRY glGetTexParameteriv(GLenum target, GLenum pname, GLint *params);
GL_APICALL void GL_APIENTRY glHint(GLenum target, GLenum mode);
GL_APICALL GLboolean GL_APIENTRY glIsEnabled(GLenum cap);
GL_APICALL GLboolean GL_APIENTRY glIsTexture(GLuint texture);
GL_APICALL void GL_APIENTRY glLineWidth(GLfloat width);
GL_APICALL void GL_APIENTRY glPixelStorei(GLenum pname, GLint param);
GL_APICALL void GL_APIENTRY glPolygonOffset(GLfloat factor, GLfloat units);
GL_APICALL void GL_APIENTRY glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);
GL_APICALL void GL_APIENTRY glScissor(GLint x, GLint y, GLsizei width, GLsizei height);
GL_APICALL void GL_APIENTRY glStencilFunc(GLenum func, GLint ref, GLuint mask);
GL_APICALL void GL_APIENTRY glStencilMask(GLuint mask);
GL_APICALL void GL_APIENTRY glStencilOp(GLenum fail, GLenum zfail, GLenum zpass);
GL_APICALL void GL_APIENTRY glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
GL_APICALL void GL_APIENTRY glTexParameterf(GLenum target, GLenum pname, GLfloat param);
GL_APICALL void GL_APIENTRY glTexParameterfv(GLenum target, GLenum pname, const GLfloat *params);
GL_APICALL void GL_APIENTRY glTexParameteri(GLenum target, GLenum pname, GLint param);
GL_APICALL void GL_APIENTRY glTexParameteriv(GLenum target, GLenum pname, const GLint *params);
GL_APICALL void GL_APIENTRY glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
GL_APICALL void GL_APIENTRY glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
#endif
#endif /* GL_ES_VERSION_2_0 */

#ifndef GL_ES_VERSION_3_0
#define GL_ES_VERSION_3_0 1
typedef unsigned short GLhalf;
#define GL_READ_BUFFER                    0x0C02
#define GL_UNPACK_ROW_LENGTH              0x0CF2
#define GL_UNPACK_SKIP_ROWS               0x0CF3
#define GL_UNPACK_SKIP_PIXELS             0x0CF4
#define GL_PACK_ROW_LENGTH                0x0D02
#define GL_PACK_SKIP_ROWS                 0x0D03
#define GL_PACK_SKIP_PIXELS               0x0D04
#define GL_COLOR                          0x1800
#define GL_DEPTH                          0x1801
#define GL_STENCIL                        0x1802
#define GL_RED                            0x1903
#define GL_RGB8                           0x8051
#define GL_RGBA8                          0x8058
#define GL_RGB10_A2                       0x8059
#define GL_TEXTURE_BINDING_3D             0x806A
#define GL_UNPACK_SKIP_IMAGES             0x806D
#define GL_UNPACK_IMAGE_HEIGHT            0x806E
#define GL_TEXTURE_3D                     0x806F
#define GL_TEXTURE_WRAP_R                 0x8072
#define GL_MAX_3D_TEXTURE_SIZE            0x8073
#define GL_UNSIGNED_INT_2_10_10_10_REV    0x8368
#define GL_MAX_ELEMENTS_VERTICES          0x80E8
#define GL_MAX_ELEMENTS_INDICES           0x80E9
#define GL_TEXTURE_MIN_LOD                0x813A
#define GL_TEXTURE_MAX_LOD                0x813B
#define GL_TEXTURE_BASE_LEVEL             0x813C
#define GL_TEXTURE_MAX_LEVEL              0x813D
#define GL_MIN                            0x8007
#define GL_MAX                            0x8008
#define GL_DEPTH_COMPONENT24              0x81A6
#define GL_MAX_TEXTURE_LOD_BIAS           0x84FD
#define GL_TEXTURE_COMPARE_MODE           0x884C
#define GL_TEXTURE_COMPARE_FUNC           0x884D
#define GL_CURRENT_QUERY                  0x8865
#define GL_QUERY_RESULT                   0x8866
#define GL_QUERY_RESULT_AVAILABLE         0x8867
#define GL_BUFFER_MAPPED                  0x88BC
#define GL_BUFFER_MAP_POINTER             0x88BD
#define GL_STREAM_READ                    0x88E1
#define GL_STREAM_COPY                    0x88E2
#define GL_STATIC_READ                    0x88E5
#define GL_STATIC_COPY                    0x88E6
#define GL_DYNAMIC_READ                   0x88E9
#define GL_DYNAMIC_COPY                   0x88EA
#define GL_MAX_DRAW_BUFFERS               0x8824
#define GL_DRAW_BUFFER0                   0x8825
#define GL_DRAW_BUFFER1                   0x8826
#define GL_DRAW_BUFFER2                   0x8827
#define GL_DRAW_BUFFER3                   0x8828
#define GL_DRAW_BUFFER4                   0x8829
#define GL_DRAW_BUFFER5                   0x882A
#define GL_DRAW_BUFFER6                   0x882B
#define GL_DRAW_BUFFER7                   0x882C
#define GL_DRAW_BUFFER8                   0x882D
#define GL_DRAW_BUFFER9                   0x882E
#define GL_DRAW_BUFFER10                  0x882F
#define GL_DRAW_BUFFER11                  0x8830
#define GL_DRAW_BUFFER12                  0x8831
#define GL_DRAW_BUFFER13                  0x8832
#define GL_DRAW_BUFFER14                  0x8833
#define GL_DRAW_BUFFER15                  0x8834
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS 0x8B49
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS  0x8B4A
#define GL_SAMPLER_3D                     0x8B5F
#define GL_SAMPLER_2D_SHADOW              0x8B62
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT 0x8B8B
#define GL_PIXEL_PACK_BUFFER              0x88EB
#define GL_PIXEL_UNPACK_BUFFER            0x88EC
#define GL_PIXEL_PACK_BUFFER_BINDING      0x88ED
#define GL_PIXEL_UNPACK_BUFFER_BINDING    0x88EF
#define GL_FLOAT_MAT2x3                   0x8B65
#define GL_FLOAT_MAT2x4                   0x8B66
#define GL_FLOAT_MAT3x2                   0x8B67
#define GL_FLOAT_MAT3x4                   0x8B68
#define GL_FLOAT_MAT4x2                   0x8B69
#define GL_FLOAT_MAT4x3                   0x8B6A
#define GL_SRGB                           0x8C40
#define GL_SRGB8                          0x8C41
#define GL_SRGB8_ALPHA8                   0x8C43
#define GL_COMPARE_REF_TO_TEXTURE         0x884E
#define GL_MAJOR_VERSION                  0x821B
#define GL_MINOR_VERSION                  0x821C
#define GL_NUM_EXTENSIONS                 0x821D
#define GL_RGBA32F                        0x8814
#define GL_RGB32F                         0x8815
#define GL_RGBA16F                        0x881A
#define GL_RGB16F                         0x881B
#define GL_VERTEX_ATTRIB_ARRAY_INTEGER    0x88FD
#define GL_MAX_ARRAY_TEXTURE_LAYERS       0x88FF
#define GL_MIN_PROGRAM_TEXEL_OFFSET       0x8904
#define GL_MAX_PROGRAM_TEXEL_OFFSET       0x8905
#define GL_MAX_VARYING_COMPONENTS         0x8B4B
#define GL_TEXTURE_2D_ARRAY               0x8C1A
#define GL_TEXTURE_BINDING_2D_ARRAY       0x8C1D
#define GL_R11F_G11F_B10F                 0x8C3A
#define GL_UNSIGNED_INT_10F_11F_11F_REV   0x8C3B
#define GL_RGB9_E5                        0x8C3D
#define GL_UNSIGNED_INT_5_9_9_9_REV       0x8C3E
#define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH 0x8C76
#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE 0x8C7F
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS 0x8C80
#define GL_TRANSFORM_FEEDBACK_VARYINGS    0x8C83
#define GL_TRANSFORM_FEEDBACK_BUFFER_START 0x8C84
#define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE 0x8C85
#define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN 0x8C88
#define GL_RASTERIZER_DISCARD             0x8C89
#define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS 0x8C8A
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS 0x8C8B
#define GL_INTERLEAVED_ATTRIBS            0x8C8C
#define GL_SEPARATE_ATTRIBS               0x8C8D
#define GL_TRANSFORM_FEEDBACK_BUFFER      0x8C8E
#define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING 0x8C8F
#define GL_RGBA32UI                       0x8D70
#define GL_RGB32UI                        0x8D71
#define GL_RGBA16UI                       0x8D76
#define GL_RGB16UI                        0x8D77
#define GL_RGBA8UI                        0x8D7C
#define GL_RGB8UI                         0x8D7D
#define GL_RGBA32I                        0x8D82
#define GL_RGB32I                         0x8D83
#define GL_RGBA16I                        0x8D88
#define GL_RGB16I                         0x8D89
#define GL_RGBA8I                         0x8D8E
#define GL_RGB8I                          0x8D8F
#define GL_RED_INTEGER                    0x8D94
#define GL_RGB_INTEGER                    0x8D98
#define GL_RGBA_INTEGER                   0x8D99
#define GL_SAMPLER_2D_ARRAY               0x8DC1
#define GL_SAMPLER_2D_ARRAY_SHADOW        0x8DC4
#define GL_SAMPLER_CUBE_SHADOW            0x8DC5
#define GL_UNSIGNED_INT_VEC2              0x8DC6
#define GL_UNSIGNED_INT_VEC3              0x8DC7
#define GL_UNSIGNED_INT_VEC4              0x8DC8
#define GL_INT_SAMPLER_2D                 0x8DCA
#define GL_INT_SAMPLER_3D                 0x8DCB
#define GL_INT_SAMPLER_CUBE               0x8DCC
#define GL_INT_SAMPLER_2D_ARRAY           0x8DCF
#define GL_UNSIGNED_INT_SAMPLER_2D        0x8DD2
#define GL_UNSIGNED_INT_SAMPLER_3D        0x8DD3
#define GL_UNSIGNED_INT_SAMPLER_CUBE      0x8DD4
#define GL_UNSIGNED_INT_SAMPLER_2D_ARRAY  0x8DD7
#define GL_BUFFER_ACCESS_FLAGS            0x911F
#define GL_BUFFER_MAP_LENGTH              0x9120
#define GL_BUFFER_MAP_OFFSET              0x9121
#define GL_DEPTH_COMPONENT32F             0x8CAC
#define GL_DEPTH32F_STENCIL8              0x8CAD
#define GL_FLOAT_32_UNSIGNED_INT_24_8_REV 0x8DAD
#define GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING 0x8210
#define GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE 0x8211
#define GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE 0x8212
#define GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE 0x8213
#define GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE 0x8214
#define GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE 0x8215
#define GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE 0x8216
#define GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE 0x8217
#define GL_FRAMEBUFFER_DEFAULT            0x8218
#define GL_FRAMEBUFFER_UNDEFINED          0x8219
#define GL_DEPTH_STENCIL_ATTACHMENT       0x821A
#define GL_DEPTH_STENCIL                  0x84F9
#define GL_UNSIGNED_INT_24_8              0x84FA
#define GL_DEPTH24_STENCIL8               0x88F0
#define GL_UNSIGNED_NORMALIZED            0x8C17
#define GL_DRAW_FRAMEBUFFER_BINDING       0x8CA6
#define GL_READ_FRAMEBUFFER               0x8CA8
#define GL_DRAW_FRAMEBUFFER               0x8CA9
#define GL_READ_FRAMEBUFFER_BINDING       0x8CAA
#define GL_RENDERBUFFER_SAMPLES           0x8CAB
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER 0x8CD4
#define GL_MAX_COLOR_ATTACHMENTS          0x8CDF
#define GL_COLOR_ATTACHMENT1              0x8CE1
#define GL_COLOR_ATTACHMENT2              0x8CE2
#define GL_COLOR_ATTACHMENT3              0x8CE3
#define GL_COLOR_ATTACHMENT4              0x8CE4
#define GL_COLOR_ATTACHMENT5              0x8CE5
#define GL_COLOR_ATTACHMENT6              0x8CE6
#define GL_COLOR_ATTACHMENT7              0x8CE7
#define GL_COLOR_ATTACHMENT8              0x8CE8
#define GL_COLOR_ATTACHMENT9              0x8CE9
#define GL_COLOR_ATTACHMENT10             0x8CEA
#define GL_COLOR_ATTACHMENT11             0x8CEB
#define GL_COLOR_ATTACHMENT12             0x8CEC
#define GL_COLOR_ATTACHMENT13             0x8CED
#define GL_COLOR_ATTACHMENT14             0x8CEE
#define GL_COLOR_ATTACHMENT15             0x8CEF
#define GL_COLOR_ATTACHMENT16             0x8CF0
#define GL_COLOR_ATTACHMENT17             0x8CF1
#define GL_COLOR_ATTACHMENT18             0x8CF2
#define GL_COLOR_ATTACHMENT19             0x8CF3
#define GL_COLOR_ATTACHMENT20             0x8CF4
#define GL_COLOR_ATTACHMENT21             0x8CF5
#define GL_COLOR_ATTACHMENT22             0x8CF6
#define GL_COLOR_ATTACHMENT23             0x8CF7
#define GL_COLOR_ATTACHMENT24             0x8CF8
#define GL_COLOR_ATTACHMENT25             0x8CF9
#define GL_COLOR_ATTACHMENT26             0x8CFA
#define GL_COLOR_ATTACHMENT27             0x8CFB
#define GL_COLOR_ATTACHMENT28             0x8CFC
#define GL_COLOR_ATTACHMENT29             0x8CFD
#define GL_COLOR_ATTACHMENT30             0x8CFE
#define GL_COLOR_ATTACHMENT31             0x8CFF
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE 0x8D56
#define GL_MAX_SAMPLES                    0x8D57
#define GL_HALF_FLOAT                     0x140B
#define GL_MAP_READ_BIT                   0x0001
#define GL_MAP_WRITE_BIT                  0x0002
#define GL_MAP_INVALIDATE_RANGE_BIT       0x0004
#define GL_MAP_INVALIDATE_BUFFER_BIT      0x0008
#define GL_MAP_FLUSH_EXPLICIT_BIT         0x0010
#define GL_MAP_UNSYNCHRONIZED_BIT         0x0020
#define GL_RG                             0x8227
#define GL_RG_INTEGER                     0x8228
#define GL_R8                             0x8229
#define GL_RG8                            0x822B
#define GL_R16F                           0x822D
#define GL_R32F                           0x822E
#define GL_RG16F                          0x822F
#define GL_RG32F                          0x8230
#define GL_R8I                            0x8231
#define GL_R8UI                           0x8232
#define GL_R16I                           0x8233
#define GL_R16UI                          0x8234
#define GL_R32I                           0x8235
#define GL_R32UI                          0x8236
#define GL_RG8I                           0x8237
#define GL_RG8UI                          0x8238
#define GL_RG16I                          0x8239
#define GL_RG16UI                         0x823A
#define GL_RG32I                          0x823B
#define GL_RG32UI                         0x823C
#define GL_VERTEX_ARRAY_BINDING           0x85B5
#define GL_R8_SNORM                       0x8F94
#define GL_RG8_SNORM                      0x8F95
#define GL_RGB8_SNORM                     0x8F96
#define GL_RGBA8_SNORM                    0x8F97
#define GL_SIGNED_NORMALIZED              0x8F9C
#define GL_PRIMITIVE_RESTART_FIXED_INDEX  0x8D69
#define GL_COPY_READ_BUFFER               0x8F36
#define GL_COPY_WRITE_BUFFER              0x8F37
#define GL_COPY_READ_BUFFER_BINDING       0x8F36
#define GL_COPY_WRITE_BUFFER_BINDING      0x8F37
#define GL_UNIFORM_BUFFER                 0x8A11
#define GL_UNIFORM_BUFFER_BINDING         0x8A28
#define GL_UNIFORM_BUFFER_START           0x8A29
#define GL_UNIFORM_BUFFER_SIZE            0x8A2A
#define GL_MAX_VERTEX_UNIFORM_BLOCKS      0x8A2B
#define GL_MAX_FRAGMENT_UNIFORM_BLOCKS    0x8A2D
#define GL_MAX_COMBINED_UNIFORM_BLOCKS    0x8A2E
#define GL_MAX_UNIFORM_BUFFER_BINDINGS    0x8A2F
#define GL_MAX_UNIFORM_BLOCK_SIZE         0x8A30
#define GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS 0x8A31
#define GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS 0x8A33
#define GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT 0x8A34
#define GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH 0x8A35
#define GL_ACTIVE_UNIFORM_BLOCKS          0x8A36
#define GL_UNIFORM_TYPE                   0x8A37
#define GL_UNIFORM_SIZE                   0x8A38
#define GL_UNIFORM_NAME_LENGTH            0x8A39
#define GL_UNIFORM_BLOCK_INDEX            0x8A3A
#define GL_UNIFORM_OFFSET                 0x8A3B
#define GL_UNIFORM_ARRAY_STRIDE           0x8A3C
#define GL_UNIFORM_MATRIX_STRIDE          0x8A3D
#define GL_UNIFORM_IS_ROW_MAJOR           0x8A3E
#define GL_UNIFORM_BLOCK_BINDING          0x8A3F
#define GL_UNIFORM_BLOCK_DATA_SIZE        0x8A40
#define GL_UNIFORM_BLOCK_NAME_LENGTH      0x8A41
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS  0x8A42
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES 0x8A43
#define GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER 0x8A44
#define GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER 0x8A46
#define GL_INVALID_INDEX                  0xFFFFFFFFu
#define GL_MAX_VERTEX_OUTPUT_COMPONENTS   0x9122
#define GL_MAX_FRAGMENT_INPUT_COMPONENTS  0x9125
#define GL_MAX_SERVER_WAIT_TIMEOUT        0x9111
#define GL_OBJECT_TYPE                    0x9112
#define GL_SYNC_CONDITION                 0x9113
#define GL_SYNC_STATUS                    0x9114
#define GL_SYNC_FLAGS                     0x9115
#define GL_SYNC_FENCE                     0x9116
#define GL_SYNC_GPU_COMMANDS_COMPLETE     0x9117
#define GL_UNSIGNALED                     0x9118
#define GL_SIGNALED                       0x9119
#define GL_ALREADY_SIGNALED               0x911A
#define GL_TIMEOUT_EXPIRED                0x911B
#define GL_CONDITION_SATISFIED            0x911C
#define GL_WAIT_FAILED                    0x911D
#define GL_SYNC_FLUSH_COMMANDS_BIT        0x00000001
#define GL_TIMEOUT_IGNORED                0xFFFFFFFFFFFFFFFFull
#define GL_VERTEX_ATTRIB_ARRAY_DIVISOR    0x88FE
#define GL_ANY_SAMPLES_PASSED             0x8C2F
#define GL_ANY_SAMPLES_PASSED_CONSERVATIVE 0x8D6A
#define GL_SAMPLER_BINDING                0x8919
#define GL_RGB10_A2UI                     0x906F
#define GL_TEXTURE_SWIZZLE_R              0x8E42
#define GL_TEXTURE_SWIZZLE_G              0x8E43
#define GL_TEXTURE_SWIZZLE_B              0x8E44
#define GL_TEXTURE_SWIZZLE_A              0x8E45
#define GL_GREEN                          0x1904
#define GL_BLUE                           0x1905
#define GL_INT_2_10_10_10_REV             0x8D9F
#define GL_TRANSFORM_FEEDBACK             0x8E22
#define GL_TRANSFORM_FEEDBACK_PAUSED      0x8E23
#define GL_TRANSFORM_FEEDBACK_ACTIVE      0x8E24
#define GL_TRANSFORM_FEEDBACK_BINDING     0x8E25
#define GL_PROGRAM_BINARY_RETRIEVABLE_HINT 0x8257
#define GL_PROGRAM_BINARY_LENGTH          0x8741
#define GL_NUM_PROGRAM_BINARY_FORMATS     0x87FE
#define GL_PROGRAM_BINARY_FORMATS         0x87FF
#define GL_COMPRESSED_R11_EAC             0x9270
#define GL_COMPRESSED_SIGNED_R11_EAC      0x9271
#define GL_COMPRESSED_RG11_EAC            0x9272
#define GL_COMPRESSED_SIGNED_RG11_EAC     0x9273
#define GL_COMPRESSED_RGB8_ETC2           0x9274
#define GL_COMPRESSED_SRGB8_ETC2          0x9275
#define GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2 0x9276
#define GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2 0x9277
#define GL_COMPRESSED_RGBA8_ETC2_EAC      0x9278
#define GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC 0x9279
#define GL_TEXTURE_IMMUTABLE_FORMAT       0x912F
#define GL_MAX_ELEMENT_INDEX              0x8D6B
#define GL_NUM_SAMPLE_COUNTS              0x9380
#define GL_TEXTURE_IMMUTABLE_LEVELS       0x82DF
typedef void (GL_APIENTRYP PFNGLREADBUFFERPROC) (GLenum src);
typedef void (GL_APIENTRYP PFNGLDRAWRANGEELEMENTSPROC) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices);
typedef void (GL_APIENTRYP PFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (GL_APIENTRYP PFNGLTEXSUBIMAGE3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
typedef void (GL_APIENTRYP PFNGLCOPYTEXSUBIMAGE3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GL_APIENTRYP PFNGLCOMPRESSEDTEXIMAGE3DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data);
typedef void (GL_APIENTRYP PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);
typedef void (GL_APIENTRYP PFNGLGENQUERIESPROC) (GLsizei n, GLuint *ids);
typedef void (GL_APIENTRYP PFNGLDELETEQUERIESPROC) (GLsizei n, const GLuint *ids);
typedef GLboolean (GL_APIENTRYP PFNGLISQUERYPROC) (GLuint id);
typedef void (GL_APIENTRYP PFNGLBEGINQUERYPROC) (GLenum target, GLuint id);
typedef void (GL_APIENTRYP PFNGLENDQUERYPROC) (GLenum target);
typedef void (GL_APIENTRYP PFNGLGETQUERYIVPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (GL_APIENTRYP PFNGLGETQUERYOBJECTUIVPROC) (GLuint id, GLenum pname, GLuint *params);
typedef GLboolean (GL_APIENTRYP PFNGLUNMAPBUFFERPROC) (GLenum target);
typedef void (GL_APIENTRYP PFNGLGETBUFFERPOINTERVPROC) (GLenum target, GLenum pname, void **params);
typedef void (GL_APIENTRYP PFNGLDRAWBUFFERSPROC) (GLsizei n, const GLenum *bufs);
typedef void (GL_APIENTRYP PFNGLUNIFORMMATRIX2X3FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GL_APIENTRYP PFNGLUNIFORMMATRIX3X2FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GL_APIENTRYP PFNGLUNIFORMMATRIX2X4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GL_APIENTRYP PFNGLUNIFORMMATRIX4X2FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GL_APIENTRYP PFNGLUNIFORMMATRIX3X4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GL_APIENTRYP PFNGLUNIFORMMATRIX4X3FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GL_APIENTRYP PFNGLBLITFRAMEBUFFERPROC) (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef void (GL_APIENTRYP PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GL_APIENTRYP PFNGLFRAMEBUFFERTEXTURELAYERPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
typedef void *(GL_APIENTRYP PFNGLMAPBUFFERRANGEPROC) (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
typedef void (GL_APIENTRYP PFNGLFLUSHMAPPEDBUFFERRANGEPROC) (GLenum target, GLintptr offset, GLsizeiptr length);
typedef void (GL_APIENTRYP PFNGLBINDVERTEXARRAYPROC) (GLuint array);
typedef void (GL_APIENTRYP PFNGLDELETEVERTEXARRAYSPROC) (GLsizei n, const GLuint *arrays);
typedef void (GL_APIENTRYP PFNGLGENVERTEXARRAYSPROC) (GLsizei n, GLuint *arrays);
typedef GLboolean (GL_APIENTRYP PFNGLISVERTEXARRAYPROC) (GLuint array);
typedef void (GL_APIENTRYP PFNGLGETINTEGERI_VPROC) (GLenum target, GLuint index, GLint *data);
typedef void (GL_APIENTRYP PFNGLBEGINTRANSFORMFEEDBACKPROC) (GLenum primitiveMode);
typedef void (GL_APIENTRYP PFNGLENDTRANSFORMFEEDBACKPROC) (void);
typedef void (GL_APIENTRYP PFNGLBINDBUFFERRANGEPROC) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (GL_APIENTRYP PFNGLBINDBUFFERBASEPROC) (GLenum target, GLuint index, GLuint buffer);
typedef void (GL_APIENTRYP PFNGLTRANSFORMFEEDBACKVARYINGSPROC) (GLuint program, GLsizei count, const GLchar *const*varyings, GLenum bufferMode);
typedef void (GL_APIENTRYP PFNGLGETTRANSFORMFEEDBACKVARYINGPROC) (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
typedef void (GL_APIENTRYP PFNGLVERTEXATTRIBIPOINTERPROC) (GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
typedef void (GL_APIENTRYP PFNGLGETVERTEXATTRIBIIVPROC) (GLuint index, GLenum pname, GLint *params);
typedef void (GL_APIENTRYP PFNGLGETVERTEXATTRIBIUIVPROC) (GLuint index, GLenum pname, GLuint *params);
typedef void (GL_APIENTRYP PFNGLVERTEXATTRIBI4IPROC) (GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void (GL_APIENTRYP PFNGLVERTEXATTRIBI4UIPROC) (GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void (GL_APIENTRYP PFNGLVERTEXATTRIBI4IVPROC) (GLuint index, const GLint *v);
typedef void (GL_APIENTRYP PFNGLVERTEXATTRIBI4UIVPROC) (GLuint index, const GLuint *v);
typedef void (GL_APIENTRYP PFNGLGETUNIFORMUIVPROC) (GLuint program, GLint location, GLuint *params);
typedef GLint (GL_APIENTRYP PFNGLGETFRAGDATALOCATIONPROC) (GLuint program, const GLchar *name);
typedef void (GL_APIENTRYP PFNGLUNIFORM1UIPROC) (GLint location, GLuint v0);
typedef void (GL_APIENTRYP PFNGLUNIFORM2UIPROC) (GLint location, GLuint v0, GLuint v1);
typedef void (GL_APIENTRYP PFNGLUNIFORM3UIPROC) (GLint location, GLuint v0, GLuint v1, GLuint v2);
typedef void (GL_APIENTRYP PFNGLUNIFORM4UIPROC) (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
typedef void (GL_APIENTRYP PFNGLUNIFORM1UIVPROC) (GLint location, GLsizei count, const GLuint *value);
typedef void (GL_APIENTRYP PFNGLUNIFORM2UIVPROC) (GLint location, GLsizei count, const GLuint *value);
typedef void (GL_APIENTRYP PFNGLUNIFORM3UIVPROC) (GLint location, GLsizei count, const GLuint *value);
typedef void (GL_APIENTRYP PFNGLUNIFORM4UIVPROC) (GLint location, GLsizei count, const GLuint *value);
typedef void (GL_APIENTRYP PFNGLCLEARBUFFERIVPROC) (GLenum buffer, GLint drawbuffer, const GLint *value);
typedef void (GL_APIENTRYP PFNGLCLEARBUFFERUIVPROC) (GLenum buffer, GLint drawbuffer, const GLuint *value);
typedef void (GL_APIENTRYP PFNGLCLEARBUFFERFVPROC) (GLenum buffer, GLint drawbuffer, const GLfloat *value);
typedef void (GL_APIENTRYP PFNGLCLEARBUFFERFIPROC) (GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
typedef const GLubyte *(GL_APIENTRYP PFNGLGETSTRINGIPROC) (GLenum name, GLuint index);
typedef void (GL_APIENTRYP PFNGLCOPYBUFFERSUBDATAPROC) (GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
typedef void (GL_APIENTRYP PFNGLGETUNIFORMINDICESPROC) (GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices);
typedef void (GL_APIENTRYP PFNGLGETACTIVEUNIFORMSIVPROC) (GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params);
typedef GLuint (GL_APIENTRYP PFNGLGETUNIFORMBLOCKINDEXPROC) (GLuint program, const GLchar *uniformBlockName);
typedef void (GL_APIENTRYP PFNGLGETACTIVEUNIFORMBLOCKIVPROC) (GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params);
typedef void (GL_APIENTRYP PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC) (GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName);
typedef void (GL_APIENTRYP PFNGLUNIFORMBLOCKBINDINGPROC) (GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
typedef void (GL_APIENTRYP PFNGLDRAWARRAYSINSTANCEDPROC) (GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
typedef void (GL_APIENTRYP PFNGLDRAWELEMENTSINSTANCEDPROC) (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount);
typedef GLsync (GL_APIENTRYP PFNGLFENCESYNCPROC) (GLenum condition, GLbitfield flags);
typedef GLboolean (GL_APIENTRYP PFNGLISSYNCPROC) (GLsync sync);
typedef void (GL_APIENTRYP PFNGLDELETESYNCPROC) (GLsync sync);
typedef GLenum (GL_APIENTRYP PFNGLCLIENTWAITSYNCPROC) (GLsync sync, GLbitfield flags, GLuint64 timeout);
typedef void (GL_APIENTRYP PFNGLWAITSYNCPROC) (GLsync sync, GLbitfield flags, GLuint64 timeout);
typedef void (GL_APIENTRYP PFNGLGETINTEGER64VPROC) (GLenum pname, GLint64 *data);
typedef void (GL_APIENTRYP PFNGLGETSYNCIVPROC) (GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);
typedef void (GL_APIENTRYP PFNGLGETINTEGER64I_VPROC) (GLenum target, GLuint index, GLint64 *data);
typedef void (GL_APIENTRYP PFNGLGETBUFFERPARAMETERI64VPROC) (GLenum target, GLenum pname, GLint64 *params);
typedef void (GL_APIENTRYP PFNGLGENSAMPLERSPROC) (GLsizei count, GLuint *samplers);
typedef void (GL_APIENTRYP PFNGLDELETESAMPLERSPROC) (GLsizei count, const GLuint *samplers);
typedef GLboolean (GL_APIENTRYP PFNGLISSAMPLERPROC) (GLuint sampler);
typedef void (GL_APIENTRYP PFNGLBINDSAMPLERPROC) (GLuint unit, GLuint sampler);
typedef void (GL_APIENTRYP PFNGLSAMPLERPARAMETERIPROC) (GLuint sampler, GLenum pname, GLint param);
typedef void (GL_APIENTRYP PFNGLSAMPLERPARAMETERIVPROC) (GLuint sampler, GLenum pname, const GLint *param);
typedef void (GL_APIENTRYP PFNGLSAMPLERPARAMETERFPROC) (GLuint sampler, GLenum pname, GLfloat param);
typedef void (GL_APIENTRYP PFNGLSAMPLERPARAMETERFVPROC) (GLuint sampler, GLenum pname, const GLfloat *param);
typedef void (GL_APIENTRYP PFNGLGETSAMPLERPARAMETERIVPROC) (GLuint sampler, GLenum pname, GLint *params);
typedef void (GL_APIENTRYP PFNGLGETSAMPLERPARAMETERFVPROC) (GLuint sampler, GLenum pname, GLfloat *params);
typedef void (GL_APIENTRYP PFNGLVERTEXATTRIBDIVISORPROC) (GLuint index, GLuint divisor);
typedef void (GL_APIENTRYP PFNGLBINDTRANSFORMFEEDBACKPROC) (GLenum target, GLuint id);
typedef void (GL_APIENTRYP PFNGLDELETETRANSFORMFEEDBACKSPROC) (GLsizei n, const GLuint *ids);
typedef void (GL_APIENTRYP PFNGLGENTRANSFORMFEEDBACKSPROC) (GLsizei n, GLuint *ids);
typedef GLboolean (GL_APIENTRYP PFNGLISTRANSFORMFEEDBACKPROC) (GLuint id);
typedef void (GL_APIENTRYP PFNGLPAUSETRANSFORMFEEDBACKPROC) (void);
typedef void (GL_APIENTRYP PFNGLRESUMETRANSFORMFEEDBACKPROC) (void);
typedef void (GL_APIENTRYP PFNGLGETPROGRAMBINARYPROC) (GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary);
typedef void (GL_APIENTRYP PFNGLPROGRAMBINARYPROC) (GLuint program, GLenum binaryFormat, const void *binary, GLsizei length);
typedef void (GL_APIENTRYP PFNGLPROGRAMPARAMETERIPROC) (GLuint program, GLenum pname, GLint value);
typedef void (GL_APIENTRYP PFNGLINVALIDATEFRAMEBUFFERPROC) (GLenum target, GLsizei numAttachments, const GLenum *attachments);
typedef void (GL_APIENTRYP PFNGLINVALIDATESUBFRAMEBUFFERPROC) (GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GL_APIENTRYP PFNGLTEXSTORAGE2DPROC) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GL_APIENTRYP PFNGLTEXSTORAGE3DPROC) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
typedef void (GL_APIENTRYP PFNGLGETINTERNALFORMATIVPROC) (GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params);
#if GL_GLES_PROTOTYPES
GLEW_FUN_EXPORT PFNGLDRAWRANGEELEMENTSPROC __glewDrawRangeElements;
GLEW_FUN_EXPORT PFNGLTEXIMAGE3DPROC __glewTexImage3D;
GLEW_FUN_EXPORT PFNGLTEXSUBIMAGE3DPROC __glewTexSubImage3D;
GLEW_FUN_EXPORT PFNGLCOPYTEXSUBIMAGE3DPROC __glewCopyTexSubImage3D;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXIMAGE3DPROC __glewCompressedTexImage3D;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC __glewCompressedTexSubImage3D;
GLEW_FUN_EXPORT PFNGLGENQUERIESPROC __glewGenQueries;
GLEW_FUN_EXPORT PFNGLDELETEQUERIESPROC __glewDeleteQueries;
GLEW_FUN_EXPORT PFNGLISQUERYPROC __glewIsQuery;
GLEW_FUN_EXPORT PFNGLBEGINQUERYPROC __glewBeginQuery;
GLEW_FUN_EXPORT PFNGLENDQUERYPROC __glewEndQuery;
GLEW_FUN_EXPORT PFNGLGETQUERYIVPROC __glewGetQueryiv;
GLEW_FUN_EXPORT PFNGLGETQUERYOBJECTUIVPROC __glewGetQueryObjectuiv;
GLEW_FUN_EXPORT PFNGLUNMAPBUFFERPROC __glewUnmapBuffer;
GLEW_FUN_EXPORT PFNGLGETBUFFERPOINTERVPROC __glewGetBufferPointerv;
GLEW_FUN_EXPORT PFNGLDRAWBUFFERSPROC __glewDrawBuffers;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX2X3FVPROC __glewUniformMatrix2x3fv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX3X2FVPROC __glewUniformMatrix3x2fv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX2X4FVPROC __glewUniformMatrix2x4fv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX4X2FVPROC __glewUniformMatrix4x2fv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX3X4FVPROC __glewUniformMatrix3x4fv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX4X3FVPROC __glewUniformMatrix4x3fv;
GLEW_FUN_EXPORT PFNGLBLITFRAMEBUFFERPROC __glewBlitFramebuffer;
GLEW_FUN_EXPORT PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC __glewRenderbufferStorageMultisample;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERTEXTURELAYERPROC __glewFramebufferTextureLayer;
GLEW_FUN_EXPORT PFNGLMAPBUFFERRANGEPROC __glewMapBufferRange;
GLEW_FUN_EXPORT PFNGLFLUSHMAPPEDBUFFERRANGEPROC __glewFlushMappedBufferRange;
GLEW_FUN_EXPORT PFNGLBINDVERTEXARRAYPROC __glewBindVertexArray;
GLEW_FUN_EXPORT PFNGLDELETEVERTEXARRAYSPROC __glewDeleteVertexArrays;
GLEW_FUN_EXPORT PFNGLGENVERTEXARRAYSPROC __glewGenVertexArrays;
GLEW_FUN_EXPORT PFNGLISVERTEXARRAYPROC __glewIsVertexArray;
GLEW_FUN_EXPORT PFNGLGETINTEGERI_VPROC __glewGetIntegeri_v;
GLEW_FUN_EXPORT PFNGLBEGINTRANSFORMFEEDBACKPROC __glewBeginTransformFeedback;
GLEW_FUN_EXPORT PFNGLENDTRANSFORMFEEDBACKPROC __glewEndTransformFeedback;
GLEW_FUN_EXPORT PFNGLBINDBUFFERRANGEPROC __glewBindBufferRange;
GLEW_FUN_EXPORT PFNGLBINDBUFFERBASEPROC __glewBindBufferBase;
GLEW_FUN_EXPORT PFNGLTRANSFORMFEEDBACKVARYINGSPROC __glewTransformFeedbackVaryings;
GLEW_FUN_EXPORT PFNGLGETTRANSFORMFEEDBACKVARYINGPROC __glewGetTransformFeedbackVarying;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBIPOINTERPROC __glewVertexAttribIPointer;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBIIVPROC __glewGetVertexAttribIiv;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBIUIVPROC __glewGetVertexAttribIuiv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4IPROC __glewVertexAttribI4i;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4UIPROC __glewVertexAttribI4ui;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4IVPROC __glewVertexAttribI4iv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4UIVPROC __glewVertexAttribI4uiv;
GLEW_FUN_EXPORT PFNGLGETUNIFORMUIVPROC __glewGetUniformuiv;
GLEW_FUN_EXPORT PFNGLGETFRAGDATALOCATIONPROC __glewGetFragDataLocation;
GLEW_FUN_EXPORT PFNGLUNIFORM1UIPROC __glewUniform1ui;
GLEW_FUN_EXPORT PFNGLUNIFORM2UIPROC __glewUniform2ui;
GLEW_FUN_EXPORT PFNGLUNIFORM3UIPROC __glewUniform3ui;
GLEW_FUN_EXPORT PFNGLUNIFORM4UIPROC __glewUniform4ui;
GLEW_FUN_EXPORT PFNGLUNIFORM1UIVPROC __glewUniform1uiv;
GLEW_FUN_EXPORT PFNGLUNIFORM2UIVPROC __glewUniform2uiv;
GLEW_FUN_EXPORT PFNGLUNIFORM3UIVPROC __glewUniform3uiv;
GLEW_FUN_EXPORT PFNGLUNIFORM4UIVPROC __glewUniform4uiv;
GLEW_FUN_EXPORT PFNGLCLEARBUFFERIVPROC __glewClearBufferiv;
GLEW_FUN_EXPORT PFNGLCLEARBUFFERUIVPROC __glewClearBufferuiv;
GLEW_FUN_EXPORT PFNGLCLEARBUFFERFVPROC __glewClearBufferfv;
GLEW_FUN_EXPORT PFNGLCLEARBUFFERFIPROC __glewClearBufferfi;
GLEW_FUN_EXPORT PFNGLGETSTRINGIPROC __glewGetStringi;
GLEW_FUN_EXPORT PFNGLCOPYBUFFERSUBDATAPROC __glewCopyBufferSubData;
GLEW_FUN_EXPORT PFNGLGETUNIFORMINDICESPROC __glewGetUniformIndices;
GLEW_FUN_EXPORT PFNGLGETACTIVEUNIFORMSIVPROC __glewGetActiveUniformsiv;
GLEW_FUN_EXPORT PFNGLGETUNIFORMBLOCKINDEXPROC __glewGetUniformBlockIndex;
GLEW_FUN_EXPORT PFNGLGETACTIVEUNIFORMBLOCKIVPROC __glewGetActiveUniformBlockiv;
GLEW_FUN_EXPORT PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC __glewGetActiveUniformBlockName;
GLEW_FUN_EXPORT PFNGLUNIFORMBLOCKBINDINGPROC __glewUniformBlockBinding;
GLEW_FUN_EXPORT PFNGLDRAWARRAYSINSTANCEDPROC __glewDrawArraysInstanced;
GLEW_FUN_EXPORT PFNGLDRAWELEMENTSINSTANCEDPROC __glewDrawElementsInstanced;
GLEW_FUN_EXPORT PFNGLFENCESYNCPROC __glewFenceSync;
GLEW_FUN_EXPORT PFNGLISSYNCPROC __glewIsSync;
GLEW_FUN_EXPORT PFNGLDELETESYNCPROC __glewDeleteSync;
GLEW_FUN_EXPORT PFNGLCLIENTWAITSYNCPROC __glewClientWaitSync;
GLEW_FUN_EXPORT PFNGLWAITSYNCPROC __glewWaitSync;
GLEW_FUN_EXPORT PFNGLGETINTEGER64VPROC __glewGetInteger64v;
GLEW_FUN_EXPORT PFNGLGETSYNCIVPROC __glewGetSynciv;
GLEW_FUN_EXPORT PFNGLGETINTEGER64I_VPROC __glewGetInteger64i_v;
GLEW_FUN_EXPORT PFNGLGETBUFFERPARAMETERI64VPROC __glewGetBufferParameteri64v;
GLEW_FUN_EXPORT PFNGLGENSAMPLERSPROC __glewGenSamplers;
GLEW_FUN_EXPORT PFNGLDELETESAMPLERSPROC __glewDeleteSamplers;
GLEW_FUN_EXPORT PFNGLISSAMPLERPROC __glewIsSampler;
GLEW_FUN_EXPORT PFNGLBINDSAMPLERPROC __glewBindSampler;
GLEW_FUN_EXPORT PFNGLSAMPLERPARAMETERIPROC __glewSamplerParameteri;
GLEW_FUN_EXPORT PFNGLSAMPLERPARAMETERIVPROC __glewSamplerParameteriv;
GLEW_FUN_EXPORT PFNGLSAMPLERPARAMETERFPROC __glewSamplerParameterf;
GLEW_FUN_EXPORT PFNGLSAMPLERPARAMETERFVPROC __glewSamplerParameterfv;
GLEW_FUN_EXPORT PFNGLGETSAMPLERPARAMETERIVPROC __glewGetSamplerParameteriv;
GLEW_FUN_EXPORT PFNGLGETSAMPLERPARAMETERFVPROC __glewGetSamplerParameterfv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBDIVISORPROC __glewVertexAttribDivisor;
GLEW_FUN_EXPORT PFNGLBINDTRANSFORMFEEDBACKPROC __glewBindTransformFeedback;
GLEW_FUN_EXPORT PFNGLDELETETRANSFORMFEEDBACKSPROC __glewDeleteTransformFeedbacks;
GLEW_FUN_EXPORT PFNGLGENTRANSFORMFEEDBACKSPROC __glewGenTransformFeedbacks;
GLEW_FUN_EXPORT PFNGLISTRANSFORMFEEDBACKPROC __glewIsTransformFeedback;
GLEW_FUN_EXPORT PFNGLPAUSETRANSFORMFEEDBACKPROC __glewPauseTransformFeedback;
GLEW_FUN_EXPORT PFNGLRESUMETRANSFORMFEEDBACKPROC __glewResumeTransformFeedback;
GLEW_FUN_EXPORT PFNGLGETPROGRAMBINARYPROC __glewGetProgramBinary;
GLEW_FUN_EXPORT PFNGLPROGRAMBINARYPROC __glewProgramBinary;
GLEW_FUN_EXPORT PFNGLPROGRAMPARAMETERIPROC __glewProgramParameteri;
GLEW_FUN_EXPORT PFNGLINVALIDATEFRAMEBUFFERPROC __glewInvalidateFramebuffer;
GLEW_FUN_EXPORT PFNGLINVALIDATESUBFRAMEBUFFERPROC __glewInvalidateSubFramebuffer;
GLEW_FUN_EXPORT PFNGLTEXSTORAGE2DPROC __glewTexStorage2D;
GLEW_FUN_EXPORT PFNGLTEXSTORAGE3DPROC __glewTexStorage3D;
GLEW_FUN_EXPORT PFNGLGETINTERNALFORMATIVPROC __glewGetInternalformativ;
#define glDrawRangeElements GLEW_GET_FUN(__glewDrawRangeElements)
#define glTexImage3D GLEW_GET_FUN(__glewTexImage3D)
#define glTexSubImage3D GLEW_GET_FUN(__glewTexSubImage3D)
#define glCopyTexSubImage3D GLEW_GET_FUN(__glewCopyTexSubImage3D)
#define glCompressedTexImage3D GLEW_GET_FUN(__glewCompressedTexImage3D)
#define glCompressedTexSubImage3D GLEW_GET_FUN(__glewCompressedTexSubImage3D)
#define glGenQueries GLEW_GET_FUN(__glewGenQueries)
#define glDeleteQueries GLEW_GET_FUN(__glewDeleteQueries)
#define glIsQuery GLEW_GET_FUN(__glewIsQuery)
#define glBeginQuery GLEW_GET_FUN(__glewBeginQuery)
#define glEndQuery GLEW_GET_FUN(__glewEndQuery)
#define glGetQueryiv GLEW_GET_FUN(__glewGetQueryiv)
#define glGetQueryObjectuiv GLEW_GET_FUN(__glewGetQueryObjectuiv)
#define glUnmapBuffer GLEW_GET_FUN(__glewUnmapBuffer)
#define glGetBufferPointerv GLEW_GET_FUN(__glewGetBufferPointerv)
#define glDrawBuffers GLEW_GET_FUN(__glewDrawBuffers)
#define glUniformMatrix2x3fv GLEW_GET_FUN(__glewUniformMatrix2x3fv)
#define glUniformMatrix3x2fv GLEW_GET_FUN(__glewUniformMatrix3x2fv)
#define glUniformMatrix2x4fv GLEW_GET_FUN(__glewUniformMatrix2x4fv)
#define glUniformMatrix4x2fv GLEW_GET_FUN(__glewUniformMatrix4x2fv)
#define glUniformMatrix3x4fv GLEW_GET_FUN(__glewUniformMatrix3x4fv)
#define glUniformMatrix4x3fv GLEW_GET_FUN(__glewUniformMatrix4x3fv)
#define glBlitFramebuffer GLEW_GET_FUN(__glewBlitFramebuffer)
#define glRenderbufferStorageMultisample GLEW_GET_FUN(__glewRenderbufferStorageMultisample)
#define glFramebufferTextureLayer GLEW_GET_FUN(__glewFramebufferTextureLayer)
#define glMapBufferRange GLEW_GET_FUN(__glewMapBufferRange)
#define glFlushMappedBufferRange GLEW_GET_FUN(__glewFlushMappedBufferRange)
#define glBindVertexArray GLEW_GET_FUN(__glewBindVertexArray)
#define glDeleteVertexArrays GLEW_GET_FUN(__glewDeleteVertexArrays)
#define glGenVertexArrays GLEW_GET_FUN(__glewGenVertexArrays)
#define glIsVertexArray GLEW_GET_FUN(__glewIsVertexArray)
#define glGetIntegeri_v GLEW_GET_FUN(__glewGetIntegeri_v)
#define glBeginTransformFeedback GLEW_GET_FUN(__glewBeginTransformFeedback)
#define glEndTransformFeedback GLEW_GET_FUN(__glewEndTransformFeedback)
#define glBindBufferRange GLEW_GET_FUN(__glewBindBufferRange)
#define glBindBufferBase GLEW_GET_FUN(__glewBindBufferBase)
#define glTransformFeedbackVaryings GLEW_GET_FUN(__glewTransformFeedbackVaryings)
#define glGetTransformFeedbackVarying GLEW_GET_FUN(__glewGetTransformFeedbackVarying)
#define glVertexAttribIPointer GLEW_GET_FUN(__glewVertexAttribIPointer)
#define glGetVertexAttribIiv GLEW_GET_FUN(__glewGetVertexAttribIiv)
#define glGetVertexAttribIuiv GLEW_GET_FUN(__glewGetVertexAttribIuiv)
#define glVertexAttribI4i GLEW_GET_FUN(__glewVertexAttribI4i)
#define glVertexAttribI4ui GLEW_GET_FUN(__glewVertexAttribI4ui)
#define glVertexAttribI4iv GLEW_GET_FUN(__glewVertexAttribI4iv)
#define glVertexAttribI4uiv GLEW_GET_FUN(__glewVertexAttribI4uiv)
#define glGetUniformuiv GLEW_GET_FUN(__glewGetUniformuiv)
#define glGetFragDataLocation GLEW_GET_FUN(__glewGetFragDataLocation)
#define glUniform1ui GLEW_GET_FUN(__glewUniform1ui)
#define glUniform2ui GLEW_GET_FUN(__glewUniform2ui)
#define glUniform3ui GLEW_GET_FUN(__glewUniform3ui)
#define glUniform4ui GLEW_GET_FUN(__glewUniform4ui)
#define glUniform1uiv GLEW_GET_FUN(__glewUniform1uiv)
#define glUniform2uiv GLEW_GET_FUN(__glewUniform2uiv)
#define glUniform3uiv GLEW_GET_FUN(__glewUniform3uiv)
#define glUniform4uiv GLEW_GET_FUN(__glewUniform4uiv)
#define glClearBufferiv GLEW_GET_FUN(__glewClearBufferiv)
#define glClearBufferuiv GLEW_GET_FUN(__glewClearBufferuiv)
#define glClearBufferfv GLEW_GET_FUN(__glewClearBufferfv)
#define glClearBufferfi GLEW_GET_FUN(__glewClearBufferfi)
#define glGetStringi GLEW_GET_FUN(__glewGetStringi)
#define glCopyBufferSubData GLEW_GET_FUN(__glewCopyBufferSubData)
#define glGetUniformIndices GLEW_GET_FUN(__glewGetUniformIndices)
#define glGetActiveUniformsiv GLEW_GET_FUN(__glewGetActiveUniformsiv)
#define glGetUniformBlockIndex GLEW_GET_FUN(__glewGetUniformBlockIndex)
#define glGetActiveUniformBlockiv GLEW_GET_FUN(__glewGetActiveUniformBlockiv)
#define glGetActiveUniformBlockName GLEW_GET_FUN(__glewGetActiveUniformBlockName)
#define glUniformBlockBinding GLEW_GET_FUN(__glewUniformBlockBinding)
#define glDrawArraysInstanced GLEW_GET_FUN(__glewDrawArraysInstanced)
#define glDrawElementsInstanced GLEW_GET_FUN(__glewDrawElementsInstanced)
#define glFenceSync GLEW_GET_FUN(__glewFenceSync)
#define glIsSync GLEW_GET_FUN(__glewIsSync)
#define glDeleteSync GLEW_GET_FUN(__glewDeleteSync)
#define glClientWaitSync GLEW_GET_FUN(__glewClientWaitSync)
#define glWaitSync GLEW_GET_FUN(__glewWaitSync)
#define glGetInteger64v GLEW_GET_FUN(__glewGetInteger64v)
#define glGetSynciv GLEW_GET_FUN(__glewGetSynciv)
#define glGetInteger64i_v GLEW_GET_FUN(__glewGetInteger64i_v)
#define glGetBufferParameteri64v GLEW_GET_FUN(__glewGetBufferParameteri64v)
#define glGenSamplers GLEW_GET_FUN(__glewGenSamplers)
#define glDeleteSamplers GLEW_GET_FUN(__glewDeleteSamplers)
#define glIsSampler GLEW_GET_FUN(__glewIsSampler)
#define glBindSampler GLEW_GET_FUN(__glewBindSampler)
#define glSamplerParameteri GLEW_GET_FUN(__glewSamplerParameteri)
#define glSamplerParameteriv GLEW_GET_FUN(__glewSamplerParameteriv)
#define glSamplerParameterf GLEW_GET_FUN(__glewSamplerParameterf)
#define glSamplerParameterfv GLEW_GET_FUN(__glewSamplerParameterfv)
#define glGetSamplerParameteriv GLEW_GET_FUN(__glewGetSamplerParameteriv)
#define glGetSamplerParameterfv GLEW_GET_FUN(__glewGetSamplerParameterfv)
#define glVertexAttribDivisor GLEW_GET_FUN(__glewVertexAttribDivisor)
#define glBindTransformFeedback GLEW_GET_FUN(__glewBindTransformFeedback)
#define glDeleteTransformFeedbacks GLEW_GET_FUN(__glewDeleteTransformFeedbacks)
#define glGenTransformFeedbacks GLEW_GET_FUN(__glewGenTransformFeedbacks)
#define glIsTransformFeedback GLEW_GET_FUN(__glewIsTransformFeedback)
#define glPauseTransformFeedback GLEW_GET_FUN(__glewPauseTransformFeedback)
#define glResumeTransformFeedback GLEW_GET_FUN(__glewResumeTransformFeedback)
#define glGetProgramBinary GLEW_GET_FUN(__glewGetProgramBinary)
#define glProgramBinary GLEW_GET_FUN(__glewProgramBinary)
#define glProgramParameteri GLEW_GET_FUN(__glewProgramParameteri)
#define glInvalidateFramebuffer GLEW_GET_FUN(__glewInvalidateFramebuffer)
#define glInvalidateSubFramebuffer GLEW_GET_FUN(__glewInvalidateSubFramebuffer)
#define glTexStorage2D GLEW_GET_FUN(__glewTexStorage2D)
#define glTexStorage3D GLEW_GET_FUN(__glewTexStorage3D)
#define glGetInternalformativ GLEW_GET_FUN(__glewGetInternalformativ)
GL_APICALL void GL_APIENTRY glReadBuffer(GLenum mode);
#endif
#endif /* GL_ES_VERSION_3_0 */

#ifndef GL_ES_VERSION_3_1
#define GL_ES_VERSION_3_1 1
#define GL_COMPUTE_SHADER                 0x91B9
#define GL_MAX_COMPUTE_UNIFORM_BLOCKS     0x91BB
#define GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS 0x91BC
#define GL_MAX_COMPUTE_IMAGE_UNIFORMS     0x91BD
#define GL_MAX_COMPUTE_SHARED_MEMORY_SIZE 0x8262
#define GL_MAX_COMPUTE_UNIFORM_COMPONENTS 0x8263
#define GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS 0x8264
#define GL_MAX_COMPUTE_ATOMIC_COUNTERS    0x8265
#define GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS 0x8266
#define GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS 0x90EB
#define GL_MAX_COMPUTE_WORK_GROUP_COUNT   0x91BE
#define GL_MAX_COMPUTE_WORK_GROUP_SIZE    0x91BF
#define GL_COMPUTE_WORK_GROUP_SIZE        0x8267
#define GL_DISPATCH_INDIRECT_BUFFER       0x90EE
#define GL_DISPATCH_INDIRECT_BUFFER_BINDING 0x90EF
#define GL_COMPUTE_SHADER_BIT             0x00000020
#define GL_DRAW_INDIRECT_BUFFER           0x8F3F
#define GL_DRAW_INDIRECT_BUFFER_BINDING   0x8F43
#define GL_MAX_UNIFORM_LOCATIONS          0x826E
#define GL_FRAMEBUFFER_DEFAULT_WIDTH      0x9310
#define GL_FRAMEBUFFER_DEFAULT_HEIGHT     0x9311
#define GL_FRAMEBUFFER_DEFAULT_SAMPLES    0x9313
#define GL_FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS 0x9314
#define GL_MAX_FRAMEBUFFER_WIDTH          0x9315
#define GL_MAX_FRAMEBUFFER_HEIGHT         0x9316
#define GL_MAX_FRAMEBUFFER_SAMPLES        0x9318
#define GL_UNIFORM                        0x92E1
#define GL_UNIFORM_BLOCK                  0x92E2
#define GL_PROGRAM_INPUT                  0x92E3
#define GL_PROGRAM_OUTPUT                 0x92E4
#define GL_BUFFER_VARIABLE                0x92E5
#define GL_SHADER_STORAGE_BLOCK           0x92E6
#define GL_ATOMIC_COUNTER_BUFFER          0x92C0
#define GL_TRANSFORM_FEEDBACK_VARYING     0x92F4
#define GL_ACTIVE_RESOURCES               0x92F5
#define GL_MAX_NAME_LENGTH                0x92F6
#define GL_MAX_NUM_ACTIVE_VARIABLES       0x92F7
#define GL_NAME_LENGTH                    0x92F9
#define GL_TYPE                           0x92FA
#define GL_ARRAY_SIZE                     0x92FB
#define GL_OFFSET                         0x92FC
#define GL_BLOCK_INDEX                    0x92FD
#define GL_ARRAY_STRIDE                   0x92FE
#define GL_MATRIX_STRIDE                  0x92FF
#define GL_IS_ROW_MAJOR                   0x9300
#define GL_ATOMIC_COUNTER_BUFFER_INDEX    0x9301
#define GL_BUFFER_BINDING                 0x9302
#define GL_BUFFER_DATA_SIZE               0x9303
#define GL_NUM_ACTIVE_VARIABLES           0x9304
#define GL_ACTIVE_VARIABLES               0x9305
#define GL_REFERENCED_BY_VERTEX_SHADER    0x9306
#define GL_REFERENCED_BY_FRAGMENT_SHADER  0x930A
#define GL_REFERENCED_BY_COMPUTE_SHADER   0x930B
#define GL_TOP_LEVEL_ARRAY_SIZE           0x930C
#define GL_TOP_LEVEL_ARRAY_STRIDE         0x930D
#define GL_LOCATION                       0x930E
#define GL_VERTEX_SHADER_BIT              0x00000001
#define GL_FRAGMENT_SHADER_BIT            0x00000002
#define GL_ALL_SHADER_BITS                0xFFFFFFFF
#define GL_PROGRAM_SEPARABLE              0x8258
#define GL_ACTIVE_PROGRAM                 0x8259
#define GL_PROGRAM_PIPELINE_BINDING       0x825A
#define GL_ATOMIC_COUNTER_BUFFER_BINDING  0x92C1
#define GL_ATOMIC_COUNTER_BUFFER_START    0x92C2
#define GL_ATOMIC_COUNTER_BUFFER_SIZE     0x92C3
#define GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS 0x92CC
#define GL_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS 0x92D0
#define GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS 0x92D1
#define GL_MAX_VERTEX_ATOMIC_COUNTERS     0x92D2
#define GL_MAX_FRAGMENT_ATOMIC_COUNTERS   0x92D6
#define GL_MAX_COMBINED_ATOMIC_COUNTERS   0x92D7
#define GL_MAX_ATOMIC_COUNTER_BUFFER_SIZE 0x92D8
#define GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS 0x92DC
#define GL_ACTIVE_ATOMIC_COUNTER_BUFFERS  0x92D9
#define GL_UNSIGNED_INT_ATOMIC_COUNTER    0x92DB
#define GL_MAX_IMAGE_UNITS                0x8F38
#define GL_MAX_VERTEX_IMAGE_UNIFORMS      0x90CA
#define GL_MAX_FRAGMENT_IMAGE_UNIFORMS    0x90CE
#define GL_MAX_COMBINED_IMAGE_UNIFORMS    0x90CF
#define GL_IMAGE_BINDING_NAME             0x8F3A
#define GL_IMAGE_BINDING_LEVEL            0x8F3B
#define GL_IMAGE_BINDING_LAYERED          0x8F3C
#define GL_IMAGE_BINDING_LAYER            0x8F3D
#define GL_IMAGE_BINDING_ACCESS           0x8F3E
#define GL_IMAGE_BINDING_FORMAT           0x906E
#define GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT 0x00000001
#define GL_ELEMENT_ARRAY_BARRIER_BIT      0x00000002
#define GL_UNIFORM_BARRIER_BIT            0x00000004
#define GL_TEXTURE_FETCH_BARRIER_BIT      0x00000008
#define GL_SHADER_IMAGE_ACCESS_BARRIER_BIT 0x00000020
#define GL_COMMAND_BARRIER_BIT            0x00000040
#define GL_PIXEL_BUFFER_BARRIER_BIT       0x00000080
#define GL_TEXTURE_UPDATE_BARRIER_BIT     0x00000100
#define GL_BUFFER_UPDATE_BARRIER_BIT      0x00000200
#define GL_FRAMEBUFFER_BARRIER_BIT        0x00000400
#define GL_TRANSFORM_FEEDBACK_BARRIER_BIT 0x00000800
#define GL_ATOMIC_COUNTER_BARRIER_BIT     0x00001000
#define GL_ALL_BARRIER_BITS               0xFFFFFFFF
#define GL_IMAGE_2D                       0x904D
#define GL_IMAGE_3D                       0x904E
#define GL_IMAGE_CUBE                     0x9050
#define GL_IMAGE_2D_ARRAY                 0x9053
#define GL_INT_IMAGE_2D                   0x9058
#define GL_INT_IMAGE_3D                   0x9059
#define GL_INT_IMAGE_CUBE                 0x905B
#define GL_INT_IMAGE_2D_ARRAY             0x905E
#define GL_UNSIGNED_INT_IMAGE_2D          0x9063
#define GL_UNSIGNED_INT_IMAGE_3D          0x9064
#define GL_UNSIGNED_INT_IMAGE_CUBE        0x9066
#define GL_UNSIGNED_INT_IMAGE_2D_ARRAY    0x9069
#define GL_IMAGE_FORMAT_COMPATIBILITY_TYPE 0x90C7
#define GL_IMAGE_FORMAT_COMPATIBILITY_BY_SIZE 0x90C8
#define GL_IMAGE_FORMAT_COMPATIBILITY_BY_CLASS 0x90C9
#define GL_READ_ONLY                      0x88B8
#define GL_WRITE_ONLY                     0x88B9
#define GL_READ_WRITE                     0x88BA
#define GL_SHADER_STORAGE_BUFFER          0x90D2
#define GL_SHADER_STORAGE_BUFFER_BINDING  0x90D3
#define GL_SHADER_STORAGE_BUFFER_START    0x90D4
#define GL_SHADER_STORAGE_BUFFER_SIZE     0x90D5
#define GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS 0x90D6
#define GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS 0x90DA
#define GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS 0x90DB
#define GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS 0x90DC
#define GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS 0x90DD
#define GL_MAX_SHADER_STORAGE_BLOCK_SIZE  0x90DE
#define GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT 0x90DF
#define GL_SHADER_STORAGE_BARRIER_BIT     0x00002000
#define GL_MAX_COMBINED_SHADER_OUTPUT_RESOURCES 0x8F39
#define GL_DEPTH_STENCIL_TEXTURE_MODE     0x90EA
#define GL_STENCIL_INDEX                  0x1901
#define GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET 0x8E5E
#define GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET 0x8E5F
#define GL_SAMPLE_POSITION                0x8E50
#define GL_SAMPLE_MASK                    0x8E51
#define GL_SAMPLE_MASK_VALUE              0x8E52
#define GL_TEXTURE_2D_MULTISAMPLE         0x9100
#define GL_MAX_SAMPLE_MASK_WORDS          0x8E59
#define GL_MAX_COLOR_TEXTURE_SAMPLES      0x910E
#define GL_MAX_DEPTH_TEXTURE_SAMPLES      0x910F
#define GL_MAX_INTEGER_SAMPLES            0x9110
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE 0x9104
#define GL_TEXTURE_SAMPLES                0x9106
#define GL_TEXTURE_FIXED_SAMPLE_LOCATIONS 0x9107
#define GL_TEXTURE_WIDTH                  0x1000
#define GL_TEXTURE_HEIGHT                 0x1001
#define GL_TEXTURE_DEPTH                  0x8071
#define GL_TEXTURE_INTERNAL_FORMAT        0x1003
#define GL_TEXTURE_RED_SIZE               0x805C
#define GL_TEXTURE_GREEN_SIZE             0x805D
#define GL_TEXTURE_BLUE_SIZE              0x805E
#define GL_TEXTURE_ALPHA_SIZE             0x805F
#define GL_TEXTURE_DEPTH_SIZE             0x884A
#define GL_TEXTURE_STENCIL_SIZE           0x88F1
#define GL_TEXTURE_SHARED_SIZE            0x8C3F
#define GL_TEXTURE_RED_TYPE               0x8C10
#define GL_TEXTURE_GREEN_TYPE             0x8C11
#define GL_TEXTURE_BLUE_TYPE              0x8C12
#define GL_TEXTURE_ALPHA_TYPE             0x8C13
#define GL_TEXTURE_DEPTH_TYPE             0x8C16
#define GL_TEXTURE_COMPRESSED             0x86A1
#define GL_SAMPLER_2D_MULTISAMPLE         0x9108
#define GL_INT_SAMPLER_2D_MULTISAMPLE     0x9109
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE 0x910A
#define GL_VERTEX_ATTRIB_BINDING          0x82D4
#define GL_VERTEX_ATTRIB_RELATIVE_OFFSET  0x82D5
#define GL_VERTEX_BINDING_DIVISOR         0x82D6
#define GL_VERTEX_BINDING_OFFSET          0x82D7
#define GL_VERTEX_BINDING_STRIDE          0x82D8
#define GL_VERTEX_BINDING_BUFFER          0x8F4F
#define GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET 0x82D9
#define GL_MAX_VERTEX_ATTRIB_BINDINGS     0x82DA
#define GL_MAX_VERTEX_ATTRIB_STRIDE       0x82E5
typedef void (GL_APIENTRYP PFNGLDISPATCHCOMPUTEPROC) (GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
typedef void (GL_APIENTRYP PFNGLDISPATCHCOMPUTEINDIRECTPROC) (GLintptr indirect);
typedef void (GL_APIENTRYP PFNGLDRAWARRAYSINDIRECTPROC) (GLenum mode, const void *indirect);
typedef void (GL_APIENTRYP PFNGLDRAWELEMENTSINDIRECTPROC) (GLenum mode, GLenum type, const void *indirect);
typedef void (GL_APIENTRYP PFNGLFRAMEBUFFERPARAMETERIPROC) (GLenum target, GLenum pname, GLint param);
typedef void (GL_APIENTRYP PFNGLGETFRAMEBUFFERPARAMETERIVPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (GL_APIENTRYP PFNGLGETPROGRAMINTERFACEIVPROC) (GLuint program, GLenum programInterface, GLenum pname, GLint *params);
typedef GLuint (GL_APIENTRYP PFNGLGETPROGRAMRESOURCEINDEXPROC) (GLuint program, GLenum programInterface, const GLchar *name);
typedef void (GL_APIENTRYP PFNGLGETPROGRAMRESOURCENAMEPROC) (GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name);
typedef void (GL_APIENTRYP PFNGLGETPROGRAMRESOURCEIVPROC) (GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props, GLsizei bufSize, GLsizei *length, GLint *params);
typedef GLint (GL_APIENTRYP PFNGLGETPROGRAMRESOURCELOCATIONPROC) (GLuint program, GLenum programInterface, const GLchar *name);
typedef void (GL_APIENTRYP PFNGLUSEPROGRAMSTAGESPROC) (GLuint pipeline, GLbitfield stages, GLuint program);
typedef void (GL_APIENTRYP PFNGLACTIVESHADERPROGRAMPROC) (GLuint pipeline, GLuint program);
typedef GLuint (GL_APIENTRYP PFNGLCREATESHADERPROGRAMVPROC) (GLenum type, GLsizei count, const GLchar *const*strings);
typedef void (GL_APIENTRYP PFNGLBINDPROGRAMPIPELINEPROC) (GLuint pipeline);
typedef void (GL_APIENTRYP PFNGLDELETEPROGRAMPIPELINESPROC) (GLsizei n, const GLuint *pipelines);
typedef void (GL_APIENTRYP PFNGLGENPROGRAMPIPELINESPROC) (GLsizei n, GLuint *pipelines);
typedef GLboolean (GL_APIENTRYP PFNGLISPROGRAMPIPELINEPROC) (GLuint pipeline);
typedef void (GL_APIENTRYP PFNGLGETPROGRAMPIPELINEIVPROC) (GLuint pipeline, GLenum pname, GLint *params);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORM1IPROC) (GLuint program, GLint location, GLint v0);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORM2IPROC) (GLuint program, GLint location, GLint v0, GLint v1);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORM3IPROC) (GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORM4IPROC) (GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORM1UIPROC) (GLuint program, GLint location, GLuint v0);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORM2UIPROC) (GLuint program, GLint location, GLuint v0, GLuint v1);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORM3UIPROC) (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORM4UIPROC) (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORM1FPROC) (GLuint program, GLint location, GLfloat v0);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORM2FPROC) (GLuint program, GLint location, GLfloat v0, GLfloat v1);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORM3FPROC) (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORM4FPROC) (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORM1IVPROC) (GLuint program, GLint location, GLsizei count, const GLint *value);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORM2IVPROC) (GLuint program, GLint location, GLsizei count, const GLint *value);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORM3IVPROC) (GLuint program, GLint location, GLsizei count, const GLint *value);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORM4IVPROC) (GLuint program, GLint location, GLsizei count, const GLint *value);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORM1UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint *value);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORM2UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint *value);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORM3UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint *value);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORM4UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint *value);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORM1FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORM2FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORM3FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORM4FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORMMATRIX2FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORMMATRIX3FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORMMATRIX4FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GL_APIENTRYP PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GL_APIENTRYP PFNGLVALIDATEPROGRAMPIPELINEPROC) (GLuint pipeline);
typedef void (GL_APIENTRYP PFNGLGETPROGRAMPIPELINEINFOLOGPROC) (GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (GL_APIENTRYP PFNGLBINDIMAGETEXTUREPROC) (GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
typedef void (GL_APIENTRYP PFNGLGETBOOLEANI_VPROC) (GLenum target, GLuint index, GLboolean *data);
typedef void (GL_APIENTRYP PFNGLMEMORYBARRIERPROC) (GLbitfield barriers);
typedef void (GL_APIENTRYP PFNGLMEMORYBARRIERBYREGIONPROC) (GLbitfield barriers);
typedef void (GL_APIENTRYP PFNGLTEXSTORAGE2DMULTISAMPLEPROC) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void (GL_APIENTRYP PFNGLGETMULTISAMPLEFVPROC) (GLenum pname, GLuint index, GLfloat *val);
typedef void (GL_APIENTRYP PFNGLSAMPLEMASKIPROC) (GLuint maskNumber, GLbitfield mask);
typedef void (GL_APIENTRYP PFNGLGETTEXLEVELPARAMETERIVPROC) (GLenum target, GLint level, GLenum pname, GLint *params);
typedef void (GL_APIENTRYP PFNGLGETTEXLEVELPARAMETERFVPROC) (GLenum target, GLint level, GLenum pname, GLfloat *params);
typedef void (GL_APIENTRYP PFNGLBINDVERTEXBUFFERPROC) (GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
typedef void (GL_APIENTRYP PFNGLVERTEXATTRIBFORMATPROC) (GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
typedef void (GL_APIENTRYP PFNGLVERTEXATTRIBIFORMATPROC) (GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
typedef void (GL_APIENTRYP PFNGLVERTEXATTRIBBINDINGPROC) (GLuint attribindex, GLuint bindingindex);
typedef void (GL_APIENTRYP PFNGLVERTEXBINDINGDIVISORPROC) (GLuint bindingindex, GLuint divisor);
#if GL_GLES_PROTOTYPES
GLEW_FUN_EXPORT PFNGLDISPATCHCOMPUTEPROC __glewDispatchCompute;
GLEW_FUN_EXPORT PFNGLDISPATCHCOMPUTEINDIRECTPROC __glewDispatchComputeIndirect;
GLEW_FUN_EXPORT PFNGLDRAWARRAYSINDIRECTPROC __glewDrawArraysIndirect;
GLEW_FUN_EXPORT PFNGLDRAWELEMENTSINDIRECTPROC __glewDrawElementsIndirect;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERPARAMETERIPROC __glewFramebufferParameteri;
GLEW_FUN_EXPORT PFNGLGETFRAMEBUFFERPARAMETERIVPROC __glewGetFramebufferParameteriv;
GLEW_FUN_EXPORT PFNGLGETPROGRAMINTERFACEIVPROC __glewGetProgramInterfaceiv;
GLEW_FUN_EXPORT PFNGLGETPROGRAMRESOURCEINDEXPROC __glewGetProgramResourceIndex;
GLEW_FUN_EXPORT PFNGLGETPROGRAMRESOURCENAMEPROC __glewGetProgramResourceName;
GLEW_FUN_EXPORT PFNGLGETPROGRAMRESOURCEIVPROC __glewGetProgramResourceiv;
GLEW_FUN_EXPORT PFNGLGETPROGRAMRESOURCELOCATIONPROC __glewGetProgramResourceLocation;
GLEW_FUN_EXPORT PFNGLUSEPROGRAMSTAGESPROC __glewUseProgramStages;
GLEW_FUN_EXPORT PFNGLACTIVESHADERPROGRAMPROC __glewActiveShaderProgram;
GLEW_FUN_EXPORT PFNGLCREATESHADERPROGRAMVPROC __glewCreateShaderProgramv;
GLEW_FUN_EXPORT PFNGLBINDPROGRAMPIPELINEPROC __glewBindProgramPipeline;
GLEW_FUN_EXPORT PFNGLDELETEPROGRAMPIPELINESPROC __glewDeleteProgramPipelines;
GLEW_FUN_EXPORT PFNGLGENPROGRAMPIPELINESPROC __glewGenProgramPipelines;
GLEW_FUN_EXPORT PFNGLISPROGRAMPIPELINEPROC __glewIsProgramPipeline;
GLEW_FUN_EXPORT PFNGLGETPROGRAMPIPELINEIVPROC __glewGetProgramPipelineiv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1IPROC __glewProgramUniform1i;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2IPROC __glewProgramUniform2i;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3IPROC __glewProgramUniform3i;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4IPROC __glewProgramUniform4i;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1UIPROC __glewProgramUniform1ui;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2UIPROC __glewProgramUniform2ui;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3UIPROC __glewProgramUniform3ui;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4UIPROC __glewProgramUniform4ui;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1FPROC __glewProgramUniform1f;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2FPROC __glewProgramUniform2f;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3FPROC __glewProgramUniform3f;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4FPROC __glewProgramUniform4f;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1IVPROC __glewProgramUniform1iv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2IVPROC __glewProgramUniform2iv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3IVPROC __glewProgramUniform3iv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4IVPROC __glewProgramUniform4iv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1UIVPROC __glewProgramUniform1uiv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2UIVPROC __glewProgramUniform2uiv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3UIVPROC __glewProgramUniform3uiv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4UIVPROC __glewProgramUniform4uiv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1FVPROC __glewProgramUniform1fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2FVPROC __glewProgramUniform2fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3FVPROC __glewProgramUniform3fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4FVPROC __glewProgramUniform4fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX2FVPROC __glewProgramUniformMatrix2fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX3FVPROC __glewProgramUniformMatrix3fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX4FVPROC __glewProgramUniformMatrix4fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC __glewProgramUniformMatrix2x3fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC __glewProgramUniformMatrix3x2fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC __glewProgramUniformMatrix2x4fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC __glewProgramUniformMatrix4x2fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC __glewProgramUniformMatrix3x4fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC __glewProgramUniformMatrix4x3fv;
GLEW_FUN_EXPORT PFNGLVALIDATEPROGRAMPIPELINEPROC __glewValidateProgramPipeline;
GLEW_FUN_EXPORT PFNGLGETPROGRAMPIPELINEINFOLOGPROC __glewGetProgramPipelineInfoLog;
GLEW_FUN_EXPORT PFNGLBINDIMAGETEXTUREPROC __glewBindImageTexture;
GLEW_FUN_EXPORT PFNGLGETBOOLEANI_VPROC __glewGetBooleani_v;
GLEW_FUN_EXPORT PFNGLMEMORYBARRIERPROC __glewMemoryBarrier;
GLEW_FUN_EXPORT PFNGLMEMORYBARRIERBYREGIONPROC __glewMemoryBarrierByRegion;
GLEW_FUN_EXPORT PFNGLTEXSTORAGE2DMULTISAMPLEPROC __glewTexStorage2DMultisample;
GLEW_FUN_EXPORT PFNGLGETMULTISAMPLEFVPROC __glewGetMultisamplefv;
GLEW_FUN_EXPORT PFNGLSAMPLEMASKIPROC __glewSampleMaski;
GLEW_FUN_EXPORT PFNGLBINDVERTEXBUFFERPROC __glewBindVertexBuffer;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBFORMATPROC __glewVertexAttribFormat;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBIFORMATPROC __glewVertexAttribIFormat;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBBINDINGPROC __glewVertexAttribBinding;
GLEW_FUN_EXPORT PFNGLVERTEXBINDINGDIVISORPROC __glewVertexBindingDivisor;
#define glDispatchCompute GLEW_GET_FUN(__glewDispatchCompute)
#define glDispatchComputeIndirect GLEW_GET_FUN(__glewDispatchComputeIndirect)
#define glDrawArraysIndirect GLEW_GET_FUN(__glewDrawArraysIndirect)
#define glDrawElementsIndirect GLEW_GET_FUN(__glewDrawElementsIndirect)
#define glFramebufferParameteri GLEW_GET_FUN(__glewFramebufferParameteri)
#define glGetFramebufferParameteriv GLEW_GET_FUN(__glewGetFramebufferParameteriv)
#define glGetProgramInterfaceiv GLEW_GET_FUN(__glewGetProgramInterfaceiv)
#define glGetProgramResourceIndex GLEW_GET_FUN(__glewGetProgramResourceIndex)
#define glGetProgramResourceName GLEW_GET_FUN(__glewGetProgramResourceName)
#define glGetProgramResourceiv GLEW_GET_FUN(__glewGetProgramResourceiv)
#define glGetProgramResourceLocation GLEW_GET_FUN(__glewGetProgramResourceLocation)
#define glUseProgramStages GLEW_GET_FUN(__glewUseProgramStages)
#define glActiveShaderProgram GLEW_GET_FUN(__glewActiveShaderProgram)
#define glCreateShaderProgramv GLEW_GET_FUN(__glewCreateShaderProgramv)
#define glBindProgramPipeline GLEW_GET_FUN(__glewBindProgramPipeline)
#define glDeleteProgramPipelines GLEW_GET_FUN(__glewDeleteProgramPipelines)
#define glGenProgramPipelines GLEW_GET_FUN(__glewGenProgramPipelines)
#define glIsProgramPipeline GLEW_GET_FUN(__glewIsProgramPipeline)
#define glGetProgramPipelineiv GLEW_GET_FUN(__glewGetProgramPipelineiv)
#define glProgramUniform1i GLEW_GET_FUN(__glewProgramUniform1i)
#define glProgramUniform2i GLEW_GET_FUN(__glewProgramUniform2i)
#define glProgramUniform3i GLEW_GET_FUN(__glewProgramUniform3i)
#define glProgramUniform4i GLEW_GET_FUN(__glewProgramUniform4i)
#define glProgramUniform1ui GLEW_GET_FUN(__glewProgramUniform1ui)
#define glProgramUniform2ui GLEW_GET_FUN(__glewProgramUniform2ui)
#define glProgramUniform3ui GLEW_GET_FUN(__glewProgramUniform3ui)
#define glProgramUniform4ui GLEW_GET_FUN(__glewProgramUniform4ui)
#define glProgramUniform1f GLEW_GET_FUN(__glewProgramUniform1f)
#define glProgramUniform2f GLEW_GET_FUN(__glewProgramUniform2f)
#define glProgramUniform3f GLEW_GET_FUN(__glewProgramUniform3f)
#define glProgramUniform4f GLEW_GET_FUN(__glewProgramUniform4f)
#define glProgramUniform1iv GLEW_GET_FUN(__glewProgramUniform1iv)
#define glProgramUniform2iv GLEW_GET_FUN(__glewProgramUniform2iv)
#define glProgramUniform3iv GLEW_GET_FUN(__glewProgramUniform3iv)
#define glProgramUniform4iv GLEW_GET_FUN(__glewProgramUniform4iv)
#define glProgramUniform1uiv GLEW_GET_FUN(__glewProgramUniform1uiv)
#define glProgramUniform2uiv GLEW_GET_FUN(__glewProgramUniform2uiv)
#define glProgramUniform3uiv GLEW_GET_FUN(__glewProgramUniform3uiv)
#define glProgramUniform4uiv GLEW_GET_FUN(__glewProgramUniform4uiv)
#define glProgramUniform1fv GLEW_GET_FUN(__glewProgramUniform1fv)
#define glProgramUniform2fv GLEW_GET_FUN(__glewProgramUniform2fv)
#define glProgramUniform3fv GLEW_GET_FUN(__glewProgramUniform3fv)
#define glProgramUniform4fv GLEW_GET_FUN(__glewProgramUniform4fv)
#define glProgramUniformMatrix2fv GLEW_GET_FUN(__glewProgramUniformMatrix2fv)
#define glProgramUniformMatrix3fv GLEW_GET_FUN(__glewProgramUniformMatrix3fv)
#define glProgramUniformMatrix4fv GLEW_GET_FUN(__glewProgramUniformMatrix4fv)
#define glProgramUniformMatrix2x3fv GLEW_GET_FUN(__glewProgramUniformMatrix2x3fv)
#define glProgramUniformMatrix3x2fv GLEW_GET_FUN(__glewProgramUniformMatrix3x2fv)
#define glProgramUniformMatrix2x4fv GLEW_GET_FUN(__glewProgramUniformMatrix2x4fv)
#define glProgramUniformMatrix4x2fv GLEW_GET_FUN(__glewProgramUniformMatrix4x2fv)
#define glProgramUniformMatrix3x4fv GLEW_GET_FUN(__glewProgramUniformMatrix3x4fv)
#define glProgramUniformMatrix4x3fv GLEW_GET_FUN(__glewProgramUniformMatrix4x3fv)
#define glValidateProgramPipeline GLEW_GET_FUN(__glewValidateProgramPipeline)
#define glGetProgramPipelineInfoLog GLEW_GET_FUN(__glewGetProgramPipelineInfoLog)
#define glBindImageTexture GLEW_GET_FUN(__glewBindImageTexture)
#define glGetBooleani_v GLEW_GET_FUN(__glewGetBooleani_v)
#define glMemoryBarrier GLEW_GET_FUN(__glewMemoryBarrier)
#define glMemoryBarrierByRegion GLEW_GET_FUN(__glewMemoryBarrierByRegion)
#define glTexStorage2DMultisample GLEW_GET_FUN(__glewTexStorage2DMultisample)
#define glGetMultisamplefv GLEW_GET_FUN(__glewGetMultisamplefv)
#define glSampleMaski GLEW_GET_FUN(__glewSampleMaski)
#define glBindVertexBuffer GLEW_GET_FUN(__glewBindVertexBuffer)
#define glVertexAttribFormat GLEW_GET_FUN(__glewVertexAttribFormat)
#define glVertexAttribIFormat GLEW_GET_FUN(__glewVertexAttribIFormat)
#define glVertexAttribBinding GLEW_GET_FUN(__glewVertexAttribBinding)
#define glVertexBindingDivisor GLEW_GET_FUN(__glewVertexBindingDivisor)
GL_APICALL void GL_APIENTRY glGetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint *params);
GL_APICALL void GL_APIENTRY glGetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat *params);
#endif
#endif /* GL_ES_VERSION_3_1 */

#ifdef __cplusplus
}
#endif

#endif

#endif
