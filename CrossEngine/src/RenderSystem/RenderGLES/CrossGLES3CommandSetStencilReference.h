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

	class CROSS_EXPORT CGLES3CommandSetStencilReference : public CGLES3CommandBase
	{
		friend class CGLES3CommandBuffer;


	protected:
		CGLES3CommandSetStencilReference(VkStencilFaceFlags faceMask, VkCompareOp frontCompareOp, VkCompareOp backCompareOp, uint32_t reference, uint32_t compareMask)
			: m_faceMask(faceMask)
			, m_frontCompareOp(frontCompareOp)
			, m_backCompareOp(backCompareOp)
			, m_reference(reference)
			, m_compareMask(compareMask)
		{

		}
		virtual ~CGLES3CommandSetStencilReference(void)
		{

		}


	protected:
		virtual void Execute(void)
		{
			switch (CGLES3Helper::glTranslateStencilFace(m_faceMask)) {
			case GL_FRONT:
				glStencilFuncSeparate(
					GL_FRONT,
					CGLES3Helper::glTranslateCompareOp(m_frontCompareOp),
					m_reference,
					m_compareMask);
				break;

			case GL_BACK:
				glStencilFuncSeparate(
					GL_BACK,
					CGLES3Helper::glTranslateCompareOp(m_backCompareOp),
					m_reference,
					m_compareMask);
				break;

			case GL_FRONT_AND_BACK:
				glStencilFuncSeparate(
					GL_FRONT,
					CGLES3Helper::glTranslateCompareOp(m_frontCompareOp),
					m_reference,
					m_compareMask);
				glStencilFuncSeparate(
					GL_BACK,
					CGLES3Helper::glTranslateCompareOp(m_backCompareOp),
					m_reference,
					m_compareMask);
				break;
			}
		}


	protected:
		VkStencilFaceFlags m_faceMask;
		VkCompareOp m_frontCompareOp;
		VkCompareOp m_backCompareOp;
		uint32_t m_reference;
		uint32_t m_compareMask;
	};

}
