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

	class CROSS_EXPORT CRendererCommandBuffer
	{
		friend class CRendererCommandPool;


	protected:
		CRendererCommandBuffer(CRendererDevice *pDevice, VkCommandBuffer vkCommandBuffer, VkCommandBufferLevel level);
		virtual ~CRendererCommandBuffer(void);


	public:
		VkCommandBuffer GetCommandBuffer(void) const;
		VkCommandBufferLevel GetCommandBufferLevel(void) const;

	public:
		VkResult BeginPrimary(VkCommandBufferUsageFlags flags) const;
		VkResult BeginSecondary(VkCommandBufferUsageFlags flags, VkFramebuffer vkFramebuffer, VkRenderPass vkRenderPass, uint32_t indexSubpass, VkBool32 occlusionQueryEnable = VK_FALSE, VkQueryControlFlags queryFlags = 0, VkQueryPipelineStatisticFlags pipelineStatistics = 0) const;
		VkResult End(void) const;


	protected:
		VkCommandBuffer m_vkCommandBuffer;
		VkCommandBufferLevel m_vkCommandBufferLevel;

	protected:
		CRendererDevice *m_pDevice;

	protected:
		CRendererCommandBuffer *pFreeNext;
		CRendererCommandBuffer *pActiveNext;
		CRendererCommandBuffer *pActivePrev;
	};

}
