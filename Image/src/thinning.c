/////////////////////////////////////////////////////////////////////
// thinning.c
//
// ��  ��: ��̬ѧϸ��
//
// ��  ��: 20:26 2005-01-02
//         12:23 2005-06-19
//         21:53 2007-01-09
//         16:38 2007-01-11
//===================================================================

#include "_image.h"

// ����
typedef struct {
	IMAGE *pimgsrc;
	IMAGE *pimgdst;
	IMAGE *pimgwrk;
} PARAM;

static void * thinning(void * pvoid);

/////////////////////////////////////////////////////////////////////
// ��  ��: IMAGE_Thinning
//
// ��  ��: ϸ��
//
// ��  ��: IMAGE * const pimgsrc ԴӰ��
//         IMAGE * const pimgdst Ŀ��Ӱ��
//
// ����ֵ: int
//===================================================================
EXPORT_C int IMAGE_Thinning(IMAGE * const pimgsrc, IMAGE * const pimgdst)
{
	int rcode = NO_ERROR;

	PARAM param;
	IMAGE imgwrk;
	IMAGE_ZeroImage(&imgwrk);

	//
	// 1. ��ȫ���
	//
	assert(pimgsrc);
	assert(pimgdst);
	if (NULL == pimgsrc->data || NULL == pimgdst->data) { // Ҫ��Ӱ����Ч��
		rcode = ERR_IMAGE; goto RET;
	}
	if (IMAGE_BITCOUNT(pimgsrc) != IMAGE_BITCOUNT(pimgdst) || 8 != IMAGE_BITCOUNT(pimgsrc)) { // Ҫ��ԭӰ����Ŀ��Ӱ��ɫ����ͬ������ֻ��8bitӰ����
		rcode = ERR_BITCOUNT; goto RET;
	}
	if (IMAGE_AREA_WIDTH(pimgsrc) < 3 || IMAGE_AREA_HEIGHT(pimgsrc) < 3) { // Ҫ��Ӱ�������򲻵�С�� 3x3
		rcode = ERR_RANGE; goto RET;
	}
	if (IMAGE_AREA_WIDTH(pimgsrc) != IMAGE_AREA_WIDTH(pimgdst) || IMAGE_AREA_HEIGHT(pimgsrc) != IMAGE_AREA_HEIGHT(pimgdst)) { // Ҫ��ԭӰ����Ŀ��Ӱ��������ߴ���ͬ
		rcode = ERR_RANGE; goto RET;
	}

	//
	// 1. ϸ��
	//
	rcode = _makeworkimage(pimgsrc, &imgwrk, 3);
	if (rcode != NO_ERROR) goto RET;

	// ���ò���
	param.pimgsrc = pimgsrc;
	param.pimgdst = pimgdst;
	param.pimgwrk = &imgwrk;

	// ����
	thinning((void*)&param);

RET:
	IMAGE_FreeImage(&imgwrk);
	return rcode;
}

static void * thinning(void * pvoid)
{
	PARAM *param = NULL;

	IMAGE *pimgsrc = NULL;
	IMAGE *pimgdst = NULL;
	IMAGE *pimgwrk = NULL;

	int x, y;
	int width, height;

	int index;
	BOOL bfinished;
	BOOL erasetable[256]={
			0,0,1,1,0,0,1,1,
			1,1,0,1,1,1,0,1,
			1,1,0,0,1,1,1,1,
			0,0,0,0,0,0,0,1,

			0,0,1,1,0,0,1,1,
			1,1,0,1,1,1,0,1,
			1,1,0,0,1,1,1,1,
			0,0,0,0,0,0,0,1,

			1,1,0,0,1,1,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,

			1,1,0,0,1,1,0,0,
			1,1,0,1,1,1,0,1,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,

			0,0,1,1,0,0,1,1,
			1,1,0,1,1,1,0,1,
			1,1,0,0,1,1,1,1,
			0,0,0,0,0,0,0,1,

			0,0,1,1,0,0,1,1,
			1,1,0,1,1,1,0,1,
			1,1,0,0,1,1,1,1,
			0,0,0,0,0,0,0,0,

			1,1,0,0,1,1,0,0,
			0,0,0,0,0,0,0,0,
			1,1,0,0,1,1,1,1,
			0,0,0,0,0,0,0,0,

			1,1,0,0,1,1,0,0,
			1,1,0,1,1,1,0,0,
			1,1,0,0,1,1,1,0,
			1,1,0,0,1,0,0,0};

	IMGGRAY *ppixel_src_top = NULL;
	IMGGRAY *ppixel_src_ths = NULL;
	IMGGRAY *ppixel_src_btm = NULL;
	IMGGRAY *ppixel_dst_ths = NULL;

	// ����
	param = (PARAM*)pvoid;
	
	pimgsrc = param->pimgsrc;
	pimgdst = param->pimgdst;
	pimgwrk = param->pimgwrk;

	// ϸ��
	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);

	bfinished = FALSE;
	while (!bfinished) {
		bfinished = TRUE;

		// 2.1 ˮƽɨ��
		for (y = 0; y < height; y++) {
			ppixel_src_top = (IMGGRAY *)(&(pimgwrk->matrix[pimgwrk->sy+y-1][pimgwrk->sx]));
			ppixel_src_ths = (IMGGRAY *)(&(pimgwrk->matrix[pimgwrk->sy+y  ][pimgwrk->sx]));
			ppixel_src_btm = (IMGGRAY *)(&(pimgwrk->matrix[pimgwrk->sy+y+1][pimgwrk->sx]));
			ppixel_dst_ths = (IMGGRAY *)(&(pimgdst->matrix[pimgdst->sy+y  ][pimgdst->sx]));

			for (x = 0; x < width; x++) {
				if (0 == ppixel_src_ths[x]) { // ��ǰ��Ϊ�ڵ�
					if (255 == ppixel_src_ths[x-1] || 255 == ppixel_src_ths[x+1]) { // �ж����������Ƿ���ڰ׵�
						index = ppixel_src_top[x-1]/255    + ppixel_src_top[x]/255*2  + ppixel_src_top[x+1]/255*4  +
								ppixel_src_ths[x-1]/255*8    /*         0         */  + ppixel_src_ths[x+1]/255*16 +
								ppixel_src_btm[x-1]/255*32 + ppixel_src_btm[x]/255*64 + ppixel_src_btm[x+1]/255*128;
						if(erasetable[index]){
							bfinished = FALSE;
							ppixel_dst_ths[x] = 255;
							ppixel_src_ths[x] = 255;
							x++;
						}
					}
				}
			}
		}

		// 2.2 ��ֱɨ��
		for (x = 0; x < width; x++) {
			for (y = 0; y < height; y++) {
				ppixel_src_top = (IMGGRAY *)(&(pimgwrk->matrix[pimgwrk->sy+y-1][pimgwrk->sx]));
				ppixel_src_ths = (IMGGRAY *)(&(pimgwrk->matrix[pimgwrk->sy+y  ][pimgwrk->sx]));
				ppixel_src_btm = (IMGGRAY *)(&(pimgwrk->matrix[pimgwrk->sy+y+1][pimgwrk->sx]));
				ppixel_dst_ths = (IMGGRAY *)(&(pimgdst->matrix[pimgdst->sy+y  ][pimgdst->sx]));

				if (0 == ppixel_src_ths[x]) { // ��ǰ��Ϊ�ڵ�
					if (255 == ppixel_src_top[x] || 255 == ppixel_src_btm[x]) { // �ж����������Ƿ���ڰ׵�
						index = ppixel_src_top[x-1]/255    + ppixel_src_top[x]/255*2  + ppixel_src_top[x+1]/255*4  +
								ppixel_src_ths[x-1]/255*8    /*         0         */  + ppixel_src_ths[x+1]/255*16 +
								ppixel_src_btm[x-1]/255*32 + ppixel_src_btm[x]/255*64 + ppixel_src_btm[x+1]/255*128;
						if(erasetable[index]){
							bfinished = FALSE;
							ppixel_dst_ths[x] = 255;
							ppixel_src_ths[x] = 255;
							y++;
						}
					}
				}
			}
		}
	}

	return NULL;
}
