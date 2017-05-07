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
#pragma once
#include "CrossEngine.h"


namespace CrossEngine {

	class CROSS_EXPORT CResourceSystem
	{
		friend class CEngine;


	public:
		typedef enum {
			RESOURCE_MANAGER_SOUND = 0,
			RESOURCE_MANAGER_SHADER,
			RESOURCE_MANAGER_TEXTURE,
			RESOURCE_MANAGER_MATERIAL,
			RESOURCE_MANAGER_SKELETON,
			RESOURCE_MANAGER_MESH,
			RESOURCE_MANAGER_EFFECT,
			RESOURCE_MANAGER_HALO,
			RESOURCE_MANAGER_COUNT
		} RESOURCE_MANAGER;


	protected:
		CResourceSystem(void);
		virtual ~CResourceSystem(void);


	public:
		CResourceManager* GetResourceManager(RESOURCE_MANAGER manager) const;

	public:
		CResource* CreateResource(RESOURCE_MANAGER manager);
		void DestroyResource(CResource *pResource);

	public:
		BOOL LoadResourceZip(const char *szZipName);
		BOOL LoadResourcePath(const char *szPathName);

		BOOL ReloadAll(void);
		BOOL ReloadGfx(void);

	public:
		void GarbageCollection(void);


	protected:
		CResourceManager *m_pResourceManager[RESOURCE_MANAGER_COUNT];
	};

}
*/