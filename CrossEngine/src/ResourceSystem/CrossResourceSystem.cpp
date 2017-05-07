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
/*
#include "_CrossEngine.h"


namespace CrossEngine {

	CResourceSystem::CResourceSystem(VOID)
	{
		m_pResourceManager[RESOURCE_MANAGER_SOUND] = SAFE_NEW(MEMTYPE_HEAP) CSoundManager;
		m_pResourceManager[RESOURCE_MANAGER_SHADER] = SAFE_NEW(MEMTYPE_HEAP) CShaderManager;
		m_pResourceManager[RESOURCE_MANAGER_TEXTURE] = SAFE_NEW(MEMTYPE_HEAP) CTextureManager;
		m_pResourceManager[RESOURCE_MANAGER_MATERIAL] = SAFE_NEW(MEMTYPE_HEAP) CMaterialManager;
		m_pResourceManager[RESOURCE_MANAGER_SKELETON] = SAFE_NEW(MEMTYPE_HEAP) CSkeletonManager;
		m_pResourceManager[RESOURCE_MANAGER_MESH] = SAFE_NEW(MEMTYPE_HEAP) CMeshManager;
		m_pResourceManager[RESOURCE_MANAGER_EFFECT] = SAFE_NEW(MEMTYPE_HEAP) CEffectManager;
		m_pResourceManager[RESOURCE_MANAGER_HALO] = NULL; // SAFE_NEW(MEMTYPE_HEAP) CHaloManager;
	}

	CResourceSystem::~CResourceSystem(VOID)
	{
		for (INT indexManager = RESOURCE_MANAGER_COUNT - 1; indexManager >= 0; indexManager--) {
			SAFE_DELETE(m_pResourceManager[indexManager]);
		}
	}

	//
	// 获得资源管理器
	//
	CResourceManager* CResourceSystem::GetResourceManager(RESOURCE_MANAGER manager) const
	{
		ASSERT(m_pResourceManager[manager]);
		return m_pResourceManager[manager];
	}

	//
	// 创建资源
	//
	CResource* CResourceSystem::CreateResource(RESOURCE_MANAGER manager)
	{
		ASSERT(m_pResourceManager[manager]);
		return m_pResourceManager[manager]->CreateResource();
	}

	//
	// 销毁资源
	//
	VOID CResourceSystem::DestroyResource(CResource *pResource)
	{
		ASSERT(pResource);
		pResource->GetResourceManager()->DestroyResource(pResource);
	}

	//
	// 加载资源包资源
	//
	BOOL CResourceSystem::LoadResourceZip(const CHAR *szZipName)
	{
		ASSERT(szZipName);

		for (INT indexManager = 0; indexManager < RESOURCE_MANAGER_COUNT; indexManager++) {
			if (m_pResourceManager[indexManager]) {
				m_pResourceManager[indexManager]->LoadFromZip(szZipName);
			}
		}

		return TRUE;
	}

	//
	// 加载文件夹资源
	//
	BOOL CResourceSystem::LoadResourcePath(const CHAR *szPathName)
	{
		ASSERT(szPathName);

		for (INT indexManager = 0; indexManager < RESOURCE_MANAGER_COUNT; indexManager++) {
			if (m_pResourceManager[indexManager]) {
				m_pResourceManager[indexManager]->LoadFromPath(szPathName);
			}
		}

		return TRUE;
	}

	//
	// 重新加载所有资源
	//
	BOOL CResourceSystem::ReloadAll(VOID)
	{
		for (INT indexManager = 0; indexManager < RESOURCE_MANAGER_COUNT; indexManager++) {
			if (m_pResourceManager[indexManager]) {
				m_pResourceManager[indexManager]->Reload();
			}
		}

		return TRUE;
	}

	//
	// 重新加载图形资源
	//
	BOOL CResourceSystem::ReloadGfx(VOID)
	{
		ASSERT(m_pResourceManager[RESOURCE_MANAGER_MESH]);
		ASSERT(m_pResourceManager[RESOURCE_MANAGER_SHADER]);
		ASSERT(m_pResourceManager[RESOURCE_MANAGER_TEXTURE]);

		m_pResourceManager[RESOURCE_MANAGER_MESH]->Reload();
		m_pResourceManager[RESOURCE_MANAGER_SHADER]->Reload();
		m_pResourceManager[RESOURCE_MANAGER_TEXTURE]->Reload();

		return TRUE;
	}

	//
	// 垃圾回收
	// 注意: 资源之间的依赖关系决定垃圾回收顺序
	//
	VOID CResourceSystem::GarbageCollection(VOID)
	{
		for (INT indexManager = RESOURCE_MANAGER_COUNT - 1; indexManager >= 0; indexManager--) {
			if (m_pResourceManager[indexManager]) {
				m_pResourceManager[indexManager]->GarbageCollection();
			}
		}
	}

}
*/