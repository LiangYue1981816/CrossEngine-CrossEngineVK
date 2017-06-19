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

	class CROSS_EXPORT CGfxResourceManager
	{
		friend class CGfxResource;
		friend class CGfxIndexBuffer;
		friend class CGfxVertexBuffer;
		friend class CGfxUniformBuffer;


	protected:
		CGfxResourceManager(void)
		{
			pthread_mutex_init(&m_mutex, NULL);
		}
		virtual ~CGfxResourceManager(void)
		{
			pthread_mutex_destroy(&m_mutex);
		}


	protected:
		virtual int Create(void)
		{
			return NO_ERROR;
		}

		virtual void Destroy(void)
		{
			for (const auto &itResource : m_pResources) {
				if (CGfxResource *pResource = itResource.second) {
					pResource->Destroy();
					SAFE_DELETE(pResource);
				}
			}

			m_pResources.clear();
		}

		virtual void FreeResource(CGfxResource *pResource)
		{
			if (pResource) {
				{
					mutex_autolock mutex(m_mutex);
					const auto &itResource = m_pResources.find(pResource);
					if (itResource != m_pResources.end()) m_pResources.erase(itResource);
				}

				pResource->Destroy();
				SAFE_DELETE(pResource);
			}
		}

		virtual void DumpLog(const char *szTitle) const
		{
			uint32_t count = 0;

			LOGI("%s\n", szTitle);
			{
				for (const auto &itResource : m_pResources) {
					if (const CGfxResource *pResource = itResource.second) {
						pResource->DumpLog();
						count++;
					}
				}
			}
			LOGI("*** %d objects found\n", count);
			LOGI("\n");
		}


	protected:
		pthread_mutex_t m_mutex;
		std::map<CGfxResource*, CGfxResource*> m_pResources;
	};

}
