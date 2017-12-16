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

	class CROSS_EXPORT CResMaterial : public CResource
	{
		friend class CResMaterialManager;


	protected:
		CResMaterial(CResourceManager *pResourceManager);
		virtual ~CResMaterial(void);


	public:
		const CGfxMaterialPtr& GetMaterial(void) const;

	public:
		virtual RESOURCE_TYPE GetType(void) const;
		virtual BOOL IsValid(void) const;

	protected:
		virtual void Init(void);
		virtual void Free(void);

	protected:
		virtual BOOL InternalLoad(BOOL bSync);
		virtual BOOL InternalPostLoad(void);
		virtual void InternalCleanup(void);

	protected:
		BOOL LoadPassPipeline(CGfxMaterialPassPtr &ptrPass, TiXmlNode *pPassNode, BOOL bSync);
		BOOL LoadPassTextures(CGfxMaterialPassPtr &ptrPass, TiXmlNode *pPassNode, BOOL bSync);
		BOOL LoadPassUniforms(CGfxMaterialPassPtr &ptrPass, TiXmlNode *pPassNode);


	protected:
		CGfxMaterialPtr m_ptrMaterial;
	};

}
