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
		m_ptrMaterial = GfxDevice()->NewMaterial();
	}

	CResMaterial::~CResMaterial(void)
	{

	}

	RESOURCE_TYPE CResMaterial::GetType(void) const
	{
		return RESOURCE_TYPE_MATERIAL;
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

	void CResMaterial::Init(void)
	{
		CResource::Init();
	}

	void CResMaterial::Free(void)
	{
		m_ptrMaterial.Release();
		CResource::Free();
	}

	BOOL CResMaterial::InternalLoad(BOOL bSync)
	{
		TiXmlDocument xmlDoc;
		if (xmlDoc.LoadFile((char *)m_stream.GetAddress(), m_stream.GetFullSize())) {
			if (TiXmlNode *pPassNode = xmlDoc.FirstChild("Pass")) {
				do {
					const char *szName = pPassNode->ToElement()->AttributeString("name");
					const uint32_t dwName = HashValue(szName);

					CGfxMaterialPassPtr &ptrPass = m_ptrMaterial->GetPass(dwName);
					LoadPassPipeline(ptrPass, pPassNode, bSync);
					LoadPassTextures(ptrPass, pPassNode, bSync);
					LoadPassUniforms(ptrPass, pPassNode, bSync);
				} while (pPassNode = pPassNode->IterateChildren("Pass", pPassNode));

				return TRUE;
			}
		}

		return FALSE;
	}

	BOOL CResMaterial::InternalPostLoad(void)
	{
		for (auto &itPass : m_ptrMaterial->GetPasses()) {
			itPass.second->UpdateDescriptorSet(0);
		}

		return TRUE;
	}

	void CResMaterial::InternalCleanup(void)
	{
		m_stream.Free();
	}

	BOOL CResMaterial::LoadPassPipeline(CGfxMaterialPassPtr &ptrPass, TiXmlNode *pPassNode, BOOL bSync)
	{
		const char *szName = pPassNode->ToElement()->AttributeString("pipeline");
		const uint32_t dwName = HashValue(szName);

		const CResGraphicsPtr &ptrGraphics = GraphicsManager()->LoadResource(dwName, bSync);
		if (ptrGraphics.IsNull()) return FALSE;

		ptrPass->SetPipeline(ptrGraphics->GetPipeline());

		return TRUE;
	}

	BOOL CResMaterial::LoadPassTextures(CGfxMaterialPassPtr &ptrPass, TiXmlNode *pPassNode, BOOL bSync)
	{
		if (TiXmlNode *pTextureNode = pPassNode->FirstChild("Texture2D")) {
			do {
				const char *szName = pTextureNode->ToElement()->AttributeString("name");
				const uint32_t dwName = HashValue(szName);

				const CResTexturePtr &ptrTexture = TextureManager()->LoadResource(dwName, bSync);
				if (ptrTexture.IsNull()) return FALSE;

				ptrPass->SetTexture(dwName, ptrTexture->GetTexture());
			} while (pTextureNode = pTextureNode->IterateChildren("Texture2D", pTextureNode));
		}

		if (TiXmlNode *pTextureNode = pPassNode->FirstChild("TextureCube")) {
			do {
				const char *szName = pTextureNode->ToElement()->AttributeString("name");
				const uint32_t dwName = HashValue(szName);

				const CResTexturePtr &ptrTexture = TextureManager()->LoadResource(dwName, bSync);
				if (ptrTexture.IsNull()) return FALSE;

				ptrPass->SetTexture(dwName, ptrTexture->GetTexture());
			} while (pTextureNode = pTextureNode->IterateChildren("TextureCube", pTextureNode));
		}

		return TRUE;
	}

	BOOL CResMaterial::LoadPassUniforms(CGfxMaterialPassPtr &ptrPass, TiXmlNode *pPassNode, BOOL bSync)
	{
		if (TiXmlNode *pFloatNode = pPassNode->FirstChild("Float")) {
			do {
				const char *szName = pFloatNode->ToElement()->AttributeString("name");
				const char *szValue = pFloatNode->ToElement()->AttributeString("value");

				const uint32_t dwName = HashValue(szName);
				const uint32_t binding = ptrPass->GetPipeline()->GetBinding(DESCRIPTOR_SET_PASS, dwName);
				if (binding == -1) return FALSE;

				float value;
				scanf(szValue, "%f", &value);

				CGfxUniformBufferPtr ptrUniform = GfxDevice()->NewUniformBuffer();
				ptrUniform->Create(sizeof(value), &value, FALSE);
				ptrUniform->SetDescriptorBufferInfo(DESCRIPTOR_SET_PASS, binding, 0, sizeof(value));

				ptrPass->SetUniform(dwName, ptrUniform);
			} while (pFloatNode = pFloatNode->IterateChildren("Float", pFloatNode));
		}

		if (TiXmlNode *pVectorNode = pPassNode->FirstChild("Vector")) {
			do {
				const char *szName = pVectorNode->ToElement()->AttributeString("name");
				const char *szValue = pVectorNode->ToElement()->AttributeString("value");

				const uint32_t dwName = HashValue(szName);
				const uint32_t binding = ptrPass->GetPipeline()->GetBinding(DESCRIPTOR_SET_PASS, dwName);
				if (binding == -1) return FALSE;

				glm::vec4 value;
				scanf(szValue, "%f %f %f %f", &value.x, &value.y, &value.z, &value.w);

				CGfxUniformBufferPtr ptrUniform = GfxDevice()->NewUniformBuffer();
				ptrUniform->Create(sizeof(value), &value, FALSE);
				ptrUniform->SetDescriptorBufferInfo(DESCRIPTOR_SET_PASS, binding, 0, sizeof(value));

				ptrPass->SetUniform(dwName, ptrUniform);
			} while (pVectorNode = pVectorNode->IterateChildren("Vector", pVectorNode));
		}

		return TRUE;
	}

}
