/////////////////////////////////////////////////////////////////////
// png.c
//
// ��  ��: �ṩ��*.pngӰ���I/O����
//
// ��  ��: 07:40 2013-07-24
//===================================================================

#include "_image.h"
#include "png.h"

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_LoadPngHeader
//
// ��  ��: ���� .png Ӱ���ļ� BITMAPINFOHEADER ����
//
// ��  ��: char             * const pszfilename    �ļ�·��
//         BITMAPINFOHEADER * const pbmpinfoheader ͷָ��
//
// ����ֵ: int
//===================================================================
EXPORT_C int IMAGE_LoadPngHeader(char * const pszfilename, BITMAPINFOHEADER * const pbmpinfoheader)
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
	rcode = IMAGE_LoadPngHeaderEx(pfile, pbmpinfoheader);
	if (rcode != NO_ERROR) goto RET;

RET:
	//
	// 3. �ر��ļ�
	//
	if (pfile) fclose(pfile);

	return rcode;
}

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_LoadPngHeaderEx
//
// ��  ��: ���� .png Ӱ���ļ� BITMAPINFOHEADER ����
//
// ��  ��: FILE             * const pfile          �ļ�ָ��
//         BITMAPINFOHEADER * const pbmpinfoheader ͷָ��
//
// ����ֵ: int
//===================================================================
EXPORT_C int IMAGE_LoadPngHeaderEx(FILE * const pfile, BITMAPINFOHEADER * const pbmpinfoheader)
{
	int rcode = NO_ERROR;

	png_structp png_ptr;
	png_infop info_ptr;
	png_uint_32 width, height, bit_depth, bit_count, color_type;

	//
	// 0. ������ȫ���
	//
	assert(pfile);
	assert(pbmpinfoheader);
	if (NULL == pfile || NULL == pbmpinfoheader) {
		return ERR_PARAMETER;
	}

	//
	// 1. ��ȡ�ļ�ͷ
	//
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	if (!png_ptr) goto ERR1;

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) goto ERR2;

	png_init_io(png_ptr, pfile);
	png_read_info(png_ptr, info_ptr);

	width = png_get_image_width(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);

	if (color_type == PNG_COLOR_TYPE_RGB) { bit_count = 24; }
	if (color_type == PNG_COLOR_TYPE_RGB_ALPHA) { bit_count = 32; }
	if (color_type == PNG_COLOR_TYPE_PALETTE) { bit_count = 24; }
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) { bit_count = 8; }
	if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) { bit_count = 32; }

	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

	//
	// 2. ���BITMAPINFOHEADER�ṹ
	//
	pbmpinfoheader->biWidth = width;
	pbmpinfoheader->biHeight = height;
	pbmpinfoheader->biBitCount = bit_count;

	goto RET;
ERR1:
	png_destroy_read_struct(&png_ptr, NULL, NULL);
	goto RET;
ERR2:
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	goto RET;
RET:
	return rcode;
}

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_LoadPng
//
// ��  ��: ���� .png Ӱ���ļ�
//
// ��  ��: char  * const pszfilename �ļ�·��
//         IMAGE * const pimage      Ӱ��ͷָ��
//
// ����ֵ��int
//===================================================================
EXPORT_C int IMAGE_LoadPng(char * const pszfilename, IMAGE * const pimage)
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
	// 2. ����Png�ļ�
	//
	rcode = IMAGE_LoadPngEx(pfile, pimage);
	if (rcode != NO_ERROR) goto RET;

RET:
	//
	// 3. �ر��ļ�
	//
	if (pfile) fclose(pfile);

	return rcode;
}

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_LoadPngEx
//
// ��  ��: ���� .png Ӱ���ļ�
//
// ��  ��: FILE  * const pfile  �ļ�ָ��
//         IMAGE * const pimage Ӱ��ͷָ��
//
// ����ֵ��int
//===================================================================
EXPORT_C int IMAGE_LoadPngEx(FILE * const pfile, IMAGE * const pimage)
{
	int rcode = NO_ERROR;

	unsigned long i;
	unsigned char temp;
	unsigned char *pdata = NULL;

	png_structp png_ptr;
	png_infop info_ptr;
	png_uint_32 y, width, height, bit_depth, bit_count, color_type;

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
	// 1. �����ļ�
	//
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	if (!png_ptr) goto ERR1;

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) goto ERR2;

	png_init_io(png_ptr, pfile);
	png_read_info(png_ptr, info_ptr);

	width = png_get_image_width(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);

	if (bit_depth == 16) png_set_strip_16(png_ptr);
	if (color_type == PNG_COLOR_TYPE_RGB) { bit_count = 24; }
	if (color_type == PNG_COLOR_TYPE_RGB_ALPHA) { bit_count = 32; }
	if (color_type == PNG_COLOR_TYPE_PALETTE) { png_set_palette_to_rgb(png_ptr); bit_count = 24; }
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) { png_set_expand_gray_1_2_4_to_8(png_ptr); bit_count = 8; }
	if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) { png_set_gray_to_rgb(png_ptr); bit_count = 32; }
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png_ptr);

	rcode = IMAGE_AllocImage(pimage, width, height, bit_count);
	if (rcode != NO_ERROR) goto ERR2;

	png_read_update_info(png_ptr, info_ptr);
	pdata = pimage->data + (IMAGE_HEIGHT(pimage)-1)*pimage->buffWidth;
	for (y = 0; y < height; y++) {
		png_read_rows(png_ptr, &pdata, NULL, 1);
		// ע�⣺����24/32bitӰ��ʹ��RGB,RGB,RGB...��ʽ��
		//		 ���洢״̬��BGR,BGR,BGR...��Ҫ����R,B
		//		 �����������ܵõ���ȷ���
		if (24 == IMAGE_BITCOUNT(pimage)) {
			for (i = 0; i < pimage->buffWidth - 3; i += 3) {
				temp = pdata[i+2]; pdata[i+2] = pdata[i]; pdata[i] = temp;
			}
		}
		if (32 == IMAGE_BITCOUNT(pimage)) {
			for (i = 0; i < pimage->buffWidth - 4; i += 4) {
				temp = pdata[i+2]; pdata[i+2] = pdata[i]; pdata[i] = temp;
			}
		}
		pdata -= pimage->buffWidth;
	}
	png_read_end(png_ptr, NULL);

	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

	//
	// 2. ���Ǳ�׼Ӱ��ת������׼Ӱ��
	//
	rcode = _converttostdimage(pimage);
	if (rcode != NO_ERROR) IMAGE_FreeImage(pimage);

	goto RET;
ERR1:
	png_destroy_read_struct(&png_ptr, NULL, NULL);
	goto RET;
ERR2:
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	goto RET;
RET:
	return rcode;
}

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_SavePng
//
// ��  ��: �����׼��ѹ���� .png Ӱ���ļ�
//
// ��  ��: char  * const pszfilename �ļ�·��
//         IMAGE * const pimage      Ӱ��ͷָ��
//
// ����ֵ��int 
//===================================================================
EXPORT_C int IMAGE_SavePng(char * const pszfilename, IMAGE * const pimage)
{
	int rcode = NO_ERROR;

	FILE *pfile = NULL;

	//
	// 0. �������
	//
	assert(pimage);
	assert(pszfilename);
	if (NULL == pszfilename) { // ���������Ч
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
	// 2. ����Png�ļ�
	//
	rcode = IMAGE_SavePngEx(pfile, pimage);
	if (rcode != NO_ERROR) goto RET;

RET:
	//
	// 3. �ر��ļ�
	//
	if (pfile) fclose(pfile);

	return rcode;
}

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_SavePngEx
//
// ��  ��: �����׼��ѹ���� .png Ӱ���ļ�
//
// ��  ��: FILE  * const pfile  �ļ�ָ��
//         IMAGE * const pimage Ӱ��ͷָ��
//
// ����ֵ��int 
//===================================================================
EXPORT_C int IMAGE_SavePngEx(FILE * const pfile, IMAGE * const pimage)
{
	int rcode = NO_ERROR;
	return rcode;
}
