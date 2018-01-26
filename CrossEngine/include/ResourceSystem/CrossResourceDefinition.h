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

	template<class T>
	class CROSS_EXPORT CResourcePtr;
	class CROSS_EXPORT CResource;
	class CROSS_EXPORT CResRenderPass;
	class CROSS_EXPORT CResShader;
	class CROSS_EXPORT CResTexture;
	class CROSS_EXPORT CResRenderTexture;
	class CROSS_EXPORT CResCompute;
	class CROSS_EXPORT CResGraphics;
	class CROSS_EXPORT CResMaterial;

	typedef CResourcePtr<CResRenderPass> CResRenderPassPtr;
	typedef CResourcePtr<CResShader> CResShaderPtr;
	typedef CResourcePtr<CResTexture> CResTexturePtr;
	typedef CResourcePtr<CResRenderTexture> CResRenderTexturePtr;
	typedef CResourcePtr<CResCompute> CResComputePtr;
	typedef CResourcePtr<CResGraphics> CResGraphicsPtr;
	typedef CResourcePtr<CResMaterial> CResMaterialPtr;

}
