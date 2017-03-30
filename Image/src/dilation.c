/////////////////////////////////////////////////////////////////////
// dilation.c
//
// 功  能: 形态学膨胀
//
// 修  订: 20:26 2005-01-02
//         12:23 2005-06-19
//         21:15 2007-01-09
//===================================================================

#include "_image.h"

static void * dilationH(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk);
static void * dilationV(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk);
static void * dilationBlock(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk);

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_DilationH
//
// 功  能: 水平膨胀
//
// 参  数: IMAGE * const pimgsrc 源影像
//         IMAGE * const pimgdst 目标影像
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_DilationH(IMAGE * const pimgsrc, IMAGE * const pimgdst)
{
	int rcode = NO_ERROR;

	IMAGE imgwrk;
	IMAGE_ZeroImage(&imgwrk);

	//
	// 0. 安全检查
	//
	assert(pimgsrc);
	assert(pimgdst);
	if (NULL == pimgsrc->data || NULL == pimgdst->data) { // 要求影像有效性
		rcode = ERR_IMAGE; goto RET;
	}
	if (IMAGE_BITCOUNT(pimgsrc) != IMAGE_BITCOUNT(pimgdst) || 8 != IMAGE_BITCOUNT(pimgsrc)) { // 要求原影像与目标影像色深相同，并且只对8bit影像处理
		rcode = ERR_BITCOUNT; goto RET;
	}
	if (IMAGE_AREA_WIDTH(pimgsrc) < 3 || IMAGE_AREA_HEIGHT(pimgsrc) < 3) { // 要求影像处理区域不得小于 3x3
		rcode = ERR_RANGE; goto RET;
	}
	if (IMAGE_AREA_WIDTH(pimgsrc) != IMAGE_AREA_WIDTH(pimgdst) || IMAGE_AREA_HEIGHT(pimgsrc) != IMAGE_AREA_HEIGHT(pimgdst)) { // 要求原影像与目标影像处理区域尺寸相同
		rcode = ERR_RANGE; goto RET;
	}

	//
	// 1. 水平膨胀运算
	//
	rcode = _makeworkimage(pimgsrc, &imgwrk, 3);
	if (rcode != NO_ERROR) goto RET;

	dilationH(pimgsrc, pimgdst, &imgwrk);

RET:
	IMAGE_FreeImage(&imgwrk);
	return rcode;
}

static void * dilationH(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk)
{
	int x, y;
	int width, height;

	IMGGRAY *ppixel_src = NULL;
	IMGGRAY *ppixel_dst = NULL;

	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	for (y = 0; y < height; y++) {
		ppixel_src = (IMGGRAY *)(&(pimgwrk->matrix[pimgwrk->sy+y][pimgwrk->sx]));
		ppixel_dst = (IMGGRAY *)(&(pimgdst->matrix[pimgdst->sy+y][pimgdst->sx]));

		for (x = 0; x < width; x++) {
			if (0 == ppixel_src[x-1] || 0 == ppixel_src[x] || 0 == ppixel_src[x+1])
				ppixel_dst[x] = 0;
			else
				ppixel_dst[x] = ppixel_src[x];
		}
	}

	return NULL;
}


/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_DilationV
//
// 功  能: 垂直膨胀
//
// 参  数: IMAGE * const pimgsrc 源影像
//         IMAGE * const pimgdst 目标影像
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_DilationV(IMAGE * const pimgsrc, IMAGE * const pimgdst)
{
	int rcode = NO_ERROR;

	IMAGE imgwrk;
	IMAGE_ZeroImage(&imgwrk);

	//
	// 0. 安全检查
	//
	assert(pimgsrc);
	assert(pimgdst);
	if (NULL == pimgsrc->data || NULL == pimgdst->data) { // 要求影像有效性
		rcode = ERR_IMAGE; goto RET;
	}
	if (IMAGE_BITCOUNT(pimgsrc) != IMAGE_BITCOUNT(pimgdst) || 8 != IMAGE_BITCOUNT(pimgsrc)) { // 要求原影像与目标影像色深相同，并且只对8bit影像处理
		rcode = ERR_BITCOUNT; goto RET;
	}
	if (IMAGE_AREA_WIDTH(pimgsrc) < 3 || IMAGE_AREA_HEIGHT(pimgsrc) < 3) { // 要求影像处理区域不得小于 3x3
		rcode = ERR_RANGE; goto RET;
	}
	if (IMAGE_AREA_WIDTH(pimgsrc) != IMAGE_AREA_WIDTH(pimgdst) || IMAGE_AREA_HEIGHT(pimgsrc) != IMAGE_AREA_HEIGHT(pimgdst)) { // 要求原影像与目标影像处理区域尺寸相同
		rcode = ERR_RANGE; goto RET;
	}

	//
	// 1. 垂直膨胀运算
	//
	rcode = _makeworkimage(pimgsrc, &imgwrk, 3);
	if (rcode != NO_ERROR) goto RET;

	dilationV(pimgsrc, pimgdst, &imgwrk);

RET:
	IMAGE_FreeImage(&imgwrk);
	return rcode;
}

static void * dilationV(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk)
{
	int x, y;
	int width, height;

	IMGGRAY *ppixel_src_top = NULL;
	IMGGRAY *ppixel_src_ths = NULL;
	IMGGRAY *ppixel_src_btm = NULL;
	IMGGRAY *ppixel_dst_ths = NULL;

	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	for (y = 0; y < height; y++) {
		ppixel_src_top = (IMGGRAY *)(&(pimgwrk->matrix[pimgwrk->sy+y-1][pimgwrk->sx]));
		ppixel_src_ths = (IMGGRAY *)(&(pimgwrk->matrix[pimgwrk->sy+y  ][pimgwrk->sx]));
		ppixel_src_btm = (IMGGRAY *)(&(pimgwrk->matrix[pimgwrk->sy+y+1][pimgwrk->sx]));
		ppixel_dst_ths = (IMGGRAY *)(&(pimgdst->matrix[pimgdst->sy+y  ][pimgdst->sx]));

		for (x = 0; x < width; x++) {
			if (0 == ppixel_src_top[x] || 0 == ppixel_src_ths[x] || 0 == ppixel_src_btm[x])
				ppixel_dst_ths[x] = 0;
			else
				ppixel_dst_ths[x] = ppixel_src_ths[x];
		}
	}

	return NULL;
}


/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_DilationBlock
//
// 功  能: 3x3块膨胀
//
// 参  数: IMAGE * const pimgsrc 源影像
//         IMAGE * const pimgdst 目标影像
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_DilationBlock(IMAGE * const pimgsrc, IMAGE * const pimgdst)
{
	int rcode = NO_ERROR;

	IMAGE imgwrk;
	IMAGE_ZeroImage(&imgwrk);

	//
	// 0. 安全检查
	//
	assert(pimgsrc);
	assert(pimgdst);
	if (NULL == pimgsrc->data || NULL == pimgdst->data) { // 要求影像有效性
		rcode = ERR_IMAGE; goto RET;
	}
	if (IMAGE_BITCOUNT(pimgsrc) != IMAGE_BITCOUNT(pimgdst) || 8 != IMAGE_BITCOUNT(pimgsrc)) { // 要求原影像与目标影像色深相同，并且只对8bit影像处理
		rcode = ERR_BITCOUNT; goto RET;
	}
	if (IMAGE_AREA_WIDTH(pimgsrc) < 3 || IMAGE_AREA_HEIGHT(pimgsrc) < 3) { // 要求影像处理区域不得小于 3x3
		rcode = ERR_RANGE; goto RET;
	}
	if (IMAGE_AREA_WIDTH(pimgsrc) != IMAGE_AREA_WIDTH(pimgdst) || IMAGE_AREA_HEIGHT(pimgsrc) != IMAGE_AREA_HEIGHT(pimgdst)) { // 要求原影像与目标影像处理区域尺寸相同
		rcode = ERR_RANGE; goto RET;
	}

	//
	// 1. 块膨胀运算
	//
	rcode = _makeworkimage(pimgsrc, &imgwrk, 3);
	if (rcode != NO_ERROR) goto RET;

	dilationBlock(pimgsrc, pimgdst, &imgwrk);

RET:
	IMAGE_FreeImage(&imgwrk);
	return rcode;
}

static void * dilationBlock(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk)
{
	int x, y;
	int width, height;

	IMGGRAY *ppixel_src_top = NULL;
	IMGGRAY *ppixel_src_ths = NULL;
	IMGGRAY *ppixel_src_btm = NULL;
	IMGGRAY *ppixel_dst_ths = NULL;

	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	for (y = 0; y < height; y++) {
		ppixel_src_top = (IMGGRAY *)(&(pimgwrk->matrix[pimgwrk->sy+y-1][pimgwrk->sx]));
		ppixel_src_ths = (IMGGRAY *)(&(pimgwrk->matrix[pimgwrk->sy+y  ][pimgwrk->sx]));
		ppixel_src_btm = (IMGGRAY *)(&(pimgwrk->matrix[pimgwrk->sy+y+1][pimgwrk->sx]));
		ppixel_dst_ths = (IMGGRAY *)(&(pimgdst->matrix[pimgdst->sy+y  ][pimgdst->sx]));

		for (x = 0; x < width; x++) {
			if (0 == ppixel_src_top[x-1] || 0 == ppixel_src_top[x] || 0 == ppixel_src_top[x+1] ||
				0 == ppixel_src_ths[x-1] || 0 == ppixel_src_ths[x] || 0 == ppixel_src_ths[x+1] ||
				0 == ppixel_src_btm[x-1] || 0 == ppixel_src_btm[x] || 0 == ppixel_src_btm[x+1])
				ppixel_dst_ths[x] = 0;
			else
				ppixel_dst_ths[x] = ppixel_src_ths[x];
		}
	}

	return NULL;
}
