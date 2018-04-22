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

	typedef enum {
		DRAWABLE_TYPE_PARTICAL = 0,
		DRAWABLE_TYPE_SKIN_MESH,
		DRAWABLE_TYPE_STATIC_MESH
	} DRAWABLE_TYPE;

	class CROSS_EXPORT CDrawable
	{
		friend class CDrawableManager;


	protected:
		CDrawable(void);
		virtual ~CDrawable(void);


	public:
		virtual DRAWABLE_TYPE GetType(void) const = 0;

	public:
		virtual uint32_t GetIndexCount(void) const = 0;
		virtual uint32_t GetFirstIndex(void) const = 0;
		virtual uint32_t GetVertexOffset(void) const = 0;

		virtual const CGfxIndexBufferPtr& GetIndexBuffer(void) const = 0;
		virtual const CGfxVertexBufferPtr& GetVertexBuffer(void) const = 0;
		virtual const glm::aabb& GetAABB(void) const = 0;

	public:
		void SetMaterial(const CResMaterialPtr &ptrMaterial);
		const CGfxMaterialPtr& GetMaterial(void) const;
		const CGfxDescriptorSetPtr& GetDescriptorSet(uint32_t dwPassName) const;

	public:
		void SetTransform(const glm::mat4 &mtxModelToWorld);
		const glm::mat4& GetTransform(void) const;

	public:
		void SetUniform(uint32_t dwPassName, uint32_t dwName, uint32_t binding, float value);
		void SetUniform(uint32_t dwPassName, uint32_t dwName, uint32_t binding, glm::vec4 &value);

	protected:
		void UpdateDescriptorSet(uint32_t dwPassName, const CGfxPipelineGraphicsPtr &ptrPipeline);


	protected:
		glm::mat4 m_mtxModelToWorld;

	protected:
		CResMaterialPtr m_ptrMaterial;
		std::map<uint32_t, CGfxDescriptorSetPtr> m_ptrDescriptorSets;
		std::map<uint32_t, std::map<uint32_t, CGfxUniformBufferPtr>> m_ptrUniformBuffers;
	};

}
