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

	class CROSS_EXPORT CRendererCommandBufferManager
	{
		friend class CRendererDevice;
		friend class CRendererDeviceCompute;
		friend class CRendererDeviceGraphics;


	public:
		enum {
			COMMAND_POOL_RESOURCE_BASE = 0,
			COMMAND_POOL_PRIMARY_DRAW_BASE = 1000,
			COMMAND_POOL_SECONDARY_DRAW_BASE = 2000,
		};


	protected:
		CRendererCommandBufferManager(CRendererDevice *pDevice);
		virtual ~CRendererCommandBufferManager(void);


	protected:
		BOOL Create(void);
		void Destroy(void);


	public:
		CRendererCommandBuffer* AllocCommandBuffer(uint32_t pool, VkCommandBufferLevel level);
		void FreeCommandBuffer(uint32_t pool, CRendererCommandBuffer *pCommandBuffer);
		void ResetCommandPool(uint32_t pool, BOOL bReleaseResources = FALSE);

		CRendererCommandPool* GetCommandPool(uint32_t pool);


	protected:
		std::map<uint32_t, CRendererCommandPool*> m_pCommandPools;

	protected:
		CRendererDevice *m_pDevice;
	};

}
