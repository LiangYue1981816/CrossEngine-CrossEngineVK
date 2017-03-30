/////////////////////////////////////////////////////////////////////
// expandcontrast.c
//
// ��  ��: �Աȶȵ���
//
// ��  ��: 09:13 2004-12-18
//         00:22 2004-12-19
//         12:23 2005-06-19
//         21:52 2007-01-08
//         17:11 2015-01-16
//===================================================================

#include "_image.h"

static void * expandcontrast8bit(IMAGE *pimgsrc, IMAGE *pimgdst, int lower, int upper);
static void * expandcontrast24bit(IMAGE *pimgsrc, IMAGE *pimgdst, int lower, int upper);

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_ExpandContrast
//
// ��  ��: �Աȶȵ���
//
// ��  ��: IMAGE * const pimgsrc ԴӰ��
//         IMAGE * const pimgdst Ŀ��Ӱ��
//         int     const nlower  ���޷�ֵ
//         int     const nupper  ���޷�ֵ
//
// ����ֵ: int
//===================================================================
EXPORT_C int IMAGE_ExpandContrast(IMAGE * const pimgsrc, IMAGE * const pimgdst, int const nlower, int const nupper)
{
	int rcode = NO_ERROR;

	//
	// 0 ��ȫ���
	//
	assert(pimgsrc);
	assert(pimgdst);
	if (nlower < 0 || nupper > 255 || nlower >= nupper ) { // Ҫ�������Χ��0~255֮�䣬���� nupper > nlower
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
	while (i <= lower) table[i++] = 0;
	while (i <  upper) table[i++] = (unsigned char)((255 * (i - lower)) / diff);
	while (i <  256)   table[i++] = 255;

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
	while (i <= lower) table[i++] = 0;
	while (i <  upper) table[i++] = (unsigned char)((255 * (i - lower)) / diff);
	while (i <  256)   table[i++] = 255;

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
