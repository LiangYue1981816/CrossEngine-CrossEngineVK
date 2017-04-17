/****************************************************************************
Copyright (c) 2017 LiangYue.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sub license, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "_CrossEngine.h"


namespace CrossEngine {

	CROSS_EXPORT DWORD HashValue(const char *szString)
	{
		ASSERT(szString);

		const char *c = szString;
		DWORD dwHashValue = 0x00000000;

		while (*c) {
			dwHashValue = (dwHashValue << 5) - dwHashValue + (*c == '/' ? '\\' : *c);
			c++;
		}

		return dwHashValue ? dwHashValue : INVALID_HASHNAME;
	}

	static DWORD dwRandomValue = 0x19810816;
	static DWORD dwRandomKernel = 0x19810130;

	CROSS_EXPORT void PseudoRandomSeed(DWORD dwSeed)
	{
		dwRandomValue = dwSeed;
	}

	CROSS_EXPORT DWORD PseudoRandomValue(void)
	{
		if (dwRandomValue & 1) {
			dwRandomValue = ((dwRandomValue^dwRandomKernel) >> 1) | 0x80000000;
		}
		else {
			dwRandomValue = dwRandomValue >> 1;
		}

		return dwRandomValue;
	}

	/***********************************************************************************/
	/* UTF-8 Bit Distribution                                                          */
	/*                                                                                 */
	/* UTF-16                                  1st Byte 2nd Byte 3rd Byte 4th Byte     */
	/* -------- -------- -------- --------     -------- -------- -------- --------     */
	/* 00000000 0xxxxxxx                       0xxxxxxx                                */
	/* 00000yyy yyxxxxxx                       110yyyyy 10xxxxxx                       */
	/* zzzzyyyy yyxxxxxx                       1110zzzz 10yyyyyy 10xxxxxx              */
	/* 110110ww wwzzzzyy 110111yy yyxxxxxx     11110uuu 10uuzzzz 10yyyyyy 10xxxxxx     */
	/*                                                                                 */
	/* NOTE:                                                                           */
	/* uuuuu=wwww+1 (to account for addition of 0x10000 as in Section 3.7, Surrogates) */
	/***********************************************************************************/
	#define BYTE_1_REP          0x80    /* if <, will be represented in 1 byte */
	#define BYTE_2_REP          0x800   /* if <, will be represented in 2 bytes */

	/*
	If the unicode value falls on or between these values, it will be
	represented as 4 bytes
	*/
	#define SURROGATE_MIN       0xd800
	#define SURROGATE_MAX       0xdfff

	/*
	Convention for naming of following defines
	SIGMASK_3_1     - Signature mask for 1st byte of 3 byte transformation
	CLEARMASK_2_1   - Clearout mask for 1st byte of 2 byte transformation
	ROR_3_2         - Rotate right value for 2nd byte of 3 byte transformation
	*/
	#define SIGMASK_2_1         0xc0
	#define SIGMASK_3_1         0xe0

	/**********************************************************************************/
	/*                                                                                */
	/* Function:     UnicodeToUTF8                                                    */
	/* Description:  Convert Unicode string to UTF8 byte sequence                     */
	/*                                                                                */
	/* Parameters:   wszSrc - Unicode string to be converted                          */
	/*               nSrcLen - Length of wszSrc                                       */
	/*               szDest - char buffer for UTF8 text                               */
	/*               nDestLen - size (in characters) of buffer                        */
	/*                                                                                */
	/* Returns:      On success, the number of bytes in the destination buffer        */
	/*               0 on failure due to insufficient buffer size                     */
	/*                                                                                */
	/* History:      Created  {DATE]  {BY} {NAME} {PRODUCT REV}                       */
	/*               Modified {DATE]  {BY} {NAME} {PRODUCT REV}                       */
	/*                                                                                */
	/**********************************************************************************/
	CROSS_EXPORT size_t UnicodeToUTF8(const wchar_t *wszSrc, int nSrcLen, char *szDest, int nDestLen)
	{
		ASSERT(wszSrc);
		ASSERT(szDest);
		ASSERT(nSrcLen > 0);
		ASSERT(nDestLen > 0);

		int i = 0;
		int i_cur_output = 0;
		char ch_tmp_byte = 0;

		for (i = 0; i < nSrcLen; i++) {
			/* 1 byte utf8 representation */
			if (BYTE_1_REP > wszSrc[i]) {
				if (i_cur_output + 1 < nDestLen) {
					szDest[i_cur_output++] = (char)wszSrc[i];
				}
				else {
					szDest[i_cur_output] = 0;
					return 0;
				}
			}
			/* 2 byte utf8 representation */
			else if (BYTE_2_REP > wszSrc[i]) {
				if (i_cur_output + 2 < nDestLen) {
					szDest[i_cur_output++] = (char)(wszSrc[i] >> 6 | 0xc0);
					szDest[i_cur_output++] = (char)((wszSrc[i] & 0x3f) | 0x80);
				}
				else {
					szDest[i_cur_output] = 0;
					return 0;
				}
			}
			/* 4 byte surrogate pair representation */
			else if (SURROGATE_MAX > wszSrc[i] && SURROGATE_MIN < wszSrc[i]) {
				if (i_cur_output + 4 < nDestLen) {
					ch_tmp_byte = (char)(((wszSrc[i] & 0x3c0) >> 6) + 1);
					szDest[i_cur_output++] = (char)(ch_tmp_byte >> 2 | 0xf0);
					szDest[i_cur_output++] = (char)(((ch_tmp_byte & 0x03) | 0x80) | (wszSrc[i] & 0x3e) >> 2);
				}
				else {
					szDest[i_cur_output] = 0;
					return 0;
				}
			}
			/* 3 byte utf8 representation */
			else {
				if (i_cur_output + 3 < nDestLen) {
					szDest[i_cur_output++] = (char)(wszSrc[i] >> 12 | 0xe0);
					szDest[i_cur_output++] = (char)(((wszSrc[i] >> 6) & 0x3f) | 0x80);
					szDest[i_cur_output++] = (char)((wszSrc[i] & 0x3f) | 0x80);
				}
				else {
					szDest[i_cur_output] = 0;
					return 0;
				}
			}
		}

		szDest[i_cur_output] = 0;
		return i_cur_output;
	}

	/**********************************************************************************/
	/*                                                                                */
	/* Function:     UTF8ToUnicode                                                    */
	/* Description:  Convert UTF8 byte sequence to Unicode string                     */
	/*                                                                                */
	/* Parameters:   szSrc - UTF8 byte sequence to be converted                       */
	/*               nSrcLen - Length of szSrc                                        */
	/*               wszDest - unicode char buffer for                                */
	/*               nDestLen - size (in characters) of buffer                        */
	/*                                                                                */
	/* Returns:      On success, the number of characters in the destination buffer   */
	/*               0 on failure due to insufficient buffer size                     */
	/*                                                                                */
	/* History:      Created  {DATE]  {BY} {NAME} {PRODUCT REV}                       */
	/*               Modified {DATE]  {BY} {NAME} {PRODUCT REV}                       */
	/*                                                                                */
	/**********************************************************************************/
	CROSS_EXPORT size_t UTF8ToUnicode(const char *szSrc, int nSrcLen, wchar_t *wszDest, int nDestLen)
	{
		ASSERT(szSrc);
		ASSERT(wszDest);
		ASSERT(nSrcLen > 0);
		ASSERT(nDestLen > 0);

		int i = 0;
		int i_cur_output = 0;
		BYTE *pszSrc = (BYTE *)szSrc;

		while (i < nSrcLen) {
			/* 1st byte of 3 byte representation */
			if (SIGMASK_3_1 <= pszSrc[i]) {
				if (i + 2 < nSrcLen && i_cur_output + 1 < nDestLen) {
					wszDest[i_cur_output++] = ((wchar_t)(((wchar_t)pszSrc[i] << 12) | (((wchar_t)pszSrc[i + 1] & 0x3f) << 6) | ((wchar_t)pszSrc[i + 2] & 0x3f))) & 0x0000ffff;
					i += 3;
				}
				else {
					wszDest[i_cur_output] = 0;
					return 0;
				}
			}
			/* 1st byte of 2 byte representation */
			else if (SIGMASK_2_1 <= pszSrc[i]) {
				if (i + 1 < nSrcLen && i_cur_output + 1 < nDestLen) {
					wszDest[i_cur_output++] = ((wchar_t)(((wchar_t)pszSrc[i] & ~0xc0) << 6 | ((wchar_t)pszSrc[i + 1] & ~0x80))) & 0x0000ffff;
					i += 2;
				}
				else {
					wszDest[i_cur_output] = 0;
					return 0;
				}
			}
			/* Single byte representation */
			else {
				if (i < nSrcLen && i_cur_output + 1 < nDestLen) {
					wszDest[i_cur_output++] = ((wchar_t)pszSrc[i]) & 0x0000ffff;
					++i;
				}
				else {
					wszDest[i_cur_output] = 0;
					return 0;
				}
			}
		}

		wszDest[i_cur_output] = 0;
		return i_cur_output;
	}

}
