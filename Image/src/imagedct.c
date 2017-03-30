/////////////////////////////////////////////////////////////////////
// imagedct.c
//
// 功  能: 影像离散余弦变换
//
// 修  订: 20:26 2005-1-2
//         12:23 2005-6-19
//         20:40 2005-7-11
//===================================================================

#include "_image.h"

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_DCT
//
// 功  能: 影像离散余弦变换
//
// 参  数: IMAGE * const pimgsrc     源影像
//         float ** cosnt ppdata      数据缓冲
//         int      const powerwidth  宽的2的幂次
//         int      const powerheight 高的2的幂次
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_DCT(IMAGE * const pimgsrc, float ** const ppdata, int const powerwidth, int const powerheight)
{
	int rcode = NO_ERROR;

	int x, y;
	int width, height;

	float *TD = NULL, *FD = NULL;
	IMGGRAY *ppixel_src = NULL;

	//
	// 0. 安全检查
	//
	assert(ppdata);
	assert(pimgsrc);
	if (NULL == pimgsrc->data || NULL == ppdata) { // 要求影像有效性
		rcode = ERR_IMAGE; goto RET;
	}
	if (IMAGE_BITCOUNT(pimgsrc) != 8) { // 要求原影像必须是8bit影像
		rcode = ERR_BITCOUNT; goto RET;
	}
	if (IMAGE_AREA_WIDTH(pimgsrc) != (unsigned int)(1<<powerwidth) || IMAGE_AREA_HEIGHT(pimgsrc) != (unsigned int)(1<<powerheight)) { // 要求影像宽高必须是2的整数次幂
		rcode = ERR_RANGE; goto RET;
	}

	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);
		
	//
	// 1. 分配存储
	//
	TD = (float *)calloc(width*height, sizeof(float));
	FD = (float *)calloc(width*height, sizeof(float));
	if (NULL == TD || NULL == FD) { rcode = ERR_MEM; goto RET; }
	
	//
	// 2. 离散余弦变换
	//
	// 2.1. 读取影像数据
	for (y = 0; y < height; y++) {
		ppixel_src = (IMGGRAY *)(&(pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->sx]));

		for (x = 0; x < width; x++) {
			TD[y*width + x] = ppixel_src[x];
		}
	}
	
	// 2.2. 行离散余弦变换
	for (y = 0; y < height; y++) {
		rcode = _DCT(&TD[y*width], &FD[y*width], powerwidth);
		if (rcode != NO_ERROR) goto RET;
	}
	
	// 2.1. 将行离散余弦计算结果FD转置保存到TD的列上
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			TD[x*height+y] = FD[y*width+x];
		}
	}
	
	// 2.3. 列离散余弦变换
	for (x = 0; x < width; x++) {
		rcode = _DCT(&TD[x*height], &FD[x*height], powerheight);
		if (rcode != NO_ERROR) goto RET;
	}
	
	//
	// 3. 将计算结果复制到输出缓冲
	//
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			ppdata[y][x] = FD[y*width+x];
		}
	}
	
RET:
	if (TD) free(TD);
	if (FD) free(FD);
	return rcode;
}

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_IDCT
//
// 功  能: 快速反离散余弦变换
//
// 参  数: float ** cosnt ppdata      数据缓冲
//         IMAGE * const pimgsrc     源影像
//         int      const powerwidth  宽的2的幂次
//         int      const powerheight 高的2的幂次
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_IDCT(float ** const ppdata, IMAGE * const pimgdst, int const powerwidth, int const powerheight)
{
	int rcode = NO_ERROR;

	int x, y;
	int width, height;

	float *TD = NULL, *FD = NULL;
	IMGGRAY *ppixel_dst = NULL;

	//
	// 0. 安全检查
	//
	assert(ppdata);
	assert(pimgdst);
	if (NULL == pimgdst->data || NULL == ppdata) { // 要求影像有效性
		rcode = ERR_IMAGE; goto RET;
	}
	if (IMAGE_BITCOUNT(pimgdst) != 8) { // 要求原影像必须是8bit影像
		rcode = ERR_BITCOUNT; goto RET;
	}
	if (IMAGE_AREA_WIDTH(pimgdst) != (unsigned int)(1<<powerwidth) || IMAGE_AREA_HEIGHT(pimgdst) != (unsigned int)(1<<powerheight)) { // 要求影像宽高必须是2的整数次幂
		rcode = ERR_RANGE; goto RET;
	}

	width  = IMAGE_AREA_WIDTH(pimgdst);
	height = IMAGE_AREA_HEIGHT(pimgdst);
		
	//
	// 1. 分配存储
	//
	TD = (float *)calloc(width*height, sizeof(float));
	FD = (float *)calloc(width*height, sizeof(float));
	if (NULL == TD || NULL == FD) { rcode = ERR_MEM; goto RET; }

	//
	// 2. 将输入复数数据复制到频域存储
	//
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			FD[y*width+x] = ppdata[y][x];
		}
	}

	//
	// 3. 反复里叶变换
	//
	// 3.1. 行反离散余弦变换
	for (y = 0; y < height; y++) {
		rcode = _IDCT(&FD[y*width], &TD[y*width], powerwidth);
		if (rcode != NO_ERROR) goto RET;
	}
	
	// 3.2. 将行反离散余弦计算结果TD转置保存到FD的列上
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			FD[x*height+y] = TD[y*width+x];
		}
	}
	
	// 3.3. 列反离散余弦变换
	for (x = 0; x < width; x++) {
		rcode = _IDCT(&FD[x*height], &TD[x*height], powerheight);
		if (rcode != NO_ERROR) goto RET;
	}
	
	//
	// 4. 保存结果到影像
	//
	for (y = 0; y < height; y++) {
		ppixel_dst = (IMGGRAY *)(&(pimgdst->matrix[pimgdst->sy+y][pimgdst->sx]));

		for (x = 0; x < width; x++) {
			if (TD[y*width + x] > 255.0) TD[y*width + x] = 255.0;
			if (TD[y*width + x] < 000.0) TD[y*width + x] = 000.0;
			ppixel_dst[x] = (IMGGRAY)(TD[y*width + x]);
		}
	}
	
RET:
	if (TD) free(TD);
	if (FD) free(FD);
	return rcode;
}
