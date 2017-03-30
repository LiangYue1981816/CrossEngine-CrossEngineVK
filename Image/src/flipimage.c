/////////////////////////////////////////////////////////////////////
// flipimage.c
//
// 功  能: 纵方向镜像影像
//
// 修  订: 16:45 2004-12-06
//         10:09 2004-12-13
//         00:22 2004-12-19
//         11:39 2005-06-19
//         23:14 2007-01-06
//         20:23 2007-01-07
//         21:03 2007-01-07
//         17:19 2015-01-16
//===================================================================

#include "_image.h"

static void * flipimage1bit(IMAGE *pimgsrc, IMAGE *pimgdst);
static void * flipimage8bit(IMAGE *pimgsrc, IMAGE *pimgdst);
static void * flipimage24bit(IMAGE *pimgsrc, IMAGE *pimgdst);

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_FlipImage
//
// 功  能: 创建临时影像
//
// 参  数: IMAGE * const pimgsrc 源影像
//         IMAGE * const pimgdst 目标影像
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_FlipImage(IMAGE * const pimgsrc, IMAGE * const pimgdst)
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
	case  1: flipimage1bit(pimgsrc, pimgdst); break;
	case  8: flipimage8bit(pimgsrc, pimgdst); break;
	case 24: flipimage24bit(pimgsrc, pimgdst); break;
	default: rcode = ERR_BITCOUNT; goto RET;
	}

RET:
	return rcode;
}

static void * flipimage1bit(IMAGE *pimgsrc, IMAGE *pimgdst)
{
	int x, y;
	int width, height;

	unsigned char *ppixel_src = NULL;
	unsigned char *ppixel_dst = NULL;

	unsigned char const offsetbits0[8] = {0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0xfe};
	unsigned char const offsetbits1[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	// 原对原
	if (pimgsrc->data == pimgdst->data) {
		int xbit;

		// 当同一个身子但同时有两个不同的头，并且头中描述
		// 的操作区域不同或者有交叉，会导致运算错误，这里反出错误。
		if (pimgsrc != pimgdst) goto RET;

		height = height/2; // 原对原所以只做一半
		for (y = 0; y < height; y++) {
			ppixel_src = &(pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->sx/8]);
			ppixel_dst = &(pimgdst->matrix[pimgdst->ey-y][pimgdst->sx/8]);

			xbit = pimgsrc->sx%8;

			for (x = 0; x < width; x++) {
				if (0 == ((*ppixel_src) & offsetbits1[xbit])) {
					// 根据目标设置原
					if (0 == ((*ppixel_dst) & offsetbits1[xbit]))
						*ppixel_src &= offsetbits0[xbit]; // 设置为0
					else
						*ppixel_src |= offsetbits1[xbit]; // 设置为1

					// 根据原设置目标
					*ppixel_dst &= offsetbits0[xbit];     // 设置为0
				}
				else {
					// 根据目标设置原
					if (0 == ((*ppixel_dst) & offsetbits1[xbit]))
						*ppixel_src &= offsetbits0[xbit]; // 设置为0
					else
						*ppixel_src |= offsetbits1[xbit]; // 设置为1

					// 根据原设置目标
					*ppixel_dst |= offsetbits1[xbit];     // 设置为1
				}

				xbit++; if (8 == xbit) { ppixel_src++; ppixel_dst++; xbit = 0; }
			}
		}
	}
	// 原对目标
	else {
		int xsrcbit, xdstbit;

		for (y = 0; y < height; y++) {
			ppixel_src = &(pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->sx/8]);
			ppixel_dst = &(pimgdst->matrix[pimgdst->ey-y][pimgdst->sx/8]);

			xsrcbit = pimgsrc->sx%8;
			xdstbit = pimgdst->sx%8;

			for (x = 0; x < width; x++) {
				if (0 == ((*ppixel_src) & offsetbits1[xsrcbit]))
					*ppixel_dst &= offsetbits0[xdstbit]; // 设置为0
				else
					*ppixel_dst |= offsetbits1[xdstbit]; // 设置为1

				xsrcbit++; if (8 == xsrcbit) { ppixel_src++; xsrcbit = 0; }
				xdstbit++; if (8 == xdstbit) { ppixel_dst++; xdstbit = 0; }
			}
		}
	}

RET:
	return NULL;
}

static void * flipimage8bit(IMAGE *pimgsrc, IMAGE *pimgdst)
{
	int y;
	int width, height;

	IMGGRAY *ppixel_tmp = NULL;
	IMGGRAY *ppixel_src = NULL;
	IMGGRAY *ppixel_dst = NULL;

	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	// 原对原
	if (pimgsrc->data == pimgdst->data) {
		// 当同一个身子但同时有两个不同的头，并且头中描述
		// 的操作区域不同或者有交叉，会导致运算错误，这里反出错误。
		if (pimgsrc != pimgdst) goto RET;

		ppixel_tmp = (IMGGRAY *)calloc(width, sizeof(unsigned char));
		if (NULL == ppixel_tmp) goto RET;

		height = height/2; // 原对原所以只做一半
		for (y = 0; y < height; y++) {
			ppixel_src = (IMGGRAY *)(&(pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->sx]));
			ppixel_dst = (IMGGRAY *)(&(pimgdst->matrix[pimgdst->ey-y][pimgdst->sx]));

			memcpy(ppixel_tmp, ppixel_src, width);
			memcpy(ppixel_src, ppixel_dst, width);
			memcpy(ppixel_dst, ppixel_tmp, width);
		}

		free(ppixel_tmp);
		ppixel_tmp = NULL;
	}
	// 原对目标
	else {
		for (y = 0; y < height; y++) {
			ppixel_src = (IMGGRAY *)(&(pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->sx]));
			ppixel_dst = (IMGGRAY *)(&(pimgdst->matrix[pimgdst->ey-y][pimgdst->sx]));

			memcpy(ppixel_dst, ppixel_src, width);
		}
	}

RET:
	if (ppixel_tmp) free(ppixel_tmp);
	return NULL;
}

static void * flipimage24bit(IMAGE *pimgsrc, IMAGE *pimgdst)
{
	int y;
	int width, height;

	IMGRGB *ppixel_tmp = NULL;
	IMGRGB *ppixel_src = NULL;
	IMGRGB *ppixel_dst = NULL;

	width = IMAGE_AREA_WIDTH(pimgsrc)*3;
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	// 原对原
	if (pimgsrc->data == pimgdst->data) {
		// 当同一个身子但同时有两个不同的头，并且头中描述
		// 的操作区域不同或者有交叉，会导致运算错误，这里反出错误。
		if (pimgsrc != pimgdst) goto RET;

		ppixel_tmp = (IMGRGB *)calloc(width, sizeof(unsigned char));
		if (NULL == ppixel_tmp) goto RET;

		height = height/2; // 原对原所以只做一半
		for (y = 0; y < height; y++) {
			ppixel_src = (IMGRGB *)(&(pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->sx*3]));
			ppixel_dst = (IMGRGB *)(&(pimgdst->matrix[pimgdst->ey-y][pimgdst->sx*3]));

			memcpy(ppixel_tmp, ppixel_src, width);
			memcpy(ppixel_src, ppixel_dst, width);
			memcpy(ppixel_dst, ppixel_tmp, width);
		}

		free(ppixel_tmp);
		ppixel_tmp = NULL;
	}
	// 原对目标
	else {
		for (y = 0; y < height; y++) {
			ppixel_src = (IMGRGB *)(&(pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->sx*3]));
			ppixel_dst = (IMGRGB *)(&(pimgdst->matrix[pimgdst->ey-y][pimgdst->sx*3]));

			memcpy(ppixel_dst, ppixel_src, width);
		}
	}

RET:
	if (ppixel_tmp) free(ppixel_tmp);
	return NULL;
}
