/////////////////////////////////////////////////////////////////////
// imagewavelet.c
//
// 功  能: 影像小波变换
//
// 修  订: 20:26 2005-1-2
//         12:23 2005-6-19
//         20:41 2005-7-11
//===================================================================

#include "_image.h"
#include "_wav_alloc.h"
#include "_wav_trf.h"
#include "_wav_gen.h"
#include "_wav_filters.h"

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_DWT
//
// 功  能: 影像小波正变换
//
// 参  数: IMAGE * const pimgsrc  源数据
//         float ** const ppdata   结果数据
//         int      const levs     小波变换阶数
//         BOOL     const bpacket  是否使用小波包
//         char     const cparam   用于选取Antonini、Haar、Nick_flip三个滤波器
//         int      const nparam   用于选择D79和D8两个滤波器
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_DWT(IMAGE * const pimgsrc, float ** const ppdata, int const levs, BOOL const bpacket, char const cparam, int const nparam)
{
	int rcode = NO_ERROR;
	
	int x, y;
	int width, height;
	
	int i, Nl, Nh;
	
	int shift_arr_r[MAX_ARR_SIZE];
	int shift_arr_c[MAX_ARR_SIZE];
	
	float **buffer_t = NULL;
	float *lp = NULL, *hp = NULL;
	IMGGRAY *ppixel_src = NULL;
	
	// Parameter check.
	assert(ppdata);
	assert(pimgsrc);
	if (NULL == pimgsrc || NULL == pimgsrc->data) { // Image pointer and it's buffer must be valid
		rcode = ERR_IMAGE; goto RET;
	}
	if (8 != IMAGE_BITCOUNT(pimgsrc)) { // Image bitcout must be 8 bit
		rcode = ERR_BITCOUNT; goto RET;
	}
	if (NULL == ppdata || levs < 0) { // Parameter must be valid
		rcode = ERR_PARAMETER; goto RET;
	}

	// Alloc main buffer for operations.
	width = IMAGE_AREA_WIDTH(pimgsrc);
	height = IMAGE_AREA_HEIGHT(pimgsrc);
	
	buffer_t = (float **)IMAGE_AllocMatrix(height, width, sizeof(float));
	if (NULL == buffer_t) { rcode = ERR_MEM; goto RET; }

	// At every level of a 1-D wavelet transform	there are two transforms possible
	// obtained by the original bank or its one pixel shifted version.
	// In L levels there are 2^L possibilities with each of these possibilities
	// forming a complete transform (i.e., invertible, etc.). If we evaluate
	// more than one then what we end up with is an overcomplete transform.
	// In 2-D there are 4 possibilities at each level and hence 4^L total
	// possibilities.
	//
	// At level l, this code selects either the "regular" (shift_arr_r[l]=0)
	// or the one shifted bank (shift_arr_r[l]=1) over rows and likewise
	// over columns using shift_arr_c[l].
	//
	// In order to generate the fully overcomplete transform we effectively
	// put the binary representation of numbers from 0 to 2^L-1 (represented
	// by oc_max) in shift_arr_r and shift_arr_c to traverse all 4^L possibilities.
	//
	// Here no shifts necessary since we are computing a single complete transform
	// (the "regular" case). See wavelet_denoise() in test_denoise.c for the use of shifts in
	// overcomplete transformations.
	for (i = levs-1; i >= 0; i--)
		shift_arr_r[i] = shift_arr_c[i] = 0;
	
	// Copy image ppdata input to the buffer.
	for (y = 0; y < height; y++) {
		ppixel_src = (IMGGRAY *)(&(pimgsrc->matrix[pimgsrc->sy+y][pimgsrc->sx]));

		for (x = 0; x < width; x++) {
			buffer_t[y][x] = ppixel_src[x];
		}
	}
	
	// Choose wavelet filter.
	choose_filter(cparam, nparam);
	
	// Wavelet transform
	// Select the forward bank of filters.
	lp = MFLP; Nl = Nflp;
	hp = MFHP; Nh = Nfhp;
				
	if (bpacket) {		
		// Packet transform.
		wavpack2d_inpl(buffer_t, height, width, levs, lp, Nl, hp, Nh, 1, shift_arr_r, shift_arr_c);
	}
	else {		
		// Regular wavelets.
		wav2d_inpl(buffer_t, height, width, levs, lp, Nl, hp, Nh, 1, shift_arr_r, shift_arr_c);
	}

	// Copy buffer ppdata to output ppdata
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			ppdata[y][x] = buffer_t[y][x];
		}
	}
	
RET:
	// Free buffers.
	if (buffer_t) IMAGE_FreeMatrix((void**)buffer_t);
	return rcode;
}

/////////////////////////////////////////////////////////////////////
// 名  称: IMAGE_IDWT
//
// 功  能: 影像小波反变换
//
// 参  数: float ** const ppdata  源数据
//         IMAGE * const pimgdst 结果影像
//         int      const levs    小波变换阶数
//         BOOL     const bpacket 是否使用小波包
//         char     const cparam  用于选取Antonini、Haar、Nick_flip三个滤波器
//         int      const nparam  用于选择D79和D8两个滤波器
//
// 返回值: int
//===================================================================
EXPORT_C int IMAGE_IDWT(float ** const ppdata, IMAGE * const pimgdst, int const levs, BOOL const bpacket, char const cparam, int const nparam)
{
	int rcode = NO_ERROR;

	int x, y;
	int height, width;
	
	int i, Nl, Nh;
	
	int shift_arr_r[MAX_ARR_SIZE];
	int shift_arr_c[MAX_ARR_SIZE];
	
	float **buffer_t = NULL;
	float *lp = NULL, *hp = NULL;
	IMGGRAY *ppixel_dst = NULL;
	
	// Parameter check.
	assert(ppdata);
	assert(pimgdst);
	if (NULL == pimgdst || NULL == pimgdst->data) { // Image pointer and it's buffer must be valid
		rcode = ERR_IMAGE; goto RET;
	}
	if (8 != IMAGE_BITCOUNT(pimgdst)) { // Image bitcout must be 8 bit
		rcode = ERR_BITCOUNT; goto RET;
	}
	if (NULL == ppdata || levs < 0) { // Parameter must be valid
		rcode = ERR_PARAMETER; goto RET;
	}
	
	// Alloc main buffer for operations.
	width  = IMAGE_AREA_WIDTH(pimgdst);
	height = IMAGE_AREA_HEIGHT(pimgdst);
	
	buffer_t = (float **)IMAGE_AllocMatrix(height, width, sizeof(float));
	if (NULL == buffer_t) { rcode = ERR_MEM; goto RET; }

	// At every level of a 1-D wavelet transform	there are two transforms possible
	// obtained by the original bank or its one pixel shifted version.
	// In L levels there are 2^L possibilities with each of these possibilities
	// forming a complete transform (i.e., invertible, etc.). If we evaluate
	// more than one then what we end up with is an overcomplete transform.
	// In 2-D there are 4 possibilities at each level and hence 4^L total
	// possibilities.
	//
	// At level l, this code selects either the "regular" (shift_arr_r[l]=0)
	// or the one shifted bank (shift_arr_r[l]=1) over rows and likewise
	// over columns using shift_arr_c[l].
	//
	// In order to generate the fully overcomplete transform we effectively
	// put the binary representation of numbers from 0 to 2^L-1 (represented
	// by oc_max) in shift_arr_r and shift_arr_c to traverse all 4^L possibilities.
	//
	// Here no shifts necessary since we are computing a single complete transform
	// (the "regular" case). See wavelet_denoise() in test_denoise.c for the use of shifts in
	// overcomplete transformations.
	for (i = levs-1; i >= 0; i--)
		shift_arr_r[i] = shift_arr_c[i] = 0;
	
	// Copy ppdata to ppdata buffer
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			buffer_t[y][x] = ppdata[y][x];
		}
	}
	
	// Choose wavelet filter.
	choose_filter(cparam, nparam);
	
	// Wavelet transform
	// Select the inverse bank of filters.
	lp = MILP; Nl = Nilp;
	hp = MIHP; Nh = Nihp;
				
	// Inverse transform.
	if (bpacket) {
		// Packet transform.
		wavpack2d_inpl(buffer_t, height, width, levs, lp, Nl, hp, Nh, 0, shift_arr_r, shift_arr_c);
	}
	else {
		// Regular wavelets.
		wav2d_inpl(buffer_t, height, width, levs, lp, Nl, hp, Nh, 0, shift_arr_r, shift_arr_c);
	}
	
	// Copy ppdata to image
	for (y = 0; y < height; y++) {
		ppixel_dst = (IMGGRAY *)(&(pimgdst->matrix[pimgdst->sy+y][pimgdst->sx]));
		
		for (x = 0; x < width; x++) {
			if (buffer_t[y][x] > 255.0f) buffer_t[y][x] = 255.0f;
			if (buffer_t[y][x] < 000.0f) buffer_t[y][x] = 000.0f;
			ppixel_dst[x] = (IMGGRAY)buffer_t[y][x];
		}
	}
	
RET:
	// Free buffers.
	if (buffer_t) IMAGE_FreeMatrix((void**)buffer_t);	
	return rcode;
}
