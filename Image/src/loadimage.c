/////////////////////////////////////////////////////////////////////
// loadimage.c
//
// 功  能: 提供影像I/O总函数
//
// 修  订: 16:45 2004-12-06
//         10:09 2004-12-13
//         11:39 2005-06-19
//         21:24 2007-09-17
//===================================================================

#include "_image.h"

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_LoadImage
//
// 功  能: 加载jpeg文件
//
// 参  数: char  * const pszfilename 文件全路经
//         IMAGE * const pimage      影像文件结构
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_LoadImage(char * const pszfilename, IMAGE * const pimage)
{
	int rcode = NO_ERROR;

	//
	// 0. 安全检查
	//
	assert(pimage);
	if (NULL == pszfilename) { // 参数错误
		return ERR_PARAMETER;
	}
	if (NULL == pimage || NULL != pimage->data) { // 影像未被初始化，或者已经被分配存储
		return ERR_IMAGE;
	}
	
	//
	// 1. 试图加载影像
	//
	// bitmap
	rcode = IMAGE_LoadBmp(pszfilename, pimage);
	if (NO_ERROR == rcode || ERR_IO_OPENFILE == rcode) goto RET; // 加载成功或者打开文件失败

	// jpeg
	rcode = IMAGE_LoadJpg(pszfilename, pimage);
	if (NO_ERROR == rcode || ERR_IO_OPENFILE == rcode) goto RET; // 加载成功或者打开文件失败

	// png
	rcode = IMAGE_LoadPng(pszfilename, pimage);
	if (NO_ERROR == rcode || ERR_IO_OPENFILE == rcode) goto RET; // 加载成功或者打开文件失败

	// ...

RET:
	return rcode;
}

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_LoadImageEx
//
// 功  能: 加载jpeg文件
//
// 参  数: FILE  * const pfile  文件指针
//         IMAGE * const pimage 影像文件结构
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_LoadImageEx(FILE * const pfile, IMAGE * const pimage)
{
	int rcode = NO_ERROR;

	//
	// 0. 安全检查
	//
	assert(pfile);
	if (NULL == pfile) { // 参数错误
		return ERR_PARAMETER;
	}
	if (NULL == pimage || NULL != pimage->data) { // 影像未被初始化，或者已经被分配存储
		return ERR_IMAGE;
	}
	
	//
	// 1. 试图加载影像
	//
	// bitmap
	rcode = IMAGE_LoadBmpEx(pfile, pimage);
	if (NO_ERROR == rcode || ERR_IO_OPENFILE == rcode) goto RET; // 加载成功或者打开文件失败

	// jpeg
	rcode = IMAGE_LoadJpgEx(pfile, pimage);
	if (NO_ERROR == rcode || ERR_IO_OPENFILE == rcode) goto RET; // 加载成功或者打开文件失败

	// png
	rcode = IMAGE_LoadPngEx(pfile, pimage);
	if (NO_ERROR == rcode || ERR_IO_OPENFILE == rcode) goto RET; // 加载成功或者打开文件失败

	// ...

RET:
	return rcode;
}
