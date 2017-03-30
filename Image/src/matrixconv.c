/////////////////////////////////////////////////////////////////////
// matrixconv.c
//
// 功  能: 3x3卷积运算
//
// 修  订: 00:25 2005-01-02
//         12:23 2005-06-19
//         13:25 2015-01-17
//===================================================================

#include "_image.h"

static void * matrixconv8bit(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk, char *kernel);
static void * matrixconv24bit(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk, char *kernel);

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_MatrixConvert
//
// 功  能: 3x3矩阵运算
//
// 参  数: IMAGE * const pimgsrc 源影像
//         IMAGE * const pimgdst 目标影像
//         char  * const kernel  3x3矩阵核
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_MatrixConvert(IMAGE * const pimgsrc, IMAGE * const pimgdst, char * const kernel)
{
	int rcode = NO_ERROR;

	IMAGE imgwrk;
	IMAGE_ZeroImage(&imgwrk);

	//
	// 0. 安全检查
	//
	assert(kernel);
	assert(pimgsrc);
	assert(pimgdst);
	if (0 == kernel[9]) { // 要求参数有效
		rcode = ERR_PARAMETER; goto RET;
	}
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
	case  8: matrixconv8bit(pimgsrc, pimgdst, &imgwrk, kernel); break;
	case 24: matrixconv24bit(pimgsrc, pimgdst, &imgwrk, kernel); break;
	default: rcode = ERR_BITCOUNT; goto RET;
	}

RET:
	IMAGE_FreeImage(&imgwrk);
	return rcode;
}

static void * matrixconv8bit(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk, char *kernel)
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
				(kernel[0]*ppixel_src_top[x-1] + kernel[1]*ppixel_src_top[x] + kernel[2]*ppixel_src_top[x+1] +
				 kernel[3]*ppixel_src_ths[x-1] + kernel[4]*ppixel_src_ths[x] + kernel[5]*ppixel_src_ths[x+1] +
				 kernel[6]*ppixel_src_btm[x-1] + kernel[7]*ppixel_src_btm[x] + kernel[8]*ppixel_src_btm[x+1])/kernel[9];

			if (pixel > 0xff) pixel = 0xff;
			if (pixel < 0x00) pixel = 0x00;

			ppixel_dst_ths[x] = (IMGGRAY)pixel;
		}
	}

	return NULL;
}

static void * matrixconv24bit(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk, char *kernel)
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
				(kernel[0]*ppixel_src_top[x-1].R + kernel[1]*ppixel_src_top[x].R + kernel[2]*ppixel_src_top[x+1].R +
				 kernel[3]*ppixel_src_ths[x-1].R + kernel[4]*ppixel_src_ths[x].R + kernel[5]*ppixel_src_ths[x+1].R +
				 kernel[6]*ppixel_src_btm[x-1].R + kernel[7]*ppixel_src_btm[x].R + kernel[8]*ppixel_src_btm[x+1].R)/kernel[9];

			if (pixel > 0xff) pixel = 0xff;
			if (pixel < 0x00) pixel = 0x00;
			
			ppixel_dst_ths[x].R = (unsigned char)pixel;

			pixel =
				(kernel[0]*ppixel_src_top[x-1].G + kernel[1]*ppixel_src_top[x].G + kernel[2]*ppixel_src_top[x+1].G +
				 kernel[3]*ppixel_src_ths[x-1].G + kernel[4]*ppixel_src_ths[x].G + kernel[5]*ppixel_src_ths[x+1].G +
				 kernel[6]*ppixel_src_btm[x-1].G + kernel[7]*ppixel_src_btm[x].G + kernel[8]*ppixel_src_btm[x+1].G)/kernel[9];
			
			if (pixel > 0xff) pixel = 0xff;
			if (pixel < 0x00) pixel = 0x00;
			
			ppixel_dst_ths[x].G = (unsigned char)pixel;

			pixel =
				(kernel[0]*ppixel_src_top[x-1].B + kernel[1]*ppixel_src_top[x].B + kernel[2]*ppixel_src_top[x+1].B +
				 kernel[3]*ppixel_src_ths[x-1].B + kernel[4]*ppixel_src_ths[x].B + kernel[5]*ppixel_src_ths[x+1].B +
				 kernel[6]*ppixel_src_btm[x-1].B + kernel[7]*ppixel_src_btm[x].B + kernel[8]*ppixel_src_btm[x+1].B)/kernel[9];
			
			if (pixel > 0xff) pixel = 0xff;
			if (pixel < 0x00) pixel = 0x00;
			
			ppixel_dst_ths[x].B = (unsigned char)pixel;
		}
	}

	return NULL;
}
