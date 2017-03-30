/////////////////////////////////////////////////////////////////////
// colortogray.c
//
// ��  ��: �ṩ��24-bitӰ��Ҷȴ������ݲ����Ĳ�ͬ��Ϊ��׼����ͨ����
//         ��ͨ������ͨ����
//
// ע  ��: ����ͼ������
//
// ��  ��: 16:45 2004-12-06
//         10:09 2004-12-13
//         00:22 2004-12-19
//         11:39 2005-06-19
//         21:30 2007-01-06
//         15:00 2007-01-07
//         20:53 2007-01-07
//         16:31 2015-01-13
//===================================================================

#include "_image.h"

static void * colortogray_std(IMAGE *pimgsrc, IMAGE *pimgdst);
static void * colortogray_red(IMAGE *pimgsrc, IMAGE *pimgdst);
static void * colortogray_grn(IMAGE *pimgsrc, IMAGE *pimgdst);
static void * colortogray_blu(IMAGE *pimgsrc, IMAGE *pimgdst);

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_ColorToGray
//
// ��  ��: ��ɫӰ�񵽻ҽ�Ӱ��
//
// ��  ��: IMAGE * const pimgsrc ԭӰ��ָ��
//         IMAGE * const pimgdst �ҽ�Ŀ��Ӱ��ָ��
//         int     const nflag   ת����ʽ
//
// ����ֵ: int
//===================================================================
EXPORT_C int IMAGE_ColorToGray(IMAGE * const pimgsrc, IMAGE * const pimgdst, int const nflag)
{
	int rcode = NO_ERROR;

	int i;
	RGBQUAD *pal8bit = NULL;

	//
	// 0. ��ȫ���
	//
	assert(pimgdst);
	assert(pimgsrc);
	if (NULL == pimgsrc || NULL == pimgsrc->data ||
		NULL == pimgdst || NULL == pimgdst->data) { // Ҫ��Ӱ����Ч��
		return ERR_IMAGE;
	}
	if (24 != IMAGE_BITCOUNT(pimgsrc) || 8 != IMAGE_BITCOUNT(pimgdst)) { // Ҫ��ԭӰ�������24-bit,Ŀ��Ӱ�������8-bit
		return ERR_BITCOUNT;
	}
	if (IMAGE_WIDTH(pimgsrc) != IMAGE_WIDTH(pimgdst) || IMAGE_HEIGHT(pimgsrc) != IMAGE_HEIGHT(pimgdst)) { // Ҫ��ԭӰ����Ŀ��Ӱ��������ߴ���ͬ
		return ERR_RANGE;
	}

	//
	// 1. ����Ŀ���ɫ��Ϊ�ҽ׵�ɫ��
	//
	pal8bit = pimgdst->bmpInfo->bmiColors;
	for (i = 0; i < 256; i++) {
		pal8bit->rgbRed	= (unsigned char)(i);
		pal8bit->rgbGreen = (unsigned char)(i);
		pal8bit->rgbBlue = (unsigned char)(i);
		pal8bit->rgbReserved = (unsigned char)(i);

		pal8bit++;
	}

	//
	// 2. 24-bit ��ɫӰ�� 8-bit �ҽ�Ӱ��
	//
	switch (nflag) {
	case GRAY_STD: colortogray_std(pimgsrc, pimgdst); break; // ��׼��ɫ->�Ҷ�
	case GRAY_RED: colortogray_red(pimgsrc, pimgdst); break; // ��ɫͨ��->�Ҷ�
	case GRAY_GRN: colortogray_grn(pimgsrc, pimgdst); break; // ��ɫͨ��->�Ҷ�
	case GRAY_BLU: colortogray_blu(pimgsrc, pimgdst); break; // ��ɫͨ��->�Ҷ�
	default: rcode = ERR_PARAMETER; goto RET;
	}

RET:
	return rcode;
}

static void * colortogray_std(IMAGE *pimgsrc, IMAGE *pimgdst)
{
	int x, y;
	int width, height;

	IMGRGB  *ppixel_src = NULL;
	IMGGRAY *ppixel_dst = NULL;

	width = IMAGE_WIDTH(pimgsrc);
	height = IMAGE_HEIGHT(pimgsrc);

	for (y = 0; y < height; y++) {
		ppixel_src = (IMGRGB *)(&(pimgsrc->matrix[y][0])); // 24-bit
		ppixel_dst = (IMGGRAY *)(&(pimgdst->matrix[y][0])); // 8-bit

		for (x = 0; x < width; x++) {
			ppixel_dst[x] = (30*ppixel_src[x].R + 59*ppixel_src[x].G + 11*ppixel_src[x].B)/100;
		}
	}

	return NULL;
}

static void * colortogray_red(IMAGE *pimgsrc, IMAGE *pimgdst)
{
	int x, y;
	int width, height;

	IMGRGB *ppixel_src = NULL;
	IMGGRAY *ppixel_dst = NULL;

	width = IMAGE_WIDTH(pimgsrc);
	height = IMAGE_HEIGHT(pimgsrc);

	for (y = 0; y < height; y++) {
		ppixel_src = (IMGRGB *)(&(pimgsrc->matrix[y][0])); // 24-bit
		ppixel_dst = (IMGGRAY *)(&(pimgdst->matrix[y][0])); // 8-bit

		for (x = 0; x < width; x++) {
			ppixel_dst[x] = ppixel_src[x].R;
		}
	}

	return NULL;
}

static void * colortogray_grn(IMAGE *pimgsrc, IMAGE *pimgdst)
{
	int x, y;
	int width, height;

	IMGRGB *ppixel_src = NULL;
	IMGGRAY *ppixel_dst = NULL;

	width = IMAGE_WIDTH(pimgsrc);
	height = IMAGE_HEIGHT(pimgsrc);

	for (y = 0; y < height; y++) {
		ppixel_src = (IMGRGB *)(&(pimgsrc->matrix[y][0])); // 24-bit
		ppixel_dst = (IMGGRAY *)(&(pimgdst->matrix[y][0])); // 8-bit

		for (x = 0; x < width; x++) {
			ppixel_dst[x] = ppixel_src[x].G;
		}
	}

	return NULL;
}

static void * colortogray_blu(IMAGE *pimgsrc, IMAGE *pimgdst)
{
	int x, y;
	int width, height;

	IMGRGB *ppixel_src = NULL;
	IMGGRAY *ppixel_dst = NULL;

	width = IMAGE_WIDTH(pimgsrc);
	height = IMAGE_HEIGHT(pimgsrc);

	for (y = 0; y < height; y++) {
		ppixel_src = (IMGRGB *)(&(pimgsrc->matrix[y][0])); // 24-bit
		ppixel_dst = (IMGGRAY *)(&(pimgdst->matrix[y][0])); // 8-bit

		for (x = 0; x < width; x++) {
			ppixel_dst[x] = ppixel_src[x].B;
		}
	}

	return NULL;
}
