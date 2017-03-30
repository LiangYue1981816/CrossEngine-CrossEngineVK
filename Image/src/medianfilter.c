/////////////////////////////////////////////////////////////////////
// medianfilter.c
//
// 功  能: 中值滤波
//
// 修  订: 10:39 2005-01-01
//         12:23 2005-06-19
//         15:37 2006-01-09
//         20:46 2015-01-17
//===================================================================

#include "_image.h"

static int compare(void const *elem1, void const *elem2);
static void * medianfilter8bit(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk, int size);
static void * medianfilter24bit(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk, int size);

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_MedianFilter
//
// 功  能: 中值滤波
//
// 参  数: IMAGE * const pimgsrc 源影像
//         IMAGE * const pimgdst 目标影像
//         int     const nsize   滤波器尺寸
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_MedianFilter(IMAGE * const pimgsrc, IMAGE * const pimgdst, int const nsize)
{
	int rcode = NO_ERROR;

	IMAGE imgwrk;
	IMAGE_ZeroImage(&imgwrk);

	//
	// 0. 安全检查
	//
	assert(pimgsrc);
	assert(pimgdst);
	if (0 == (nsize&1) || nsize < 3) { // 要求参数有效
		rcode = ERR_PARAMETER; goto RET;
	}
	if (NULL == pimgsrc->data || NULL == pimgdst->data) { // 要求影像有效性
		rcode = ERR_IMAGE; goto RET;
	}
	if (IMAGE_BITCOUNT(pimgsrc) != IMAGE_BITCOUNT(pimgdst)) { // 要求原影像与目标影像色深相同
		rcode = ERR_BITCOUNT; goto RET;
	}
	if (IMAGE_AREA_WIDTH(pimgsrc) < 3 || IMAGE_AREA_HEIGHT(pimgsrc) < 3) { // 要求影像处理区域不得小于 3x3
		rcode = ERR_RANGE; goto RET;
	}
	if (IMAGE_AREA_WIDTH(pimgsrc) != IMAGE_AREA_WIDTH(pimgdst) || IMAGE_AREA_HEIGHT(pimgsrc) != IMAGE_AREA_HEIGHT(pimgdst)) { // 要求原影像与目标影像处理区域尺寸相同
		rcode = ERR_RANGE; goto RET;
	}

	//
	// 1 根据不同色深分别调用不同函数
	//
	rcode = _makeworkimage(pimgsrc, &imgwrk, nsize);
	if (rcode != NO_ERROR) goto RET;

	switch (IMAGE_BITCOUNT(pimgsrc)) {
	case  8: medianfilter8bit(pimgsrc, pimgdst, &imgwrk, nsize); break;
	case 24: medianfilter24bit(pimgsrc, pimgdst, &imgwrk, nsize); break;
	default: rcode = ERR_BITCOUNT; goto RET;
	}

RET:
	IMAGE_FreeImage(&imgwrk);
	return rcode;
}

static int compare(void const *elem1, void const *elem2)
{
	if (*(unsigned char*)elem1 == *(unsigned char*)elem2)
		return  0;
	else if (*(unsigned char*)elem1 > *(unsigned char*)elem2)
		return  1;
	else
		return -1;
}

static void * medianfilter8bit(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk, int size)
{
	int x, y;
	int xx, yy, k;
	int width, height;

	IMGGRAY *ppixel_buf = NULL;
	IMGGRAY *ppixel_src = NULL;
	IMGGRAY *ppixel_dst = NULL;
	IMGGRAY *ppixel_tmp = NULL;

	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	// 分配缓冲存储
	ppixel_buf = (IMGGRAY *)calloc(size*size, sizeof(IMGGRAY));
	if (NULL == ppixel_buf) goto RET;

	for (y = 0; y < height; y++) {
		ppixel_src = (IMGGRAY *)(&(pimgwrk->matrix[pimgwrk->sy+y][pimgwrk->sx]));
		ppixel_dst = (IMGGRAY *)(&(pimgdst->matrix[pimgdst->sy+y][pimgdst->sx]));

		for (x = 0; x < width; x++) {
			// 填充数据缓冲
			for (k = 0, yy = -size/2; yy <= size/2; yy++) {
				ppixel_tmp = (IMGGRAY *)((unsigned char *)&ppixel_src[x] + yy*pimgwrk->buffWidth);

				for (xx = -size/2; xx <= size/2; xx++, k++) {
					ppixel_buf[k] = ppixel_tmp[xx];
				}
			}

			// 排序，取中值
			qsort(ppixel_buf, k, sizeof(unsigned char), compare);
			ppixel_dst[x] = ppixel_buf[k/2];
		}
	}

RET:
	if (ppixel_buf) free(ppixel_buf);
	return NULL;
}

static void * medianfilter24bit(IMAGE *pimgsrc, IMAGE *pimgdst, IMAGE *pimgwrk, int size)
{
	int rcode = NO_ERROR;

	int x, y;
	int xx, yy, k;
	int width, height;

	unsigned char tabred[255];
	unsigned char tabgrn[255];
	unsigned char tabblu[255];

	IMGGRAY *ppixel_buf = NULL;
	IMGGRAY *ppixel_gry = NULL;
	IMGGRAY *ppixel_gry_tmp = NULL;

	IMGRGB *ppixel_src = NULL;
	IMGRGB *ppixel_dst = NULL;
	IMGRGB *ppixel_rgb_tmp = NULL;

	IMAGE imggry;
	IMAGE_ZeroImage(&imggry);

	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	// 根据工作影像计算其灰度影像
	rcode = IMAGE_AllocImage(&imggry, IMAGE_WIDTH(pimgwrk), IMAGE_HEIGHT(pimgwrk), 8);
	if (rcode != NO_ERROR) goto RET;

	rcode = IMAGE_ColorToGray(pimgwrk, &imggry, GRAY_STD);
	if (rcode != NO_ERROR) goto RET;

	imggry.sx = pimgwrk->sx; imggry.sy = pimgwrk->sy;
	imggry.ex = pimgwrk->ex; imggry.ey = pimgwrk->ey;

	// 分配缓冲存储
	ppixel_buf = (IMGGRAY *)calloc(size*size, sizeof(IMGGRAY));
	if (NULL == ppixel_buf) { rcode = ERR_MEM; goto RET; }

	for (y = 0; y < height; y++) {
		ppixel_gry = (IMGGRAY *)(&(imggry.matrix[imggry.sy+y][imggry.sx]));
		ppixel_src = (IMGRGB  *)(&(pimgwrk->matrix[pimgwrk->sy+y][pimgwrk->sx*3]));
		ppixel_dst = (IMGRGB  *)(&(pimgdst->matrix[pimgdst->sy+y][pimgdst->sx*3]));

		for (x = 0; x < width; x++) {
			// 填充数据缓冲
			for (k = 0, yy = -size/2; yy <= size/2; yy++) {
				ppixel_gry_tmp = (IMGGRAY *)((unsigned char *)&ppixel_gry[x] + yy*imggry.buffWidth);
				ppixel_rgb_tmp = (IMGRGB *)((unsigned char *)&ppixel_src[x] + yy*pimgwrk->buffWidth);

				for (xx = -size/2; xx <= size/2; xx++, k++) {
					ppixel_buf[k] = ppixel_gry_tmp[xx];

					tabred[ppixel_gry_tmp[xx]] = ppixel_rgb_tmp[xx].R;
					tabgrn[ppixel_gry_tmp[xx]] = ppixel_rgb_tmp[xx].G;
					tabblu[ppixel_gry_tmp[xx]] = ppixel_rgb_tmp[xx].B;
				}
			}

			// 排序中值滤波
			qsort(ppixel_buf, k, sizeof(unsigned char), compare);
			ppixel_dst[x].R = tabred[ppixel_buf[k/2]];
			ppixel_dst[x].G = tabgrn[ppixel_buf[k/2]];
			ppixel_dst[x].B = tabblu[ppixel_buf[k/2]];
		}
	}

RET:
	IMAGE_FreeImage(&imggry);
	if (ppixel_buf) free(ppixel_buf);

	return NULL;
}
