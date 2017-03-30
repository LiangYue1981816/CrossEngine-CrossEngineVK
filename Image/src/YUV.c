/////////////////////////////////////////////////////////////////////
// YUV.c
//
// 功  能: 提供对24-bit影像的YUV与RGB之间的互相变换
//
// 修  订: 16:45 2004-12-06
//         10:09 2004-12-13
//         00:22 2004-12-19
//         11:39 2005-06-19
//         10:07 2007-01-08
//         09:35 2015-01-20
//===================================================================

#include "_image.h"

static void * RGB2YUV(IMAGE *prgbsrc, YUV **pyuvdst);
static void * YUV2RGB(YUV **pyuvsrc, IMAGE *prgbdst);

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_RGB2YUV
//
// 功  能: 24bit RGB影像转换到YUV
//
// 参  数: IMAGE * const prgbsrc RGB原影像
//         YUV  ** const pyuvdst YUV目标影像
//
// 返回值: int
//
// 注  意: 由于对pyuvdst二维矩阵存储分配方法不明，故不能使用更快的操作
//===================================================================
EXPORT_C int IMAGE_RGB2YUV(IMAGE * const prgbsrc, YUV ** const pyuvdst)
{
	//
	// 0. 安全检查
	//
	assert(prgbsrc);
	assert(pyuvdst);
	if (NULL == pyuvdst) { // YUV数据缓冲无效
		return ERR_PARAMETER;
	}
	if (NULL == prgbsrc->data) { // 要求影像有效性
		return ERR_IMAGE;
	}
	if (24 != IMAGE_BITCOUNT(prgbsrc)) { // 要求原影像必须是24-bit
		return ERR_BITCOUNT;
	}

	//
	// 1. 24-bit RGB 转换为 YUV
	//
	RGB2YUV(prgbsrc, pyuvdst);

	return NO_ERROR;
}

static void * RGB2YUV(IMAGE *prgbsrc, YUV **pyuvdst)
{
	int x, y;
	int width, height;

	YUV *pYUV = NULL;
	IMGRGB *pRGB = NULL;

	width = IMAGE_AREA_WIDTH(prgbsrc);
	height = IMAGE_AREA_HEIGHT(prgbsrc);

	for (y = 0; y < height; y++) {
		pRGB = (IMGRGB *)(&(prgbsrc->matrix[prgbsrc->sy+y][prgbsrc->sx*3]));

		for (x = 0; x < width; x++) {
			pYUV = &pyuvdst[y][x];

			pYUV->Y =  0.299f*pRGB[x].R + 0.587f*pRGB[x].G + 0.114f*pRGB[x].B;
			pYUV->U = -0.148f*pRGB[x].R - 0.289f*pRGB[x].G + 0.437f*pRGB[x].B;
			pYUV->V =  0.615f*pRGB[x].R - 0.515f*pRGB[x].G - 0.100f*pRGB[x].B;
		}
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_YUV2RGB
//
// 功  能: YUV 转换到 24bit RGB
//
// 参  数: YUV  ** const pyuvsrc YUV原影像
//         IMAGE * const prgbdst RGB目标影像
//
// 返回值: int
//
// 注  意: 由于对prgbdst二维矩阵存储分配方法不明，故不能使用更快的操作
//===================================================================
EXPORT_C int IMAGE_YUV2RGB(YUV ** const pyuvsrc, IMAGE * const prgbdst)
{
	//
	// 0. 安全检查
	//
	assert(pyuvsrc);
	assert(prgbdst);
	if (NULL == pyuvsrc) { // YUV缓冲无效
		return ERR_PARAMETER;
	}
	if (NULL == prgbdst->data) { // 要求影像有效性
		return ERR_IMAGE;
	}
	if (24 != IMAGE_BITCOUNT(prgbdst)) { // 要求原影像必须是24-bit
		return ERR_BITCOUNT;
	}

	//
	// 1. YUV 转换到 24bit RGB
	//
	YUV2RGB(pyuvsrc, prgbdst);

	return NO_ERROR;
}

static void * YUV2RGB(YUV **pyuvsrc, IMAGE *prgbdst)
{
	int x, y;
	int width, height;

	int icolor;
	float fcolor;

	YUV *pYUV = NULL;
	IMGRGB *pRGB = NULL;

	width = IMAGE_AREA_WIDTH(prgbdst);
	height = IMAGE_AREA_HEIGHT(prgbdst);

	for (y = 0; y < height; y++) {
		pRGB = (IMGRGB *)(&(prgbdst->matrix[prgbdst->sy+y][prgbdst->sx*3]));

		for (x = 0; x < width; x++) {
			pYUV = &pyuvsrc[y][x];

			fcolor = 1.000f*pYUV->Y + 0.000f*pYUV->U + 1.140f*pYUV->V + 0.5f; icolor = _float2int(fcolor); if (icolor < 0) icolor = 0; if (icolor > 255) icolor = 255; pRGB[x].R = (unsigned char)icolor;
			fcolor = 1.000f*pYUV->Y - 0.395f*pYUV->U - 0.581f*pYUV->V + 0.5f; icolor = _float2int(fcolor); if (icolor < 0) icolor = 0; if (icolor > 255) icolor = 255; pRGB[x].G = (unsigned char)icolor;
			fcolor = 1.000f*pYUV->Y + 2.032f*pYUV->U + 0.000f*pYUV->V + 0.5f; icolor = _float2int(fcolor); if (icolor < 0) icolor = 0; if (icolor > 255) icolor = 255; pRGB[x].B = (unsigned char)icolor;
		}
	}

	return NULL;
}
