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
			if (TiXmlNode *pPassNode = xmlDoc.FirstChild("Pass")) {
				do {
					if (LoadPassPipeline(pPassNode, bSyncPostLoad) == FALSE) return FALSE;
					if (LoadPassTextures(pPassNode, bSyncPostLoad) == FALSE) return FALSE;
					if (LoadPassUniforms(pPassNode) == FALSE) return FALSE;
				} while (pPassNode = pPassNode->IterateChildren("Pass", pPassNode));

				return TRUE;
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
			m_ptrMaterial->GetPass(dwPassName)->SetPipeline(itGraphics.second->GetPipeline());
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

		for (const auto &itFloats : m_floats) {
			for (const auto &itFloat : itFloats.second) {
				uint32_t dwPassName = itFloats.first;
				uint32_t dwUniformName = itFloat.first;
				uint32_t binding = m_ptrMaterial->GetPass(dwPassName)->GetPipeline()->GetBinding(DESCRIPTOR_SET_PASS, dwUniformName);
				if (binding == -1) return FALSE;

				CGfxUniformBufferPtr ptrUniform = GfxDevice()->NewUniformBuffer();
				ptrUniform->Create(sizeof(itFloat.second), &itFloat.second, FALSE);
				ptrUniform->SetDescriptorBufferInfo(DESCRIPTOR_SET_PASS, binding, 0, sizeof(itFloat.second));

				m_ptrMaterial->GetPass(dwPassName)->SetUniform(dwUniformName, ptrUniform);
			}
		}

		for (const auto &itVectors : m_vectors) {
			for (const auto &itVector : itVectors.second) {
				uint32_t dwPassName = itVectors.first;
				uint32_t dwUniformName = itVector.first;
				uint32_t binding = m_ptrMaterial->GetPass(dwPassName)->GetPipeline()->GetBinding(DESCRIPTOR_SET_PASS, dwUniformName);
				if (binding == -1) return FALSE;

				CGfxUniformBufferPtr ptrUniform = GfxDevice()->NewUniformBuffer();
				ptrUniform->Create(sizeof(itVector.second), &itVector.second, FALSE);
				ptrUniform->SetDescriptorBufferInfo(DESCRIPTOR_SET_PASS, binding, 0, sizeof(itVector.second));

				m_ptrMaterial->GetPass(dwPassName)->SetUniform(dwUniformName, ptrUniform);
			}
		}

		for (auto &itPass : m_ptrMaterial->GetPasses()) {
			itPass.second->UpdateDescriptorSet(0);
		}

		return TRUE;
	}

	void CResMaterial::InternalLoadFail(void)
	{
		m_ptrMaterial.Release();
		m_ptrGraphices.clear();
		m_ptrTextures.clear();
		m_floats.clear();
		m_vectors.clear();
		CResource::InternalLoadFail();
	}

	void CResMaterial::InternalLoadSuccess(void)
	{
		CResource::InternalLoadSuccess();
	}

	BOOL CResMaterial::LoadPassPipeline(TiXmlNode *pPassNode, BOOL bSyncPostLoad)
	{
		const char *szPassName = pPassNode->ToElement()->AttributeString("name");
		const char *szName = pPassNode->ToElement()->AttributeString("graphics");
		const uint32_t dwPassName = HashValue(szPassName);
		const uint32_t dwName = HashValue(szName);

		m_ptrGraphices[dwPassName] = GraphicsManager()->LoadResource(dwName, TRUE, bSyncPostLoad);
		return m_ptrGraphices[dwPassName].IsNull() ? FALSE : TRUE;
	}

	BOOL CResMaterial::LoadPassTextures(TiXmlNode *pPassNode, BOOL bSyncPostLoad)
	{
		const char *szPassName = pPassNode->ToElement()->AttributeString("name");
		const uint32_t dwPassName = HashValue(szPassName);

		if (TiXmlNode *pTextureNode = pPassNode->FirstChild("Texture2D")) {
			do {
				const char *szName = pTextureNode->ToElement()->AttributeString("name");
				const uint32_t dwName = HashValue(szName);

				m_ptrTextures[dwPassName][dwName] = TextureManager()->LoadResource(dwName, TRUE, bSyncPostLoad);
				if (m_ptrTextures[dwPassName][dwName].IsNull()) return FALSE;
			} while (pTextureNode = pTextureNode->IterateChildren("Texture2D", pTextureNode));
		}

		if (TiXmlNode *pTextureNode = pPassNode->FirstChild("TextureCube")) {
			do {
				const char *szName = pTextureNode->ToElement()->AttributeString("name");
				const uint32_t dwName = HashValue(szName);

				m_ptrTextures[dwPassName][dwName] = TextureManager()->LoadResource(dwName, TRUE, bSyncPostLoad);
				if (m_ptrTextures[dwPassName][dwName].IsNull()) return FALSE;
			} while (pTextureNode = pTextureNode->IterateChildren("TextureCube", pTextureNode));
		}

		return TRUE;
	}

	BOOL CResMaterial::LoadPassUniforms(TiXmlNode *pPassNode)
	{
		const char *szPassName = pPassNode->ToElement()->AttributeString("name");
		const uint32_t dwPassName = HashValue(szPassName);

		if (TiXmlNode *pFloatNode = pPassNode->FirstChild("Float")) {
			do {
				const char *szName = pFloatNode->ToElement()->AttributeString("name");
				const char *szValue = pFloatNode->ToElement()->AttributeString("value");
				const uint32_t dwName = HashValue(szName);

				float value;
				scanf(szValue, "%f", &value);
				m_floats[dwPassName][dwName] = value;
			} while (pFloatNode = pFloatNode->IterateChildren("Float", pFloatNode));
		}

		if (TiXmlNode *pVectorNode = pPassNode->FirstChild("Vector")) {
			do {
				const char *szName = pVectorNode->ToElement()->AttributeString("name");
				const char *szValue = pVectorNode->ToElement()->AttributeString("value");
				const uint32_t dwName = HashValue(szName);

				glm::vec4 value;
				scanf(szValue, "%f %f %f %f", &value.x, &value.y, &value.z, &value.w);
				m_vectors[dwPassName][dwName] = value;
			} while (pVectorNode = pVectorNode->IterateChildren("Vector", pVectorNode));
		}

		return TRUE;
	}

}
