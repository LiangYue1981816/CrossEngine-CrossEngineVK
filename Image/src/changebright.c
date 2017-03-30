/////////////////////////////////////////////////////////////////////
// changebright.c
//
// ��  ��: ���ȵ���
//
// ��  ��: 09:13 2004-12-18
//         00:22 2004-12-19
//         12:23 2005-06-19
//         16:17 2007-01-08
//         14:56 2015-01-13
//===================================================================

#include "_image.h"

static void * changebright8bit(IMAGE *pimgsrc, IMAGE *pimgdst, int amount);
static void * changebright24bit(IMAGE *pimgsrc, IMAGE *pimgdst, int amount);

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_ChangeBright
//
// ��  ��: ���ȵ���
//
// ��  ��: IMAGE * const pimgsrc ԴӰ��
//         IMAGE * const pimgdst Ŀ��Ӱ��
//         int     const namount ����ֵ
//
// ����ֵ: int
//===================================================================
EXPORT_C int IMAGE_ChangeBright(IMAGE * const pimgsrc, IMAGE * const pimgdst, int const namount)
{
	int rcode = NO_ERROR;

	//
	// 0 ��ȫ���
	//
	assert(pimgsrc);
	assert(pimgdst);
	if (namount < -255 || namount > 255) { // Ҫ�������Ч
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
	case  8: changebright8bit(pimgsrc, pimgdst, namount); break;
	case 24: changebright24bit(pimgsrc, pimgdst, namount); break;
	default: rcode = ERR_BITCOUNT; goto RET;
	}

RET:
	return rcode;
}

static void * changebright8bit(IMAGE *pimgsrc, IMAGE *pimgdst, int amount)
{
	int x, y;
	int width, height;

	int i, byte;
	unsigned char table[256];

	IMGGRAY *ppixel_src = NULL;
	IMGGRAY *ppixel_dst = NULL;

	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	for (i = 0; i < 256; i++) {
		byte = i + amount;
		if (byte > 0xff) byte = 0xff;
		if (byte < 0x00) byte = 0x00;
		table[i] = (unsigned char)byte;
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

static void * changebright24bit(IMAGE *pimgsrc, IMAGE *pimgdst, int amount)
{
	int x, y;
	int width, height;

	int i, byte;
	unsigned char table[256];

	IMGRGB *ppixel_src = NULL;
	IMGRGB *ppixel_dst = NULL;

	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	for (i = 0; i < 256; i++) {
		byte = i + amount;
		if (byte > 0xff) byte = 0xff;
		if (byte < 0x00) byte = 0x00;
		table[i] = (unsigned char)byte;
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
