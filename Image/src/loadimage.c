/////////////////////////////////////////////////////////////////////
// loadimage.c
//
// ��  ��: �ṩӰ��I/O�ܺ���
//
// ��  ��: 16:45 2004-12-06
//         10:09 2004-12-13
//         11:39 2005-06-19
//         21:24 2007-09-17
//===================================================================

#include "_image.h"

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_LoadImage
//
// ��  ��: ����jpeg�ļ�
//
// ��  ��: char  * const pszfilename �ļ�ȫ·��
//         IMAGE * const pimage      Ӱ���ļ��ṹ
//
// ����ֵ: int
//===================================================================
EXPORT_C int IMAGE_LoadImage(char * const pszfilename, IMAGE * const pimage)
{
	int rcode = NO_ERROR;

	//
	// 0. ��ȫ���
	//
	assert(pimage);
	if (NULL == pszfilename) { // ��������
		return ERR_PARAMETER;
	}
	if (NULL == pimage || NULL != pimage->data) { // Ӱ��δ����ʼ���������Ѿ�������洢
		return ERR_IMAGE;
	}
	
	//
	// 1. ��ͼ����Ӱ��
	//
	// bitmap
	rcode = IMAGE_LoadBmp(pszfilename, pimage);
	if (NO_ERROR == rcode || ERR_IO_OPENFILE == rcode) goto RET; // ���سɹ����ߴ��ļ�ʧ��

	// jpeg
	rcode = IMAGE_LoadJpg(pszfilename, pimage);
	if (NO_ERROR == rcode || ERR_IO_OPENFILE == rcode) goto RET; // ���سɹ����ߴ��ļ�ʧ��

	// png
	rcode = IMAGE_LoadPng(pszfilename, pimage);
	if (NO_ERROR == rcode || ERR_IO_OPENFILE == rcode) goto RET; // ���سɹ����ߴ��ļ�ʧ��

	// ...

RET:
	return rcode;
}

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_LoadImageEx
//
// ��  ��: ����jpeg�ļ�
//
// ��  ��: FILE  * const pfile  �ļ�ָ��
//         IMAGE * const pimage Ӱ���ļ��ṹ
//
// ����ֵ: int
//===================================================================
EXPORT_C int IMAGE_LoadImageEx(FILE * const pfile, IMAGE * const pimage)
{
	int rcode = NO_ERROR;

	//
	// 0. ��ȫ���
	//
	assert(pfile);
	if (NULL == pfile) { // ��������
		return ERR_PARAMETER;
	}
	if (NULL == pimage || NULL != pimage->data) { // Ӱ��δ����ʼ���������Ѿ�������洢
		return ERR_IMAGE;
	}
	
	//
	// 1. ��ͼ����Ӱ��
	//
	// bitmap
	rcode = IMAGE_LoadBmpEx(pfile, pimage);
	if (NO_ERROR == rcode || ERR_IO_OPENFILE == rcode) goto RET; // ���سɹ����ߴ��ļ�ʧ��

	// jpeg
	rcode = IMAGE_LoadJpgEx(pfile, pimage);
	if (NO_ERROR == rcode || ERR_IO_OPENFILE == rcode) goto RET; // ���سɹ����ߴ��ļ�ʧ��

	// png
	rcode = IMAGE_LoadPngEx(pfile, pimage);
	if (NO_ERROR == rcode || ERR_IO_OPENFILE == rcode) goto RET; // ���سɹ����ߴ��ļ�ʧ��

	// ...

RET:
	return rcode;
}
