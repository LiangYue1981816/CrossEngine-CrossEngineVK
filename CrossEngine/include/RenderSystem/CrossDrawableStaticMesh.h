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

	class CROSS_EXPORT CDrawableStaticMesh : public CDrawable
	{
	protected:
		CDrawableStaticMesh(void);
		virtual ~CDrawableStaticMesh(void);


	public:
		virtual DRAWABLE_TYPE GetType(void) const;

	public:
		virtual uint32_t GetIndexCount(void) const;
		virtual uint32_t GetFirstIndex(void) const;
		virtual uint32_t GetVertexOffset(void) const;

	public:
		virtual const CGfxMaterialPtr& GetMaterial(void) const;
		virtual const CGfxIndexBufferPtr& GetIndexBuffer(void) const;
		virtual const CGfxVertexBufferPtr& GetVertexBuffer(void) const;
		virtual const CGfxUniformBufferPtr& GetUniformBuffer(void) const;
		virtual const CGfxDescriptorSetPtr& GetDescriptorSet(void) const;
		virtual const glm::mat4& GetModelToWorldMatrix(void) const;

	public:
		virtual const glm::aabb& GetAABB(void) const;


	protected:
		glm::mat4 m_mtxModelToWorld;

		CResMeshPtr m_ptrMesh;
		CResMaterialPtr m_ptrMaterial;

		CGfxUniformBufferPtr m_ptrUniformBuffer;
		CGfxDescriptorSetPtr m_ptrDescriptorSet;
	};

}
