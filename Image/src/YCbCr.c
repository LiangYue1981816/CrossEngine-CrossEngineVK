/////////////////////////////////////////////////////////////////////
// YCbCr.c
//
// ��  ��: �ṩ��24-bitӰ���YCbCr��RGB֮��Ļ���任
//
// ��  ��: 16:45 2004-12-06
//         10:09 2004-12-13
//         00:22 2004-12-19
//         11:39 2005-06-19
//         10:11 2007-01-08
//         09:27 2015-01-20
//===================================================================

#include "_image.h"

static void * RGB2YCbCr(IMAGE *prgbsrc, YCbCr **pycbcrdst);
static void * YCbCr2RGB(YCbCr **pycbcrsrc, IMAGE *prgbdst);

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_RGB2YCbCr
//
// ��  ��: 24bit RGBӰ��ת����ycbcr
//
// ��  ��: IMAGE  * const prgbsrc   RGBԭӰ��
//         YCbCr ** const pycbcrdst YCbCrĿ��Ӱ��
//
// ����ֵ: int
//
// ע  ��: ���ڶ�pycbcrdst��ά����洢���䷽���������ʲ���ʹ�ø���Ĳ���
//===================================================================
EXPORT_C int IMAGE_RGB2YCbCr(IMAGE * const prgbsrc, YCbCr ** const pycbcrdst)
{
	//
	// 0. ��ȫ���
	//
	assert(prgbsrc);
	assert(pycbcrdst);
	if (NULL == pycbcrdst) { // YCbCr���ݻ�����Ч
		return ERR_PARAMETER;
	}
	if (NULL == prgbsrc || NULL == prgbsrc->data) { // Ҫ��Ӱ����Ч��
		return ERR_IMAGE;
	}
	if (24 != IMAGE_BITCOUNT(prgbsrc)) { // Ҫ��ԭӰ�������24-bit
		return ERR_BITCOUNT;
	}

	//
	// 1. 24-bit RGB ת��Ϊ YCbCr
	//
	RGB2YCbCr(prgbsrc, pycbcrdst);
	
	return NO_ERROR;
}

static void * RGB2YCbCr(IMAGE *prgbsrc, YCbCr **pycbcrdst)
{
	int x, y;
	int width, height;

	IMGRGB *pRGB = NULL;
	YCbCr *pYCbCr = NULL;

	width = IMAGE_AREA_WIDTH(prgbsrc);
	height = IMAGE_AREA_HEIGHT(prgbsrc);

	for (y = 0; y < height; y++) {
		pRGB = (IMGRGB *)(&(prgbsrc->matrix[prgbsrc->sy+y][prgbsrc->sx*3]));

		for (x = 0; x < width; x++) {
			pYCbCr = &pycbcrdst[y][x];

			pYCbCr->Y  =  0.2990f*pRGB[x].R + 0.5870f*pRGB[x].G + 0.1140f*pRGB[x].B;
			pYCbCr->Cb = -0.1687f*pRGB[x].R - 0.3313f*pRGB[x].G + 0.5000f*pRGB[x].B + 128.0f;
			pYCbCr->Cr =  0.5000f*pRGB[x].R - 0.4187f*pRGB[x].G - 0.0813f*pRGB[x].B + 128.0f;
		}
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_YCbCr2RGB
//
// ��  ��: YCbCr ת���� 24bit RGB
//
// ��  ��: YCbCr ** const pycbcrsrc YCbCrԭӰ��
//         IMAGE  * const prgbdst   RGBĿ��Ӱ��
//
// ����ֵ: int
//
// ע  ��: ���ڶ�pycbcrsrc��ά����洢���䷽���������ʲ���ʹ�ø���Ĳ���
//===================================================================
EXPORT_C int IMAGE_YCbCr2RGB(YCbCr ** const pycbcrsrc, IMAGE * const prgbdst)
{
	//
	// 0. ��ȫ���
	//
	assert(prgbdst);
	assert(pycbcrsrc);
	if (NULL == pycbcrsrc) { // YCbCr���ݻ�����Ч
		return ERR_PARAMETER;
	}
	if (NULL == prgbdst || NULL == prgbdst->data) { // Ҫ��Ӱ����Ч��
		return ERR_IMAGE;
	}
	if (24 != IMAGE_BITCOUNT(prgbdst)) { // Ҫ��ԭӰ�������24-bit
		return ERR_BITCOUNT;
	}

	//
	// 1. YCbCr ת���� 24bit RGB
	//
	YCbCr2RGB(pycbcrsrc, prgbdst);
	
	return NO_ERROR;
}

static void * YCbCr2RGB(YCbCr **pycbcrsrc, IMAGE *prgbdst)
{
	int x, y;
	int width, height;

	int icolor;
	float fcolor;

	IMGRGB *pRGB = NULL;
	YCbCr *pYCbCr = NULL;

	width = IMAGE_AREA_WIDTH(prgbdst);
	height = IMAGE_AREA_HEIGHT(prgbdst);

	for (y = 0; y < height; y++) {
		pRGB = (IMGRGB *)(&(prgbdst->matrix[prgbdst->sy+y][prgbdst->sx*3]));

		for (x = 0; x < width; x++) {
			pYCbCr = &pycbcrsrc[y][x];

			fcolor = 1.00000f*pYCbCr->Y + 1.40200f*(pYCbCr->Cb-128.0f) + 0.00000f*(pYCbCr->Cr-128.0f) + 0.5f; icolor = _float2int(fcolor); if (icolor < 0) icolor = 0; if (icolor > 255) icolor = 255; pRGB[x].R = (unsigned char)icolor;
			fcolor = 1.00000f*pYCbCr->Y - 0.34414f*(pYCbCr->Cb-128.0f) - 0.71414f*(pYCbCr->Cr-128.0f) + 0.5f; icolor = _float2int(fcolor); if (icolor < 0) icolor = 0; if (icolor > 255) icolor = 255; pRGB[x].G = (unsigned char)icolor;
			fcolor = 1.00000f*pYCbCr->Y + 1.77200f*(pYCbCr->Cb-128.0f) + 0.00000f*(pYCbCr->Cr-128.0f) + 0.5f; icolor = _float2int(fcolor); if (icolor < 0) icolor = 0; if (icolor > 255) icolor = 255; pRGB[x].B = (unsigned char)icolor;
		}
	}

	return NULL;
}
