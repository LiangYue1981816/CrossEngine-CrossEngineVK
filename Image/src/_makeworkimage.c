/////////////////////////////////////////////////////////////////////
// _makeworkimage.c
//
// 功  能: 制作工作影像
//
// 备  注: 内部调用函数
//
// 修  订: 11:07 2004-12-26
//===================================================================

#include "_image.h"

int _makeworkimage(IMAGE * const pimgsrc, IMAGE * const pimgdst, int const nsize)
{
	int rcode = NO_ERROR;

	int width, height;
	unsigned int sx, ex, sy, ey;

	//
	// 0. 参数安全检查
	//
	assert(pimgsrc);
	assert(pimgdst);
	if (NULL == pimgsrc || NULL == pimgdst || 0 == (nsize&1)) {
		return ERR_PARAMETER;
	}
	if (NULL == pimgsrc->data || NULL != pimgdst->data) {
		return ERR_IMAGE;
	}

	//
	// 1. 分配目标影像存储
	//
	// 工作影像需要留出边缘处理区域
	width = IMAGE_AREA_WIDTH(pimgsrc) + nsize - 1;
	height = IMAGE_AREA_HEIGHT(pimgsrc) + nsize - 1;

	rcode = IMAGE_AllocImage(pimgdst, width, height, IMAGE_BITCOUNT(pimgsrc));
	if (rcode != NO_ERROR) goto RET;

	//
	// 2. 复制原影像处理区域到目标影像
	//
	sx = pimgsrc->sx; ex = pimgsrc->ex;
	sy = pimgsrc->sy; ey = pimgsrc->ey;

	IMAGE_SetImageArea(pimgsrc, sx-nsize/2, sy-nsize/2, ex+1+nsize/2, ey+1+nsize/2);
	if (sx == pimgsrc->sx) pimgdst->sx += nsize/2;
	if (sy == pimgsrc->sy) pimgdst->sy += nsize/2;
	if (ex == pimgsrc->ex) pimgdst->ex -= nsize/2;
	if (ey == pimgsrc->ey) pimgdst->ey -= nsize/2;
	rcode = IMAGE_CopyImageArea(pimgsrc, pimgdst);
	IMAGE_SetImageArea(pimgsrc, sx, sy, ex+1, ey+1);
	if (rcode != NO_ERROR) goto ERR;
	
	IMAGE_SetImageArea(pimgdst, nsize/2, nsize/2, width-nsize/2, height-nsize/2);

	goto RET;
ERR:
	IMAGE_FreeImage(pimgdst);
RET:
	return rcode;
}
