/////////////////////////////////////////////////////////////////////
// close.c
//
// ��  ��: ��̬ѧ�ղ���
//
// ��  ��: 20:26 2005-01-02
//         12:23 2005-06-19
//===================================================================

#include "_image.h"

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_CloseH
//
// ��  ��: ˮƽ��
//
// ��  ��: IMAGE * const pimgsrc ԴӰ��
//         IMAGE * const pimgdst Ŀ��Ӱ��
//
// ����ֵ: int
//===================================================================
EXPORT_C int IMAGE_CloseH(IMAGE * const pimgsrc, IMAGE * const pimgdst)
{
	int rcode = NO_ERROR;

	rcode = IMAGE_DilationH(pimgsrc, pimgdst);
	if (rcode != NO_ERROR) goto RET;
	rcode = IMAGE_ErosionH(pimgsrc, pimgdst);

RET:
	return rcode;
}


/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_CloseV
//
// ��  ��: ��ֱ��
//
// ��  ��: IMAGE * const pimgsrc ԴӰ��
//         IMAGE * const pimgdst Ŀ��Ӱ��
//
// ����ֵ: int
//===================================================================
EXPORT_C int IMAGE_CloseV(IMAGE * const pimgsrc, IMAGE * const pimgdst)
{
	int rcode = NO_ERROR;

	rcode = IMAGE_DilationV(pimgsrc, pimgdst);
	if (rcode != NO_ERROR) goto RET;
	rcode = IMAGE_ErosionV(pimgsrc, pimgdst);

RET:
	return rcode;
}


/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_CloseBlock
//
// ��  ��: 3x3���
//
// ��  ��: IMAGE * const pimgsrc ԴӰ��
//         IMAGE * const pimgdst Ŀ��Ӱ��
//
// ����ֵ: int
//===================================================================
EXPORT_C int IMAGE_CloseBlock(IMAGE * const pimgsrc, IMAGE * const pimgdst)
{
	int rcode = NO_ERROR;

	rcode = IMAGE_DilationBlock(pimgsrc, pimgdst);
	if (rcode != NO_ERROR) goto RET;
	rcode = IMAGE_ErosionBlock(pimgsrc, pimgdst);

RET:
	return rcode;
}
