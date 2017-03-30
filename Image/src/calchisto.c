/////////////////////////////////////////////////////////////////////
// calchisto.c
//
// 功  能: 统计直方图
//
// 修  订: 09:13 2004-12-18
//         00:22 2004-12-19
//         12:23 2005-06-19
//===================================================================

#include "_image.h"

static int calchisto8bit(IMAGE  * const pimage, unsigned long * const graytab);
static int calchisto24bit(IMAGE * const pimage, unsigned long * const redtab, unsigned long * const grntab, unsigned long * const blutab);

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_CalcHisto
//
// 功  能: 统计直方图
//
// 参  数: IMAGE         * const pimgsrc 源影像
//         unsigned long * const redtab  红值计数
//         unsigned long * const grntab  绿值计数
//         unsigned long * const blutab  蓝值计数
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_CalcHisto(IMAGE * const pimage, unsigned long * const redtab, unsigned long * const grntab, unsigned long * const blutab)
{
	int rcode = NO_ERROR;

	//
	// 0. 安全检查
	//
	assert(pimage);
	assert(redtab);
	assert(grntab);
	assert(blutab);
	if (NULL == pimage || NULL == pimage->data) { rcode = ERR_IMAGE; goto RET; } // 要求影像有效性
	if (NULL == redtab || NULL == grntab || NULL == blutab) { rcode = ERR_PARAMETER; goto RET; }
	
	//
	// 1. 初始化
	//
	memset(redtab, 0, 256*sizeof(unsigned long));
	memset(grntab, 0, 256*sizeof(unsigned long));
	memset(blutab, 0, 256*sizeof(unsigned long));

	//
	// 2 根据不同色深分别调用不同函数
	//
	switch (IMAGE_BITCOUNT(pimage)) {
	case 8: rcode = calchisto8bit( pimage, redtab); break; // 这里的 8-bit 影像一定是灰阶影像
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
