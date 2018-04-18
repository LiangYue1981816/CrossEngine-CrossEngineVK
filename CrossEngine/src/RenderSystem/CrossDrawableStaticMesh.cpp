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

	CDrawableStaticMesh::CDrawableStaticMesh(void)
	{

	}

	CDrawableStaticMesh::~CDrawableStaticMesh(void)
	{

	}

	CDrawable::DRAWABLE_TYPE CDrawableStaticMesh::GetType(void) const
	{
		return DRAWABLE_TYPE::DRAWABLE_TYPE_STATIC_MESH;
	}

	void CDrawableStaticMesh::SetTransform(const glm::mat4 &mtxModelToWorld)
	{
		m_mtxModelToWorld = mtxModelToWorld;
	}

	void CDrawableStaticMesh::SetMesh(const CResMeshPtr &ptrMesh)
	{
		m_ptrMesh = ptrMesh;
	}

	void CDrawableStaticMesh::SetMaterial(const CResMaterialPtr &ptrMaterial)
	{
		m_ptrMaterial = ptrMaterial;
	}

	uint32_t CDrawableStaticMesh::GetIndexCount(void) const
	{
		return m_ptrMesh->GetIndexCount();
	}

	uint32_t CDrawableStaticMesh::GetFirstIndex(void) const
	{
		return 0;
	}

	uint32_t CDrawableStaticMesh::GetVertexOffset(void) const
	{
		return 0;
	}

	const CGfxMaterialPtr& CDrawableStaticMesh::GetMaterial(void) const
	{
		return m_ptrMaterial->GetMaterial();
	}

	const CGfxIndexBufferPtr& CDrawableStaticMesh::GetIndexBuffer(void) const
	{
		return m_ptrMesh->GetIndexBuffer();
	}

	const CGfxVertexBufferPtr& CDrawableStaticMesh::GetVertexBuffer(void) const
	{
		return m_ptrMesh->GetVertexBuffer();
	}

	const CGfxUniformBufferPtr& CDrawableStaticMesh::GetUniformBuffer(void) const
	{
		return m_ptrUniformBuffer;
	}

	const CGfxDescriptorSetPtr& CDrawableStaticMesh::GetDescriptorSet(void) const
	{
		return m_ptrDescriptorSet;
	}

	const glm::mat4& CDrawableStaticMesh::GetModelToWorldMatrix(void) const
	{
		return m_mtxModelToWorld;
	}

	const glm::aabb& CDrawableStaticMesh::GetAABB(void) const
	{
		return m_ptrMesh->GetAABB();
	}

}
