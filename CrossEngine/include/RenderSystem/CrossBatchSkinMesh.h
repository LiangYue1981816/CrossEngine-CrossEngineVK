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

	class CROSS_EXPORT CBatchSkinMesh : public CBatch
	{
		friend class CCamera;
		friend class CRenderer;


	protected:
		typedef struct BatchData {
			glm::mat4 transform;
		} BatchData;


	protected:
		CBatchSkinMesh(void);
		virtual ~CBatchSkinMesh(void);


	public:
		static CGfxVertexBufferPtr& GetBatchBuffer(void);
		static void ClearBatchBuffer(void);
		static void CreateBatchBuffer(void);
		static void DestroyBatchBuffer(void);

	protected:
		virtual void UpdateBatchBuffer(void);
		virtual void BuildCommandBuffer(CGfxCommandBufferPtr &ptrCommandBuffer);


	protected:
		static std::vector<BatchData> m_batchs;
		static CGfxVertexBufferPtr m_ptrBatchBuffer;
	};

}
