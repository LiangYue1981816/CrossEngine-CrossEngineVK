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

	CScreen::CScreen(void)
	{

	}

	CScreen::~CScreen(void)
	{

	}

	BOOL CScreen::Create(const CGfxRenderPassPtr &ptrRenderPass)
	{
		m_ptrDepthTexture = GfxDevice()->NewRenderTexture();

		if (m_ptrDepthTexture->CreateDepthStencilTarget(VK_FORMAT_D24_UNORM_S8_UINT, GfxSwapChain()->GetWidth(), GfxSwapChain()->GetHeight(), VK_SAMPLE_COUNT_1_BIT, VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT) == FALSE) {
			return FALSE;
		}

		for (int indexImage = 0; indexImage < CGfxSwapchain::SWAPCHAIN_IMAGE_COUNT; indexImage++) {
			m_ptrFrameBuffers[indexImage] = GfxDevice()->NewFrameBuffer(ptrRenderPass->GetAttachmentCount());

			m_ptrFrameBuffers[indexImage]->SetPresentAttachment(0, VK_FORMAT_B8G8R8A8_UNORM, GfxSwapChain()->GetWidth(), GfxSwapChain()->GetHeight(), GfxSwapChain()->GetImageHandle(indexImage));
			m_ptrFrameBuffers[indexImage]->SetDepthStencilAttachment(1, m_ptrDepthTexture);

			if (m_ptrFrameBuffers[indexImage]->Create(ptrRenderPass->GetHandle()) == FALSE) {
				return FALSE;
			}
		}

		return TRUE;
	}

	void CScreen::Destroy(void)
	{
		m_ptrDepthTexture.Release();

		for (int index = 0; index < CGfxSwapchain::SWAPCHAIN_IMAGE_COUNT; index++) {
			m_ptrFrameBuffers[index].Release();
		}
	}

	const CGfxFrameBufferPtr& CScreen::GetFrameBuffer(uint32_t indexImage) const
	{
		static const CGfxFrameBufferPtr ptrFrameBufferNull;
		return indexImage < CGfxSwapchain::SWAPCHAIN_IMAGE_COUNT ? m_ptrFrameBuffers[indexImage] : ptrFrameBufferNull;
	}

}
