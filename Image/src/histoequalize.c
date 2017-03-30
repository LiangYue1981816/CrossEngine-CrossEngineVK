/////////////////////////////////////////////////////////////////////
// histoequalize.c
//
// 功  能: 直方图均衡
//
// 修  订: 09:13 2004-12-18
//         00:22 2004-12-19
//         12:23 2005-06-19
//         10:54 2007-01-09
//         14:18 2007-01-16
//         13:20 2015-01-17
//===================================================================

#include "_image.h"

static void generategraymap(IMAGE * const pimage, unsigned char * const graymap);
static void generatergbmap(IMAGE * const pimage, unsigned char * const redmap, unsigned char * const grnmap, unsigned char * const blumap);

static void * histoequalize8bit(IMAGE *pimgsrc, IMAGE *pimgdst, unsigned char *graymap);
static void * histoequalize24bit(IMAGE *pimgsrc, IMAGE *pimgdst, unsigned char *redmap, unsigned char *grnmap, unsigned char *blumap);

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_Histoequalize
//
// 功  能: 直方图均衡
//
// 参  数: IMAGE * const pimgsrc 源影像
//         IMAGE * const pimgdst 目标影像
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_Histoequalize(IMAGE * const pimgsrc, IMAGE * const pimgdst)
{
	int rcode = NO_ERROR;

	unsigned char redmap[256];
	unsigned char grnmap[256];
	unsigned char blumap[256];
	unsigned char graymap[256];

	//
	// 0. 安全检查
	//
	assert(pimgsrc);
	assert(pimgdst);
	if (NULL == pimgsrc->data || NULL == pimgdst->data) { // 要求影像有效性
		rcode = ERR_IMAGE; goto RET;
	}
	if (IMAGE_BITCOUNT(pimgsrc) != IMAGE_BITCOUNT(pimgdst)) { // 要求原影像与目标影像色深相同
		rcode = ERR_BITCOUNT; goto RET;
	}
	if (0 == IMAGE_AREA_WIDTH(pimgsrc) || 0 == IMAGE_AREA_HEIGHT(pimgsrc)) { // 要求处理区域面积不能为零
		rcode = ERR_RANGE; goto RET;
	}
	if (IMAGE_AREA_WIDTH(pimgsrc) != IMAGE_AREA_WIDTH(pimgdst) || IMAGE_AREA_HEIGHT(pimgsrc) != IMAGE_AREA_HEIGHT(pimgdst)) { // 要求原影像与目标影像处理区域尺寸相同
		rcode = ERR_RANGE; goto RET;
	}

	//
	// 2 根据不同色深分别调用不同函数
	//
	switch (IMAGE_BITCOUNT(pimgsrc)) {
	case  8: generategraymap(pimgsrc, graymap); break;
	case 24: generatergbmap(pimgsrc, redmap, grnmap, blumap); break;
	default: rcode = ERR_BITCOUNT; goto RET;
	}

	switch (IMAGE_BITCOUNT(pimgsrc)) {
	case  8: histoequalize8bit(pimgsrc, pimgdst, graymap); break;
	case 24: histoequalize24bit(pimgsrc, pimgdst, redmap, grnmap, blumap); break;
	default: rcode = ERR_BITCOUNT; goto RET;
	}

RET:
	return rcode;
}

static void generategraymap(IMAGE * const pimage, unsigned char * const graymap)
{
	int i, j;
	int width, height;

	unsigned long temp, area;
	unsigned long graytab[256];

	width = IMAGE_AREA_WIDTH(pimage);
	height = IMAGE_AREA_HEIGHT(pimage);

	IMAGE_CalcHisto(pimage, graytab, graytab, graytab);

	area = width*height;
	for (i = 0; i < 256; i++) {
		temp = 0;

		for (j = 0; j <= i; j++) {
			temp += graytab[j];
		}

		graymap[i] = (unsigned char)((temp*510+area)/(area+area));
	}
}

static void generatergbmap(IMAGE * const pimage, unsigned char * const redmap, unsigned char * const grnmap, unsigned char * const blumap)
{
	int i, j;
	int width, height;

	unsigned long temp_red, temp_grn, temp_blu, area;
	unsigned long redtab[256], grntab[256], blutab[256];

	width = IMAGE_AREA_WIDTH(pimage);
	height = IMAGE_AREA_HEIGHT(pimage);

	IMAGE_CalcHisto(pimage, redtab, grntab, blutab);

	area = width*height;
	for (i = 0; i < 256; i++) {
		temp_red = 0;
		temp_grn = 0;
		temp_blu = 0;

		for (j = 0; j <= i; j++) {
			temp_red += redtab[j];
			temp_grn += grntab[j];
			temp_blu += blutab[j];
		}

		redmap[i] = (unsigned char)((temp_red*510+area)/(area+area));
		grnmap[i] = (unsigned char)((temp_grn*510+area)/(area+area));
		blumap[i] = (unsigned char)((temp_blu*510+area)/(area+area));
	}
}

static void * histoequalize8bit(IMAGE *pimgsrc, IMAGE *pimgdst, unsigned char *graymap)
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
			ppixel_dst[x] = graymap[ppixel_src[x]];
		}
	}

	return NULL;
}

static void * histoequalize24bit(IMAGE *pimgsrc, IMAGE *pimgdst, unsigned char *redmap, unsigned char *grnmap, unsigned char *blumap)
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
			ppixel_dst[x].R = redmap[ppixel_src[x].R];
			ppixel_dst[x].G = grnmap[ppixel_src[x].G];
			ppixel_dst[x].B = blumap[ppixel_src[x].B];
		}
	}

	return NULL;
}
