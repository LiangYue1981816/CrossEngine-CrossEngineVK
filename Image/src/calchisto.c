/////////////////////////////////////////////////////////////////////
// calchisto.c
//
// ��  ��: ͳ��ֱ��ͼ
//
// ��  ��: 09:13 2004-12-18
//         00:22 2004-12-19
//         12:23 2005-06-19
//===================================================================

#include "_image.h"

static int calchisto8bit(IMAGE  * const pimage, unsigned long * const graytab);
static int calchisto24bit(IMAGE * const pimage, unsigned long * const redtab, unsigned long * const grntab, unsigned long * const blutab);

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_CalcHisto
//
// ��  ��: ͳ��ֱ��ͼ
//
// ��  ��: IMAGE         * const pimgsrc ԴӰ��
//         unsigned long * const redtab  ��ֵ����
//         unsigned long * const grntab  ��ֵ����
//         unsigned long * const blutab  ��ֵ����
//
// ����ֵ: int
//===================================================================
EXPORT_C int IMAGE_CalcHisto(IMAGE * const pimage, unsigned long * const redtab, unsigned long * const grntab, unsigned long * const blutab)
{
	int rcode = NO_ERROR;

	//
	// 0. ��ȫ���
	//
	assert(pimage);
	assert(redtab);
	assert(grntab);
	assert(blutab);
	if (NULL == pimage || NULL == pimage->data) { rcode = ERR_IMAGE; goto RET; } // Ҫ��Ӱ����Ч��
	if (NULL == redtab || NULL == grntab || NULL == blutab) { rcode = ERR_PARAMETER; goto RET; }
	
	//
	// 1. ��ʼ��
	//
	memset(redtab, 0, 256*sizeof(unsigned long));
	memset(grntab, 0, 256*sizeof(unsigned long));
	memset(blutab, 0, 256*sizeof(unsigned long));

	//
	// 2 ���ݲ�ͬɫ��ֱ���ò�ͬ����
	//
	switch (IMAGE_BITCOUNT(pimage)) {
	case 8: rcode = calchisto8bit( pimage, redtab); break; // ����� 8-bit Ӱ��һ���ǻҽ�Ӱ��
	case 24:rcode = calchisto24bit(pimage, redtab, grntab, blutab); break;
	default:rcode = ERR_BITCOUNT; break;
	}

RET:
	return rcode;
}

int calchisto8bit(IMAGE * const pimage, unsigned long * const graytab)
{
	int x, y;
	int width, height;

	IMGGRAY *ppixel = NULL;

	width  = IMAGE_AREA_WIDTH(pimage);
	height = IMAGE_AREA_HEIGHT(pimage);

	for (y = 0; y < height; y++) {
		ppixel = (IMGGRAY *)(&(pimage->matrix[y+pimage->sy][pimage->sx]));

		for (x = 0; x < width; x++) {
			graytab[ppixel[x]]++;
		}
	}

	return NO_ERROR;
}

int calchisto24bit(IMAGE * const pimage, unsigned long * const redtab, unsigned long * const grntab, unsigned long * const blutab)
{
	int x, y;
	int width, height;

	IMGRGB *ppixel = NULL;

	width  = IMAGE_AREA_WIDTH(pimage);
	height = IMAGE_AREA_HEIGHT(pimage);

	for (y = 0; y < height; y++) {
		ppixel = (IMGRGB *)(&(pimage->matrix[y+pimage->sy][pimage->sx*3]));

		for (x = 0; x < width; x++) {
			redtab[ppixel[x].R]++;
			grntab[ppixel[x].G]++;
			blutab[ppixel[x].B]++;
		}
	}

	return NO_ERROR;
}
