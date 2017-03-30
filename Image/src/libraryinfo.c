/////////////////////////////////////////////////////////////////////
// libraryinfo.c
//
// ��  ��: IMAGE Library��Ϣ
//
// ��  ��: 16:45 2004-12-06
//         10:09 2004-12-13
//         11:39 2005-06-19
//===================================================================

#include "_image.h"

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_GetLibraryInfo
//
// ��  ��: ���Ӱ�����Ϣ
//
// ��  ��: char * const pszinfo
//
// ����ֵ: void
//===================================================================
EXPORT_C void IMAGE_GetLibraryInfo(char * const pszinfo)
{
	//
	// 0. ������ȫ���
	//
	assert(pszinfo);

	//
	// 1. ��ӡ��Ϣ
	//
	if (strlen(pszinfo) > 200)
		sprintf(pszinfo, "Image Processing Library.\n\t Copyright (c) 2007~2015 Liang.Yue. All Rights Reserved.\n");
}

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_GetLibraryInfo
//
// ��  ��: ���Ӱ�����Ϣ
//
// ��  ��: char * const pszinfo
//
// ����ֵ: unsigned long
//===================================================================
EXPORT_C unsigned long IMAGE_GetVersion(void)
{
	return VERSION;
}
