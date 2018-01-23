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
#include "CrossResourceDefinition.h"
#include "CrossResource.h"
#include "CrossResourceManager.h"
#include "CrossResRenderPass.h"
#include "CrossResShader.h"
#include "CrossResTexture.h"
#include "CrossResRenderTexture.h"
#include "CrossResCompute.h"
#include "CrossResGraphics.h"
#include "CrossResMaterial.h"
#include "CrossResRenderPassManager.h"
#include "CrossResShaderManager.h"
#include "CrossResTextureManager.h"
#include "CrossResRenderTextureManager.h"
#include "CrossResComputeManager.h"
#include "CrossResGraphicsManager.h"
#include "CrossResMaterialManager.h"


namespace CrossEngine {

	class CROSS_EXPORT CResourceSystem
	{
		friend class CEngine;
		friend class CResourceHandle;


	protected:
		CResourceSystem(void);
		virtual ~CResourceSystem(void);


	protected:
		BOOL Create(void);
		void Destroy(void);

	public:
		CResourceManager* GetResourceManager(RESOURCE_TYPE type) const;

	public:
		BOOL PreLoadResourcePath(const char *szPathName);
		BOOL PreLoadResourcePack(const char *szPackName);

	protected:
		BOOL RequestLoad(CResourceHandle *pResourceHandle);
		BOOL RequestPostLoad(CResourceHandle *pResourceHandle);

		void Load(void);
		void PostLoad(void);

	protected:
		void GarbageCollection(void);

	protected:
		static void* WorkThread(void *pParams);


	protected:
		pthread_t m_thread;
		event_t m_eventExit;

		pthread_mutex_t m_mutexPendingList;
		pthread_mutex_t m_mutexPostLoadList;
		std::list<CResourceHandle*> m_pPendingList;
		std::list<CResourceHandle*> m_pPostLoadList;

	protected:
		CResourceManager *m_pResourceManager[RESOURCE_TYPE::COUNT];
	};

}
