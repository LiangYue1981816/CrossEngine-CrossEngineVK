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

	class CROSS_EXPORT CVulkanPipeline : public CVulkanResource
	{
	protected:
		CVulkanPipeline(CVulkanDevice *pDevice, CVulkanResourceManager *pResourceManager);
		virtual ~CVulkanPipeline(void);


	public:
		virtual void Destroy(void);
		virtual void DumpLog(void) const;

	protected:
		BOOL CreateDescriptorSetLayouts(std::vector<VkDescriptorSetLayout> &layouts);
		BOOL CreateShaderStages(std::vector<VkPipelineShaderStageCreateInfo> &shaderStages);

	public:
		VkPipeline GetPipeline(void) const;
		VkPipelineLayout GetPipelineLayout(void) const;
		const CVulkanDescriptorSetLayoutPtr& GetDescriptorSetLayout(uint32_t set) const;


	protected:
		VkPipeline m_vkPipeline;
		VkPipelineLayout m_vkPipelineLayout;
		std::map<uint32_t, CVulkanDescriptorSetLayoutPtr> m_ptrDescriptorSetLayouts;

	protected:
		std::map<VkShaderStageFlagBits, spirv::module_type> m_shaderModules;
		std::map<VkShaderStageFlagBits, VkPipelineShaderStageCreateInfo> m_shaderStages;
	};

	class CROSS_EXPORT CVulkanPipelinePtr : public CVulkanResourcePtr<CVulkanPipeline>
	{
	public:
		CVulkanPipelinePtr(void) : CVulkanResourcePtr<CVulkanPipeline>() {}
		CVulkanPipelinePtr(const CVulkanPipeline *p) : CVulkanResourcePtr<CVulkanPipeline>(p) {}
		CVulkanPipelinePtr(const CVulkanPipelinePtr &ptr) : CVulkanResourcePtr<CVulkanPipeline>(ptr) {}
	};

}
