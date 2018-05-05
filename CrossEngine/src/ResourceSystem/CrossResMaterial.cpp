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

	CResMaterial::CResMaterial(CResourceManager *pResourceManager)
		: CResource(pResourceManager)
	{

	}

	CResMaterial::~CResMaterial(void)
	{

	}

	RESOURCE_TYPE CResMaterial::GetType(void) const
	{
		return RESOURCE_TYPE::RESOURCE_TYPE_MATERIAL;
	}

	const CGfxMaterialPtr& CResMaterial::GetMaterial(void) const
	{
		return m_ptrMaterial;
	}

	BOOL CResMaterial::IsValid(void) const
	{
		if (IsLoaded() == FALSE) {
			return FALSE;
		}

		if (m_ptrMaterial.IsNull() || m_ptrMaterial->GetHandle() == NULL) {
			return FALSE;
		}

		return TRUE;
	}

	BOOL CResMaterial::InternalLoad(BOOL bSyncPostLoad)
	{
		TiXmlDocument xmlDoc;

		if (xmlDoc.LoadFile((char *)m_stream.GetAddress(), m_stream.GetFullSize())) {
			if (TiXmlNode *pMaterialNode = xmlDoc.FirstChild("Material")) {
				return LoadMaterial(pMaterialNode, bSyncPostLoad);
			}
		}

		return FALSE;
	}

	BOOL CResMaterial::InternalPostLoad(void)
	{
		m_ptrMaterial = GfxDevice()->NewMaterial();

		for (const auto &itGraphics : m_ptrGraphices) {
			if (itGraphics.second->IsValid() == FALSE) {
				return FALSE;
			}

			uint32_t dwPassName = itGraphics.first;
			CGfxMaterialPassPtr &ptrMaterialPass = m_ptrMaterial->AddPass(dwPassName);
			ptrMaterialPass->SetPipeline(itGraphics.second->GetPipeline());
			ptrMaterialPass->SetRenderPass(itGraphics.second->GetRenderPass(), itGraphics.second->GetSubpassIndex());
		}

		for (const auto &itTextures : m_ptrTextures) {
			for (const auto &itTexture : itTextures.second) {
				if (itTexture.second->IsValid() == FALSE) {
					return FALSE;
				}

				uint32_t dwPassName = itTextures.first;
				uint32_t dwTextureName = itTexture.first;
				m_ptrMaterial->GetPass(dwPassName)->SetTexture(dwTextureName, itTexture.second->GetTexture());
			}
		}

		for (const auto &itUniformFloats : m_uniformFloats) {
			for (const auto &itUniformFloat : itUniformFloats.second) {
				uint32_t dwPassName = itUniformFloats.first;
				uint32_t dwUniformName = itUniformFloat.first;
				uint32_t binding = m_ptrMaterial->GetPass(dwPassName)->GetPipeline()->GetBinding(DESCRIPTORSET_PASS, dwUniformName);
				if (binding == -1) return FALSE;

				CGfxUniformBufferPtr ptrUniformBuffer = GfxDevice()->NewUniformBuffer();
				ptrUniformBuffer->Create(sizeof(itUniformFloat.second), &itUniformFloat.second, FALSE);
				ptrUniformBuffer->SetDescriptorBufferInfo(DESCRIPTORSET_PASS, binding, 0, sizeof(itUniformFloat.second));

				m_ptrMaterial->GetPass(dwPassName)->SetUniformBuffer(dwUniformName, ptrUniformBuffer);
			}
		}

		for (const auto &itUniformVectors : m_uniformVectors) {
			for (const auto &itUniformVector : itUniformVectors.second) {
				uint32_t dwPassName = itUniformVectors.first;
				uint32_t dwUniformName = itUniformVector.first;
				uint32_t binding = m_ptrMaterial->GetPass(dwPassName)->GetPipeline()->GetBinding(DESCRIPTORSET_PASS, dwUniformName);
				if (binding == -1) return FALSE;

				CGfxUniformBufferPtr ptrUniformBuffer = GfxDevice()->NewUniformBuffer();
				ptrUniformBuffer->Create(sizeof(itUniformVector.second), &itUniformVector.second, FALSE);
				ptrUniformBuffer->SetDescriptorBufferInfo(DESCRIPTORSET_PASS, binding, 0, sizeof(itUniformVector.second));

				m_ptrMaterial->GetPass(dwPassName)->SetUniformBuffer(dwUniformName, ptrUniformBuffer);
			}
		}

		for (auto &itPass : m_ptrMaterial->GetPasses()) {
			itPass.second->UpdateDescriptorSet(thread_id());
		}

		return TRUE;
	}

	void CResMaterial::InternalLoadFail(void)
	{
		m_ptrGraphices.clear();
		m_ptrTextures.clear();
		m_uniformFloats.clear();
		m_uniformVectors.clear();
		m_ptrMaterial.Release();
		CResource::InternalLoadFail();
	}

	void CResMaterial::InternalLoadSuccess(void)
	{
		CResource::InternalLoadSuccess();
	}

	BOOL CResMaterial::LoadMaterial(TiXmlNode *pMaterialNode, BOOL bSyncPostLoad)
	{
		if (TiXmlNode *pPassNode = pMaterialNode->FirstChild("Pass")) {
			do {
				if (LoadPassPipeline(pPassNode, bSyncPostLoad) == FALSE) return FALSE;
				if (LoadPassTextures(pPassNode, bSyncPostLoad) == FALSE) return FALSE;
				if (LoadPassUniforms(pPassNode) == FALSE) return FALSE;
			} while (pPassNode = pMaterialNode->IterateChildren("Pass", pPassNode));

			return TRUE;
		}

		return FALSE;
	}

	BOOL CResMaterial::LoadPassPipeline(TiXmlNode *pPassNode, BOOL bSyncPostLoad)
	{
		const uint32_t dwPassName = HashValue(pPassNode->ToElement()->AttributeString("name"));
		const uint32_t dwName = HashValue(pPassNode->ToElement()->AttributeString("graphics"));

		m_ptrGraphices[dwPassName] = GraphicsManager()->LoadResource(dwName);
		return m_ptrGraphices[dwPassName].IsNull() || m_ptrGraphices[dwPassName]->IsValid() == FALSE ? FALSE : TRUE;
	}

	BOOL CResMaterial::LoadPassTextures(TiXmlNode *pPassNode, BOOL bSyncPostLoad)
	{
		const uint32_t dwPassName = HashValue(pPassNode->ToElement()->AttributeString("name"));

		if (TiXmlNode *pTextureNode = pPassNode->FirstChild("Texture2D")) {
			do {
				const uint32_t dwName = HashValue(pTextureNode->ToElement()->AttributeString("name"));
				const uint32_t dwTexName = HashValue(pTextureNode->ToElement()->AttributeString("value"));

				m_ptrTextures[dwPassName][dwName] = TextureManager()->LoadResource(dwTexName, TRUE, bSyncPostLoad);
				if (m_ptrTextures[dwPassName][dwName].IsNull()) return FALSE;
			} while (pTextureNode = pPassNode->IterateChildren("Texture2D", pTextureNode));
		}

		if (TiXmlNode *pTextureNode = pPassNode->FirstChild("TextureCube")) {
			do {
				const uint32_t dwName = HashValue(pTextureNode->ToElement()->AttributeString("name"));
				const uint32_t dwTexName = HashValue(pTextureNode->ToElement()->AttributeString("value"));

				m_ptrTextures[dwPassName][dwName] = TextureManager()->LoadResource(dwTexName, TRUE, bSyncPostLoad);
				if (m_ptrTextures[dwPassName][dwName].IsNull()) return FALSE;
			} while (pTextureNode = pPassNode->IterateChildren("TextureCube", pTextureNode));
		}

		return TRUE;
	}

	BOOL CResMaterial::LoadPassUniforms(TiXmlNode *pPassNode)
	{
		const uint32_t dwPassName = HashValue(pPassNode->ToElement()->AttributeString("name"));

		if (TiXmlNode *pFloatNode = pPassNode->FirstChild("Float")) {
			do {
				const uint32_t dwName = HashValue(pFloatNode->ToElement()->AttributeString("name"));
				const char *szValue = pFloatNode->ToElement()->AttributeString("value");

				float value;
				scanf(szValue, "%f", &value);
				m_uniformFloats[dwPassName][dwName] = value;
			} while (pFloatNode = pPassNode->IterateChildren("Float", pFloatNode));
		}

		if (TiXmlNode *pVectorNode = pPassNode->FirstChild("Vector")) {
			do {
				const uint32_t dwName = HashValue(pVectorNode->ToElement()->AttributeString("name"));
				const char *szValue = pVectorNode->ToElement()->AttributeString("value");

				glm::vec4 value;
				scanf(szValue, "%f %f %f %f", &value.x, &value.y, &value.z, &value.w);
				m_uniformVectors[dwPassName][dwName] = value;
			} while (pVectorNode = pPassNode->IterateChildren("Vector", pVectorNode));
		}

		return TRUE;
	}

}
