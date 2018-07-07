/****************************************************************************
Copyright (c) 2017 LiangYue.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sub license, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "_CrossEngine.h"


namespace CrossEngine {

	typedef struct ScissorParam {
		GLint x;
		GLint y;
		GLsizei width;
		GLsizei height;
	} ScissorParam;

	typedef struct ViewportParam {
		GLint x;
		GLint y;
		GLsizei width;
		GLsizei height;
	} ViewportParam;

	typedef struct CullFaceParam {
		GLenum mode;
	} CullFaceParam;

	typedef struct FrontFaceParam {
		GLenum mode;
	} FrontFaceParam;

	typedef struct LineWidthParam {
		GLfloat width;
	} LineWidthParam;

	typedef struct PolygonOffsetParam {
		GLfloat factor;
		GLfloat units;
	} PolygonOffsetParam;

	typedef struct SampleMaskiParam {
		GLuint maskNumber;
		GLbitfield mask;
	} SampleMaskiParam;

	typedef struct DepthRangefParam {
		GLfloat n;
		GLfloat f;
	} DepthRangefParam;

	typedef struct DepthFuncParam {
		GLenum func;
	} DepthFuncParam;

	typedef struct DepthMaskParam {
		GLboolean flag;
	} DepthMaskParam;

	typedef struct ColorMaskParam {
		GLboolean red;
		GLboolean green;
		GLboolean blue;
		GLboolean alpha;
	} ColorMaskParam;

	typedef struct StencilFuncParam {
		GLenum func;
		GLint ref;
		GLuint mask;
	} StencilFuncParam;

	typedef struct StencilOpParam {
		GLenum sfail;
		GLenum dpfail;
		GLenum dppass;
	} StencilOpParam;

	typedef struct StencilMaskParam {
		GLuint mask;
	} StencilMaskParam;

	typedef struct BlendFuncParam {
		GLenum srcRGB;
		GLenum dstRGB;
		GLenum srcAlpha;
		GLenum dstAlpha;
	} BlendFuncParam;

	typedef struct BlendEquationParam {
		GLenum modeRGB;
		GLenum modeAlpha;
	} BlendEquationParam;

	typedef struct BlendColorParam {
		GLfloat red;
		GLfloat green;
		GLfloat blue;
		GLfloat alpha;
	} BlendColorParam;

	typedef struct BufferBaseParam {
		GLuint index;
		GLuint buffer;
	} BufferBaseParam;

	typedef struct BufferRangeParam {
		GLuint index;
		GLuint buffer;
		GLintptr offset;
		GLsizeiptr size;
	} BufferRangeParam;

	typedef struct ProgramPipelineParam {
		GLuint pipeline;
	} ProgramPipelineParam;


	static std::map<GLenum, GLboolean> Caps;
	static std::map<GLenum, GLuint> Buffers;
	static std::map<GLenum, BufferBaseParam> BufferBases;
	static std::map<GLenum, BufferRangeParam> BufferRanges;
	static std::map<GLenum, GLuint> Framebuffers;
	static std::map<GLenum, StencilFuncParam> StencilFuncs;
	static std::map<GLenum, StencilOpParam> StencilOps;
	static std::map<GLenum, StencilMaskParam> StencilMasks;

	static ScissorParam Scissor;
	static ViewportParam Viewport;
	static CullFaceParam CullFace;
	static FrontFaceParam FrontFace;
	static LineWidthParam LineWidth;
	static PolygonOffsetParam PolygonOffset;
	static SampleMaskiParam SampleMaski;
	static DepthRangefParam DepthRangef;
	static DepthFuncParam DepthFunc;
	static DepthMaskParam DepthMask;
	static ColorMaskParam ColorMask;
	static BlendFuncParam BlendFunc;
	static BlendEquationParam BlendEquation;
	static BlendColorParam BlendColor;
	static ProgramPipelineParam ProgramPipeline;
	

	void GLResetContext(void)
	{
		Caps.clear();
		Buffers.clear();
		BufferBases.clear();
		BufferRanges.clear();
		Framebuffers.clear();
		ProgramPipeline.pipeline = -1;
	}

	void GLEnable(GLenum cap)
	{
		switch (cap) {
		case GL_BLEND:
		case GL_CULL_FACE:
		case GL_DEPTH_TEST:
		case GL_DITHER:
		case GL_POLYGON_OFFSET_FILL:
		case GL_PRIMITIVE_RESTART_FIXED_INDEX:
		case GL_RASTERIZER_DISCARD:
		case GL_SAMPLE_ALPHA_TO_COVERAGE:
		case GL_SAMPLE_COVERAGE:
		case GL_SAMPLE_MASK:
		case GL_SCISSOR_TEST:
		case GL_STENCIL_TEST:
			if (Caps.find(cap) == Caps.end() || Caps[cap] != GL_TRUE) {
				Caps[cap] = GL_TRUE;
				glEnable(cap);
			}
			break;
		}
	}

	void GLDisable(GLenum cap)
	{
		switch (cap) {
		case GL_BLEND:
		case GL_CULL_FACE:
		case GL_DEPTH_TEST:
		case GL_DITHER:
		case GL_POLYGON_OFFSET_FILL:
		case GL_PRIMITIVE_RESTART_FIXED_INDEX:
		case GL_RASTERIZER_DISCARD:
		case GL_SAMPLE_ALPHA_TO_COVERAGE:
		case GL_SAMPLE_COVERAGE:
		case GL_SAMPLE_MASK:
		case GL_SCISSOR_TEST:
		case GL_STENCIL_TEST:
			if (Caps.find(cap) == Caps.end() || Caps[cap] != GL_FALSE) {
				Caps[cap] = GL_FALSE;
				glDisable(cap);
			}
			break;
		}
	}

	void GLScissor(GLint x, GLint y, GLsizei width, GLsizei height)
	{

	}

	void GLViewport(GLint x, GLint y, GLsizei width, GLsizei height)
	{

	}

	void GLCullFace(GLenum mode)
	{

	}

	void GLFrontFace(GLenum mode)
	{

	}

	void GLLineWidth(GLfloat width)
	{

	}

	void GLPolygonOffset(GLfloat factor, GLfloat units)
	{

	}

	void GLSampleMaski(GLuint maskNumber, GLbitfield mask)
	{

	}

	void GLDepthRangef(GLfloat n, GLfloat f)
	{

	}

	void GLDepthFunc(GLenum func)
	{

	}

	void GLDepthMask(GLboolean flag)
	{

	}

	void GLColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
	{

	}

	void GLStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask)
	{

	}

	void GLStencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)
	{

	}

	void GLStencilMaskSeparate(GLenum face, GLuint mask)
	{

	}

	void GLBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)
	{

	}

	void GLBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha)
	{

	}

	void GLBlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
	{

	}

	void GLBindBuffer(GLenum target, GLuint buffer)
	{
		switch (target) {
		case GL_ARRAY_BUFFER:
		case GL_ATOMIC_COUNTER_BUFFER:
		case GL_COPY_READ_BUFFER:
		case GL_COPY_WRITE_BUFFER:
		case GL_DRAW_INDIRECT_BUFFER:
		case GL_DISPATCH_INDIRECT_BUFFER:
		case GL_ELEMENT_ARRAY_BUFFER:
		case GL_PIXEL_PACK_BUFFER:
		case GL_PIXEL_UNPACK_BUFFER:
		case GL_SHADER_STORAGE_BUFFER:
		case GL_TRANSFORM_FEEDBACK_BUFFER:
		case GL_UNIFORM_BUFFER:
			if (Buffers.find(target) == Buffers.end() || Buffers[target] != buffer) {
				Buffers[target] = buffer;
				glBindBuffer(target, buffer);
			}
			break;
		}
	}

	void GLBindBufferBase(GLenum target, GLuint index, GLuint buffer)
	{
		switch (target) {
		case GL_ATOMIC_COUNTER_BUFFER:
		case GL_SHADER_STORAGE_BUFFER:
		case GL_TRANSFORM_FEEDBACK_BUFFER:
		case GL_UNIFORM_BUFFER:
			if (BufferBases.find(target) == BufferBases.end() || BufferBases[target].index != index || BufferBases[target].buffer != buffer) {
				BufferBases[target].index = index;
				BufferBases[target].buffer = buffer;
				glBindBufferBase(target, index, buffer);
			}
			break;
		}
	}

	void GLBindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)
	{
		switch (target) {
		case GL_ATOMIC_COUNTER_BUFFER:
		case GL_SHADER_STORAGE_BUFFER:
		case GL_TRANSFORM_FEEDBACK_BUFFER:
		case GL_UNIFORM_BUFFER:
			if (BufferRanges.find(target) == BufferRanges.end() || BufferRanges[target].index != index || BufferRanges[target].buffer != buffer || BufferRanges[target].offset != offset || BufferRanges[target].size != size) {
				BufferRanges[target].index = index;
				BufferRanges[target].buffer = buffer;
				BufferRanges[target].offset = offset;
				BufferRanges[target].size = size;
				glBindBufferRange(target, index, buffer, offset, size);
			}
			break;
		}
	}

	void GLBindFramebuffer(GLenum target, GLuint framebuffer)
	{
		switch (target) {
		case GL_FRAMEBUFFER:
		case GL_DRAW_FRAMEBUFFER:
		case GL_READ_FRAMEBUFFER:
			if (Framebuffers.find(target) == Framebuffers.end() || Framebuffers[target] != framebuffer) {
				Framebuffers[target] = framebuffer;
				glBindFramebuffer(target, framebuffer);
			}
			break;
		}
	}

	void GLBindProgramPipeline(GLuint pipeline)
	{
		if (ProgramPipeline.pipeline != pipeline) {
			ProgramPipeline.pipeline  = pipeline;
			glBindProgramPipeline(pipeline);
		}
	}

}
