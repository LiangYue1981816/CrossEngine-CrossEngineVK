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

	typedef struct {
		uint32_t binding;
		VkDescriptorSetLayout vkDescriptorSetLayout;
	} VkBinding;

	class CROSS_EXPORT CRendererPipeline : public CRendererResource
	{
	protected:
		CRendererPipeline(CRendererDevice *pDevice, CRendererResourceManager *pManager);
		virtual ~CRendererPipeline(void);


	public:
		virtual void Destroy(void);
		virtual void DumpLog(void) const;

	public:
		VkPipeline GetPipeline(void) const;
		VkPipelineLayout GetPipelineLayout(void) const;
		const VkBinding* GetDescriptorSetLayout(VkDescriptorType type, const char *szName) const;

	protected:
		BOOL CreateDescriptorSetLayouts(std::vector<VkDescriptorSetLayout> &layouts);
		BOOL CreateShaderStages(std::vector<VkPipelineShaderStageCreateInfo> &shaderStages);


	protected:
		std::map<uint32_t, std::map<std::string, VkBinding>> m_bindings;
		std::vector<CRendererDescriptorSetLayout*> m_pDescriptorSetLayouts;

		std::map<VkShaderStageFlagBits, spirv::module_type> m_shaderModules;
		std::map<VkShaderStageFlagBits, VkPipelineShaderStageCreateInfo> m_shaderStages;

	protected:
		VkPipeline m_vkPipeline;
		VkPipelineLayout m_vkPipelineLayout;
	};

}
