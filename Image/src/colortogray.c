/////////////////////////////////////////////////////////////////////
// colortogray.c
//
// 功  能: 提供对24-bit影像灰度处理，根据参数的不同分为标准、红通道、
//         绿通道、蓝通道。
//
// 注  意: 对整图操作。
//
// 修  订: 16:45 2004-12-06
//         10:09 2004-12-13
//         00:22 2004-12-19
//         11:39 2005-06-19
//         21:30 2007-01-06
//         15:00 2007-01-07
//         20:53 2007-01-07
//         16:31 2015-01-13
//===================================================================

#include "_image.h"

static void * colortogray_std(IMAGE *pimgsrc, IMAGE *pimgdst);
static void * colortogray_red(IMAGE *pimgsrc, IMAGE *pimgdst);
static void * colortogray_grn(IMAGE *pimgsrc, IMAGE *pimgdst);
static void * colortogray_blu(IMAGE *pimgsrc, IMAGE *pimgdst);

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_ColorToGray
//
// 功  能: 彩色影像到灰阶影像
//
// 参  数: IMAGE * const pimgsrc 原影像指针
//         IMAGE * const pimgdst 灰阶目标影像指针
//         int     const nflag   转换方式
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_ColorToGray(IMAGE * const pimgsrc, IMAGE * const pimgdst, int const nflag)
{
	int rcode = NO_ERROR;

	int i;
	RGBQUAD *pal8bit = NULL;

	//
	// 0. 安全检查
	//
	assert(pimgdst);
	assert(pimgsrc);
	if (NULL == pimgsrc || NULL == pimgsrc->data ||
		NULL == pimgdst || NULL == pimgdst->data) { // 要求影像有效性
		return ERR_IMAGE;
	}
	if (24 != IMAGE_BITCOUNT(pimgsrc) || 8 != IMAGE_BITCOUNT(pimgdst)) { // 要求原影像必须是24-bit,目标影像必须是8-bit
		return ERR_BITCOUNT;
	}
	if (IMAGE_WIDTH(pimgsrc) != IMAGE_WIDTH(pimgdst) || IMAGE_HEIGHT(pimgsrc) != IMAGE_HEIGHT(pimgdst)) { // 要求原影像与目标影像处理区域尺寸相同
		return ERR_RANGE;
	}

	//
	// 1. 调整目标调色板为灰阶调色板
	//
	pal8bit = pimgdst->bmpInfo->bmiColors;
	for (i = 0; i < 256; i++) {
		pal8bit->rgbRed	= (unsigned char)(i);
		pal8bit->rgbGreen = (unsigned char)(i);
		pal8bit->rgbBlue = (unsigned char)(i);
		pal8bit->rgbReserved = (unsigned char)(i);

		pal8bit++;
	}

	//
	// 2. 24-bit 彩色影像到 8-bit 灰阶影像
	//
	switch (nflag) {
	case GRAY_STD: colortogray_std(pimgsrc, pimgdst); break; // 标准彩色->灰度
	case GRAY_RED: colortogray_red(pimgsrc, pimgdst); break; // 红色通道->灰度
	case GRAY_GRN: colortogray_grn(pimgsrc, pimgdst); break; // 绿色通道->灰度
	case GRAY_BLU: colortogray_blu(pimgsrc, pimgdst); break; // 蓝色通道->灰度
	default: rcode = ERR_PARAMETER; goto RET;
	}

RET:
	return rcode;
}

static void * colortogray_std(IMAGE *pimgsrc, IMAGE *pimgdst)
{
	int x, y;
	int width, height;

	IMGRGB  *ppixel_src = NULL;
	IMGGRAY *ppixel_dst = NULL;

	width = IMAGE_WIDTH(pimgsrc);
	height = IMAGE_HEIGHT(pimgsrc);

	for (y = 0; y < height; y++) {
		ppixel_src = (IMGRGB *)(&(pimgsrc->matrix[y][0])); // 24-bit
		ppixel_dst = (IMGGRAY *)(&(pimgdst->matrix[y][0])); // 8-bit

		for (x = 0; x < width; x++) {
			ppixel_dst[x] = (30*ppixel_src[x].R + 59*ppixel_src[x].G + 11*ppixel_src[x].B)/100;
		}
	}

	return NULL;
}

static void * colortogray_red(IMAGE *pimgsrc, IMAGE *pimgdst)
{
	int x, y;
	int width, height;

	IMGRGB *ppixel_src = NULL;
	IMGGRAY *ppixel_dst = NULL;

	width = IMAGE_WIDTH(pimgsrc);
	height = IMAGE_HEIGHT(pimgsrc);

	for (y = 0; y < height; y++) {
		ppixel_src = (IMGRGB *)(&(pimgsrc->matrix[y][0])); // 24-bit
		ppixel_dst = (IMGGRAY *)(&(pimgdst->matrix[y][0])); // 8-bit

		for (x = 0; x < width; x++) {
			ppixel_dst[x] = ppixel_src[x].R;
		}
	}

	return NULL;
}

static void * colortogray_grn(IMAGE *pimgsrc, IMAGE *pimgdst)
{
	int x, y;
	int width, height;

	IMGRGB *ppixel_src = NULL;
	IMGGRAY *ppixel_dst = NULL;

	width = IMAGE_WIDTH(pimgsrc);
	height = IMAGE_HEIGHT(pimgsrc);

	for (y = 0; y < height; y++) {
		ppixel_src = (IMGRGB *)(&(pimgsrc->matrix[y][0])); // 24-bit
		ppixel_dst = (IMGGRAY *)(&(pimgdst->matrix[y][0])); // 8-bit

		for (x = 0; x < width; x++) {
			ppixel_dst[x] = ppixel_src[x].G;
		}
	}

	return NULL;
}

static void * colortogray_blu(IMAGE *pimgsrc, IMAGE *pimgdst)
{
	int x, y;
	int width, height;

	IMGRGB *ppixel_src = NULL;
	IMGGRAY *ppixel_dst = NULL;

	width = IMAGE_WIDTH(pimgsrc);
	height = IMAGE_HEIGHT(pimgsrc);

	for (y = 0; y < height; y++) {
		ppixel_src = (IMGRGB *)(&(pimgsrc->matrix[y][0])); // 24-bit
		ppixel_dst = (IMGGRAY *)(&(pimgdst->matrix[y][0])); // 8-bit

		for (x = 0; x < width; x++) {
			ppixel_dst[x] = ppixel_src[x].B;
		}
	}

	return NULL;
}
