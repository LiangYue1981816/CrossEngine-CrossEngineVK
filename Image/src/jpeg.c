/////////////////////////////////////////////////////////////////////
// jpeg.c
//
// ��  ��: �ṩ��*.jpgӰ���I/O����
//
// ��  ��: 16:45 2004-12-06
//         10:09 2004-12-13
//         11:39 2005-06-19
//         10:27 2006-03-15
//         13:22 2006-03-24
//         09:45 2006-03-25
//         10:53 2006-03-31
//         21:11 2007-09-17
//===================================================================

#include "_image.h"
#include "jerror.h"
#include "jpeglib.h"
#include "jconfig.h"

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_LoadJpgHeader
//
// ��  ��: ���jpeg�ļ�BITMAPͷ��Ϣ
//
// ��  ��: char             * const pszfilename    �ļ�ȫ·��
//         BITMAPINFOHEADER * const pbmpinfoheader BITMAP��Ϣͷ
//
// ����ֵ: int
//===================================================================
EXPORT_C int IMAGE_LoadJpgHeader(char * const pszfilename, BITMAPINFOHEADER * const pbmpinfoheader)
{
	int rcode = NO_ERROR;

	FILE *pfile = NULL;

	//
	// 0. ������ȫ���
	//
	assert(pszfilename);
	assert(pbmpinfoheader);
	if (NULL == pszfilename || NULL == pbmpinfoheader) {
		return ERR_PARAMETER;
	}

	//
	// 1. ���ļ�
	//
	pfile = fopen(pszfilename, "rb");
	if (NULL == pfile) { rcode = ERR_IO_OPENFILE; goto RET; }

	//
	// 2. ��ȡ�ļ�ͷ
	//
	rcode = IMAGE_LoadJpgHeaderEx(pfile, pbmpinfoheader);
	if (rcode != NO_ERROR) goto RET;

RET:
	//
	// 3. �ر��ļ�
	//
	if (pfile) fclose(pfile);

	return rcode;
}

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_LoadJpgHeaderEx
//
// ��  ��: ���jpeg�ļ�BITMAPͷ��Ϣ
//
// ��  ��: FILE             * const pfile          �ļ�ָ��
//         BITMAPINFOHEADER * const pbmpinfoheader BITMAP��Ϣͷ
//
// ����ֵ: int
//===================================================================
EXPORT_C int IMAGE_LoadJpgHeaderEx(FILE * const pfile, BITMAPINFOHEADER * const pbmpinfoheader)
{
	int rcode = NO_ERROR;

	unsigned char mark[2];

	struct jpeg_error_mgr jerr;
	struct jpeg_decompress_struct cinfo;

	//
	// 0. ������ȫ���
	//
	assert(pfile);
	assert(pbmpinfoheader);
	if (NULL == pfile || NULL == pbmpinfoheader) {
		return ERR_PARAMETER;
	}

	//
	// 1. ����ѹ��
	//
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, pfile);

	//
	// 2. ��ȡ�ļ�ͷ�������и�ʽ���
	//
	fread(mark, 2, sizeof(unsigned char), pfile); fseek(pfile, -2, SEEK_CUR);
	if (0xFF != mark[0] || 0xD8 != mark[1]) { rcode = ERR_IO_FORMAT; goto RET; }

	jpeg_read_header(&cinfo, TRUE);
	if ((1 != cinfo.num_components && 3 != cinfo.num_components) || // Ӱ��ɫ����
		 0 == cinfo.image_width || 0 == cinfo.image_height ||		// Ӱ��ߴ���
		 JCS_UNKNOWN == cinfo.jpeg_color_space) {					// Ӱ���ɫ����
		rcode = ERR_IO_FORMAT; goto RET;
	}

	//
	// 3. ���BITMAPINFOHEADER�ṹ
	//
	pbmpinfoheader->biWidth = cinfo.image_width;
	pbmpinfoheader->biHeight = cinfo.image_height;
	pbmpinfoheader->biBitCount = cinfo.num_components*8;

RET:
	return rcode;
}

////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_LoadJpg
//
// ��  ��: ����jpeg�ļ�
//
// ��  ��: char  * const pszfilename �ļ�ȫ·��
//         IMAGE * const pimage      Ӱ���ļ��ṹ
//
// ����ֵ: int
//==============================================================
EXPORT_C int IMAGE_LoadJpg(char * const pszfilename, IMAGE * const pimage)
{
	int rcode = NO_ERROR;

	FILE *pfile = NULL;

	//
	// 0. ������ȫ���
	//
	assert(pimage);
	assert(pszfilename);
	if (NULL == pszfilename) { // �����������
		return ERR_PARAMETER;
	}
	if (NULL == pimage || NULL != pimage->data) { // Ӱ��δ����ʼ���������Ѿ�������洢
		return ERR_IMAGE;
	}

	//
	// 1. ���ļ�
	//
	pfile = fopen(pszfilename, "rb");
	if (NULL == pfile) { rcode = ERR_IO_OPENFILE; goto RET; }

	//
	// 2. ����Jpeg�ļ�
	//
	rcode = IMAGE_LoadJpgEx(pfile, pimage);
	if (rcode != NO_ERROR) goto RET;

RET:
	//
	// 3. �ر��ļ�
	//
	if (pfile) fclose(pfile);

	return rcode;
}

////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_LoadJpgEx
//
// ��  ��: ����jpeg�ļ�
//
// ��  ��: FILE  * const pfile  �ļ�ָ��
//         IMAGE * const pimage Ӱ���ļ��ṹ
//
// ����ֵ: int
//==============================================================
EXPORT_C int IMAGE_LoadJpgEx(FILE * const pfile, IMAGE * const pimage)
{
	int rcode = NO_ERROR;

	unsigned long i;
	unsigned char temp;
	unsigned char *pdata = NULL;
	unsigned char mark[2];

	struct jpeg_error_mgr jerr;
	struct jpeg_decompress_struct cinfo;

	//
	// 0. ������ȫ���
	//
	assert(pfile);
	assert(pimage);
	if (NULL == pfile) { // �����������
		return ERR_PARAMETER;
	}
	if (NULL == pimage || NULL != pimage->data) { // Ӱ��δ����ʼ���������Ѿ�������洢
		return ERR_IMAGE;
	}

	//
	// 1. ����ѹ��
	//
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, pfile);

	//
	// 2. ��ȡ�ļ�ͷ�������и�ʽ���
	//
	fread(mark, 2, sizeof(unsigned char), pfile); fseek(pfile, -2, SEEK_CUR);
	if (0xFF != mark[0] || 0xD8 != mark[1]) { rcode = ERR_IO_FORMAT; goto RET; }

	jpeg_read_header(&cinfo, TRUE);
	if ((1 != cinfo.num_components && 3 != cinfo.num_components) || // Ӱ��ɫ����
		 0 == cinfo.image_width || 0 == cinfo.image_height ||		// Ӱ��ߴ���
		 JCS_UNKNOWN == cinfo.jpeg_color_space) {					// Ӱ���ɫ����
		rcode = ERR_IO_FORMAT; goto RET;
	}

	//
	// 3. ����Ӱ����Ϣ����Ӱ��
	//
	rcode = IMAGE_AllocImage(pimage, cinfo.image_width, cinfo.image_height, cinfo.num_components*8);
	if (rcode != NO_ERROR) goto RET;

	//
	// 4. ��ʼ��ѹ��
	//
	jpeg_start_decompress(&cinfo);
	pdata = pimage->data + (IMAGE_HEIGHT(pimage)-1)*pimage->buffWidth;
	while (cinfo.output_scanline < cinfo.output_height) {
		jpeg_read_scanlines(&cinfo, &pdata, 1);
		// ע�⣺����24bitӰ��ѹ��ʹ��RGB,RGB,RGB...��
		//		 ʽ�����洢״̬��BGR,BGR,BGR...��Ҫ����R,B
		//		 �����������ܵõ���ȷ���
		if (24 == IMAGE_BITCOUNT(pimage)) {
			for (i = 0; i < pimage->buffWidth - 3; i += 3) {
				temp = pdata[i+2]; pdata[i+2] = pdata[i]; pdata[i] = temp;
			}
		}
		pdata -= pimage->buffWidth;
	}
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	//
	// 5. ���Ǳ�׼Ӱ��ת������׼Ӱ��
	//
	rcode = _converttostdimage(pimage);
	if (rcode != NO_ERROR) IMAGE_FreeImage(pimage);

RET:
	return rcode;
}

////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_SaveJpg
//
// ��  ��: ����jpeg�ļ�
//
// ��  ��: char  * const pszfilename �ļ�ȫ·��
//         IMAGE * const pimage      Ӱ���ļ��ṹ
//         int     const nquality    ��������
//
// ����ֵ: int
//==============================================================
EXPORT_C int IMAGE_SaveJpg(char * const pszfilename, IMAGE * const pimage, int const nquality)
{
	int rcode = NO_ERROR;

	FILE *pfile = NULL;

	//
	// 0. ������飬ֻ֧��8bit��24bitӰ��
	//
	assert(pimage);
	assert(pszfilename);
	if (NULL == pszfilename || nquality < 0 || nquality > 100) { // ���������Ч
		return ERR_PARAMETER;
	}
	if (NULL == pimage || NULL == pimage->data) { // Ӱ����Ч
		return ERR_IMAGE;
	}
	if (1 == IMAGE_BITCOUNT(pimage)) { // ɫ����Ч
		return ERR_BITCOUNT;
	}

	//
	// 1. �Զ����Ʒ�ʽ���ļ�
	//
	pfile = fopen(pszfilename, "wb");
	if (NULL == pfile) { rcode = ERR_IO_OPENFILE; goto RET; }

	//
	// 2. ����Jpeg�ļ�
	//
	rcode = IMAGE_SaveJpgEx(pfile, pimage, nquality);
	if (rcode != NO_ERROR) goto RET;

RET:
	//
	// 3. �ر��ļ�
	//
	if (pfile) fclose(pfile);

	return rcode;
}

////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_SaveJpgEx
//
// ��  ��: ����jpeg�ļ�
//
// ��  ��: FILE  * const pfile    �ļ�ָ��
//         IMAGE * const pimage   Ӱ���ļ��ṹ
//         int     const nquality ��������
//
// ����ֵ: int
//==============================================================
EXPORT_C int IMAGE_SaveJpgEx(FILE * const pfile, IMAGE * const pimage, int const nquality)
{
	unsigned long i;
	unsigned char temp;
	unsigned char *pdata = NULL;

	struct jpeg_error_mgr jerr;
	struct jpeg_compress_struct cinfo;

	//
	// 0. ������飬ֻ֧��8bit��24bitӰ��
	//
	assert(pfile);
	assert(pimage);
	if (NULL == pfile || nquality < 0 || nquality > 100) { // ���������Ч
		return ERR_PARAMETER;
	}
	if (NULL == pimage || NULL == pimage->data) { // Ӱ����Ч
		return ERR_IMAGE;
	}
	if (1 == IMAGE_BITCOUNT(pimage)) { // ɫ����Ч
		return ERR_BITCOUNT;
	}

	//
	// 1. ����ѹ��
	//
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, pfile);

	//
	// 2. ��дѹ��Ӱ�����Ϣ
	//
	cinfo.image_width		= IMAGE_WIDTH(pimage);
	cinfo.image_height		= IMAGE_HEIGHT(pimage);
	cinfo.input_components	= IMAGE_BITCOUNT(pimage)/8; // 1��Ӧ8bit��3��Ӧ24bit
	cinfo.in_color_space	= IMAGE_BITCOUNT(pimage)==24 ? JCS_RGB:JCS_GRAYSCALE;
	
	//
	// 3. ����ѹ�������Լ�ѹ������(0~100)
	//
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, nquality, TRUE);

	//
	// 4. ����ѹ��
	//
	jpeg_start_compress(&cinfo, TRUE);
	pdata = pimage->data + (IMAGE_HEIGHT(pimage)-1)*pimage->buffWidth;
	while (cinfo.next_scanline < cinfo.image_height) {
		// ע�⣺����24bitӰ��ѹ��ʹ��RGB,RGB,RGB...��
		//		 ʽ�����洢״̬��BGR,BGR,BGR...��Ҫ����R,B
		//		 �����������ܵõ���ȷ���
		if (24 == IMAGE_BITCOUNT(pimage)) {
			for (i = 0; i < pimage->buffWidth - 3; i += 3) {
				temp = pdata[i+2]; pdata[i+2] = pdata[i]; pdata[i] = temp;
			}
		}
		jpeg_write_scanlines(&cinfo, &pdata, TRUE);
		pdata -= pimage->buffWidth;
	}
	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);

	return NO_ERROR;
}
