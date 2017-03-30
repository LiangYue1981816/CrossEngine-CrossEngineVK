/////////////////////////////////////////////////////////////////////
// blackwhite.c
//
// ��  ��: ��ֵ��
//
// ��  ��: 11:52 2004-12-26
//         12:23 2005-06-19
//         11:10 2007-01-09
//         14:26 2015-01-13
//===================================================================

#include "_image.h"

static void * blackwhite8bit(IMAGE *pimgsrc, IMAGE *pimgdst, int thresh);
static void * blackwhite24bit(IMAGE *pimgsrc, IMAGE *pimgdst, int thresh);

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_BlackWhite
//
// ��  ��: ��ֵ��
//
// ��  ��: IMAGE * const pimgsrc ԴӰ��
//         IMAGE * const pimgdst Ŀ��Ӱ��
//         int     const nthresh ��ֵ
//
// ����ֵ: int
//===================================================================
EXPORT_C int IMAGE_BlackWhite(IMAGE * const pimgsrc, IMAGE * const pimgdst, int const nthresh)
{
	int rcode = NO_ERROR;

	//
	// 0 ��ȫ���
	//
	assert(pimgsrc);
	assert(pimgdst);
	if (nthresh < 0 || nthresh > 255) { // Ҫ�������Ч
		rcode = ERR_PARAMETER; goto RET;
	}
	if (NULL == pimgsrc->data || NULL == pimgdst->data) { // Ҫ��Ӱ����Ч��
		rcode = ERR_IMAGE; goto RET;
	}
	if (IMAGE_BITCOUNT(pimgsrc) != IMAGE_BITCOUNT(pimgdst)) { // Ҫ��ԭӰ����Ŀ��Ӱ��ɫ����ͬ
		rcode = ERR_BITCOUNT; goto RET;
	}
	if (IMAGE_AREA_WIDTH(pimgsrc) != IMAGE_AREA_WIDTH(pimgdst) || IMAGE_AREA_HEIGHT(pimgsrc) != IMAGE_AREA_HEIGHT(pimgdst)) { // Ҫ��ԭӰ����Ŀ��Ӱ��������ߴ���ͬ
		rcode = ERR_RANGE; goto RET;
	}

	//
	// 1 ���ݲ�ͬɫ��ֱ���ò�ͬ����
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
