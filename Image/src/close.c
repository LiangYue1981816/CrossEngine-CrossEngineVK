/////////////////////////////////////////////////////////////////////
// close.c
//
// 功  能: 形态学闭操作
//
// 修  订: 20:26 2005-01-02
//         12:23 2005-06-19
//===================================================================

#include "_image.h"

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_CloseH
//
// 功  能: 水平闭
//
// 参  数: IMAGE * const pimgsrc 源影像
//         IMAGE * const pimgdst 目标影像
//
// 返回值: int
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
// 名  称: IMAGE_CloseV
//
// 功  能: 垂直闭
//
// 参  数: IMAGE * const pimgsrc 源影像
//         IMAGE * const pimgdst 目标影像
//
// 返回值: int
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
// 名  称: IMAGE_CloseBlock
//
// 功  能: 3x3块闭
//
// 参  数: IMAGE * const pimgsrc 源影像
//         IMAGE * const pimgdst 目标影像
//
// 返回值: int
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
