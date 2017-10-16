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

	struct PipelineGraphicsData {
		struct Shader {
			uint32_t vertex;
			uint32_t fragment;
		};

		struct InputAssembly {
			uint32_t topology;
		};

		struct Rasterization {
			uint32_t polygonMode;
			uint32_t cullMode;
			uint32_t depthBiasEnable;
			float depthBiasConstantFactor;
			float depthBiasSlopeFactor;
		};

		struct Multisample {
			uint32_t rasterizationSamples;
			uint32_t alphaToCoverageEnable;
			uint32_t alphaToOneEnable;
		};

		struct Depth {
			uint32_t depthTestEnable;
			uint32_t depthWriteEnable;
			uint32_t depthCompareOp;
		};

		struct Stencil {
			uint32_t stencilTestEnable;
			uint32_t frontFailOp;
			uint32_t frontPassOp;
			uint32_t frontDepthFailOp;
			uint32_t frontCompareOp;
			uint32_t frontCompareMask;
			uint32_t frontWriteMask;
			uint32_t frontReference;
			uint32_t backFailOp;
			uint32_t backPassOp;
			uint32_t backDepthFailOp;
			uint32_t backCompareOp;
			uint32_t backCompareMask;
			uint32_t backWriteMask;
			uint32_t backReference;
		};

		struct ColorBlendAttachment {
			uint32_t blendEnable;
			uint32_t srcColorBlendFactor;
			uint32_t dstColorBlendFactor;
			uint32_t colorBlendOp;
			uint32_t srcAlphaBlendFactor;
			uint32_t dstAlphaBlendFactor;
			uint32_t alphaBlendOp;
			uint32_t colorWriteMask;
		};

		uint32_t mark;
		uint32_t size;

		uint32_t renderPass;
		uint32_t subPass;

		Shader shader;
		InputAssembly inputAssembly;
		Rasterization rasterization;
		Multisample multisample;
		Depth depth;
		Stencil stencil;
		std::map<uint32_t, ColorBlendAttachment> colorBlendAttachments;
	};

	class CROSS_EXPORT CPipelineGraphics : public CResource
	{
		friend class CPipelineGraphicsManager;


	protected:
		CPipelineGraphics(CResourceManager *pResourceManager);
		virtual ~CPipelineGraphics(void);


	public:
		virtual RESOURCE_TYPE GetType(void) const;

	public:
		const CGfxPipelineGraphicsPtr& GetGfxPipeline(void) const;

	protected:
		virtual BOOL Load(void);
		virtual BOOL PostLoad(BOOL bSync);

	protected:
		BOOL LoadData(void);
		BOOL LoadShaders(void);
		BOOL LoadInputAssemblyState(void);
		BOOL LoadTessellationState(void);
		BOOL LoadRasterizationState(void);
		BOOL LoadMultisampleState(void);
		BOOL LoadDepthStencilState(void);
		BOOL LoadColorBlendState(void);


	protected:
		PipelineGraphicsData m_data;

		CGfxRenderPassPtr m_ptrGfxRenderPass;
		CGfxPipelineGraphicsPtr m_ptrGfxPipeline;
	};

}
