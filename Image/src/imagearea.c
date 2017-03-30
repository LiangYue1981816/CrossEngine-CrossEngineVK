/////////////////////////////////////////////////////////////////////
// imagearea.c
//
// ��  ��: �ṩ��Ӱ������������/�ָ��Ĳ���
//
// ��  ��: 16:45 2004-12-06
//         10:09 2004-12-13
//         09:01 2004-12-19
//         11:39 2005-06-19
//===================================================================

#include "_image.h"

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_SetImageArea
//
// ��  ��: ����Ӱ��������
//
// ��  ��: IMAGE * const pimage Ӱ��
//         int     const sx, sy ��ʼ��
//         int     const ex, ey ��ֹ��
//
// ����ֵ��void
//===================================================================
EXPORT_C void IMAGE_SetImageArea(IMAGE * const pimage, int const sx, int const sy, int const ex, int const ey)
{
	//
	// 0. ������ȫ���
	//
	assert(pimage);

	if (NULL == pimage->bmpInfo || NULL == pimage->matrix || NULL == pimage->data) {
		return;
	}

	//
	// 1. ����Ӱ��������
	//
	if (sx >  ex || sy >  ey ||
		sx <  0  || sx >= IMAGE_WIDTH(pimage) ||
		ex <  0  || ex >= IMAGE_WIDTH(pimage) ||
		sy <  0  || sy >= IMAGE_HEIGHT(pimage)||
		ey <  0  || ey >= IMAGE_HEIGHT(pimage)) {
		return;
	}

	//
	// 1. ��������������
	//
	pimage->sx = sx; pimage->ex = ex;
	pimage->sy = sy; pimage->ey = ey;

	return;
}

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_ResetImageArea
//
// ��  ��: �ָ�Ӱ��������
//
// ��  ��: IMAGE * const pimage Ӱ��
//
// ����ֵ: void
//===================================================================
EXPORT_C void IMAGE_ResetImageArea(IMAGE * const pimage)
{
	//
	// 0. ������ȫ���
	//
	assert(pimage);

	if (NULL == pimage->bmpInfo || NULL == pimage->matrix || NULL == pimage->data) {
		return;
	}

	//
	// 1. �ָ�Ӱ��������
	//
	pimage->sx = 0; pimage->ex = IMAGE_WIDTH(pimage) - 1;
	pimage->sy = 0; pimage->ey = IMAGE_HEIGHT(pimage) - 1;
}
