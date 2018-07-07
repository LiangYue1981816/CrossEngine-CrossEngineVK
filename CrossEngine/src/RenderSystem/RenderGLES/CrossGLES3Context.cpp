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
		GLuint flag;
	} DepthMaskParam;

	typedef struct ColorMaskParam {
		GLuint red;
		GLuint green;
		GLuint blue;
		GLuint alpha;
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

	typedef struct TextureParam {
		GLenum target;
		GLuint texture;
	} TextureParam;

	typedef struct FrameBufferAttachmentParam {
		GLenum target;
		GLuint texture;
		GLint level;
	} FrameBufferAttachmentParam;

	typedef struct FrameBufferParam {
		GLuint framebuffer;
		std::map<GLenum, FrameBufferAttachmentParam> attachments;
	} FrameBufferParam;


	static std::map<GLenum, GLboolean> Caps;
	static std::map<GLenum, GLuint> Buffers;
	static std::map<GLenum, BufferBaseParam> BufferBases;
	static std::map<GLenum, BufferRangeParam> BufferRanges;
	static std::map<GLenum, FrameBufferParam> FrameBuffers;
	static std::map<GLenum, StencilFuncParam> StencilFuncs;
	static std::map<GLenum, StencilOpParam> StencilOps;
	static std::map<GLenum, StencilMaskParam> StencilMasks;
	static std::map<GLuint, GLuint> Samplers;
	static std::map<GLuint, TextureParam> Textures;

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
		FrameBuffers.clear();
		StencilFuncs.clear();
		StencilOps.clear();
		StencilMasks.clear();
		Samplers.clear();
		Textures.clear();

		Scissor.x = GL_INVALID_VALUE;
		Scissor.y = GL_INVALID_VALUE;
		Scissor.width = GL_INVALID_VALUE;
		Scissor.height = GL_INVALID_VALUE;
		Viewport.x = GL_INVALID_VALUE;
		Viewport.y = GL_INVALID_VALUE;
		Viewport.width = GL_INVALID_VALUE;
		Viewport.height = GL_INVALID_VALUE;
		CullFace.mode = GL_INVALID_ENUM;
		FrontFace.mode = GL_INVALID_ENUM;
		LineWidth.width = GL_INVALID_VALUE;
		PolygonOffset.factor = GL_INVALID_VALUE;
		PolygonOffset.units = GL_INVALID_VALUE;
		SampleMaski.maskNumber = GL_INVALID_VALUE;
		SampleMaski.mask = GL_INVALID_VALUE;
		DepthRangef.n = GL_INVALID_VALUE;
		DepthRangef.f = GL_INVALID_VALUE;
		DepthFunc.func = GL_INVALID_ENUM;
		DepthMask.flag = GL_INVALID_VALUE;
		ColorMask.red = GL_INVALID_VALUE;
		ColorMask.green = GL_INVALID_VALUE;
		ColorMask.blue = GL_INVALID_VALUE;
		ColorMask.alpha = GL_INVALID_VALUE;
		BlendFunc.srcRGB = GL_INVALID_ENUM;
		BlendFunc.dstRGB = GL_INVALID_ENUM;
		BlendFunc.srcAlpha = GL_INVALID_ENUM;
		BlendFunc.dstAlpha = GL_INVALID_ENUM;
		BlendEquation.modeRGB = GL_INVALID_ENUM;
		BlendEquation.modeAlpha = GL_INVALID_ENUM;
		BlendColor.red = GL_INVALID_VALUE;
		BlendColor.green = GL_INVALID_VALUE;
		BlendColor.blue = GL_INVALID_VALUE;
		BlendColor.alpha = GL_INVALID_VALUE;
		ProgramPipeline.pipeline = GL_INVALID_VALUE;
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
		if (Scissor.x != x || Scissor.y != y || Scissor.width != width || Scissor.height != height) {
			Scissor.x = x;
			Scissor.y = y;
			Scissor.width = width;
			Scissor.height = height;
			glScissor(x, y, width, height);
		}
	}

	void GLViewport(GLint x, GLint y, GLsizei width, GLsizei height)
	{
		if (Viewport.x != x || Viewport.y != y || Viewport.width != width || Viewport.height != height) {
			Viewport.x = x;
			Viewport.y = y;
			Viewport.width = width;
			Viewport.height = height;
			glViewport(x, y, width, height);
		}
	}

	void GLCullFace(GLenum mode)
	{
		if (CullFace.mode != mode) {
			CullFace.mode = mode;
			glCullFace(mode);
		}
	}

	void GLFrontFace(GLenum mode)
	{
		if (FrontFace.mode != mode) {
			FrontFace.mode = mode;
			glFrontFace(mode);
		}
	}

	void GLLineWidth(GLfloat width)
	{
		if (LineWidth.width != width) {
			LineWidth.width = width;
			glLineWidth(width);
		}
	}

	void GLPolygonOffset(GLfloat factor, GLfloat units)
	{
		if (PolygonOffset.factor != factor || PolygonOffset.units != units) {
			PolygonOffset.factor = factor;
			PolygonOffset.units = units;
			glPolygonOffset(factor, units);
		}
	}

	void GLSampleMaski(GLuint maskNumber, GLbitfield mask)
	{
		if (SampleMaski.maskNumber != maskNumber || SampleMaski.mask != mask) {
			SampleMaski.maskNumber = maskNumber;
			SampleMaski.mask = mask;
			glSampleMaski(maskNumber, mask);
		}
	}

	void GLDepthRangef(GLfloat n, GLfloat f)
	{
		if (DepthRangef.n != n || DepthRangef.f != f) {
			DepthRangef.n = n;
			DepthRangef.f = f;
			glDepthRangef(n, f);
		}
	}

	void GLDepthFunc(GLenum func)
	{
		if (DepthFunc.func != func) {
			DepthFunc.func = func;
			glDepthFunc(func);
		}
	}

	void GLDepthMask(GLboolean flag)
	{
		if (DepthMask.flag != flag) {
			DepthMask.flag = flag;
			glDepthMask(flag);
		}
	}

	void GLColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
	{
		if (ColorMask.red != red || ColorMask.green != green || ColorMask.blue != blue || ColorMask.alpha != alpha) {
			ColorMask.red = red;
			ColorMask.green = green;
			ColorMask.blue = blue;
			ColorMask.alpha = alpha;
			glColorMask(red, green, blue, alpha);
		}
	}

	void GLStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask)
	{
		if (StencilFuncs.find(face) == StencilFuncs.end() || StencilFuncs[face].func != func || StencilFuncs[face].ref != ref || StencilFuncs[face].mask != mask) {
			StencilFuncs[face].func = func;
			StencilFuncs[face].ref = ref;
			StencilFuncs[face].mask = mask;
			glStencilFuncSeparate(face, func, ref, mask);
		}
	}

	void GLStencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)
	{
		if (StencilOps.find(face) == StencilOps.end() || StencilOps[face].sfail != sfail || StencilOps[face].dpfail != dpfail || StencilOps[face].dppass != dppass) {
			StencilOps[face].sfail = sfail;
			StencilOps[face].dpfail = dpfail;
			StencilOps[face].dppass = dppass;
			glStencilOpSeparate(face, sfail, dpfail, dppass);
		}
	}

	void GLStencilMaskSeparate(GLenum face, GLuint mask)
	{
		if (StencilMasks.find(face) == StencilMasks.end() || StencilMasks[face].mask != mask) {
			StencilMasks[face].mask = mask;
			glStencilMaskSeparate(face, mask);
		}
	}

	void GLBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)
	{
		if (BlendFunc.srcRGB != srcRGB || BlendFunc.dstRGB != dstRGB || BlendFunc.srcAlpha != srcAlpha || BlendFunc.dstAlpha != dstAlpha) {
			BlendFunc.srcRGB = srcRGB;
			BlendFunc.dstRGB = dstRGB;
			BlendFunc.srcAlpha = srcAlpha;
			BlendFunc.dstAlpha = dstAlpha;
			glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
		}
	}

	void GLBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha)
	{
		if (BlendEquation.modeRGB != modeRGB || BlendEquation.modeAlpha != modeAlpha) {
			BlendEquation.modeRGB = modeRGB;
			BlendEquation.modeAlpha = modeAlpha;
			glBlendEquationSeparate(modeRGB, modeAlpha);
		}
	}

	void GLBlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
	{
		if (BlendColor.red != red || BlendColor.green != green || BlendColor.blue != blue || BlendColor.alpha != alpha) {
			BlendColor.red = red;
			BlendColor.green = green;
			BlendColor.blue = blue;
			BlendColor.alpha = alpha;
			glBlendColor(red, green, blue, alpha);
		}
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

	void GLBindProgramPipeline(GLuint pipeline)
	{
		if (ProgramPipeline.pipeline != pipeline) {
			ProgramPipeline.pipeline  = pipeline;
			glBindProgramPipeline(pipeline);
		}
	}

	void GLBindSampler(GLuint unit, GLuint sampler)
	{
		if (Samplers.find(unit) == Samplers.end() || Samplers[unit] != sampler) {
			Samplers[unit] = sampler;
			glBindSampler(unit, sampler);
		}
	}

	void GLBindTexture(GLuint unit, GLenum target, GLuint texture)
	{
		if (Textures.find(unit) == Textures.end() || Textures[unit].target != target || Textures[unit].texture != texture) {
			Textures[unit].target = target;
			Textures[unit].texture = texture;
			glBindTexture(target, texture);
		}
	}

	void GLBindFramebuffer(GLenum target, GLuint framebuffer)
	{
		switch (target) {
		case GL_FRAMEBUFFER:
		case GL_DRAW_FRAMEBUFFER:
		case GL_READ_FRAMEBUFFER:
			if (FrameBuffers.find(target) == FrameBuffers.end() || FrameBuffers[target].framebuffer != framebuffer) {
				FrameBuffers[target].framebuffer = framebuffer;
				glBindFramebuffer(target, framebuffer);
			}
			break;
		}
	}

	void GLBindFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
	{
		if (FrameBuffers.find(target) != FrameBuffers.end()) {
			if (FrameBuffers[target].attachments.find(attachment) == FrameBuffers[target].attachments.end() || FrameBuffers[target].attachments[attachment].target != textarget || FrameBuffers[target].attachments[attachment].texture != texture || FrameBuffers[target].attachments[attachment].level != level) {
				FrameBuffers[target].attachments[attachment].target = textarget;
				FrameBuffers[target].attachments[attachment].texture = texture;
				FrameBuffers[target].attachments[attachment].level = level;
				glFramebufferTexture2D(target, attachment, textarget, texture, level);
			}
		}
	}

}
