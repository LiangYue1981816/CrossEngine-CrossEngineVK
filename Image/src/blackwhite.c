/////////////////////////////////////////////////////////////////////
// blackwhite.c
//
// 功  能: 二值化
//
// 修  订: 11:52 2004-12-26
//         12:23 2005-06-19
//         11:10 2007-01-09
//         14:26 2015-01-13
//===================================================================

#include "_image.h"

static void * blackwhite8bit(IMAGE *pimgsrc, IMAGE *pimgdst, int thresh);
static void * blackwhite24bit(IMAGE *pimgsrc, IMAGE *pimgdst, int thresh);

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_BlackWhite
//
// 功  能: 二值化
//
// 参  数: IMAGE * const pimgsrc 源影像
//         IMAGE * const pimgdst 目标影像
//         int     const nthresh 阀值
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_BlackWhite(IMAGE * const pimgsrc, IMAGE * const pimgdst, int const nthresh)
{
	int rcode = NO_ERROR;

	//
	// 0 安全检查
	//
	assert(pimgsrc);
	assert(pimgdst);
	if (nthresh < 0 || nthresh > 255) { // 要求参数有效
		rcode = ERR_PARAMETER; goto RET;
	}
	if (NULL == pimgsrc->data || NULL == pimgdst->data) { // 要求影像有效性
		rcode = ERR_IMAGE; goto RET;
	}
	if (IMAGE_BITCOUNT(pimgsrc) != IMAGE_BITCOUNT(pimgdst)) { // 要求原影像与目标影像色深相同
		rcode = ERR_BITCOUNT; goto RET;
	}
	if (IMAGE_AREA_WIDTH(pimgsrc) != IMAGE_AREA_WIDTH(pimgdst) || IMAGE_AREA_HEIGHT(pimgsrc) != IMAGE_AREA_HEIGHT(pimgdst)) { // 要求原影像与目标影像处理区域尺寸相同
		rcode = ERR_RANGE; goto RET;
	}

	//
	// 1 根据不同色深分别调用不同函数
	//
	switch (IMAGE_BITCOUNT(pimgsrc)) {
	case  8: blackwhite8bit(pimgsrc, pimgdst, nthresh); break;
	case 24: blackwhite24bit(pimgsrc, pimgdst, nthresh); break;
	default: rcode = ERR_BITCOUNT; goto RET;
	}

RET:
	return rcode;
}

static void * blackwhite8bit(IMAGE *pimgsrc, IMAGE *pimgdst, int thresh)
{
	int x, y;
	int width, height;

	int i;
	unsigned char table[256];

	IMGGRAY *ppixel_src = NULL;
	IMGGRAY *ppixel_dst = NULL;

	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	for (i = 0; i <= thresh; i++) table[i] = 0;
	for (; i < 256; i++) table[i] = 255;

	for (y = 0; y < height; y++) {
		ppixel_src = (IMGGRAY *)(&(pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->sx]));
		ppixel_dst = (IMGGRAY *)(&(pimgdst->matrix[pimgdst->sy+y][pimgdst->sx]));

		for (x = 0; x < width; x++) {
			ppixel_dst[x] = table[ppixel_src[x]];
		}
	}

	return NULL;
}

static void * blackwhite24bit(IMAGE *pimgsrc, IMAGE *pimgdst, int thresh)
{
	int x, y;
	int width, height;

	int i, pixel, index;
	unsigned char table[256];

	IMGRGB *ppixel_src = NULL;
	IMGRGB *ppixel_dst = NULL;

	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	for (i = 0; i <= thresh; i++) table[i] = 0;
	for (; i < 256; i++) table[i] = 255;

	for (y = 0; y < height; y++) {
		ppixel_src = (IMGRGB *)(&(pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->sx*3]));
		ppixel_dst = (IMGRGB *)(&(pimgdst->matrix[pimgdst->sy+y][pimgdst->sx*3]));

		for (x = 0; x < width; x++) {
			index = (ppixel_src[x].R + ppixel_src[x].G + ppixel_src[x].B)/3;
			pixel = table[index];

			ppixel_dst[x].R = (unsigned char)pixel;
			ppixel_dst[x].G = (unsigned char)pixel;
			ppixel_dst[x].B = (unsigned char)pixel;
		}
	}

	return NULL;
}
