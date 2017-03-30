/////////////////////////////////////////////////////////////////////
// open.c
//
// 功  能: 形态学开
//
// 修  订: 20:26 2005-01-02
//         12:23 2005-06-19
//===================================================================

#include "_image.h"

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_OpenH
//
// 功  能: 水平开
//
// 参  数: IMAGE * const pimgsrc 源影像
//         IMAGE * const pimgdst 目标影像
//
// 返回值: int
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
// 名  称: IMAGE_OpenV
//
// 功  能: 垂直开
//
// 参  数: IMAGE * const pimgsrc 源影像
//         IMAGE * const pimgdst 目标影像
//
// 返回值: int
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
// 名  称: IMAGE_OpenBlock
//
// 功  能: 3x3块开
//
// 参  数: IMAGE * const pimgsrc 源影像
//         IMAGE * const pimgdst 目标影像
//
// 返回值: int
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
