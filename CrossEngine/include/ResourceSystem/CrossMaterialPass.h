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

	class CROSS_EXPORT CMaterialPass
	{
	protected:
		typedef struct Texture {
			CGfxTexturePtr ptrTexture;
			CGfxDescriptorSetPtr ptrDescriptorSet;
		} Texture;

		typedef struct Uniform {
			CGfxUniformBufferPtr ptrUniform;
			CGfxDescriptorSetPtr ptrDescriptorSet;
		} Uniform;


	protected:
		CMaterialPass(void);
		virtual ~CMaterialPass(void);


	public:
		const CGfxPipelineGraphicsPtr& GetGfxPipeline(void) const;
		const std::map<uint32_t, Texture>& GetTextures(void) const;
		const std::map<uint32_t, Uniform>& GetUniforms(void) const;

	public:
		BOOL Load(TiXmlNode *pPassNode);
		BOOL PostLoad(void);

	protected:
		BOOL LoadShaderVertex(TiXmlNode *pPassNode);
		BOOL LoadShaderFragment(TiXmlNode *pPassNode);
		BOOL LoadPrimitiveTopology(TiXmlNode *pPassNode);
		BOOL LoadTessellationPatchControlPoints(TiXmlNode *pPassNode);
		BOOL LoadPolygonMode(TiXmlNode *pPassNode);
		BOOL LoadCullMode(TiXmlNode *pPassNode);
		BOOL LoadFrontFace(TiXmlNode *pPassNode);
		BOOL LoadDepthClamp(TiXmlNode *pPassNode);
		BOOL LoadDepthBias(TiXmlNode *pPassNode);
		BOOL LoadRasterizerDiscard(TiXmlNode *pPassNode);
		BOOL LoadSampleCounts(TiXmlNode *pPassNode);
		BOOL LoadSampleShading(TiXmlNode *pPassNode);
		BOOL LoadSampleMask(TiXmlNode *pPassNode);
		BOOL LoadSampleAlphaToCoverage(TiXmlNode *pPassNode);
		BOOL LoadSampleAlphaToOne(TiXmlNode *pPassNode);
		BOOL LoadDepthTest(TiXmlNode *pPassNode);
		BOOL LoadDepthBoundsTest(TiXmlNode *pPassNode);
		BOOL LoadStencilTest(TiXmlNode *pPassNode);
		BOOL LoadColorBlendLogic(TiXmlNode *pPassNode);
		BOOL LoadColorBlendConstants(TiXmlNode *pPassNode);
		BOOL LoadColorBlendAttachment(TiXmlNode *pPassNode);

	public:
		BOOL SetTexture(const char *szTextureName, uint32_t unit);
		BOOL SetUniformFloat(const char *szUniformName, float value);
		BOOL SetUniformFloat4(const char *szUniformName, float *values);
		BOOL SetUniformMatrix4(const char *szUniformName, float *values);


	protected:
		std::map<uint32_t, Texture> m_textures;
		std::map<uint32_t, Uniform> m_uniforms;
		CGfxPipelineGraphicsPtr m_ptrGfxPipeline;

	public:
		CMaterialPass *pNext;
	};

}
