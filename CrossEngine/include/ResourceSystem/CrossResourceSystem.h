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

	class CROSS_EXPORT CResourceSystem
	{
		friend class CEngine;


	protected:
		CResourceSystem(void);
		virtual ~CResourceSystem(void);


	public:
		CResourceManager* GetResourceManager(RESOURCE_TYPE type) const;

	public:
		CResource* CreateResource(RESOURCE_TYPE type);
		void DestroyResource(CResource *pResource);

	public:
		BOOL PreLoadResourcePath(const char *szPathName);
		BOOL PreLoadResourcePack(const char *szPackName);

	public:
		void GarbageCollection(void);


	protected:
		pthread_t m_thread;
		pthread_mutex_t m_mutexPending;
		pthread_mutex_t m_mutexLoading;

		std::list<CResourceHandle*> m_pending;
		std::list<CResourceHandle*> m_loading;

	protected:
		CResourceManager *m_pResourceManager[RESOURCE_TYPE::COUNT];
	};

}
