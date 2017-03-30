/////////////////////////////////////////////////////////////////////
// imagearea.c
//
// 功  能: 提供对影像处理区域设置/恢复的操作
//
// 修  订: 16:45 2004-12-06
//         10:09 2004-12-13
//         09:01 2004-12-19
//         11:39 2005-06-19
//===================================================================

#include "_image.h"

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_SetImageArea
//
// 功  能: 设置影像处理区域
//
// 参  数: IMAGE * const pimage 影像
//         int     const sx, sy 起始点
//         int     const ex, ey 终止点
//
// 返回值：void
//===================================================================
EXPORT_C void IMAGE_SetImageArea(IMAGE * const pimage, int const sx, int const sy, int const ex, int const ey)
{
	//
	// 0. 参数安全检查
	//
	assert(pimage);

	if (NULL == pimage->bmpInfo || NULL == pimage->matrix || NULL == pimage->data) {
		return;
	}

	//
	// 1. 设置影像处理区域
	//
	if (sx >  ex || sy >  ey ||
		sx <  0  || sx >= IMAGE_WIDTH(pimage) ||
		ex <  0  || ex >= IMAGE_WIDTH(pimage) ||
		sy <  0  || sy >= IMAGE_HEIGHT(pimage)||
		ey <  0  || ey >= IMAGE_HEIGHT(pimage)) {
		return;
	}

	//
	// 1. 设置纹理处理区域
	//
	pimage->sx = sx; pimage->ex = ex;
	pimage->sy = sy; pimage->ey = ey;

	return;
}

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_ResetImageArea
//
// 功  能: 恢复影像处理区域
//
// 参  数: IMAGE * const pimage 影像
//
// 返回值: void
//===================================================================
EXPORT_C void IMAGE_ResetImageArea(IMAGE * const pimage)
{
	//
	// 0. 参数安全检查
	//
	assert(pimage);

	if (NULL == pimage->bmpInfo || NULL == pimage->matrix || NULL == pimage->data) {
		return;
	}

	//
	// 1. 恢复影像处理区域
	//
	pimage->sx = 0; pimage->ex = IMAGE_WIDTH(pimage) - 1;
	pimage->sy = 0; pimage->ey = IMAGE_HEIGHT(pimage) - 1;
}
