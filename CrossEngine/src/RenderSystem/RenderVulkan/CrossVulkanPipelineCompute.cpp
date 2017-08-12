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

	CVulkanPipelineCompute::CVulkanPipelineCompute(CVulkanDevice *pDevice, CGfxResourceManager *pResourceManager)
		: CVulkanPipeline(pDevice)
		, CGfxPipelineCompute(pResourceManager)
	{

	}

	CVulkanPipelineCompute::~CVulkanPipelineCompute(void)
	{

	}

	CVulkanDevice* CVulkanPipelineCompute::GetDevice(void) const
	{
		return m_pDevice;
	}

	HANDLE CVulkanPipelineCompute::GetHandle(void) const
	{
		return m_vkPipeline;
	}

	BOOL CVulkanPipelineCompute::Create(const CGfxShaderPtr &ptrShader)
	{
		m_ptrShaders[VK_SHADER_STAGE_COMPUTE_BIT] = ptrShader;

		try {
			std::vector<VkDescriptorSetLayout> layouts;
			CALL_BOOL_FUNCTION_THROW(CreateDescriptorSetLayouts(layouts));

			VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
			pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipelineLayoutCreateInfo.pNext = NULL;
			pipelineLayoutCreateInfo.flags = 0;
			pipelineLayoutCreateInfo.setLayoutCount = layouts.size();
			pipelineLayoutCreateInfo.pSetLayouts = layouts.data();
			pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
			pipelineLayoutCreateInfo.pPushConstantRanges = NULL;
			CALL_VK_FUNCTION_THROW(vkCreatePipelineLayout(m_pDevice->GetDevice(), &pipelineLayoutCreateInfo, ((CVulkanInstance *)m_pDevice->GetInstance())->GetAllocator()->GetAllocationCallbacks(), &m_vkPipelineLayout));

			VkComputePipelineCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
			createInfo.pNext = NULL;
			createInfo.flags = 0;
			createInfo.stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			createInfo.stage.pNext = NULL;
			createInfo.stage.flags = 0;
			createInfo.stage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
			createInfo.stage.module = (VkShaderModule)ptrShader->GetHandle();
			createInfo.stage.pName = "main";
			createInfo.stage.pSpecializationInfo = NULL;
			createInfo.layout = m_vkPipelineLayout;
			createInfo.basePipelineHandle = VK_NULL_HANDLE;
			createInfo.basePipelineIndex = 0;
			CALL_VK_FUNCTION_THROW(vkCreateComputePipelines(m_pDevice->GetDevice(), ((CVulkanPipelineManager *)m_pResourceManager)->GetPipelineCache(), 1, &createInfo, ((CVulkanInstance *)m_pDevice->GetInstance())->GetAllocator()->GetAllocationCallbacks(), &m_vkPipeline));

			return TRUE;
		}
		catch (VkResult err) {
			CVulkanInstance::SetLastError(err);
			Destroy();

			return FALSE;
		}
	}

	void CVulkanPipelineCompute::Destroy(void)
	{
		CVulkanPipeline::Destroy();
	}

	void CVulkanPipelineCompute::DumpLog(void) const
	{
		if (m_vkPipeline) {
			LOGI("\t\tPipelineCompute 0x%x\n", m_vkPipeline);
		}
	}

}
