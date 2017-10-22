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

	class CROSS_EXPORT CResMaterialPass
	{
		friend class CResMaterial;


	protected:
		CResMaterialPass(void);
		virtual ~CResMaterialPass(void);


	public:
		const CResGraphicsPtr& GetPipeline(void) const;
		const CGfxDescriptorSetPtr& GetDescriptorSet(void) const;

	public:
		BOOL Load(TiXmlNode *pPassNode, BOOL bSync);
		BOOL PostLoad(void);

	protected:
		BOOL LoadPipeline(TiXmlNode *pPassNode, BOOL bSync);
		BOOL LoadTextures(TiXmlNode *pPassNode, BOOL bSync);
		BOOL LoadUniforms(TiXmlNode *pPassNode, BOOL bSync);

	protected:
		BOOL IsReady(void) const;


	protected:
		CResGraphicsPtr m_ptrPipeline;
		CGfxDescriptorSetPtr m_ptrDescriptorSet;

		std::map<uint32_t, CResTexturePtr> m_textures;
		std::map<uint32_t, CGfxUniformBufferPtr> m_uniformFloats;
		std::map<uint32_t, CGfxUniformBufferPtr> m_uniformVectors;

	public:
		CResMaterialPass *pNext;
	};

}
