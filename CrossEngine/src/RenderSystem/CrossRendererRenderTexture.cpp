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

	CRendererRenderTexture::CRendererRenderTexture(CRendererDevice *pDevice, CRendererResourceManager *pManager)
		: CRendererImage(pDevice, pManager)
	{

	}

	CRendererRenderTexture::~CRendererRenderTexture(void)
	{

	}

	BOOL CRendererRenderTexture::CreateColorTarget(VkFormat format, uint32_t width, uint32_t height)
	{
		if (CRendererHelper::vkIsFormatDepthOnly(format)) {
			return FALSE;
		}

		if (CRendererHelper::vkIsFormatStencilOnly(format)) {
			return FALSE;
		}

		if (CRendererHelper::vkIsFormatDepthStencil(format)) {
			return FALSE;
		}

		return Create(VK_IMAGE_VIEW_TYPE_2D, format, VK_IMAGE_ASPECT_COLOR_BIT, width, height, 1, 1, 1, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT);
	}

	BOOL CRendererRenderTexture::CreateDepthStencilTarget(VkFormat format, uint32_t width, uint32_t height)
	{
		if (CRendererHelper::vkIsFormatDepthOnly(format)) {
			return Create(VK_IMAGE_VIEW_TYPE_2D, format, VK_IMAGE_ASPECT_DEPTH_BIT, width, height, 1, 1, 1, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT);
		}

		if (CRendererHelper::vkIsFormatStencilOnly(format)) {
			return Create(VK_IMAGE_VIEW_TYPE_2D, format, VK_IMAGE_ASPECT_STENCIL_BIT, width, height, 1, 1, 1, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT);
		}
		
		if (CRendererHelper::vkIsFormatDepthStencil(format)) {
			return Create(VK_IMAGE_VIEW_TYPE_2D, format, VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT, width, height, 1, 1, 1, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT);
		}

		return FALSE;
	}

	void CRendererRenderTexture::Destroy(void)
	{
		CRendererImage::Destroy();
	}

	void CRendererRenderTexture::DumpLog(void) const
	{
		CRendererImage::DumpLog();
	}

}
