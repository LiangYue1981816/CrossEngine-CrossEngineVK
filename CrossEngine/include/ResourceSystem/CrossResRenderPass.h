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

	class CROSS_EXPORT CResRenderPass : public CResource
	{
		friend class CResRenderPassManager;


	protected:
		typedef struct AttachmentPresentParam {
			uint32_t indexAttachment;
			VkFormat format;
			VkAttachmentLoadOp loadOp;
			VkAttachmentStoreOp storeOp;
			VkClearValue clearValue;
			VkSampleCountFlagBits samples;
		} AttachmentPresentParam;

		typedef struct AttachmentColorTextureParam {
			uint32_t indexAttachment;
			VkFormat format;
			VkAttachmentLoadOp loadOp;
			VkAttachmentStoreOp storeOp;
			VkClearValue clearValue;
			VkSampleCountFlagBits samples;
			VkImageLayout finalLayout;
		} AttachmentColorTextureParam;

		typedef struct AttachmentDepthStencilTextureParam {
			uint32_t indexAttachment;
			VkFormat format;
			VkAttachmentLoadOp loadOp;
			VkAttachmentStoreOp storeOp;
			VkAttachmentLoadOp stencilLoadOp;
			VkAttachmentStoreOp stencilStoreOp;
			VkClearValue clearValue;
			VkSampleCountFlagBits samples;
			VkImageLayout finalLayout;
		} AttachmentDepthStencilTextureParam;

		typedef struct SubPassParam {
			uint32_t indexSubPass;
			int inputColorReference;
			int inputDepthStencilReference;
			int outputColorReference;
			int outputDepthStencilReference;
			int preserveReference;
			int resolveColorReference;
			VkImageLayout resolveColorImageLayout;
		} SubPassParam;

		typedef struct DependencyParam {
			uint32_t indexDependency;
			uint32_t indexSrcSubpass;
			uint32_t indexDstSubpass;
			VkPipelineStageFlags srcStageMask;
			VkPipelineStageFlags dstStageMask;
			VkAccessFlags srcAccessMask;
			VkAccessFlags dstAccessMask;
			VkDependencyFlags dependencyFlags;
		} DependencyParam;

		typedef struct RenderPassParam {
			eastl::vector<AttachmentPresentParam> attachmentPresents;
			eastl::vector<AttachmentColorTextureParam> attachmentColors;
			eastl::vector<AttachmentDepthStencilTextureParam> attachmentDepthStencils;
			eastl::vector<SubPassParam> subpasses;
			eastl::vector<DependencyParam> dependencies;
		} RenderPassParam;


	protected:
		CResRenderPass(CResourceManager *pResourceManager);
		virtual ~CResRenderPass(void);


	public:
		const CGfxRenderPassPtr& GetRenderPass(void) const;

	public:
		virtual RESOURCE_TYPE GetType(void) const;
		virtual BOOL IsValid(void) const;

	protected:
		virtual BOOL InternalLoad(BOOL bSyncPostLoad);
		virtual BOOL InternalPostLoad(void);
		virtual void InternalLoadFail(void);
		virtual void InternalLoadSuccess(void);

	protected:
		BOOL LoadRenderPass(TiXmlNode *pRenderPassNode);
		BOOL LoadAttachmentPresents(TiXmlNode *pAttachmentNodes);
		BOOL LoadAttachmentColors(TiXmlNode *pAttachmentNodes);
		BOOL LoadAttachmentDepthStencils(TiXmlNode *pAttachmentNodes);
		BOOL LoadSubPasses(TiXmlNode *pSubPassNodes);
		BOOL LoadDependencies(TiXmlNode *pDependencyNodes);


	protected:
		RenderPassParam m_param;

	protected:
		CGfxRenderPassPtr m_ptrRenderPass;
	};

}
