/////////////////////////////////////////////////////////////////////
// YIQ.c
//
// ��  ��: �ṩ��24-bitӰ���YIQ��RGB֮��Ļ���任
//
// ��  ��: 16:45 2004-12-06
//         10:09 2004-12-13
//         00:22 2004-12-19
//         11:39 2005-06-19
//         09:38 2007-01-08
//         09:31 2015-01-20
//===================================================================

#include "_image.h"

static void * RGB2YIQ(IMAGE *prgbsrc, YIQ **pyiqdst);
static void * YIQ2RGB(YIQ **pyiqsrc, IMAGE *prgbdst);

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_RGB2YIQ
//
// ��  ��: 24bit RGBӰ��ת����YIQ
//
// ��  ��: IMAGE * const prgbsrc RGBԭӰ��
//         YIQ  ** const pyuvdst YIQĿ��Ӱ��
//
// ����ֵ: int
//
// ע  ��: ���ڶ�pyiqdst��ά����洢���䷽���������ʲ���ʹ�ø���Ĳ���
//===================================================================
EXPORT_C int IMAGE_RGB2YIQ(IMAGE * const prgbsrc, YIQ ** const pyiqdst)
{
	//
	// 0. ��ȫ���
	//
	assert(prgbsrc);
	assert(pyiqdst);
	if (NULL == pyiqdst) { // YIQ���ݻ�����Ч
		return ERR_PARAMETER;
	}
	if (NULL == prgbsrc->data) { // Ҫ��Ӱ����Ч��
		return ERR_IMAGE;
	}
	if (24 != IMAGE_BITCOUNT(prgbsrc)) { // Ҫ��ԭӰ�������24-bit
		return ERR_BITCOUNT;
	}

	//
	// 1. 24-bit RGB ת��Ϊ YIQ
	//
	RGB2YIQ(prgbsrc, pyiqdst);

	return NO_ERROR;
}

static void * RGB2YIQ(IMAGE *prgbsrc, YIQ **pyiqdst)
{
	int x, y;
	int width, height;

	YIQ *pYIQ = NULL;
	IMGRGB *pRGB = NULL;

	width = IMAGE_AREA_WIDTH(prgbsrc);
	height = IMAGE_AREA_HEIGHT(prgbsrc);

	for (y = 0; y < height; y++) {
		pRGB = (IMGRGB *)(&(prgbsrc->matrix[prgbsrc->sy+y][prgbsrc->sx*3]));

		for (x = 0; x < width; x++) {
			pYIQ = &(pyiqdst[y][x]);

			pYIQ->Y =  0.299f*pRGB[x].R + 0.587f*pRGB[x].G + 0.114f*pRGB[x].B;
			pYIQ->I =  0.596f*pRGB[x].R - 0.274f*pRGB[x].G - 0.322f*pRGB[x].B;
			pYIQ->Q =  0.211f*pRGB[x].R - 0.523f*pRGB[x].G + 0.312f*pRGB[x].B;
		}
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_YIQ2RGB
//
// ��  ��: YIQ ת���� 24bit RGB
//
// ��  ��: YIQ  ** const pyuvsrc YIQԭӰ��
//         IMAGE * const prgbdst RGBĿ��Ӱ��
//
// ����ֵ: int
//
// ע  ��: ���ڶ�pyiqsrc��ά����洢���䷽���������ʲ���ʹ�ø���Ĳ���
//===================================================================
EXPORT_C int IMAGE_YIQ2RGB(YIQ ** const pyiqsrc, IMAGE * const prgbdst)
{
	//
	// 0. ��ȫ���
	//
	assert(pyiqsrc);
	assert(prgbdst);
	if (NULL == pyiqsrc) { // YIQ���ݻ�����Ч
		return ERR_PARAMETER;
	}
	if (NULL == prgbdst->data) { // Ҫ��Ӱ����Ч��
		return ERR_IMAGE;
	}
	if (24 != IMAGE_BITCOUNT(prgbdst)) { // Ҫ��ԭӰ�������24-bit
		return ERR_BITCOUNT;
	}

	//
	// 1. YIQ ת���� 24bit RGB
	//
	YIQ2RGB(pyiqsrc, prgbdst);
	
	return NO_ERROR;
}

static void * YIQ2RGB(YIQ **pyiqsrc, IMAGE *prgbdst)
{
	int x, y;
	int width, height;

	int icolor;
	float fcolor;

	YIQ *pYIQ = NULL;
	IMGRGB *pRGB = NULL;

	width = IMAGE_AREA_WIDTH(prgbdst);
	height = IMAGE_AREA_HEIGHT(prgbdst);

	for (y = 0; y < height; y++) {
		pRGB = (IMGRGB *)(&(prgbdst->matrix[prgbdst->sy+y][prgbdst->sx*3]));

		for (x = 0; x < width; x++) {
			pYIQ = &pyiqsrc[y][x];

			fcolor = 1.000f*pYIQ->Y + 0.956f*pYIQ->I + 0.621f*pYIQ->Q + 0.5f; icolor = _float2int(fcolor); if (icolor < 0) icolor = 0; if (icolor > 255) icolor = 255; pRGB[x].R = (unsigned char)icolor;
			fcolor = 1.000f*pYIQ->Y - 0.272f*pYIQ->I - 0.647f*pYIQ->Q + 0.5f; icolor = _float2int(fcolor); if (icolor < 0) icolor = 0; if (icolor > 255) icolor = 255; pRGB[x].G = (unsigned char)icolor;
			fcolor = 1.000f*pYIQ->Y - 1.106f*pYIQ->I - 1.703f*pYIQ->Q + 0.5f; icolor = _float2int(fcolor); if (icolor < 0) icolor = 0; if (icolor > 255) icolor = 255; pRGB[x].B = (unsigned char)icolor;
		}
	}

	return NULL;
}
