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

#pragma once
#include "CrossEngine.h"


namespace CrossEngine {

	extern void GLResetContext(void);

	extern void GLEnable(GLenum cap);
	extern void GLDisable(GLenum cap);

	extern void GLScissor(GLint x, GLint y, GLsizei width, GLsizei height);
	extern void GLViewport(GLint x, GLint y, GLsizei width, GLsizei height);
	extern void GLCullFace(GLenum mode);
	extern void GLFrontFace(GLenum mode);
	extern void GLLineWidth(GLfloat width);
	extern void GLPolygonOffset(GLfloat factor, GLfloat units);
	extern void GLSampleMaski(GLuint maskNumber, GLbitfield mask);
	extern void GLDepthRangef(GLfloat n, GLfloat f);
	extern void GLDepthFunc(GLenum func);
	extern void GLDepthMask(GLboolean flag);
	extern void GLColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
	extern void GLStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask);
	extern void GLStencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
	extern void GLStencilMaskSeparate(GLenum face, GLuint mask);
	extern void GLBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
	extern void GLBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha);
	extern void GLBlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

	extern void GLBindBuffer(GLenum target, GLuint buffer);
	extern void GLBindBufferBase(GLenum target, GLuint index, GLuint buffer);
	extern void GLBindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
	extern void GLBindFramebuffer(GLenum target, GLuint framebuffer);
	extern void GLBindProgramPipeline(GLuint pipeline);
	extern void GLBindSampler(GLuint unit, GLuint sampler);
	extern void GLBindTexture(GLuint unit, GLenum target, GLuint texture);

}
