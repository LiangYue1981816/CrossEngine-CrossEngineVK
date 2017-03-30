/////////////////////////////////////////////////////////////////////
// memory.c
//
// 功  能: 提供影像存储、矩阵存储、链表存储的初始化、分配以及释放操作
//
// 修  订: 16:45 2004-12-06
//         10:09 2004-12-13
//         00:22 2004-12-19
//         09:01 2004-12-19
//         11:39 2005-06-19
//===================================================================

#include "_image.h"

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_AllocImage
//
// 功  能: 分配影像存储空间
//
// 参  数: IMAGE * const pimage    影像头指针
//         int     const nwidth    影像宽(in pixel)
//         int     const nheight   影像高(in pixel)
//         int     const nbitcount 影像色深(1, 8, 24, 32)
//
// 返回值：int
//
// 注  意：只支持 1-bit, 8-bit, 16-bit, 24-bit, 32-bit且不使用压缩格式
//===================================================================
EXPORT_C int IMAGE_AllocImage(IMAGE * const pimage, int const nwidth, int const nheight, int const nbitcount)
{
	int rcode = NO_ERROR;

	int i, nheadersize, nbuffersize, nlinesize;
	unsigned char *pbuffer = NULL, **ppline = NULL, *pbody = NULL;

	unsigned long rgb565Mask;

	//
	// 0. 参数安全检查
	//
	assert(pimage);
	if (NULL == pimage) { // 影像指针检查
		return ERR_PARAMETER;
	}
	if (nwidth <= 0 || nheight <= 0) { // 尺寸范围检查
		return ERR_RANGE;
	}
	if (NULL != pimage->data || NULL != pimage->matrix || NULL != pimage->bmpInfo) { // 影像未被初始化，或者已经被分配存储
		return ERR_IMAGE;
	}

	//
	// 1. 初始化填充 IMAGE 信息
	//
	pimage->sx = 0; pimage->ex = nwidth -1; // 默认横向处理范围是整个影像宽
	pimage->sy = 0; pimage->ey = nheight-1; // 默认纵向处理范围是整个影像高
	pimage->buffWidth = ((nwidth*nbitcount+31)/32)*4; // 存储行宽必须是4的整数倍

	//
	// 2. 创建 BITMAPINFO
	//
	// 2.1. 根据不同的色深分配 BITMAPINFO 存储
	switch (nbitcount) {
	case 1: // 1-bit
		//
		// 2.1.1. 分配存储(黑白2色调色板)
		//
		pimage->bmpInfo = (BITMAPINFO *)calloc(1, (sizeof(BITMAPINFOHEADER) + 2*sizeof(RGBQUAD)));
		if (NULL == pimage->bmpInfo) {	rcode = ERR_MEM; goto RET; }

		//
		// 2.1.2. 设置颜色数(黑白2色)
		//
		pimage->bmpInfo->bmiHeader.biClrUsed      = 2;
		pimage->bmpInfo->bmiHeader.biClrImportant = 2;

		//
		// 2.1.3. 设置调色板(黑白2色调色板)
		//
		pimage->bmpInfo->bmiColors[0].rgbRed      = 0;
		pimage->bmpInfo->bmiColors[0].rgbGreen    = 0;
		pimage->bmpInfo->bmiColors[0].rgbBlue     = 0;
		pimage->bmpInfo->bmiColors[0].rgbReserved = 0;

		pimage->bmpInfo->bmiColors[1].rgbRed      = 255;
		pimage->bmpInfo->bmiColors[1].rgbGreen    = 255;
		pimage->bmpInfo->bmiColors[1].rgbBlue     = 255;
		pimage->bmpInfo->bmiColors[1].rgbReserved = 255;

		break;
	case 8: // 8-bit
		//
		// 2.1.1. 分配存储(256色调色板)
		//
		pimage->bmpInfo = (BITMAPINFO *)calloc(1, (sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD)));
		if (NULL == pimage->bmpInfo) {	rcode = ERR_MEM; goto RET; }

		//
		// 2.1.2. 设置颜色数(256色调色板)
		//
		pimage->bmpInfo->bmiHeader.biClrUsed      = 256;
		pimage->bmpInfo->bmiHeader.biClrImportant = 256;

		//
		// 2.1.3. 设置调色板(256色灰阶调色板)
		//
		for (i = 0; i < 256; i++) {
			pimage->bmpInfo->bmiColors[i].rgbRed      = (unsigned char)i;
			pimage->bmpInfo->bmiColors[i].rgbGreen    = (unsigned char)i;
			pimage->bmpInfo->bmiColors[i].rgbBlue     = (unsigned char)i;
			pimage->bmpInfo->bmiColors[i].rgbReserved = (unsigned char)i;
		}

		break;
	case 16: // 16-bit
		//
		// 2.1.1. 分配存储(16Bit使用掩码)
		//
		pimage->bmpInfo = (BITMAPINFO *)calloc(1, sizeof(BITMAPINFOHEADER) + 3*sizeof(RGBQUAD));
		if (!pimage->bmpInfo) { rcode = ERR_MEM; goto RET; }

		//
		// 2.1.2. 设置调色板
		//
		pimage->bmpInfo->bmiHeader.biClrUsed      = 3;
		pimage->bmpInfo->bmiHeader.biClrImportant = 65536;

		rgb565Mask = 0x0000f800; pimage->bmpInfo->bmiColors[0] = *((RGBQUAD *)(&rgb565Mask));
		rgb565Mask = 0x000007e0; pimage->bmpInfo->bmiColors[1] = *((RGBQUAD *)(&rgb565Mask));
		rgb565Mask = 0x0000001f; pimage->bmpInfo->bmiColors[2] = *((RGBQUAD *)(&rgb565Mask));

		break;
	case 24: // 24-bit
	case 32: // 32-bit
		//
		// 2.1.1. 分配存储(真彩色不使用调色板)
		//
		pimage->bmpInfo = (BITMAPINFO *)calloc(1, (sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)));
		if (NULL == pimage->bmpInfo) {	rcode = ERR_MEM; goto RET; }

		break;
	default: rcode = ERR_BITCOUNT; goto RET; // 非法色深
	}

	// 2.2. 填充 BITMAPINFO 信息
	pimage->bmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pimage->bmpInfo->bmiHeader.biPlanes = 1;
	pimage->bmpInfo->bmiHeader.biCompression = BI_RGB; // 不使用压缩格式

	pimage->bmpInfo->bmiHeader.biWidth = nwidth;
	pimage->bmpInfo->bmiHeader.biHeight = nheight;
	pimage->bmpInfo->bmiHeader.biBitCount = (WORD)nbitcount;

	pimage->bmpInfo->bmiHeader.biSizeImage = pimage->buffWidth*nheight;

	//
	// 3. 创建影像矩阵
	//
	// 3.1. 计算影像矩阵头，行，身子所需要的存储容量
	nlinesize   = pimage->buffWidth;
	nheadersize = ((nheight*sizeof(void *)+3)/4)*4;
	nbuffersize = nheadersize + nlinesize*nheight;

	// 3.2. 分配存储
	pbuffer = (unsigned char *)calloc(nbuffersize, sizeof(unsigned char));
	if (NULL == pbuffer) { rcode = ERR_MEM; goto ERR; }

	// 3.3. 设置影像矩阵各行指针
	//      注意：这里使用倒序的方式设置矩阵存储，目的与影像数据存储方式保持一致
	ppline = (unsigned char **)pbuffer;
	pbody = pbuffer + nbuffersize - nlinesize;
	for (i = 0; i < nheight; i++) { *ppline = pbody; pbody -= nlinesize; ppline++; }

	// 3.4. 设置影像矩阵及影像数据指针
	pimage->matrix = (unsigned char **)pbuffer;
	pimage->data = pbuffer + nheadersize;

	goto RET;
ERR:
	IMAGE_FreeImage(pimage);
RET:
	return rcode;
}

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_FreeImage
//
// 功  能: 释放影像存储
//
// 参  数: IMAGE * const pimage 影像头指针
//
// 返回值: void
//
// 注  意: 该函数只能释放被 IMAGE_AllocImage 分配过的影像，否则会造成
//         内存泄漏或者死机
//===================================================================
EXPORT_C void IMAGE_FreeImage(IMAGE * const pimage)
{
	//
	// 0. 参数安全检查
	//
	assert(pimage);

	//
	// 1. 释放影像存储
	//
	if (pimage) {
		if (pimage->matrix) free(pimage->matrix);
		if (pimage->bmpInfo) free(pimage->bmpInfo);
		IMAGE_ZeroImage(pimage);
	}
}

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_ZeroImage
//
// 功  能: 初始化影像头
//
// 参  数: IMAGE * const pimage 影像头指针(未经分配过，或者已经释放过)
//
// 返回值: void
//===================================================================
EXPORT_C void IMAGE_ZeroImage(IMAGE * const pimage)
{
	//
	// 0. 参数安全检查
	//
	assert(pimage);

	//
	// 1. 初始化影像头结构
	//
	if (pimage) memset(pimage, 0, sizeof(IMAGE));
}



/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_AllocMatrix
//
// 功  能: 分配矩阵二维数组存储
//
// 参  数: int const nrows 矩阵行数
//         int cosnt ncols 矩阵列数
//         int cosnt nsize 矩阵中每个元素所占存储的容量
//
// 返回值: void ** 返回指向矩阵存储的指针，若分配存储失败，返回NULL
/////////////////////////////////////////////////////////////////////
EXPORT_C void** IMAGE_AllocMatrix(int const nrows, int const ncols, int const nsize)
{
	int i, nheadersize, nbuffersize, nlinesize;
	unsigned char *pbuffer = NULL, **ppline = NULL, *pbody = NULL;

	//
	// 0. 参数安全检查
	//
	if (nrows < 0 || ncols < 0 || nsize <= 0)
		return NULL;

	//
	// 1. 计算矩阵头，行，身子所需要的存储容量
	//
	nlinesize   = ((ncols*nsize+3)/4)*4;
	nheadersize = ((nrows*sizeof(void *)+3)/4)*4;
	nbuffersize = nheadersize + nlinesize*nrows;

	//
	// 2. 分配存储
	//
	pbuffer = (unsigned char *)calloc(nbuffersize, sizeof(unsigned char));
	if (NULL == pbuffer) return NULL;

	//
	// 3. 设置矩阵各行指针
	//    注意：这里使用倒序的方式设置矩阵存储，目的与影像数据存储方式保持一致
	ppline = (unsigned char **)pbuffer;
	pbody = pbuffer + nbuffersize - nlinesize;
	for (i = 0; i < nrows; i++) { *ppline = pbody; pbody -= nlinesize; ppline++; }

	//
	// 4. 返回首行指针地址作为矩阵指针
	//
	ppline = (unsigned char **)pbuffer;

	return (void**)ppline;
}

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_FreeMatrix
//
// 功  能: 释放矩阵二维数组存储
//
// 参  数: void ** cosnt pmatrix 矩阵存储指针
//
// 返回值: void
/////////////////////////////////////////////////////////////////////
EXPORT_C void IMAGE_FreeMatrix(void ** const pmatrix)
{
	//
	// 0. 参数安全检查
	//
	assert(pmatrix);

	//
	// 1. 释放矩阵存储
	//
	if (pmatrix) free(pmatrix);
}

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_CopyMatrix
//
// 功  能: 复制二维矩阵
//
// 参  数: void ** const pmtxsrc 原矩阵
//         void ** const pmtxdst 目标矩阵
//         int     cosnt nrows   矩阵行数
//         int     cosnt ncols   矩阵列数
//         int     const nsize   元素大小
//
// 返回值: int
/////////////////////////////////////////////////////////////////////
EXPORT_C int IMAGE_CopyMatrix(void ** const pmtxsrc, void ** const pmtxdst, int const nrows, int const ncols, int const nsize)
{
	int i;

	//
	// 0. 参数安全检查
	//
	assert(pmtxsrc);
	assert(pmtxdst);
	if (NULL == pmtxsrc || NULL == pmtxdst) { // 矩阵存储无效
		return ERR_MATRIX;
	}
	if (nrows < 0 || ncols < 0 || nsize <= 0) { // 矩阵操作范围越界
		return ERR_PARAMETER;
	}

	//
	// 1. 逐行复制矩阵
	//
	for (i = 0; i < nrows; i++) memcpy(pmtxdst[i], pmtxsrc[i], ncols*nsize);

	return NO_ERROR;
}



static void copyimagearea1bit(IMAGE  * const pimgsrc, IMAGE * const pimgdst);
static void copyimagearea8bit(IMAGE  * const pimgsrc, IMAGE * const pimgdst);
static void copyimagearea24bit(IMAGE * const pimgsrc, IMAGE * const pimgdst);
static void copyimagearea32bit(IMAGE * const pimgsrc, IMAGE * const pimgdst);

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_CopyImage
//
// 功  能: 复制全尺寸影像数据
//
// 参  数: IMAGE * const pimgsrc 原影像
//         IMAGE * const pimgdst 目标影像
//
// 返回值：int
//===================================================================
EXPORT_C int IMAGE_CopyImage(IMAGE * const pimgsrc, IMAGE * const pimgdst)
{
	//
	// 0. 参数安全检查
	//
	assert(pimgsrc);
	assert(pimgdst);
	assert(pimgsrc != pimgdst);
	assert(pimgsrc->data != pimgdst->data);
	if (NULL == pimgsrc || NULL == pimgsrc->data ||
		NULL == pimgdst || NULL == pimgdst->data) { // 要求影像有效性
		return ERR_IMAGE;
	}
	if (IMAGE_BITCOUNT(pimgsrc) != IMAGE_BITCOUNT(pimgdst)) { // 要求原影像与目标影像色深相同
		return ERR_BITCOUNT;
	}
	if (IMAGE_WIDTH(pimgsrc) != IMAGE_WIDTH(pimgdst) ||	IMAGE_HEIGHT(pimgsrc) != IMAGE_HEIGHT(pimgdst)) { // 要求原影像与目标影像尺寸相同
		return ERR_RANGE;
	}

	//
	// 1. 复制影像
	//
	switch (IMAGE_BITCOUNT(pimgsrc)) {
	case  1:
	case  8:
	case 24:
	case 32: memcpy(pimgdst->data, pimgsrc->data, pimgdst->bmpInfo->bmiHeader.biSizeImage); break;
	default: return ERR_BITCOUNT;
	}
	
	//
	// 2. 复制处理区域坐标
	//
	pimgdst->sx = pimgsrc->sx; pimgdst->ex = pimgsrc->ex;
	pimgdst->sy = pimgsrc->sy; pimgdst->ey = pimgsrc->ey;

	return NO_ERROR;
}

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_CopyImageArea
//
// 功  能: 复制区域影像数据
//
// 参  数: IMAGE * const pimgsrc 原影像
//         IMAGE * const pimgdst 目标影像
//
// 返回值：int
//===================================================================
EXPORT_C int IMAGE_CopyImageArea(IMAGE * const pimgsrc, IMAGE * const pimgdst)
{	
	//
	// 0. 参数安全检查
	//
	assert(pimgsrc);
	assert(pimgdst);
	if (NULL == pimgsrc || NULL == pimgsrc->data ||
		NULL == pimgdst || NULL == pimgdst->data) { // 要求影像有效性
		return ERR_IMAGE;
	}
	if (IMAGE_BITCOUNT(pimgsrc) != IMAGE_BITCOUNT(pimgdst)) { // 要求原影像与目标影像色深相同
		return ERR_BITCOUNT;
	}
	if (IMAGE_AREA_WIDTH(pimgsrc) != IMAGE_AREA_WIDTH(pimgdst) || IMAGE_AREA_HEIGHT(pimgsrc) != IMAGE_AREA_HEIGHT(pimgdst)) { // 要求原影像与目标影像处理区域尺寸相同
		return ERR_RANGE;
	}

	//
	// 1. 根据色深的不同调用不同的拷贝函数
	//
	switch (IMAGE_BITCOUNT(pimgsrc)) {
	case  1: copyimagearea1bit(pimgsrc, pimgdst); break;
	case  8: copyimagearea8bit(pimgsrc, pimgdst); break;
	case 24: copyimagearea24bit(pimgsrc,pimgdst); break;
	case 32: copyimagearea32bit(pimgsrc,pimgdst); break;
	default: return ERR_BITCOUNT;
	}

	return NO_ERROR;
}

void copyimagearea1bit(IMAGE * const pimgsrc, IMAGE * const pimgdst)
{
	int x, y;
	int width, height;

	int xsrcbit, xdstbit;

	unsigned char *ppixel_src = NULL;
	unsigned char *ppixel_dst = NULL;

	unsigned char const offsetbits0[8] = {0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0xfe};
	unsigned char const offsetbits1[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	for (y = 0; y < height; y++) {
		xsrcbit = pimgsrc->sx%8;
		xdstbit = pimgdst->sx%8;

		ppixel_src = &(pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->sx/8]);
		ppixel_dst = &(pimgdst->matrix[pimgdst->sy+y][pimgdst->sx/8]);

		for (x = 0; x < width; x++) {
			if (0 == ((*ppixel_src) & offsetbits1[xsrcbit]))
				*ppixel_dst &= offsetbits0[xdstbit]; // 设置为0
			else
				*ppixel_dst |= offsetbits1[xdstbit]; // 设置为1

			xsrcbit++; if (8 == xsrcbit) { ppixel_src++; xsrcbit = 0; }
			xdstbit++; if (8 == xdstbit) { ppixel_dst++; xdstbit = 0; }
		}
	}
}

void copyimagearea8bit(IMAGE * const pimgsrc, IMAGE * const pimgdst)
{
	int y;
	int width, height;

	unsigned char *ppixel_src = NULL;
	unsigned char *ppixel_dst = NULL;

	width = IMAGE_AREA_WIDTH(pimgsrc); // 对于 8-bit 影像来说每行复制的字节数就是区域宽度
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	for (y = 0; y < height; y++) {
		ppixel_src = &(pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->sx]);
		ppixel_dst = &(pimgdst->matrix[pimgdst->sy+y][pimgdst->sx]);

		memcpy(ppixel_dst, ppixel_src, width);
	}
}

void copyimagearea24bit(IMAGE * const pimgsrc, IMAGE * const pimgdst)
{
	int y;
	int width, height;

	unsigned char *ppixel_src = NULL;
	unsigned char *ppixel_dst = NULL;

	width = IMAGE_AREA_WIDTH(pimgsrc)*3; // 对于 24-bit 影像来说每行复制的字节数就是区域宽度的3倍
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	for (y = 0; y < height; y++) {
		ppixel_src = &(pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->sx*3]);
		ppixel_dst = &(pimgdst->matrix[pimgdst->sy+y][pimgdst->sx*3]);

		memcpy(ppixel_dst, ppixel_src, width);
	}
}

static void copyimagearea32bit(IMAGE * const pimgsrc, IMAGE * const pimgdst)
{
	int y;
	int width, height;

	unsigned char *ppixel_src = NULL;
	unsigned char *ppixel_dst = NULL;

	width = IMAGE_AREA_WIDTH(pimgsrc)*4; // 对于 32-bit 影像来说每行复制的字节数就是区域宽度的4倍
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	for (y = 0; y < height; y++) {
		ppixel_src = &(pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->sx*4]);
		ppixel_dst = &(pimgdst->matrix[pimgdst->sy+y][pimgdst->sx*4]);

		memcpy(ppixel_dst, ppixel_src, width);
	}
}
