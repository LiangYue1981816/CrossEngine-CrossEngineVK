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

	CVulkanMemory::CVulkanMemory(CVulkanMemoryAllocator *pAllocator, CVulkanDevice *pDevice, VkDeviceMemory vkMemory, VkFlags flags, VkDeviceSize size, VkDeviceSize offset)
		: m_pDevice(pDevice)

		, m_vkMemory(vkMemory)
		, m_pAllocator(pAllocator)

		, m_flags(flags)
		, m_size(size)
		, m_offset(offset)

		, bInUse(FALSE)

		, pNext(NULL)
		, pPrev(NULL)
		, pFreeNext(NULL)
		, pFreePrev(NULL)
	{

	}

	CVulkanMemory::~CVulkanMemory(void)
	{

	}

	CVulkanMemoryAllocator* CVulkanMemory::GetAllocator(void) const
	{
		return m_pAllocator;
	}

	BOOL CVulkanMemory::IsHostVisible(void) const
	{
		return m_flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ? TRUE : FALSE;
	}

	BOOL CVulkanMemory::IsHostCoherent(void) const
	{
		return m_flags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT ? TRUE : FALSE;
	}

	BOOL CVulkanMemory::IsHostCached(void) const
	{
		return m_flags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT ? TRUE : FALSE;
	}

	VkDeviceSize CVulkanMemory::GetSize(void) const
	{
		return m_size;
	}

	VkResult CVulkanMemory::BindImage(VkImage vkImage) const
	{
#ifdef _DEBUG
		VkMemoryRequirements requirements;
		vkGetImageMemoryRequirements(m_pDevice->GetDevice(), vkImage, &requirements);
		ASSERT(ALIGN_1KBYTE(requirements.alignment) == ALIGN_1KBYTE(1));
		ASSERT(ALIGN_1KBYTE(requirements.size) == m_size);
		ASSERT(ALIGN_1KBYTE(m_offset) == m_offset);
#endif
		return vkBindImageMemory(m_pDevice->GetDevice(), vkImage, m_vkMemory, m_offset);
	}

	VkResult CVulkanMemory::BindBuffer(VkBuffer vkBuffer) const
	{
#ifdef _DEBUG
		VkMemoryRequirements requirements;
		vkGetBufferMemoryRequirements(m_pDevice->GetDevice(), vkBuffer, &requirements);
		ASSERT(ALIGN_1KBYTE(requirements.alignment) == ALIGN_1KBYTE(1));
		ASSERT(ALIGN_1KBYTE(requirements.size) == m_size);
		ASSERT(ALIGN_1KBYTE(m_offset) == m_offset);
#endif
		return vkBindBufferMemory(m_pDevice->GetDevice(), vkBuffer, m_vkMemory, m_offset);
	}

	VkResult CVulkanMemory::BeginMapMemory(VkDeviceSize offset, VkDeviceSize size, void **ppAddress) const
	{
		return vkMapMemory(m_pDevice->GetDevice(), m_vkMemory, m_offset + offset, size, 0, ppAddress);
	}

	VkResult CVulkanMemory::EndMapMemory(void) const
	{
		vkUnmapMemory(m_pDevice->GetDevice(), m_vkMemory);
		return VK_SUCCESS;
	}

	VkResult CVulkanMemory::FlushMappedMemory(VkDeviceSize offset, VkDeviceSize size) const
	{
		if (IsHostCoherent()) {
			return VK_SUCCESS;
		}

		VkMappedMemoryRange range = {};
		range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		range.pNext = NULL;
		range.memory = m_vkMemory;
		range.offset = m_offset + offset;
		range.size = size;
		return vkFlushMappedMemoryRanges(m_pDevice->GetDevice(), 1, &range);
	}

}
