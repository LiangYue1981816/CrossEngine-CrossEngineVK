#include "stdafx.h"


BOOL LoadShader(const char *szFileName, char *szShader, size_t length)
{
	FILE *pFile = fopen(szFileName, "rb");
	if (pFile == NULL) return FALSE;

	memset(szShader, 0, length);
	fread(szShader, 1, length, pFile);

	fclose(pFile);
	return TRUE;
}
