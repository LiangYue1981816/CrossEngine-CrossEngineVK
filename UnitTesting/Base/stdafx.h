#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <CrossEngine.h>

extern void Create(HINSTANCE hInstance, HWND hWnd);
extern void Destroy(void);
extern void Render(void);

extern BOOL LoadShader(const char *szFileName, char *szShader, size_t length);
