/////////////////////////////////////////////////////////////////////
// outline.c
//
// 功  能: 边缘化
//
// 修  订: 10:39 2005-01-01
//         12:23 2005-06-19
//         15:29 2007-01-09
//         20:12 2015-01-19
//===================================================================

#include "_image.h"

// 卷积算子
// -1, -1, -1,
// -1,  8, -1,
// -1, -1, -1, 1
static void * outline8bit(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk);
static void * outline24bit(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk);

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_Outline
//
// 功  能: 边缘化
//
// 参  数: IMAGE * const pimgsrc 源影像
//         IMAGE * const pimgdst 目标影像
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_Outline(IMAGE * const pimgsrc, IMAGE * const pimgdst)
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
	if (IMAGE_BITCOUNT(pimgsrc) != IMAGE_BITCOUNT(pimgdst)) { // 要求原影像与目标影像色深相同
		rcode = ERR_BITCOUNT; goto RET;
	}
	if (IMAGE_AREA_WIDTH(pimgsrc) < 3 || IMAGE_AREA_HEIGHT(pimgsrc) < 3) { // 要求影像处理区域不得小于 3x3
		rcode = ERR_RANGE; goto RET;
	}
	if (IMAGE_AREA_WIDTH(pimgsrc) != IMAGE_AREA_WIDTH(pimgdst) || IMAGE_AREA_HEIGHT(pimgsrc) != IMAGE_AREA_HEIGHT(pimgdst)) { // 要求原影像与目标影像处理区域尺寸相同
		rcode = ERR_RANGE; goto RET;
	}

	//
	// 1 根据不同色深分别调用不同函数
	//
	rcode = _makeworkimage(pimgsrc, &imgwrk, 3);
	if (rcode != NO_ERROR) goto RET;

	switch (IMAGE_BITCOUNT(pimgsrc)) {
	case  8: outline8bit(pimgsrc, pimgdst, &imgwrk); break;
	case 24: outline24bit(pimgsrc, pimgdst, &imgwrk); break;
	default: rcode = ERR_BITCOUNT; goto RET;
	}

RET:
	IMAGE_FreeImage(&imgwrk);
	return rcode;
}

static void * outline8bit(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk)
{
	int x, y;
	int width, height;

	int pixel;

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
			pixel =
				-ppixel_src_top[x-1] -   ppixel_src_top[x] - ppixel_src_top[x+1]
				-ppixel_src_ths[x-1] + 8*ppixel_src_ths[x] - ppixel_src_ths[x+1]
				-ppixel_src_btm[x-1] -   ppixel_src_btm[x] - ppixel_src_btm[x+1];

			if (pixel > 0xff) pixel = 0xff;
			if (pixel < 0x00) pixel = 0x00;

			ppixel_dst_ths[x] = (IMGGRAY)pixel;
		}
	}

	return NULL;
}

static void * outline24bit(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk)
{
	int x, y;
	int width, height;

	int pixel;

	IMGRGB *ppixel_src_top = NULL;
	IMGRGB *ppixel_src_ths = NULL;
	IMGRGB *ppixel_src_btm = NULL;
	IMGRGB *ppixel_dst_ths = NULL;
	
	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	for (y = 0; y < height; y++) {
		ppixel_src_top = (IMGRGB *)(&(pimgwrk->matrix[pimgwrk->sy+y-1][pimgwrk->sx*3]));
		ppixel_src_ths = (IMGRGB *)(&(pimgwrk->matrix[pimgwrk->sy+y  ][pimgwrk->sx*3]));
		ppixel_src_btm = (IMGRGB *)(&(pimgwrk->matrix[pimgwrk->sy+y+1][pimgwrk->sx*3]));
		ppixel_dst_ths = (IMGRGB *)(&(pimgdst->matrix[pimgdst->sy+y  ][pimgdst->sx*3]));

		for (x = 0; x < width; x++) {
			pixel =
				-ppixel_src_top[x-1].R -   ppixel_src_top[x].R - ppixel_src_top[x+1].R
				-ppixel_src_ths[x-1].R + 8*ppixel_src_ths[x].R - ppixel_src_ths[x+1].R
				-ppixel_src_btm[x-1].R -   ppixel_src_btm[x].R - ppixel_src_btm[x+1].R;

			if (pixel > 0xff) pixel = 0xff;
			if (pixel < 0x00) pixel = 0x00;
			
			ppixel_dst_ths[x].R = (unsigned char)pixel;

			pixel =
				-ppixel_src_top[x-1].G -   ppixel_src_top[x].G - ppixel_src_top[x+1].G
				-ppixel_src_ths[x-1].G + 8*ppixel_src_ths[x].G - ppixel_src_ths[x+1].G
				-ppixel_src_btm[x-1].G -   ppixel_src_btm[x].G - ppixel_src_btm[x+1].G;
			
			if (pixel > 0xff) pixel = 0xff;
			if (pixel < 0x00) pixel = 0x00;
			
			ppixel_dst_ths[x].G = (unsigned char)pixel;

			pixel =
				-ppixel_src_top[x-1].B -   ppixel_src_top[x].B - ppixel_src_top[x+1].B
				-ppixel_src_ths[x-1].B + 8*ppixel_src_ths[x].B - ppixel_src_ths[x+1].B
				-ppixel_src_btm[x-1].B -   ppixel_src_btm[x].B - ppixel_src_btm[x+1].B;
			
			if (pixel > 0xff) pixel = 0xff;
			if (pixel < 0x00) pixel = 0x00;
			
			ppixel_dst_ths[x].B = (unsigned char)pixel;
		}
	}

	return NULL;
}
