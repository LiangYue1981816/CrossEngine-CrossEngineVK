/////////////////////////////////////////////////////////////////////
// open.c
//
// ��  ��: ��̬ѧ��
//
// ��  ��: 20:26 2005-01-02
//         12:23 2005-06-19
//===================================================================

#include "_image.h"

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_OpenH
//
// ��  ��: ˮƽ��
//
// ��  ��: IMAGE * const pimgsrc ԴӰ��
//         IMAGE * const pimgdst Ŀ��Ӱ��
//
// ����ֵ: int
//===================================================================
EXPORT_C int IMAGE_OpenH(IMAGE * const pimgsrc, IMAGE * const pimgdst)
{
	int rcode = NO_ERROR;

	rcode = IMAGE_ErosionH(pimgsrc, pimgdst);
	if (rcode != NO_ERROR) goto RET;
	rcode = IMAGE_DilationH(pimgsrc, pimgdst);

RET:
	return rcode;
}


/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_OpenV
//
// ��  ��: ��ֱ��
//
// ��  ��: IMAGE * const pimgsrc ԴӰ��
//         IMAGE * const pimgdst Ŀ��Ӱ��
//
// ����ֵ: int
//===================================================================
EXPORT_C int IMAGE_OpenV(IMAGE * const pimgsrc, IMAGE * const pimgdst)
{
	int rcode = NO_ERROR;

	rcode = IMAGE_ErosionV(pimgsrc, pimgdst);
	if (rcode != NO_ERROR) goto RET;
	rcode = IMAGE_DilationV(pimgsrc, pimgdst);

RET:
	return rcode;
}


/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_OpenBlock
//
// ��  ��: 3x3�鿪
//
// ��  ��: IMAGE * const pimgsrc ԴӰ��
//         IMAGE * const pimgdst Ŀ��Ӱ��
//
// ����ֵ: int
//===================================================================
EXPORT_C int IMAGE_OpenBlock(IMAGE * const pimgsrc, IMAGE * const pimgdst)
{
	int rcode = NO_ERROR;

	rcode = IMAGE_ErosionBlock(pimgsrc, pimgdst);
	if (rcode != NO_ERROR) goto RET;
	rcode = IMAGE_DilationBlock(pimgsrc, pimgdst);

RET:
	return rcode;
}
