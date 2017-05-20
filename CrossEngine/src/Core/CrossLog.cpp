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

	static BOOL bEnableLog = FALSE;
	static char szLogText[1024 * 1024];
	static char szLogFileName[_MAX_STRING];

	CROSS_EXPORT void EnableLog(BOOL bEnable, const char *szFileName)
	{
		bEnableLog = bEnable;

		if (szFileName) {
			strcpy(szLogFileName, szFileName);
			remove(szLogFileName);
		}
		else {
			strcpy(szLogFileName, "");
		}
	}

	CROSS_EXPORT void WriteLogI(const char *szFileName, int nLineNumber, const char *szFormat, ...)
	{
		ASSERT(szFormat);

		if (bEnableLog) {
			va_list vaList;
			va_start(vaList, szFormat);
			vsprintf(szLogText, szFormat, vaList);
			va_end(vaList);

			if (FILE *pFile = fopen(szLogFileName, "a+")) {
				fprintf(pFile, "I: %s", szLogText);
				fclose(pFile);
			}

#ifdef PLATFORM_WINDOWS

			OutputDebugString(szLogText);

#elif PLATFORM_ANDROID

			__android_log_print(ANDROID_LOG_INFO, TAG_NAME, szLogText);

#else

			printf(szLogText);

#endif
		}
	}

	CROSS_EXPORT void WriteLogW(const char *szFileName, int nLineNumber, const char *szFormat, ...)
	{
		ASSERT(szFormat);

		if (bEnableLog) {
			va_list vaList;
			va_start(vaList, szFormat);
			vsprintf(szLogText, szFormat, vaList);
			va_end(vaList);

			if (FILE *pFile = fopen(szLogFileName, "a+")) {
				fprintf(pFile, "W: %s", szLogText);
				fclose(pFile);
			}

#ifdef PLATFORM_WINDOWS

			OutputDebugString(szLogText);

#elif PLATFORM_ANDROID

			__android_log_print(ANDROID_LOG_WARN, TAG_NAME, szLogText);

#else

			printf(szLogText);

#endif
		}

		ASSERT(FALSE);
	}

	CROSS_EXPORT void WriteLogE(const char *szFileName, int nLineNumber, const char *szFormat, ...)
	{
		ASSERT(szFormat);

		if (bEnableLog) {
			va_list vaList;
			va_start(vaList, szFormat);
			vsprintf(szLogText, szFormat, vaList);
			va_end(vaList);

			if (FILE *pFile = fopen(szLogFileName, "a+")) {
				fprintf(pFile, "E: %s", szLogText);
				fclose(pFile);
			}

#ifdef PLATFORM_WINDOWS

			OutputDebugString(szLogText);

#elif PLATFORM_ANDROID

			__android_log_print(ANDROID_LOG_ERROR, TAG_NAME, szLogText);

#else

			printf(szLogText);

#endif
		}

		ASSERT(FALSE);
	}

}
