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

	CGLES3RenderTexture::CGLES3RenderTexture(CGfxResourceManager *pResourceManager)
		: CGfxTexture(pResourceManager)
	{

	}

	CGLES3RenderTexture::~CGLES3RenderTexture(void)
	{

	}

	HANDLE CGLES3RenderTexture::GetHandle(void) const
	{
		return (HANDLE)m_texture;
	}

	BOOL CGLES3RenderTexture::CreateColorTarget(VkFormat format, uint32_t width, uint32_t height, VkSampleCountFlagBits samples)
	{
		return TRUE;
	}

	BOOL CGLES3RenderTexture::CreateDepthStencilTarget(VkFormat format, uint32_t width, uint32_t height, VkSampleCountFlagBits samples)
	{
		return TRUE;
	}

	void CGLES3RenderTexture::Destroy(void)
	{
		CGLES3Image::Destroy();
	}

	uint32_t CGLES3RenderTexture::GetWidth(void) const
	{
		return m_width;
	}

	uint32_t CGLES3RenderTexture::GetHeight(void) const
	{
		return m_height;
	}

	uint32_t CGLES3RenderTexture::GetDepth(void) const
	{
		return m_depth;
	}

	uint32_t CGLES3RenderTexture::GetSamples(void) const
	{
		return m_samples;
	}

	size_t CGLES3RenderTexture::GetMemorySize(void) const
	{
		return m_size;
	}

	void CGLES3RenderTexture::DumpLog(void) const
	{
		if (m_texture) {
			LOGI("\t\tRenderTexture 0x%x: sampler = 0x%x size = %d type = %s format = %s width = %d height = %d depth = %d mips = %d arrays = %d samples = %s min filter = %s mag filter = %s address mode = %s\n",
				m_texture,
				m_sampler,
				m_size,
				CGLES3Helper::glEnumToString(m_target),
				CGLES3Helper::glEnumToString(m_format),
				m_width, m_height, m_depth, m_mipLevels, m_arrayLayers,
				m_samples,
				CGLES3Helper::glEnumToString(m_minFilter),
				CGLES3Helper::glEnumToString(m_magFilter),
				CGLES3Helper::glEnumToString(m_addressMode));
		}
	}

}
