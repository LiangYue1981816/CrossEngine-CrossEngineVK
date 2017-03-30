/////////////////////////////////////////////////////////////////////
// pixel.c
//
// ��  ��: �ṩ��1-bit��8-bit��24-bitӰ������ز���
//
// ��  ��: 16:45 2004-12-06
//         10:09 2004-12-13
//         09:01 2004-12-19
//         11:39 2005-06-19
//===================================================================

#include "_image.h"

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_GetPixelColor
//
// ��  ��: ���Ӱ������ (x,y) ������ RGB ֵ
//
// ��  ����IMAGE * const pimage Ӱ��
//         int     const x      ������
//         int     const y      ������
//
// ����ֵ��unsigned long
//===================================================================
EXPORT_C unsigned long IMAGE_GetPixelColor(IMAGE * const pimage, int const x, int const y)
{
	unsigned char *pbyte = NULL;
	
	unsigned long color = 0;
	unsigned char nred, ngrn, nblu, nalp;

	unsigned char const offsetbits1[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

	assert(pimage);
	assert(pimage->data);
	assert(x >= 0 && x < IMAGE_WIDTH(pimage));
	assert(y >= 0 && y < IMAGE_HEIGHT(pimage));

	// ��λ�������ֽڵ�ַ
	pbyte = (unsigned char *)(&(pimage->matrix[y][x*IMAGE_BITCOUNT(pimage)/8]));

	// ���ݲ�ͬɫ���ȡ��ɫ
	switch (IMAGE_BITCOUNT(pimage)) {
	case 1: // 1 bit
		color = (*pbyte) & offsetbits1[x%8] ? 1:0;
		break;
	case 8: // 8 bit gray
		color = *pbyte;
		break;
	case 16:// 16 bit
		color = *((unsigned short*)pbyte);
		break;
	case 24:// 24 bit
		nblu = *(pbyte  );
		ngrn = *(pbyte+1);
		nred = *(pbyte+2);
		color = RGB(nred, ngrn, nblu);
		break;
	case 32: // 32 bit
		nblu = *(pbyte  );
		ngrn = *(pbyte+1);
		nred = *(pbyte+2);
		nalp = *(pbyte+3);
		color = RGBA(nred, ngrn, nblu, nalp);
		break;
	}

	return color;
}

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_SetPixelColor
//
// ��  ��: ����Ӱ������ (x,y) ������ RGB ֵ
//
// ��  ����IMAGE       * const pimage Ӱ��
//         int           const x      ������
//         int           const y      ������
//         unsigned long const color  RGBֵ
//
// ����ֵ��void
//===================================================================
EXPORT_C void IMAGE_SetPixelColor(IMAGE * const pimage, int const x, int const y, unsigned long const color)
{	
	unsigned char *pbyte = NULL;

	unsigned char const offsetbits0[8] = {0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0xfe};
	unsigned char const offsetbits1[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

	assert(pimage);
	assert(pimage->data);
	assert(x >= 0 && x < IMAGE_WIDTH(pimage));
	assert(y >= 0 && y < IMAGE_HEIGHT(pimage));

	// ��λ�������ֽڵ�ַ
	pbyte = (unsigned char *)(&(pimage->matrix[y][x*IMAGE_BITCOUNT(pimage)/8]));

	// ���ݲ�ͬɫ�������ɫ����
	switch (IMAGE_BITCOUNT(pimage)) {
	case 1: // 1 bit
		*pbyte = (0 == color) ? (*pbyte &= offsetbits0[x%8]) : (*pbyte |= offsetbits1[x%8]);
		break;
	case 8: // 8 bit gray
		*pbyte = (unsigned char)color;
		break;
	case 16:// 16 bit
		*((unsigned short *)pbyte) = (unsigned short)color;
		break;
	case 24:// 24 bit
		*(pbyte  ) = GET_BLU(color);
		*(pbyte+1) = GET_GRN(color);
		*(pbyte+2) = GET_RED(color);
		break;
	case 32: // 32 bit
		*(pbyte  ) = GET_BLU(color);
		*(pbyte+1) = GET_GRN(color);
		*(pbyte+2) = GET_RED(color);
		*(pbyte+3) = GET_ALP(color);
		break;
	}
}
