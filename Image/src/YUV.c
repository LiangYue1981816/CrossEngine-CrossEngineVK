/////////////////////////////////////////////////////////////////////
// YUV.c
//
// ��  ��: �ṩ��24-bitӰ���YUV��RGB֮��Ļ���任
//
// ��  ��: 16:45 2004-12-06
//         10:09 2004-12-13
//         00:22 2004-12-19
//         11:39 2005-06-19
//         10:07 2007-01-08
//         09:35 2015-01-20
//===================================================================

#include "_image.h"

static void * RGB2YUV(IMAGE *prgbsrc, YUV **pyuvdst);
static void * YUV2RGB(YUV **pyuvsrc, IMAGE *prgbdst);

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_RGB2YUV
//
// ��  ��: 24bit RGBӰ��ת����YUV
//
// ��  ��: IMAGE * const prgbsrc RGBԭӰ��
//         YUV  ** const pyuvdst YUVĿ��Ӱ��
//
// ����ֵ: int
//
// ע  ��: ���ڶ�pyuvdst��ά����洢���䷽���������ʲ���ʹ�ø���Ĳ���
//===================================================================
EXPORT_C int IMAGE_RGB2YUV(IMAGE * const prgbsrc, YUV ** const pyuvdst)
{
	//
	// 0. ��ȫ���
	//
	assert(prgbsrc);
	assert(pyuvdst);
	if (NULL == pyuvdst) { // YUV���ݻ�����Ч
		return ERR_PARAMETER;
	}
	if (NULL == prgbsrc->data) { // Ҫ��Ӱ����Ч��
		return ERR_IMAGE;
	}
	if (24 != IMAGE_BITCOUNT(prgbsrc)) { // Ҫ��ԭӰ�������24-bit
		return ERR_BITCOUNT;
	}

	//
	// 1. 24-bit RGB ת��Ϊ YUV
	//
	RGB2YUV(prgbsrc, pyuvdst);

	return NO_ERROR;
}

static void * RGB2YUV(IMAGE *prgbsrc, YUV **pyuvdst)
{
	int x, y;
	int width, height;

	YUV *pYUV = NULL;
	IMGRGB *pRGB = NULL;

	width = IMAGE_AREA_WIDTH(prgbsrc);
	height = IMAGE_AREA_HEIGHT(prgbsrc);

	for (y = 0; y < height; y++) {
		pRGB = (IMGRGB *)(&(prgbsrc->matrix[prgbsrc->sy+y][prgbsrc->sx*3]));

		for (x = 0; x < width; x++) {
			pYUV = &pyuvdst[y][x];

			pYUV->Y =  0.299f*pRGB[x].R + 0.587f*pRGB[x].G + 0.114f*pRGB[x].B;
			pYUV->U = -0.148f*pRGB[x].R - 0.289f*pRGB[x].G + 0.437f*pRGB[x].B;
			pYUV->V =  0.615f*pRGB[x].R - 0.515f*pRGB[x].G - 0.100f*pRGB[x].B;
		}
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_YUV2RGB
//
// ��  ��: YUV ת���� 24bit RGB
//
// ��  ��: YUV  ** const pyuvsrc YUVԭӰ��
//         IMAGE * const prgbdst RGBĿ��Ӱ��
//
// ����ֵ: int
//
// ע  ��: ���ڶ�prgbdst��ά����洢���䷽���������ʲ���ʹ�ø���Ĳ���
//===================================================================
EXPORT_C int IMAGE_YUV2RGB(YUV ** const pyuvsrc, IMAGE * const prgbdst)
{
	//
	// 0. ��ȫ���
	//
	assert(pyuvsrc);
	assert(prgbdst);
	if (NULL == pyuvsrc) { // YUV������Ч
		return ERR_PARAMETER;
	}
	if (NULL == prgbdst->data) { // Ҫ��Ӱ����Ч��
		return ERR_IMAGE;
	}
	if (24 != IMAGE_BITCOUNT(prgbdst)) { // Ҫ��ԭӰ�������24-bit
		return ERR_BITCOUNT;
	}

	//
	// 1. YUV ת���� 24bit RGB
	//
	YUV2RGB(pyuvsrc, prgbdst);

	return NO_ERROR;
}

static void * YUV2RGB(YUV **pyuvsrc, IMAGE *prgbdst)
{
	int x, y;
	int width, height;

	int icolor;
	float fcolor;

	YUV *pYUV = NULL;
	IMGRGB *pRGB = NULL;

	width = IMAGE_AREA_WIDTH(prgbdst);
	height = IMAGE_AREA_HEIGHT(prgbdst);

	for (y = 0; y < height; y++) {
		pRGB = (IMGRGB *)(&(prgbdst->matrix[prgbdst->sy+y][prgbdst->sx*3]));

		for (x = 0; x < width; x++) {
			pYUV = &pyuvsrc[y][x];

			fcolor = 1.000f*pYUV->Y + 0.000f*pYUV->U + 1.140f*pYUV->V + 0.5f; icolor = _float2int(fcolor); if (icolor < 0) icolor = 0; if (icolor > 255) icolor = 255; pRGB[x].R = (unsigned char)icolor;
			fcolor = 1.000f*pYUV->Y - 0.395f*pYUV->U - 0.581f*pYUV->V + 0.5f; icolor = _float2int(fcolor); if (icolor < 0) icolor = 0; if (icolor > 255) icolor = 255; pRGB[x].G = (unsigned char)icolor;
			fcolor = 1.000f*pYUV->Y + 2.032f*pYUV->U + 0.000f*pYUV->V + 0.5f; icolor = _float2int(fcolor); if (icolor < 0) icolor = 0; if (icolor > 255) icolor = 255; pRGB[x].B = (unsigned char)icolor;
		}
	}

	return NULL;
}
