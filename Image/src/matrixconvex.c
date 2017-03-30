/////////////////////////////////////////////////////////////////////
// matrixconvex.c
//
// 功  能: 卷积运算扩展
//
// 修  订: 00:25 2005-01-02
//         12:23 2005-06-19
//         16:14 2007-01-09
//         13:29 2015-01-17
//===================================================================

#include "_image.h"

static void * matrixconvex8bit(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk, char *kernel, int size, int divide);
static void * matrixconvex24bit(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk, char *kernel, int size, int divide);

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_MatrixConvertEx
//
// 功  能: nxn矩阵运算
//
// 参  数: IMAGE * const pimgsrc 源影像
//         IMAGE * const pimgdst 目标影像
//         char  * const kernel  矩阵核
//         int     const nsize   矩阵尺寸
//         int     const ndivide 被除数
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_MatrixConvertEx(IMAGE * const pimgsrc, IMAGE * const pimgdst, char * const kernel, int const nsize, int const ndivide)
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
	if (0 == ndivide || 0 == (nsize&1) || nsize < 3) { // 要求参数有效
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
	rcode = _makeworkimage(pimgsrc, &imgwrk, nsize);
	if (rcode != NO_ERROR) goto RET;

	switch (IMAGE_BITCOUNT(pimgsrc)) {
	case  8: matrixconvex8bit(pimgsrc, pimgdst, &imgwrk, kernel, nsize, ndivide); break;
	case 24: matrixconvex24bit(pimgsrc, pimgdst, &imgwrk, kernel, nsize, ndivide); break;
	default: rcode = ERR_BITCOUNT; goto RET;
	}

RET:
	IMAGE_FreeImage(&imgwrk);
	return rcode;
}

static void * matrixconvex8bit(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk, char *kernel, int size, int divide)
{
	int x, y;
	int xx, yy, k;
	int width, height;

	int pixel;

	IMGGRAY *ppixel_src_top = NULL;
	IMGGRAY *ppixel_src_ths = NULL;
	IMGGRAY *ppixel_src_btm = NULL;
	IMGGRAY *ppixel_src_tmp = NULL;
	IMGGRAY *ppixel_dst_ths = NULL;
	
	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	for (y = 0; y < height; y++) {
		ppixel_src_top = (IMGGRAY *)(&(pimgwrk->matrix[pimgwrk->sy+y-1][pimgwrk->sx]));
		ppixel_src_ths = (IMGGRAY *)(&(pimgwrk->matrix[pimgwrk->sy+y  ][pimgwrk->sx]));
		ppixel_src_btm = (IMGGRAY *)(&(pimgwrk->matrix[pimgwrk->sy+y+1][pimgwrk->sx]));
		ppixel_dst_ths = (IMGGRAY *)(&(pimgdst->matrix[pimgdst->sy+y  ][pimgdst->sx]));

		for (x = 0; x < width; x++) {
			pixel = 0;
			for (k = 0, yy = -size/2; yy <= size/2; yy++) {
				ppixel_src_tmp = (IMGGRAY *)((unsigned char *)&ppixel_src_ths[x] + yy*pimgwrk->buffWidth);

				for (xx = -size/2; xx <= size/2; xx++, k++) {
					pixel += kernel[k]*ppixel_src_tmp[xx];
				}
			}
			pixel /= divide;

			if (pixel > 0xff) pixel = 0xff;
			if (pixel < 0x00) pixel = 0x00;

			ppixel_dst_ths[x] = (IMGGRAY)pixel;
		}
	}

	return NULL;
}

static void * matrixconvex24bit(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk, char *kernel, int size, int divide)
{
	int x, y;
	int xx, yy, k;
	int width, height;

	int pixel_red, pixel_grn, pixel_blu;

	IMGRGB *ppixel_src_top = NULL;
	IMGRGB *ppixel_src_ths = NULL;
	IMGRGB *ppixel_src_btm = NULL;
	IMGRGB *ppixel_src_tmp = NULL;
	IMGRGB *ppixel_dst_ths = NULL;
	
	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	for (y = 0; y < height; y++) {
		ppixel_src_top = (IMGRGB *)(&(pimgwrk->matrix[pimgwrk->sy+y-1][pimgwrk->sx*3]));
		ppixel_src_ths = (IMGRGB *)(&(pimgwrk->matrix[pimgwrk->sy+y  ][pimgwrk->sx*3]));
		ppixel_src_btm = (IMGRGB *)(&(pimgwrk->matrix[pimgwrk->sy+y+1][pimgwrk->sx*3]));
		ppixel_dst_ths = (IMGRGB *)(&(pimgdst->matrix[pimgdst->sy+y  ][pimgdst->sx*3]));

		for (x = 0; x < width; x++) {
			pixel_red = 0;
			pixel_grn = 0;
			pixel_blu = 0;
			for (k = 0, yy = -size/2; yy <= size/2; yy++) {
				ppixel_src_tmp = (IMGRGB *)((unsigned char *)&ppixel_src_ths[x] + yy*pimgwrk->buffWidth);

				for (xx = -size/2; xx <= size/2; xx++, k++) {
					pixel_red += kernel[k]*ppixel_src_tmp[xx].R;
					pixel_grn += kernel[k]*ppixel_src_tmp[xx].G;
					pixel_blu += kernel[k]*ppixel_src_tmp[xx].B;
				}
			}
			pixel_red /= divide;
			pixel_grn /= divide;
			pixel_blu /= divide;

			if (pixel_red > 0xff) pixel_red = 0xff;
			if (pixel_red < 0x00) pixel_red = 0x00;
			if (pixel_grn > 0xff) pixel_grn = 0xff;
			if (pixel_grn < 0x00) pixel_grn = 0x00;
			if (pixel_blu > 0xff) pixel_blu = 0xff;
			if (pixel_blu < 0x00) pixel_blu = 0x00;

			ppixel_dst_ths[x].R = (unsigned char)pixel_red;
			ppixel_dst_ths[x].G = (unsigned char)pixel_grn;
			ppixel_dst_ths[x].B = (unsigned char)pixel_blu;
		}
	}

	return NULL;
}
