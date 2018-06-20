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

	class CROSS_EXPORT CGLES3DescriptorSetLayout : public CGfxDescriptorSetLayout
	{
		friend class CGLES3Device;


	protected:
		CGLES3DescriptorSetLayout(CGLES3Device *pDevice, uint32_t set);
		virtual ~CGLES3DescriptorSetLayout(void);

		
	protected:
		void Release(void);


	public:
		CGLES3Device* GetDevice(void) const;
		HANDLE GetHandle(void) const;

	public:
		BOOL Create(void);
		void Destroy(void);

	public:
		BOOL SetUniformBinding(const char *szName, uint32_t binding, VkShaderStageFlags flags);
		BOOL SetSampledImageBinding(const char *szName, uint32_t binding, VkShaderStageFlags flags);
		BOOL SetInputAttachmentBinding(const char *szName, uint32_t binding, VkShaderStageFlags flags);

	public:
		const uint32_t GetSet(void) const;
		const uint32_t GetBinding(uint32_t dwName) const;
		const uint32_t* GetTypesUsedCount(void) const;

	public:
		BOOL IsCompatible(const CGfxPipelineGraphicsPtr &ptrPipelineGraphics) const;

	public:
		const std::map<uint32_t, uint32_t>& GetUniformBlockBindings(GLuint program);
		const std::map<uint32_t, uint32_t>& GetSampledImageBindings(GLuint program);
		const std::map<uint32_t, uint32_t>& GetInputAttachmentBindings(GLuint program);


	protected:
		uint32_t m_numTypesUsedCount[VK_DESCRIPTOR_TYPE_RANGE_SIZE];

	protected:
		uint32_t m_set;
		std::map<uint32_t, uint32_t> m_nameBindings;
		std::map<std::string, uint32_t> m_uniformBlockNameBindings;
		std::map<std::string, uint32_t> m_sampledImageNameBindings;
		std::map<std::string, uint32_t> m_inputAttachmentNameBindings;
		std::map<uint32_t, std::map<uint32_t, uint32_t>> m_uniformBlockBindings;
		std::map<uint32_t, std::map<uint32_t, uint32_t>> m_sampledImageBindings;
		std::map<uint32_t, std::map<uint32_t, uint32_t>> m_inputAttachmentBindings;

	protected:
		CGLES3Device *m_pDevice;
	};

}
