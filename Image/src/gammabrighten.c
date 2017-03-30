/////////////////////////////////////////////////////////////////////
// gammabrighten.c
//
// 功  能: gamma调节
//
// 修  订: 09:13 2004-12-18
//         00:22 2004-12-19
//         12:23 2005-06-19
//         22:01 2007-01-08
//         17:27 2015-01-16
//===================================================================

#include "_image.h"

static void * gammabrighten8bit(IMAGE *pimgsrc, IMAGE *pimgdst, float gamma);
static void * gammabrighten24bit(IMAGE *pimgsrc, IMAGE *pimgdst, float gamma);

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_GammaBrighten
//
// 功  能: gamma调节
//
// 参  数: IMAGE * const pimgsrc 源影像
//         IMAGE * const pimgdst 目标影像
//         float   const fgamma  gamma值
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_GammaBrighten(IMAGE * const pimgsrc, IMAGE * const pimgdst, float const fgamma)
{
	int rcode = NO_ERROR;

	//
	// 0 安全检查
	//
	assert(pimgsrc);
	assert(pimgdst);
	if (fgamma <= 0.0f) { // 要求参数有效
		rcode = ERR_PARAMETER; goto RET;
	}
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
	// 1. 根据不同色深分别调用不同函数
	//
	switch (IMAGE_BITCOUNT(pimgsrc)) {
	case  8: gammabrighten8bit(pimgsrc, pimgdst, fgamma); break;
	case 24: gammabrighten24bit(pimgsrc, pimgdst, fgamma); break;
	default: rcode = ERR_BITCOUNT; goto RET;
	}

RET:
	return rcode;
}

static void * gammabrighten8bit(IMAGE *pimgsrc, IMAGE *pimgdst, float gamma)
{
	int x, y;
	int width, height;

	int i;
	float fmaxvalue;

	int ipow;
	float fpow;

	unsigned char table[256];

	IMGGRAY *ppixel_src = NULL;
	IMGGRAY *ppixel_dst = NULL;

	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	fmaxvalue = (float)pow(255.0f, gamma)/255.0f;
	for (i = 0; i < 256; i++) {
		fpow = (float)pow((float)i, gamma)/fmaxvalue;
		ipow = _float2int(fpow);

		table[i] = (unsigned char)min(ipow, 255);
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

static void * gammabrighten24bit(IMAGE *pimgsrc, IMAGE *pimgdst, float gamma)
{
	int x, y;
	int width, height;

	int i;
	float fmaxvalue;

	int ipow;
	float fpow;

	unsigned char table[256];

	IMGRGB *ppixel_src = NULL;
	IMGRGB *ppixel_dst = NULL;

	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	fmaxvalue = (float)pow(255.0f, gamma)/255.0f;
	for (i = 0; i < 256; i++) {
		fpow = (float)pow((float)i, gamma)/fmaxvalue;
		ipow = _float2int(fpow);
		table[i] = (unsigned char)min(ipow, 255);
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
