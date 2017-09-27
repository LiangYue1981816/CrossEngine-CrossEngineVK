/////////////////////////////////////////////////////////////////////
// expandcontrast.c
//
// 功  能: 对比度调节
//
// 修  订: 09:13 2004-12-18
//         00:22 2004-12-19
//         12:23 2005-06-19
//         21:52 2007-01-08
//         17:11 2015-01-16
//===================================================================

#include "_image.h"

static void * expandcontrast8bit(IMAGE *pimgsrc, IMAGE *pimgdst, int lower, int upper);
static void * expandcontrast24bit(IMAGE *pimgsrc, IMAGE *pimgdst, int lower, int upper);

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_ExpandContrast
//
// 功  能: 对比度调节
//
// 参  数: IMAGE * const pimgsrc 源影像
//         IMAGE * const pimgdst 目标影像
//         int     const nlower  下限阀值
//         int     const nupper  上限阀值
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_ExpandContrast(IMAGE * const pimgsrc, IMAGE * const pimgdst, int const nlower, int const nupper)
{
	int rcode = NO_ERROR;

	//
	// 0 安全检查
	//
	assert(pimgsrc);
	assert(pimgdst);
	if (nlower < 0 || nupper > 255 || nlower >= nupper ) { // 要求参数范围在0~255之间，并且 nupper > nlower
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
	case  8: expandcontrast8bit(pimgsrc, pimgdst, nlower, nupper); break;
	case 24: expandcontrast24bit(pimgsrc, pimgdst, nlower, nupper); break;
	default: rcode = ERR_BITCOUNT; goto RET;
	}

RET:
	return rcode;
}

static void * expandcontrast8bit(IMAGE *pimgsrc, IMAGE *pimgdst, int lower, int upper)
{
	int x, y;
	int width, height;

	int i, diff;

	unsigned char table[256];

	IMGGRAY *ppixel_src = NULL;
	IMGGRAY *ppixel_dst = NULL;

	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	i = 0; diff = upper - lower;
	while (i <= lower) { table[i] = 0; i++; }
	while (i <  upper) { table[i] = (unsigned char)((255 * (i - lower)) / diff); i++; }
	while (i <  256)   { table[i] = 255; i++; }

	for (y = 0; y < height; y++) {
		ppixel_src = (IMGGRAY *)(&(pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->sx]));
		ppixel_dst = (IMGGRAY *)(&(pimgdst->matrix[pimgdst->sy+y][pimgdst->sx]));

		for (x = 0; x < width; x++) {
			ppixel_dst[x] = table[ppixel_src[x]];
		}
	}

	return NULL;
}

static void * expandcontrast24bit(IMAGE *pimgsrc, IMAGE *pimgdst, int lower, int upper)
{
	int x, y;
	int width, height;

	int i, diff;

	unsigned char table[256];

	IMGRGB *ppixel_src = NULL;
	IMGRGB *ppixel_dst = NULL;

	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	i = 0; diff = upper - lower;
	while (i <= lower) { table[i] = 0; i++; }
	while (i <  upper) { table[i] = (unsigned char)((255 * (i - lower)) / diff); i++; }
	while (i <  256)   { table[i] = 255; i++; }

	for (y = 0; y < height; y++) {
		ppixel_src = (IMGRGB *)(&(pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->sx*3]));
		ppixel_dst = (IMGRGB *)(&(pimgdst->matrix[pimgdst->sy+y][pimgdst->sx*3]));

		for (x = 0; x < width; x++) {
			ppixel_dst[x].R = table[ppixel_src[x].R];
			ppixel_dst[x].G = table[ppixel_src[x].G];
			ppixel_dst[x].B = table[ppixel_src[x].B];
		}
	}

	return NULL;
}
