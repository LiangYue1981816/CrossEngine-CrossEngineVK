/////////////////////////////////////////////////////////////////////
// pixel.c
//
// 功  能: 提供对1-bit、8-bit、24-bit影像的像素操作
//
// 修  订: 16:45 2004-12-06
//         10:09 2004-12-13
//         09:01 2004-12-19
//         11:39 2005-06-19
//===================================================================

#include "_image.h"

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_GetPixelColor
//
// 功  能: 获得影像坐标 (x,y) 处像素 RGB 值
//
// 参  数：IMAGE * const pimage 影像
//         int     const x      横坐标
//         int     const y      纵坐标
//
// 返回值：unsigned long
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

	// 定位操作的字节地址
	pbyte = (unsigned char *)(&(pimage->matrix[y][x*IMAGE_BITCOUNT(pimage)/8]));

	// 根据不同色深获取颜色
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
// 名  称: IMAGE_SetPixelColor
//
// 功  能: 设置影像坐标 (x,y) 处像素 RGB 值
//
// 参  数：IMAGE       * const pimage 影像
//         int           const x      横坐标
//         int           const y      纵坐标
//         unsigned long const color  RGB值
//
// 返回值：void
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

	// 定位操作的字节地址
	pbyte = (unsigned char *)(&(pimage->matrix[y][x*IMAGE_BITCOUNT(pimage)/8]));

	// 根据不同色深进行颜色设置
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
