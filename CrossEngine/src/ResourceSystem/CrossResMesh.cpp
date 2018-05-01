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

	CResMesh::CResMesh(CResourceManager *pResourceManager)
		: CResource(pResourceManager)
		, m_format(0)

		, m_pIndexBuffer(NULL)
		, m_indexBufferSize(0)
		, m_indexBufferOffset(0)

		, m_pVertexBuffer(NULL)
		, m_vertexBufferSize(0)
		, m_vertexBufferOffset(0)
	{

	}

	CResMesh::~CResMesh(void)
	{

	}

	RESOURCE_TYPE CResMesh::GetType(void) const
	{
		return RESOURCE_TYPE::RESOURCE_TYPE_MESH;
	}

	uint32_t CResMesh::GetFormat(void) const
	{
		return m_format;
	}

	uint32_t CResMesh::GetIndexCount(void) const
	{
		return m_indexBufferSize / sizeof(unsigned short);
	}

	uint32_t CResMesh::GetVertexCount(void) const
	{
		return m_vertexBufferSize / GfxDevice()->GetVertexStride(m_format);
	}

	const glm::aabb& CResMesh::GetAABB(void) const
	{
		return m_aabb;
	}

	const CGfxIndexBufferPtr& CResMesh::GetIndexBuffer(void) const
	{
		return m_ptrIndexBuffer;
	}

	const CGfxVertexBufferPtr& CResMesh::GetVertexBuffer(void) const
	{
		return m_ptrVertexBuffer;
	}

	BOOL CResMesh::IsValid(void) const
	{
		if (IsLoaded() == FALSE) {
			return FALSE;
		}

		if (m_ptrIndexBuffer.IsNull() || m_ptrIndexBuffer->GetHandle() == NULL) {
			return FALSE;
		}

		if (m_ptrVertexBuffer.IsNull() || m_ptrVertexBuffer->GetHandle() == NULL) {
			return FALSE;
		}

		return TRUE;
	}

	BOOL CResMesh::InternalLoad(BOOL bSyncPostLoad)
	{
		return LoadMesh(&m_stream);
	}

	BOOL CResMesh::InternalPostLoad(void)
	{
		m_ptrIndexBuffer = GfxDevice()->NewIndexBuffer();
		m_ptrVertexBuffer = GfxDevice()->NewVertexBuffer();

		if (m_ptrIndexBuffer->Create(m_indexBufferSize, m_pIndexBuffer, FALSE) == FALSE) {
			return FALSE;
		}

		if (m_ptrVertexBuffer->Create(m_vertexBufferSize, m_pVertexBuffer, FALSE, m_format, VERTEX_BUFFER_BINDING) == FALSE) {
			return FALSE;
		}

		return TRUE;
	}

	void CResMesh::InternalLoadFail(void)
	{
		m_ptrIndexBuffer.Release();
		m_ptrVertexBuffer.Release();
		CResource::InternalLoadFail();
	}

	void CResMesh::InternalLoadSuccess(void)
	{
		CResource::InternalLoadSuccess();
	}

	BOOL CResMesh::LoadMesh(CStream *pStream)
	{
		if (LoadHeader(pStream) == FALSE) return FALSE;
		if (LoadFormat(pStream) == FALSE) return FALSE;
		if (LoadBounds(pStream) == FALSE) return FALSE;
		if (LoadIndexBuffer(pStream) == FALSE) return FALSE;
		if (LoadVertexBuffer(pStream) == FALSE) return FALSE;

		return TRUE;
	}

	BOOL CResMesh::LoadHeader(CStream *pStream)
	{
		pStream->Read(&m_indexBufferSize, sizeof(m_indexBufferSize), 1);
		pStream->Read(&m_indexBufferOffset, sizeof(m_indexBufferOffset), 1);

		pStream->Read(&m_vertexBufferSize, sizeof(m_vertexBufferSize), 1);
		pStream->Read(&m_vertexBufferOffset, sizeof(m_vertexBufferOffset), 1);

		return TRUE;
	}

	BOOL CResMesh::LoadFormat(CStream *pStream)
	{
		pStream->Read(&m_format, sizeof(m_format), 1);
		return TRUE;
	}

	BOOL CResMesh::LoadBounds(CStream *pStream)
	{
		pStream->Read(&m_aabb.minVertex.x, sizeof(m_aabb.minVertex.x), 1);
		pStream->Read(&m_aabb.minVertex.y, sizeof(m_aabb.minVertex.y), 1);
		pStream->Read(&m_aabb.minVertex.z, sizeof(m_aabb.minVertex.z), 1);
		pStream->Read(&m_aabb.maxVertex.x, sizeof(m_aabb.maxVertex.x), 1);
		pStream->Read(&m_aabb.maxVertex.y, sizeof(m_aabb.maxVertex.y), 1);
		pStream->Read(&m_aabb.maxVertex.z, sizeof(m_aabb.maxVertex.z), 1);
		m_aabb.normalize();

		return TRUE;
	}

	BOOL CResMesh::LoadIndexBuffer(CStream *pStream)
	{
		pStream->Seek(m_indexBufferOffset, SEEK_MODE_SET);
		m_pIndexBuffer = pStream->GetCurrentAddress();
		return TRUE;
	}

	BOOL CResMesh::LoadVertexBuffer(CStream *pStream)
	{
		pStream->Seek(m_vertexBufferOffset, SEEK_MODE_SET);
		m_pVertexBuffer = pStream->GetCurrentAddress();
		return TRUE;
	}

}
