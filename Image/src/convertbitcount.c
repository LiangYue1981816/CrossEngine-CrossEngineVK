/////////////////////////////////////////////////////////////////////
// convertbitcount.c
//
// 功  能: 提供对底色深影像相高色深影像转换
//
// 注  意: 对整图操作。
//
// 修  订: 16:45 2004-12-06
//         10:09 2004-12-13
//         00:22 2004-12-19
//         09:01 2004-12-19
//         11:39 2005-06-19
//         22:59 2007-01-06
//         13:04 2007-01-07
//         20:42 2007-01-07
//         17:15 2015-01-13
//===================================================================

#include "_image.h"

static void * convert1bitto8bit(IMAGE *pimgsrc, IMAGE *pimgdst);
static void * convert8bitto24bit(IMAGE *pimgsrc, IMAGE *pimgdst);

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_Convert1BitTo8Bit
//
// 功  能: 将1-bit影像转换为8-bit影像
//
// 参  数: IMAGE * const pimg1bitsrc 源  1-bit影像
//         IMAGE * const pimg8bitdst 目标8-bit影像
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_Convert1BitTo8Bit(IMAGE * const pimg1bitsrc, IMAGE * const pimg8bitdst)
{
	int i;
	RGBQUAD *pal8bit = NULL;

	//
	// 0. 安全检查
	//
	assert(pimg1bitsrc);
	assert(pimg8bitdst);
	if (NULL == pimg1bitsrc || NULL == pimg1bitsrc->data ||
		NULL == pimg8bitdst || NULL == pimg8bitdst->data) { // 要求影像有效性
		return ERR_IMAGE;
	}
	if (1 != IMAGE_BITCOUNT(pimg1bitsrc) || 8 != IMAGE_BITCOUNT(pimg8bitdst)) { // 要求原影像必须是1bit,目标影像必须是8bit
		return ERR_BITCOUNT;
	}
	if (IMAGE_WIDTH(pimg1bitsrc) != IMAGE_WIDTH(pimg8bitdst) || IMAGE_HEIGHT(pimg1bitsrc) != IMAGE_HEIGHT(pimg8bitdst)) { // 要求原影像与目标影像尺寸相同
		return ERR_RANGE;
	}

	//
	// 1. 对 8-bit 目标影像的彩色调色板重置为灰阶调色板
	//
	pal8bit = pimg8bitdst->bmpInfo->bmiColors;
	for (i = 0; i < 256; i++) {
		pal8bit->rgbRed	= (unsigned char)(i);
		pal8bit->rgbGreen = (unsigned char)(i);
		pal8bit->rgbBlue = (unsigned char)(i);
		pal8bit->rgbReserved = (unsigned char)(i);

		pal8bit++;
	}

	//
	// 2. 将 1-bit 影像转换为 8-bit 灰阶影像
	//    0 -> 0, 1 -> 255
	//
	convert1bitto8bit(pimg1bitsrc, pimg8bitdst);

	return NO_ERROR;
}

static void * convert1bitto8bit(IMAGE *pimgsrc, IMAGE *pimgdst)
{
	int x, y;
	int width, height;
	
	int xsrcbit;

	unsigned char *ppixel_src = NULL;
	unsigned char *ppixel_dst = NULL;

	unsigned char const offsetbits1[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

	width = IMAGE_WIDTH(pimgsrc);
	height = IMAGE_HEIGHT(pimgsrc);

	for (y = 0; y < height; y++) {
		ppixel_src = &(pimgsrc->matrix[y][0]); // 1-bit
		ppixel_dst = &(pimgdst->matrix[y][0]); // 8-bit

		xsrcbit = 0;
		for (x = 0; x < width; x++) {
			*ppixel_dst = 0 == ((*ppixel_src) & offsetbits1[xsrcbit]) ? 0 : 255;
			 ppixel_dst++; xsrcbit++; if (8 == xsrcbit) { ppixel_src++; xsrcbit = 0; }
		}
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_Convert8BitTo24Bit
//
// 功  能: 将8-bit影像转换为24-bit影像
//
// 参  数: IMAGE * const pimgsrc 源  8-bit影像
//         IMAGE * const pimgdst 目标24-bit影像
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_Convert8BitTo24Bit(IMAGE * const pimg8bitsrc, IMAGE * const pimg24bitdst)
{
	//
	// 0. 安全检查
	//
	assert(pimg8bitsrc);
	assert(pimg24bitdst);
	if (NULL == pimg8bitsrc  || NULL == pimg8bitsrc->data ||
		NULL == pimg24bitdst || NULL == pimg24bitdst->data) { // 要求影像有效性
		return ERR_IMAGE;
	}
	if (8 != IMAGE_BITCOUNT(pimg8bitsrc) || 24 != IMAGE_BITCOUNT(pimg24bitdst)) { // 要求原影像必须是8bit,目标影像必须是24bit
		return ERR_BITCOUNT;
	}
	if (IMAGE_WIDTH(pimg8bitsrc) != IMAGE_WIDTH(pimg24bitdst) || IMAGE_HEIGHT(pimg8bitsrc) != IMAGE_HEIGHT(pimg24bitdst)) { // 要求原影像与目标影像尺寸相同
		return ERR_RANGE;
	}

	//
	// 1. 将 8-bit 影像转换为 24-bit 影像
	//
	convert8bitto24bit(pimg8bitsrc, pimg24bitdst);

	return NO_ERROR;
}

static void * convert8bitto24bit(IMAGE *pimgsrc, IMAGE *pimgdst)
{
	int x, y;
	int width, height;
	
	int index;

	RGBQUAD *pal8bit = NULL;

	IMGGRAY *ppixel_src = NULL;
	IMGRGB  *ppixel_dst = NULL;

	width = IMAGE_WIDTH(pimgsrc);
	height = IMAGE_HEIGHT(pimgsrc);

	pal8bit = pimgsrc->bmpInfo->bmiColors;

	for (y = 0; y < height; y++) {
		ppixel_src = (IMGGRAY *)(&(pimgsrc->matrix[y][0])); // 8-bit
		ppixel_dst = (IMGRGB *)(&(pimgdst->matrix[y][0])); // 24-bit

		for (x = 0; x < width; x++) {
			index = ppixel_src[x];

			ppixel_dst[x].R = pal8bit[index].rgbRed;
			ppixel_dst[x].G = pal8bit[index].rgbGreen;
			ppixel_dst[x].B = pal8bit[index].rgbBlue;
		}
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////
// 名  称: _converttostdimage
//
// 功  能: 将IMAGE Library所不支持的非标准影像转换为支持的影像格式
//
// 参  数: IMAGE * const pimage
//
// 返回值: int
//
// 说  明: 具体而言就是将8-bit color影像转换为24-bit影像
//         该函数在完成I/O动作之后调用，以保证传入IMAGE Library
//         中影像格式的正确性
//===================================================================
int _converttostdimage(IMAGE * const pimage)
{
	int rcode = NO_ERROR;

	int i;
	
	BOOL isgray;
	RGBQUAD *pal8bit = NULL;

	IMAGE imgstd;
	IMAGE_ZeroImage(&imgstd);

	//
	// 0. 安全检查
	//
	assert(pimage);
	if (NULL == pimage || NULL == pimage->data) { // 要求影像有效性
		return ERR_IMAGE;
	}

	//
	// 2. 根据不同色深进行不同处理
	//
	switch (IMAGE_BITCOUNT(pimage)) {
	case 1: case 24: case 32: goto RET; // 1bit，24bit 不做任何处理
	case 8: // 将8bit彩色影像转换为24bit彩色影像
		//
		// 2.1. 通过检查 8-bit 影像调色板判断是否是灰阶影像
		//      如果是灰阶影像不作任何处理
		//      (注意：这里假定调色板顺序是 0x00~0xff)
		//
		isgray = TRUE;
		pal8bit = pimage->bmpInfo->bmiColors;
		for (i = 0; i < 256; i++) {
			if (pal8bit->rgbRed	  != (unsigned char)(i) ||
				pal8bit->rgbGreen != (unsigned char)(i) ||
				pal8bit->rgbBlue  != (unsigned char)(i)) {
				isgray = FALSE; break; // 8-bit 彩色影像，终止循环
			}

			pal8bit++;
		}
		if (isgray) goto RET; // 灰阶影像不做任何处理，直接返回

		//
		// 2.2. 将 8-bit 彩色影像转换为 24-bit 影像
		//
		rcode = IMAGE_AllocImage(&imgstd, IMAGE_WIDTH(pimage), IMAGE_HEIGHT(pimage), 24);
		if (rcode != NO_ERROR) goto RET;

		rcode = IMAGE_Convert8BitTo24Bit(pimage, &imgstd);
		if (rcode != NO_ERROR) goto ERR;

		IMAGE_FreeImage(pimage);
		memcpy(pimage, &imgstd, sizeof(IMAGE));

		break;
	default: rcode = ERR_BITCOUNT; goto RET; // 不支持的色深
	}


	goto RET;
ERR:
	IMAGE_FreeImage(&imgstd);
RET:
	return rcode;
}
