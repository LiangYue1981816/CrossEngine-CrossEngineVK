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

	class CROSS_EXPORT CResGraphics : public CResource
	{
		friend class CResGraphicsManager;


	protected:
		typedef struct ShaderParam {
			uint32_t dwVertexName;
			uint32_t dwFragmentName;
		} ShaderParam;

		typedef struct RenderPassParam {
			uint32_t dwName;
			uint32_t indexSubPass;
		} RenderPassParam;

		typedef struct AssemblyParam {
			VkPrimitiveTopology topology;
		} AssemblyParam;

		typedef struct RasterizationParam {
			VkPolygonMode polygonMode;
			VkCullModeFlags cullMode;

			VkBool32 bDepthBiasEnable;
			float depthBiasConstantFactor;
			float depthBiasSlopeFactor;
		} RasterizationParam;

		typedef struct MultisampleParam {
			VkSampleCountFlagBits samples;

			VkBool32 bAlphaToCoverageEnable;
			VkBool32 bAlphaToOneEnable;
		} MultisampleParam;

		typedef struct DepthParam {
			VkBool32 bDepthTestEnable;
			VkBool32 bDepthWriteEnable;
			VkCompareOp depthCompareOp;
		} DepthParam;

		typedef struct StencilParam {
			VkBool32 bStencilTestEnable;

			VkStencilOp frontFailOp;
			VkStencilOp frontPassOp;
			VkStencilOp frontDepthFailOp;
			VkCompareOp frontCompareOp;
			uint32_t frontCompareMask;
			uint32_t frontWriteMask;
			uint32_t frontReference;

			VkStencilOp backFailOp;
			VkStencilOp backPassOp;
			VkStencilOp backDepthFailOp;
			VkCompareOp backCompareOp;
			uint32_t backCompareMask;
			uint32_t backWriteMask;
			uint32_t backReference;
		} StencilParam;

		typedef struct BlendParam {
			VkBool32 bBlendEnable;

			VkBlendFactor srcColorBlendFactor;
			VkBlendFactor dstColorBlendFactor;
			VkBlendOp colorBlendOp;

			VkBlendFactor srcAlphaBlendFactor;
			VkBlendFactor dstAlphaBlendFactor;
			VkBlendOp alphaBlendOp;

			VkColorComponentFlags colorWriteMask;
		} BlendParam;

		typedef struct PipelineGraphicsParam {
			ShaderParam shader;
			RenderPassParam renderpass;
			AssemblyParam assembly;
			RasterizationParam rasterization;
			MultisampleParam multisample;
			DepthParam depth;
			StencilParam stencil;
			std::map<uint32_t, BlendParam> blends;
		} PipelineGraphicsParam;


	protected:
		CResGraphics(CResourceManager *pResourceManager);
		virtual ~CResGraphics(void);


	public:
		const CGfxRenderPassPtr& GetRenderPass(void) const;
		const CGfxPipelineGraphicsPtr& GetPipeline(void) const;

	public:
		virtual RESOURCE_TYPE GetType(void) const;
		virtual BOOL IsValid(void) const;

	protected:
		virtual BOOL InternalLoad(BOOL bSyncPostLoad);
		virtual BOOL InternalPostLoad(void);
		virtual void InternalLoadFail(void);
		virtual void InternalLoadSuccess(void);

	protected:
		BOOL LoadShader(TiXmlNode *pShaderNode);
		BOOL LoadRenderPass(TiXmlNode *pRenderPassNode);
		BOOL LoadAssembly(TiXmlNode *pAssemblyNode);
		BOOL LoadRasterization(TiXmlNode *pRasterizationNode);
		BOOL LoadMultisample(TiXmlNode *pMultisampleNode);
		BOOL LoadDepth(TiXmlNode *pDepthNode);
		BOOL LoadStencil(TiXmlNode *pStencilNode);
		BOOL LoadBlend(TiXmlNode *pBlendNode);

		BOOL SetShaders(void);
		BOOL SetAssemblyState(void);
		BOOL SetTessellationState(void);
		BOOL SetRasterizationState(void);
		BOOL SetMultisampleState(void);
		BOOL SetDepthStencilState(void);
		BOOL SetColorBlendState(void);


	protected:
		PipelineGraphicsParam m_param;

	protected:
		CResRenderPassPtr m_ptrRenderPass;
		CGfxPipelineGraphicsPtr m_ptrPipeline;
	};

}
