/////////////////////////////////////////////////////////////////////
// changebrightex.c
//
// ��  ��: ���ȵ���
//
// ��  ��: 09:13 2004-12-18
//         00:22 2004-12-19
//         12:23 2005-06-19
//         16:27 2007-01-08
//         15:01 2015-01-13
//===================================================================

#include "_image.h"

static void * changebrightex8bit(IMAGE *pimgsrc, IMAGE *pimgdst, float amount);
static void * changebrightex24bit(IMAGE *pimgsrc, IMAGE *pimgdst, float amount);

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_ChangeBrightEx
//
// ��  ��: ���ȵ���
//
// ��  ��: IMAGE * const pimgsrc ԴӰ��
//         IMAGE * const pimgdst Ŀ��Ӱ��
//         float   const famount ���ڷ���
//
// ����ֵ: int
//===================================================================
EXPORT_C int IMAGE_ChangeBrightEx(IMAGE * const pimgsrc, IMAGE * const pimgdst, float const famount)
{
	int rcode = NO_ERROR;

	//
	// 0 ��ȫ���
	//
	assert(pimgsrc);
	assert(pimgdst);
	if (famount < 0.0f) { // Ҫ�������Ч
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
	// 1. ���ݲ�ͬɫ��ֱ���ò�ͬ����
	//
	switch (IMAGE_BITCOUNT(pimgsrc)) {
	case  8: changebrightex8bit(pimgsrc, pimgdst, famount); break;
	case 24: changebrightex24bit(pimgsrc, pimgdst, famount); break;
	default: rcode = ERR_BITCOUNT; goto RET;
	}

RET:
	return rcode;
}

static void * changebrightex8bit(IMAGE *pimgsrc, IMAGE *pimgdst, float amount)
{
	int x, y;
	int width, height;

	int i;
	int ibyte;
	float fbyte;

	unsigned char table[256];

	IMGGRAY *ppixel_src = NULL;
	IMGGRAY *ppixel_dst = NULL;

	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	for (i = 0; i < 256; i++) {
		fbyte = i * amount;
		ibyte = _float2int(fbyte);

		if (ibyte > 0xff) ibyte = 0xff;
		if (ibyte < 0x00) ibyte = 0x00;
		table[i] = (unsigned char)ibyte;
	}

	for (y = 0; y < height; y++) {
		ppixel_src = (IMGGRAY *)(&(pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->sx]));
		ppixel_dst = (IMGGRAY *)(&(pimgdst->matrix[pimgdst->sy+y][pimgdst->sx]));

		for (x = 0; x < width; x++) {
			ppixel_dst[x] = table[ppixel_src[x]];
		}
	}

	return NULL;
}

static void * changebrightex24bit(IMAGE *pimgsrc, IMAGE *pimgdst, float amount)
{
	int x, y;
	int width, height;

	int i;
	int ibyte;
	float fbyte;

	unsigned char table[256];

	IMGRGB *ppixel_src = NULL;
	IMGRGB *ppixel_dst = NULL;

	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	for (i = 0; i < 256; i++) {
		fbyte = i * amount;
		ibyte = _float2int(fbyte);

		if (ibyte > 0xff) ibyte = 0xff;
		if (ibyte < 0x00) ibyte = 0x00;
		table[i] = (unsigned char)ibyte;
	}

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
