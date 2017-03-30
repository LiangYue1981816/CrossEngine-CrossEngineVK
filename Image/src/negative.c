/////////////////////////////////////////////////////////////////////
// negative.c
//
// 功  能: 反色
//
// 修  订: 11:52 2004-12-26
//         12:23 2005-06-19
//         11:03 2007-01-09
//         20:08 2015-01-19
//===================================================================

#include "_image.h"

static void * negative1bit(IMAGE *pimgsrc, IMAGE *pimgdst);
static void * negative8bit(IMAGE *pimgsrc, IMAGE *pimgdst);
static void * negative24bit(IMAGE *pimgsrc, IMAGE *pimgdst);

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_Negative
//
// 功  能: 反色
//
// 参  数: IMAGE * const pimgsrc 源影像
//         IMAGE * const pimgdst 目标影像
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_Negative(IMAGE * const pimgsrc, IMAGE * const pimgdst)
{
	int rcode = NO_ERROR;

	//
	// 0 安全检查
	//
	assert(pimgsrc);
	assert(pimgdst);
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
	case  1: negative1bit(pimgsrc, pimgdst); break;
	case  8: negative8bit(pimgsrc, pimgdst); break;
	case 24: negative24bit(pimgsrc, pimgdst); break;
	default: rcode = ERR_BITCOUNT; goto RET;
	}

RET:
	return rcode;
}

static void * negative1bit(IMAGE *pimgsrc, IMAGE *pimgdst)
{
	int x, y;
	int width, height;

	int xsrcbit, xdstbit;

	unsigned char *ppixel_src = NULL;
	unsigned char *ppixel_dst = NULL;

	unsigned char const offsetbits0[8] = {0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0xfe};
	unsigned char const offsetbits1[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	for (y = 0; y < height; y++) {
		ppixel_src = (unsigned char *)(&(pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->sx/8]));
		ppixel_dst = (unsigned char *)(&(pimgdst->matrix[pimgdst->sy+y][pimgdst->sx/8]));

		xsrcbit = pimgsrc->sx%8;
		xdstbit = pimgdst->sx%8;

		for (x = 0; x < width; x++) {
			if (0 != ((*ppixel_src) & offsetbits1[xsrcbit]))
				*ppixel_dst &= offsetbits0[xdstbit]; // 设置为0
			else
				*ppixel_dst |= offsetbits1[xdstbit]; // 设置为1

			xsrcbit++; if (8 == xsrcbit) { ppixel_src++; xsrcbit = 0; }
			xdstbit++; if (8 == xdstbit) { ppixel_dst++; xdstbit = 0; }
		}
	}

	return NULL;
}

static void * negative8bit(IMAGE *pimgsrc, IMAGE *pimgdst)
{
	int x, y;
	int width, height;

	IMGGRAY *ppixel_src = NULL;
	IMGGRAY *ppixel_dst = NULL;

	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	for (y = 0; y < height; y++) {
		ppixel_src = (IMGGRAY *)(&(pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->sx]));
		ppixel_dst = (IMGGRAY *)(&(pimgdst->matrix[pimgdst->sy+y][pimgdst->sx]));

		for (x = 0; x < width; x++) {
			ppixel_dst[x] = 0xff - ppixel_src[x];
		}
	}

	return NULL;
}

static void * negative24bit(IMAGE *pimgsrc, IMAGE *pimgdst)
{
	int x, y;
	int width, height;

	IMGRGB *ppixel_src = NULL;
	IMGRGB *ppixel_dst = NULL;

	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	for (y = 0; y < height; y++) {
		ppixel_src = (IMGRGB *)(&(pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->sx*3]));
		ppixel_dst = (IMGRGB *)(&(pimgdst->matrix[pimgdst->sy+y][pimgdst->sx*3]));

		for (x = 0; x < width; x++) {
			ppixel_dst[x].R = 0xff - ppixel_src[x].R;
			ppixel_dst[x].G = 0xff - ppixel_src[x].G;
			ppixel_dst[x].B = 0xff - ppixel_src[x].B;
		}
	}

	return NULL;
}
