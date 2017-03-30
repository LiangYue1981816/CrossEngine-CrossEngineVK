/////////////////////////////////////////////////////////////////////
// mirrorimage.c
//
// 功  能: 水平方向镜像影像
//
// 修  订: 16:45 2004-12-06
//         10:09 2004-12-13
//         00:22 2004-12-19
//         09:01 2004-12-19
//         11:39 2005-06-19
//         20:27 2007-01-07
//         21:08 2007-01-07
//         20:03 2015-01-19
//===================================================================

#include "_image.h"

static void * mirrorimage1bit(IMAGE *pimgsrc, IMAGE *pimgdst);
static void * mirrorimage8bit(IMAGE *pimgsrc, IMAGE *pimgdst);
static void * mirrorimage24bit(IMAGE *pimgsrc, IMAGE *pimgdst);

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_MirrorImage
//
// 功  能: 创建临时影像
//
// 参  数: IMAGE * const pimgsrc 源影像
//         IMAGE * const pimgdst 目标影像
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_MirrorImage(IMAGE * const pimgsrc, IMAGE * const pimgdst)
{
	int rcode = NO_ERROR;

	//
	// 0. 安全检查
	//
	assert(pimgsrc);
	assert(pimgdst);
	if (NULL == pimgsrc || NULL == pimgsrc->data ||
		NULL == pimgdst || NULL == pimgdst->data) { // 要求影像有效性
		return ERR_IMAGE;
	}
	if (IMAGE_BITCOUNT(pimgsrc) != IMAGE_BITCOUNT(pimgdst)) { // 要求原影像与目标影像色深相同
		return ERR_BITCOUNT;
	}
	if (IMAGE_AREA_WIDTH(pimgsrc) != IMAGE_AREA_WIDTH(pimgdst) || IMAGE_AREA_HEIGHT(pimgsrc) != IMAGE_AREA_HEIGHT(pimgdst)) { // 要求原影像与目标影像处理区域尺寸相同
		return ERR_RANGE;
	}

	//
	// 1. 根据不同色深分别调用不同函数
	//
	switch (IMAGE_BITCOUNT(pimgsrc)) {
	case  1: mirrorimage1bit(pimgsrc, pimgdst); break;
	case  8: mirrorimage8bit(pimgsrc, pimgdst); break;
	case 24: mirrorimage24bit(pimgsrc, pimgdst); break;
	default: rcode = ERR_BITCOUNT; goto RET;
	}

RET:
	return rcode;
}

static void * mirrorimage1bit(IMAGE *pimgsrc, IMAGE *pimgdst)
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

	// 原对原
	if (pimgsrc->data == pimgdst->data) {
		// 当同一个身子但同时有两个不同的头，并且头中描述
		// 的操作区域不同或者有交叉，会导致运算错误，这里反出错误。
		if (pimgsrc != pimgdst) goto RET;

		width = width/2; // 原对原，因此只做一半
		for (y = 0; y < height; y++) {
			ppixel_src = &pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->ex/8];
			ppixel_dst = &pimgdst->matrix[pimgdst->sy+y][pimgdst->sx/8];

			xsrcbit = pimgsrc->ex%8;
			xdstbit = pimgdst->sx%8;

			for (x = 0; x < width; x++) {
				if (0 == ((*ppixel_src) & offsetbits1[xsrcbit])) {
					// 根据目标设置原
					if (0 == ((*ppixel_dst) & offsetbits1[xdstbit]))
						*ppixel_src &= offsetbits0[xsrcbit]; // 设置为0
					else
						*ppixel_src |= offsetbits1[xsrcbit]; // 设置为1

					// 根据原设置目标
					*ppixel_dst &= offsetbits0[xdstbit];     // 设置为0
				}
				else {
					// 根据目标设置原
					if (0 == ((*ppixel_dst) & offsetbits1[xdstbit]))
						*ppixel_src &= offsetbits0[xsrcbit]; // 设置为0
					else
						*ppixel_src |= offsetbits1[xsrcbit]; // 设置为1

					// 根据原设置目标
					*ppixel_dst |= offsetbits1[xdstbit];     // 设置为1
				}

				xsrcbit--; if (-1 == xsrcbit) { ppixel_src--; xsrcbit = 7; }
				xdstbit++; if (8  == xdstbit) { ppixel_dst++; xdstbit = 0; }
			}
		}
	}
	// 原对目标
	else {
		for (y = 0; y < height; y++) {
			ppixel_src = &pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->ex/8];
			ppixel_dst = &pimgdst->matrix[pimgdst->sy+y][pimgdst->sx/8];

			xsrcbit = pimgsrc->ex%8;
			xdstbit = pimgdst->sx%8;

			for (x = 0; x < width; x++) {
				if (0 == ((*ppixel_src)&offsetbits1[xsrcbit]))
					*ppixel_dst &= offsetbits0[xdstbit]; // 设置为0
				else
					*ppixel_dst |= offsetbits1[xdstbit]; // 设置为1

				xsrcbit--; if (-1 == xsrcbit) { ppixel_src--; xsrcbit = 7; }
				xdstbit++; if (8  == xdstbit) { ppixel_dst++; xdstbit = 0; }
			}
		}
	}

RET:
	return NULL;
}

static void * mirrorimage8bit(IMAGE *pimgsrc, IMAGE *pimgdst)
{
	int x, y;
	int width, height;

	IMGGRAY *ppixel_src = NULL;
	IMGGRAY *ppixel_dst = NULL;

	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	// 原对原
	if (pimgsrc->data == pimgdst->data) {
		IMGGRAY pixel_tmp;

		// 当同一个身子但同时有两个不同的头，并且头中描述
		// 的操作区域不同或者有交叉，会导致运算错误，这里反出错误。
		if (pimgsrc != pimgdst) goto RET;

		width = width/2; // 原对原，因此只做一半
		for (y = 0; y < height; y++) {
			ppixel_src = (IMGGRAY *)(&pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->ex]);
			ppixel_dst = (IMGGRAY *)(&pimgdst->matrix[pimgdst->sy+y][pimgdst->sx]);

			for (x = 0; x < width; x++) {
				pixel_tmp = *ppixel_src; *ppixel_src = *ppixel_dst; *ppixel_dst = pixel_tmp;

				ppixel_src--;
				ppixel_dst++;
			}
		}
	}
	// 原对目标
	else {
		for (y = 0; y < height; y++) {
			ppixel_src = &pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->ex];
			ppixel_dst = &pimgdst->matrix[pimgdst->sy+y][pimgdst->sx];

			for (x = 0; x < width; x++) {
				*ppixel_dst = *ppixel_src;

				ppixel_src--;
				ppixel_dst++;
			}
		}
	}

RET:
	return NULL;
}

static void * mirrorimage24bit(IMAGE *pimgsrc, IMAGE *pimgdst)
{
	int x, y;
	int width, height;

	IMGRGB *ppixel_src = NULL;
	IMGRGB *ppixel_dst = NULL;

	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	// 原对原
	if (pimgsrc->data == pimgdst->data) {
		IMGRGB pixel_tmp;

		// 当同一个身子但同时有两个不同的头，并且头中描述
		// 的操作区域不同或者有交叉，会导致运算错误，这里反出错误。
		if (pimgsrc != pimgdst) goto RET;

		width = width/2; // 原对原，因此只做一半
		for (y = 0; y < height; y++) {
			ppixel_src = (IMGRGB *)(&pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->ex*3]);
			ppixel_dst = (IMGRGB *)(&pimgdst->matrix[pimgdst->sy+y][pimgdst->sx*3]);

			for (x = 0; x < width; x++) {
				pixel_tmp = *ppixel_src; *ppixel_src = *ppixel_dst; *ppixel_dst = pixel_tmp;

				ppixel_src--;
				ppixel_dst++;
			}
		}
	}
	// 原对目标
	else {
		for (y = 0; y < height; y++) {
			ppixel_src = (IMGRGB *)(&pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->ex*3]);
			ppixel_dst = (IMGRGB *)(&pimgdst->matrix[pimgdst->sy+y][pimgdst->sx*3]);

			for (x = 0; x < width; x++) {
				*ppixel_dst = *ppixel_src;

				ppixel_src--;
				ppixel_dst++;
			}
		}
	}

RET:
	return NULL;
}
