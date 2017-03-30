#include "stdafx.h"


CrossEngine::CRendererTexture* LoadTexture2D(const char *szFileName, CrossEngine::CRendererDevice *pDevice)
{
	CrossEngine::CRendererTexture *pTexture = pDevice->GetTextureManager()->AllocTexture();
	pTexture->CreateTexture2D(gli::texture2d(gli::load(szFileName)));
	return pTexture;
}

CrossEngine::CRendererTexture* LoadTexture2DArray(const char *szFileName, CrossEngine::CRendererDevice *pDevice)
{
	CrossEngine::CRendererTexture *pTexture = pDevice->GetTextureManager()->AllocTexture();
	pTexture->CreateTexture2DArray(gli::texture2d_array(gli::load(szFileName)));
	return pTexture;
}

CrossEngine::CRendererTexture* LoadTextureCube(const char *szFileName, CrossEngine::CRendererDevice *pDevice)
{
	CrossEngine::CRendererTexture *pTexture = pDevice->GetTextureManager()->AllocTexture();
	pTexture->CreateTextureCube(gli::texture_cube(gli::load(szFileName)));
	return pTexture;
}

void TestLoadTexture(CrossEngine::CRendererDevice *pDevice)
{
	char szCubemapFileNames[][_MAX_PATH] = {
		"../Data/cubemap_space.ktx",
		"../Data/cubemap_vulkan.ktx",
		"../Data/cubemap_yokohama.ktx"
	};

	char szArrayFileNames[][_MAX_PATH] = {
		"../Data/texturearray_bc3.ktx",
		"../Data/texturearray_plants_bc3.ktx",
		"../Data/texturearray_rocks_bc3.ktx"
	};

	char szTextureFileNames[][_MAX_PATH] = {
		"../Data/kueken7_bgra8_srgb.dds",
		"../Data/kueken7_bgra8_srgb.ktx",
		"../Data/kueken7_bgra8_unorm.dds",
		"../Data/kueken7_bgra8_unorm.ktx",
		"../Data/kueken7_r16_unorm.dds",
		"../Data/kueken7_r5g6b5_unorm.dds",
		"../Data/kueken7_r5g6b5_unorm.ktx",
		"../Data/kueken7_r8_sint.dds",
		"../Data/kueken7_r8_uint.dds",
		"../Data/kueken7_rg11b10_ufloat.dds",
		"../Data/kueken7_rg11b10_ufloat.ktx",
		"../Data/kueken7_rgb10a2u.dds",
		"../Data/kueken7_rgb10a2_unorm.dds",
		"../Data/kueken7_rgb5a1_unorm.dds",
		"../Data/kueken7_rgb5a1_unorm.ktx",
		"../Data/kueken7_rgb8_srgb.ktx",
		"../Data/kueken7_rgb9e5_ufloat.dds",
		"../Data/kueken7_rgb9e5_ufloat.ktx",
		"../Data/kueken7_rgba16_sfloat.dds",
		"../Data/kueken7_rgba16_sfloat.ktx",
		"../Data/kueken7_rgba4_unorm.dds",
		"../Data/kueken7_rgba4_unorm.ktx",
		"../Data/kueken7_rgba8_snorm.dds",
		"../Data/kueken7_rgba8_srgb.dds",
		"../Data/kueken7_rgba8_srgb.ktx",
		"../Data/kueken7_rgba8_unorm.dds",
		"../Data/kueken7_rgba8_unorm.ktx",
		"../Data/kueken7_rgba_astc12x12_srgb.ktx",
		"../Data/kueken7_rgba_astc4x4_srgb.dds",
		"../Data/kueken7_rgba_astc4x4_srgb.ktx",
		"../Data/kueken7_rgba_astc8x5_srgb.ktx",
		"../Data/kueken7_rgba_dxt1_srgb.dds",
		"../Data/kueken7_rgba_dxt1_unorm.dds",
		"../Data/kueken7_rgba_dxt1_unorm_decompressed.dds",
		"../Data/kueken7_rgba_dxt3_unorm.dds",
		"../Data/kueken7_rgba_dxt3_unorm_decompressed.dds",
		"../Data/kueken7_rgba_dxt5_srgb.dds",
		"../Data/kueken7_rgba_dxt5_srgb.ktx",
		"../Data/kueken7_rgba_dxt5_unorm.dds",
		"../Data/kueken7_rgba_dxt5_unorm.ktx",
		"../Data/kueken7_rgba_dxt5_unorm1.dds",
		"../Data/kueken7_rgba_dxt5_unorm2.dds",
		"../Data/kueken7_rgba_dxt5_unorm_decompressed.dds",
		"../Data/kueken7_rgba_etc2_a1_srgb.ktx",
		"../Data/kueken7_rgba_etc2_srgb.ktx",
		"../Data/kueken7_rgb_dxt1_srgb.ktx",
		"../Data/kueken7_rgb_dxt1_unorm.ktx",
		"../Data/kueken7_rgb_etc2_srgb.dds",
		"../Data/kueken7_rgb_etc2_srgb.ktx",
		"../Data/kueken7_rg_ati2n_unorm.dds",
		"../Data/kueken7_rg_ati2n_unorm_decompressed.dds",
		"../Data/kueken7_rg_eac_snorm.ktx",
		"../Data/kueken7_rg_eac_unorm.ktx",
		"../Data/kueken7_r_ati1n_unorm.dds",
		"../Data/kueken7_r_ati1n_unorm_decompressed.dds",
		"../Data/kueken7_r_eac_snorm.ktx",
		"../Data/kueken7_r_eac_unorm.ktx",
		"../Data/kueken8_rgba8_srgb.dds",
		"../Data/kueken8_rgba8_srgb.ktx",
		"../Data/kueken8_rgba_dxt1_unorm.dds"
	};

	CrossEngine::CRendererTexture *pTexture = NULL;

	for (int index = 0; index < sizeof(szCubemapFileNames) / sizeof(szCubemapFileNames[0]); index++) {
		LOGI("LoadTextureCube: %s\n", szCubemapFileNames[index]);
		pTexture = LoadTextureCube(szCubemapFileNames[index], pDevice);
		pDevice->GetTextureManager()->Free(pTexture);
	}

	for (int index = 0; index < sizeof(szArrayFileNames) / sizeof(szArrayFileNames[0]); index++) {
		LOGI("LoadTexture2DArray: %s\n", szArrayFileNames[index]);
		pTexture = LoadTexture2DArray(szArrayFileNames[index], pDevice);
		pDevice->GetTextureManager()->Free(pTexture);
	}

	for (int index = 0; index < sizeof(szTextureFileNames) / sizeof(szTextureFileNames[0]); index++) {
		LOGI("LoadTexture2D: %s\n", szTextureFileNames[index]);
		pTexture = LoadTexture2D(szTextureFileNames[index], pDevice);
		pDevice->GetTextureManager()->Free(pTexture);
	}
}
