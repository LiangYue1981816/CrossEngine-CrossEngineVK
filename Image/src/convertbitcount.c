/////////////////////////////////////////////////////////////////////
// convertbitcount.c
//
// ��  ��: �ṩ�Ե�ɫ��Ӱ�����ɫ��Ӱ��ת��
//
// ע  ��: ����ͼ������
//
// ��  ��: 16:45 2004-12-06
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
// ��  ��: IMAGE_Convert1BitTo8Bit
//
// ��  ��: ��1-bitӰ��ת��Ϊ8-bitӰ��
//
// ��  ��: IMAGE * const pimg1bitsrc Դ  1-bitӰ��
//         IMAGE * const pimg8bitdst Ŀ��8-bitӰ��
//
// ����ֵ: int
//===================================================================
EXPORT_C int IMAGE_Convert1BitTo8Bit(IMAGE * const pimg1bitsrc, IMAGE * const pimg8bitdst)
{
	int i;
	RGBQUAD *pal8bit = NULL;

	//
	// 0. ��ȫ���
	//
	assert(pimg1bitsrc);
	assert(pimg8bitdst);
	if (NULL == pimg1bitsrc || NULL == pimg1bitsrc->data ||
		NULL == pimg8bitdst || NULL == pimg8bitdst->data) { // Ҫ��Ӱ����Ч��
		return ERR_IMAGE;
	}
	if (1 != IMAGE_BITCOUNT(pimg1bitsrc) || 8 != IMAGE_BITCOUNT(pimg8bitdst)) { // Ҫ��ԭӰ�������1bit,Ŀ��Ӱ�������8bit
		return ERR_BITCOUNT;
	}
	if (IMAGE_WIDTH(pimg1bitsrc) != IMAGE_WIDTH(pimg8bitdst) || IMAGE_HEIGHT(pimg1bitsrc) != IMAGE_HEIGHT(pimg8bitdst)) { // Ҫ��ԭӰ����Ŀ��Ӱ��ߴ���ͬ
		return ERR_RANGE;
	}

	//
	// 1. �� 8-bit Ŀ��Ӱ��Ĳ�ɫ��ɫ������Ϊ�ҽ׵�ɫ��
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
	// 2. �� 1-bit Ӱ��ת��Ϊ 8-bit �ҽ�Ӱ��
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
// ��  ��: IMAGE_Convert8BitTo24Bit
//
// ��  ��: ��8-bitӰ��ת��Ϊ24-bitӰ��
//
// ��  ��: IMAGE * const pimgsrc Դ  8-bitӰ��
//         IMAGE * const pimgdst Ŀ��24-bitӰ��
//
// ����ֵ: int
//===================================================================
EXPORT_C int IMAGE_Convert8BitTo24Bit(IMAGE * const pimg8bitsrc, IMAGE * const pimg24bitdst)
{
	//
	// 0. ��ȫ���
	//
	assert(pimg8bitsrc);
	assert(pimg24bitdst);
	if (NULL == pimg8bitsrc  || NULL == pimg8bitsrc->data ||
		NULL == pimg24bitdst || NULL == pimg24bitdst->data) { // Ҫ��Ӱ����Ч��
		return ERR_IMAGE;
	}
	if (8 != IMAGE_BITCOUNT(pimg8bitsrc) || 24 != IMAGE_BITCOUNT(pimg24bitdst)) { // Ҫ��ԭӰ�������8bit,Ŀ��Ӱ�������24bit
		return ERR_BITCOUNT;
	}
	if (IMAGE_WIDTH(pimg8bitsrc) != IMAGE_WIDTH(pimg24bitdst) || IMAGE_HEIGHT(pimg8bitsrc) != IMAGE_HEIGHT(pimg24bitdst)) { // Ҫ��ԭӰ����Ŀ��Ӱ��ߴ���ͬ
		return ERR_RANGE;
	}

	//
	// 1. �� 8-bit Ӱ��ת��Ϊ 24-bit Ӱ��
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
// ��  ��: _converttostdimage
//
// ��  ��: ��IMAGE Library����֧�ֵķǱ�׼Ӱ��ת��Ϊ֧�ֵ�Ӱ���ʽ
//
// ��  ��: IMAGE * const pimage
//
// ����ֵ: int
//
// ˵  ��: ������Ծ��ǽ�8-bit colorӰ��ת��Ϊ24-bitӰ��
//         �ú��������I/O����֮����ã��Ա�֤����IMAGE Library
//         ��Ӱ���ʽ����ȷ��
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
	// 0. ��ȫ���
	//
	assert(pimage);
	if (NULL == pimage || NULL == pimage->data) { // Ҫ��Ӱ����Ч��
		return ERR_IMAGE;
	}

	//
	// 2. ���ݲ�ͬɫ����в�ͬ����
	//
	switch (IMAGE_BITCOUNT(pimage)) {
	case 1: case 24: case 32: goto RET; // 1bit��24bit �����κδ���
	case 8: // ��8bit��ɫӰ��ת��Ϊ24bit��ɫӰ��
		//
		// 2.1. ͨ����� 8-bit Ӱ���ɫ���ж��Ƿ��ǻҽ�Ӱ��
		//      ����ǻҽ�Ӱ�����κδ���
		//      (ע�⣺����ٶ���ɫ��˳���� 0x00~0xff)
		//
		isgray = TRUE;
		pal8bit = pimage->bmpInfo->bmiColors;
		for (i = 0; i < 256; i++) {
			if (pal8bit->rgbRed	  != (unsigned char)(i) ||
				pal8bit->rgbGreen != (unsigned char)(i) ||
				pal8bit->rgbBlue  != (unsigned char)(i)) {
				isgray = FALSE; break; // 8-bit ��ɫӰ����ֹѭ��
			}

			pal8bit++;
		}
		if (isgray) goto RET; // �ҽ�Ӱ�����κδ���ֱ�ӷ���

		//
		// 2.2. �� 8-bit ��ɫӰ��ת��Ϊ 24-bit Ӱ��
		//
		rcode = IMAGE_AllocImage(&imgstd, IMAGE_WIDTH(pimage), IMAGE_HEIGHT(pimage), 24);
		if (rcode != NO_ERROR) goto RET;

		rcode = IMAGE_Convert8BitTo24Bit(pimage, &imgstd);
		if (rcode != NO_ERROR) goto ERR;

		IMAGE_FreeImage(pimage);
		memcpy(pimage, &imgstd, sizeof(IMAGE));

		break;
	default: rcode = ERR_BITCOUNT; goto RET; // ��֧�ֵ�ɫ��
	}


	goto RET;
ERR:
	IMAGE_FreeImage(&imgstd);
RET:
	return rcode;
}
