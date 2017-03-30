/////////////////////////////////////////////////////////////////////
// png.c
//
// 功  能: 提供对*.png影像的I/O操作
//
// 修  订: 07:40 2013-07-24
//===================================================================

#include "_image.h"
#include "png.h"

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_LoadPngHeader
//
// 功  能: 加载 .png 影像文件 BITMAPINFOHEADER 部分
//
// 参  数: char             * const pszfilename    文件路径
//         BITMAPINFOHEADER * const pbmpinfoheader 头指针
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_LoadPngHeader(char * const pszfilename, BITMAPINFOHEADER * const pbmpinfoheader)
{
	int rcode = NO_ERROR;

	FILE *pfile = NULL;

	//
	// 0. 参数安全检查
	//
	assert(pszfilename);
	assert(pbmpinfoheader);
	if (NULL == pszfilename || NULL == pbmpinfoheader) {
		return ERR_PARAMETER;
	}

	//
	// 1. 打开文件
	//
	pfile = fopen(pszfilename, "rb");
	if (NULL == pfile) { rcode = ERR_IO_OPENFILE; goto RET; }

	//
	// 2. 读取文件头
	//
	rcode = IMAGE_LoadPngHeaderEx(pfile, pbmpinfoheader);
	if (rcode != NO_ERROR) goto RET;

RET:
	//
	// 3. 关闭文件
	//
	if (pfile) fclose(pfile);

	return rcode;
}

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_LoadPngHeaderEx
//
// 功  能: 加载 .png 影像文件 BITMAPINFOHEADER 部分
//
// 参  数: FILE             * const pfile          文件指针
//         BITMAPINFOHEADER * const pbmpinfoheader 头指针
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_LoadPngHeaderEx(FILE * const pfile, BITMAPINFOHEADER * const pbmpinfoheader)
{
	int rcode = NO_ERROR;

	png_structp png_ptr;
	png_infop info_ptr;
	png_uint_32 width, height, bit_depth, bit_count, color_type;

	//
	// 0. 参数安全检查
	//
	assert(pfile);
	assert(pbmpinfoheader);
	if (NULL == pfile || NULL == pbmpinfoheader) {
		return ERR_PARAMETER;
	}

	//
	// 1. 读取文件头
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
	// 2. 填充BITMAPINFOHEADER结构
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
// 名  称: IMAGE_LoadPng
//
// 功  能: 加载 .png 影像文件
//
// 参  数: char  * const pszfilename 文件路径
//         IMAGE * const pimage      影像头指针
//
// 返回值：int
//===================================================================
EXPORT_C int IMAGE_LoadPng(char * const pszfilename, IMAGE * const pimage)
{
	int rcode = NO_ERROR;

	FILE *pfile = NULL;

	//
	// 0. 参数安全检查
	//
	assert(pimage);
	assert(pszfilename);
	if (NULL == pszfilename) { // 输入参数错误
		return ERR_PARAMETER;
	}
	if (NULL == pimage || NULL != pimage->data) { // 影像未被初始化，或者已经被分配存储
		return ERR_IMAGE;
	}

	//
	// 1. 打开文件
	//
	pfile = fopen(pszfilename, "rb");
	if (NULL == pfile) { rcode = ERR_IO_OPENFILE; goto RET; }

	//
	// 2. 加载Png文件
	//
	rcode = IMAGE_LoadPngEx(pfile, pimage);
	if (rcode != NO_ERROR) goto RET;

RET:
	//
	// 3. 关闭文件
	//
	if (pfile) fclose(pfile);

	return rcode;
}

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_LoadPngEx
//
// 功  能: 加载 .png 影像文件
//
// 参  数: FILE  * const pfile  文件指针
//         IMAGE * const pimage 影像头指针
//
// 返回值：int
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
	// 0. 参数安全检查
	//
	assert(pfile);
	assert(pimage);
	if (NULL == pfile) { // 输入参数错误
		return ERR_PARAMETER;
	}
	if (NULL == pimage || NULL != pimage->data) { // 影像未被初始化，或者已经被分配存储
		return ERR_IMAGE;
	}

	//
	// 1. 加载文件
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
		// 注意：对于24/32bit影像，使用RGB,RGB,RGB...形式，
		//		 而存储状态是BGR,BGR,BGR...需要进行R,B
		//		 交换操作才能得到正确结果
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
	// 2. 将非标准影像转换到标准影像
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
// 名  称: IMAGE_SavePng
//
// 功  能: 保存标准非压缩的 .png 影像文件
//
// 参  数: char  * const pszfilename 文件路径
//         IMAGE * const pimage      影像头指针
//
// 返回值：int 
//===================================================================
EXPORT_C int IMAGE_SavePng(char * const pszfilename, IMAGE * const pimage)
{
	int rcode = NO_ERROR;

	FILE *pfile = NULL;

	//
	// 0. 参数检查
	//
	assert(pimage);
	assert(pszfilename);
	if (NULL == pszfilename) { // 输入参数无效
		return ERR_PARAMETER;
	}
	if (NULL == pimage || NULL == pimage->data) { // 影像无效
		return ERR_IMAGE;
	}
	if (1 == IMAGE_BITCOUNT(pimage)) { // 色深无效
		return ERR_BITCOUNT;
	}

	//
	// 1. 以二进制方式打开文件
	//
	pfile = fopen(pszfilename, "wb");
	if (NULL == pfile) { rcode = ERR_IO_OPENFILE; goto RET; }

	//
	// 2. 保存Png文件
	//
	rcode = IMAGE_SavePngEx(pfile, pimage);
	if (rcode != NO_ERROR) goto RET;

RET:
	//
	// 3. 关闭文件
	//
	if (pfile) fclose(pfile);

	return rcode;
}

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_SavePngEx
//
// 功  能: 保存标准非压缩的 .png 影像文件
//
// 参  数: FILE  * const pfile  文件指针
//         IMAGE * const pimage 影像头指针
//
// 返回值：int 
//===================================================================
EXPORT_C int IMAGE_SavePngEx(FILE * const pfile, IMAGE * const pimage)
{
	int rcode = NO_ERROR;
	return rcode;
}
