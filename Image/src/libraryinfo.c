/////////////////////////////////////////////////////////////////////
// libraryinfo.c
//
// 功  能: IMAGE Library信息
//
// 修  订: 16:45 2004-12-06
//         10:09 2004-12-13
//         11:39 2005-06-19
//===================================================================

#include "_image.h"

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_GetLibraryInfo
//
// 功  能: 获得影像库信息
//
// 参  数: char * const pszinfo
//
// 返回值: void
//===================================================================
EXPORT_C void IMAGE_GetLibraryInfo(char * const pszinfo)
{
	//
	// 0. 参数安全检查
	//
	assert(pszinfo);

	//
	// 1. 打印信息
	//
	if (strlen(pszinfo) > 200)
		sprintf(pszinfo, "Image Processing Library.\n\t Copyright (c) 2007~2015 Liang.Yue. All Rights Reserved.\n");
}

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_GetLibraryInfo
//
// 功  能: 获得影像库信息
//
// 参  数: char * const pszinfo
//
// 返回值: unsigned long
//===================================================================
EXPORT_C unsigned long IMAGE_GetVersion(void)
{
	return VERSION;
}
